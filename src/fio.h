#pragma once
#ifndef FIO_H
#define FIO_H

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

/*
 * Checks if the given file exists
 *
 * Parameters:
 * fname - Name of file to check
 *
 * Return Values:
 * 0 - Success
 * -1 - Unknown error
 * -2 - File not found error
 */
int fcheck(const char *fname);

/*
 * Checks if yt-dlp is installed on the user's machine and is accessible
 *
 * Parameters:
 * fname - Path to check for yt-dlp
 *
 * Return Values:
 * 0 - Success
 * -1 - yt-dlp could not be found or accessed
 */
int check_ytdlp(const char *fname);

/*
 * Checks if a list.txt is present and accessible. If not, prompts user for one
 * to be created
 *
 * Parameters:
 * fname - Path to check for list.txt
 *
 * Return Values:
 * 0 - Success
 * 1 - list.txt was successfully created after being prompted
 * -1 - list.txt could not be found or accessed
 */
int check_list(const char *fname);

/*
 * Checks if a config.ini is present and accessible. If not, prompts user for
 * one to be created and calls the necessary construction functions
 *
 * Parameters:
 * fname - Path to check for config.ini
 *
 * Return Values:
 * 0 - Success
 * 1 - config.ini was successfully created after being prompted
 * -1 - config.ini could not be found or accessed
 * -2 - config.ini was successfully created, but the default data could not be
 * written
 */
int check_config(const char *fname);

#endif
