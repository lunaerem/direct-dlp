#include <stdio.h>
#include <unistd.h>

#define VERSION "0.1.0"

int legacyui();

int main(int argc, char *argv[]) {

  int opt;
  int rv = 0;

  // Checking that there is at least one argument
  if (argc < 2) {
    fprintf(stderr, "Usage error: At least one argument must be given. See "
                    "direct-dlp -h for details.\n");
    return -1;
  }

  // Checking through the arguments
  while ((opt = getopt(argc, argv, "vhld:")) != -1) {
    switch (opt) {
    case 'v':
      printf("direct-dlp version: %s\n", VERSION);
      break;
    case 'l':
      rv = legacyui();
      break;
    case 'd':
      printf("Download: %s\n", optarg);
      fprintf(stderr, "Currently not implemented yet, sorry.\n");
      break;
    case 'h':
      printf("Help\n");
      fprintf(stderr, "Currently not implemented yet, sorry.\n");
      break;
    case '?':
      break;
    }
  }

  return rv;
}

// Code for the legacy UI system
// Will be deprecated in the future
// -------------------------------
#include "fio.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int legacyui() {

  // Declaring and initialising commands
  // This will get cleaned up once things are rewritten for custom options
  // For now, this will stay pretty unoptimised and messy until then
  char *list[11] = {"python3",
                   "yt-dlp",
                   "-a",
                   "list.txt",
                   "-x",
                   "--output=%(title)s.%(ext)s",
                   "--audio-format=mp3",
                   "--audio-quality=0",
                   "--js-runtimes",
                   "node",
                   NULL};

  char *list2[10] = {
      "python3",
      "yt-dlp",
      "-a",
      "list.txt",
      "--output=%(title)s.%(ext)s",
      "-f",
      "bestvideo[ext!=webm][height<=1080]+bestaudio[ext!=webm]/best[ext!=webm]",
      "--js-runtimes",
      "node",
      NULL};

  char *list3[12] = {"python3",
                    "yt-dlp",
                    "-a",
                    "list.txt",
                    "--output=%(title)s.%(ext)s",
                    "-f",
                    "bestvideo[ext!=webm]+bestaudio[ext!=webm]/best[ext!=webm]",
                    "--js-runtimes",
                    "node",
                    "--playlist-start",
                    "210",
                    NULL};

  char *list4[4] = {"python3", "yt-dlp", "-U", NULL};

  // Checking if yt-dlp is installed
  switch (fcheck("yt-dlp")) {
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

  // Checking if a list.txt exists and prompting if not
  switch (fcheck("list.txt")) {
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
    return 0;
    break;
  default:
    break;
  }

  printf("\e[1;1H\e[2J");

  int option = -1;

  // Prompt the user
  printf("Please select how to download:\n");
  printf("1) MP3\n");
  printf("2) MP4 1080p\n");
  printf("3) MP4 4K\n");
  printf("4) Update yt-dlp\n");
  printf("5) Cancel\n");

  if (scanf(" %d", &option) == EOF) {
    perror("Input error");
  }

  // Check which option the user selected
  switch (option) {
  case 1:
    execvp("python3", list);
    break;
  case 2:
    execvp("python3", list2);
    break;
  case 3:
    execvp("python3", list3);
    break;
  case 4:
    execvp("python3", list4);
    break;
  case 5:
    return 0;
    break;
  default:
    errno = EIO;
    perror("Invalid input provided");
    return errno;
    break;
  }

  perror("Failed to execute python and yt-dlp");

  return errno;
}
