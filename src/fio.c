// Code for handling file I/O
#include "fio.h"
#include "config.h"
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int fcheck(const char *fname) {

  // TODO: Replace access because it is not portable
  if (access(fname, F_OK) != 0) {
    if (errno != ENOENT) {
      // Unknown error
      return -1;
    } else {
      // File not found error
      return -2;
    }
  }

  // Success
  return 0;
}

int check_ytdlp(const char *fname) {
  // TODO: Implement better custom yt-dlp directory support so that it actually
  // checks if yt-dlp exists there instead of whether the given directory exists
  switch (fcheck(fname)) {
  case -1:
    perror("Unknown error");
    return -1;
    break;
  case -2:
    fprintf(stderr, "Failed to find yt-dlp. Please ensure it is installed and "
                    "in the current directory.\n");
    return -1;
    break;
  default:
    break;
  }
  return 0;
}

int check_list(const char *fname) {
  // TODO: Implement better custom list.txt directory support so that it
  // actually checks if list.txt exists there instead of whether the given
  // directory exists Also need to create the list.txt in the desired directory
  // instead of defaulting to the current directory that the program is
  // installed in
  switch (fcheck(fname)) {
  case -1:
    perror("Unknown error");
    return -1;
    break;
  case -2:
    fprintf(stderr, "No list.txt found.\n");
    printf("Would you like one to be created? (y/n) ");
    char selection;
    scanf(" %c", &selection);
    if (selection == 'y') {
      FILE *fp = fopen("list.txt", "a");
      if (fp != NULL) {
        printf("list.txt successfully created.\n");
      } else {
        fprintf(stderr,
                "Failed to create list.txt (Check file permissions).\n");
        return -1;
      }
      fclose(fp);
    }
    return 1;
    break;
  default:
    break;
  }
  return 0;
}

int check_config(const char *fname) {
  // TODO: Implement custom ini locations?
  switch (fcheck(fname)) {
  case -1:
    perror("Unknown error");
    return -1;
    break;
  case -2:
    fprintf(stderr, "No configuration file (config.ini) found.\n");
    printf("Would you like one to be created? (y/n) ");
    char selection;
    scanf(" %c", &selection);
    if (selection == 'y') {
      int ev = 0;
      FILE *fp = fopen("config.ini", "a");
      if (fp != NULL) {
        printf("Configuration file successfully created.\n");
      } else {
        fprintf(stderr,
                "Failed to create config.ini (Check file permissions).\n");
        return -1;
      }
      ev = write_config(fp);
      fclose(fp);
      if (ev) {
        fprintf(stderr,
                "Failed to write to config.ini (Check file permissions).\n");
        return -2;
      }
    }
    return 1;
    break;
  default:
    break;
  }
  return 0;
}

int fbackup(const char *fname, const char *fdupe) {

  FILE *copier = fopen(fname, "rb");
  char input;
  // TODO: Rewrite to get rid of magic numbers 512 and 4096
  char full[512];
  char buff[4096];
  size_t bytes_read;
  size_t bytes_written;

  // TODO Rewrite this, temp
  const char *desired_name = "config.ini.bak";

  snprintf(full, sizeof(full), "%s/%s", fdupe, desired_name);

  printf("Backing up user configuration file to %s\n", full);

  // Checking if file to copy was opened
  if (copier == NULL) {
    perror("Unable to open user configuration file.");
    return -1;
  }

  // Opening the desired directory
  DIR *dir = opendir(fdupe);

  if (dir == NULL) {
    perror("Unable to open desired directory");
    fclose(copier);
    return -1;
  }

  struct dirent *entry;

  errno = 0;

  // Checking if there exists a backup at the location
  while ((entry = readdir(dir))) {
    if (strcmp(desired_name, entry->d_name) == 0) {
      // Prompting the user if a backup config is already there
      fprintf(stderr,
              "[Warning] A backup user configuration file already exists at "
              "the given destination (%s).\n",
              fdupe);
      printf("The prexisting backup file will be overwritten. Proceed? [y/n] ");
      // TODO: Maybe change to not use scanf
      scanf(" %c", &input);
      if (input == 'n' || input == 'N') {
        closedir(dir);
        fclose(copier);
        printf("Execution halted, no backup configuration created.\n");
        return 0;
      }
    }
  }

  closedir(dir);

  // Checking to see if there was an error with readdir
  if (errno) {
    perror("Failed to search directory to duplicate user configuration file.");
    fclose(copier);
    return -1;
  }

  // Copy the file
  // TODO: Get rid of debug printing
  printf("[Debug] Full path: %s\n", full);

  FILE *copy = fopen(full, "wb");

  // Checking if file to copy to was opened
  if (copy == NULL) {
    perror("Unable to open user configuration backup to copy to.");
    fclose(copier);
    return -1;
  }

  // TODO rewrite to get rid of magic number 4096
  while ((bytes_read = fread(buff, 1, 4096, copier)) > 0) {
    bytes_written = fwrite(buff, 1, bytes_read, copy);
    if (bytes_read > bytes_written) {
      perror("Read/Write mismatch. Number of bytes read from config file "
             "greater than number written to backup file.");
      fprintf(stderr,
              "[Warning] Config backup failed during read/write, there may be "
              "a partial configuration backup at %s\n",
              full);
      fclose(copier);
      fclose(copy);
      return -1;
    }
  }

  printf("Backup user configuration file successfully created at %s\n", full);

  // Cleaning up
  fclose(copier);
  fclose(copy);

  return 0;
}
