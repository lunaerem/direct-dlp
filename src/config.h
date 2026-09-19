#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>

#define MAP_SIZE 100

typedef struct config_entry {
  char *section;
  char *name;
  char *value;
} config_entry;

/*
 *
 * Writes a default configuration file
 */
int write_config(FILE *fp);

/*
 *
 * Reads from the user's configuration file into memory
 */
int read_config(const char *fname, config_entry *config);

/*
 *
 * Fetches a configuration value given the section it is under and the config's namespace
 */
char *fetch_config(config_entry *config, char *section, char *name);

/*
 *
 * Frees the memory used to store the user's configs
 */
int free_config(config_entry *config);

#endif
