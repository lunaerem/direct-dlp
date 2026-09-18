// Code for handling file I/O
#include "fio.h"
#include "config.h"

int fcheck(const char *fname) {

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
  // TODO: Allow for custom yt-dlp paths based on the user's config
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
  // TODO: Implement optional usage of a list.txt based on user config and
  // custom paths for it
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
