// Code for handling configuration information

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int write_config(FILE *fp) {
  fputs("[Test]", fp);
  return 0;
}

int read_config(const char *fname, config_entry *config) {
  char buff[512];
  char section[64];
  char name[128];
  char value[128];
  int i = 0;

  // Open config.ini for reading
  FILE *fp = fopen(fname, "r");
  if (fp == NULL) {
    fprintf(stderr, "Failed to open config.ini (Check file permissions).\n");
    return -1;
  }

  while (fgets(buff, sizeof(buff), fp)) {
    // Remove the '\n' at the end of each line
    buff[strlen(buff) - 1] = '\0';

    // Remove all empty lines or comments from consideration
    if (buff[0] == '\0' || buff[0] == ';' || buff[0] == '#') {
      continue;
    }

    // Determine if the read line is a section
    if (buff[0] == '[' && buff[strlen(buff) - 1] == ']') {
      // TODO: Rewrite this so it is not using sscanf
      sscanf(buff, "[%63[^]]]", section);
      continue;
    }

    // If none of the above, it is a new entry and needs to be added
    config[i].section = (char *)malloc(strlen(section));
    strcpy(config[i].section, section);

    // TODO: Rewrite this so it is not using sscanf
    if (sscanf(buff, " %127[^=]= %127[^\n]", name, value) == 2) {
      config[i].name = (char *)malloc(strlen(name));
      strcpy(config[i].name, name);
      config[i].value = (char *)malloc(strlen(value));
      strcpy(config[i].value, value);
    } else {
      fprintf(stderr,
              "[Error] Failed to read configuration #%d from config.ini\n",
              i + 1);
    }

    i++;
  }

  fclose(fp);

  return 0;
}

char *fetch_config(config_entry *config, char *section, char *name) {
  // TODO: Once the config struct has been rewritten, rewrite this to use a
  // better searching algorithm and remove the magic number
  for (int i = 0; i < 100; i++) {
    if (strcmp(config[i].section, section) == 0 &&
        strcmp(config[i].name, name) == 0) {
      return config[i].value;
    }
  }

  return NULL;
}

int free_config(config_entry *config) {
  // TODO: Remove the magic number from this once it the config array
  // initialisation is rewritten
  for (int i = 0; i < 100; i++) {
    if (config[i].name != NULL) {
      printf("[Debug] Freeing config #%d, %s, %s, %s\n", i, config[i].name,
             config[i].value, config[i].section);
      free(config[i].name);
      free(config[i].value);
      free(config[i].section);
    }
  }

  printf("[Debug] Freeing config\n");
  free(config);

  return 0;
}
