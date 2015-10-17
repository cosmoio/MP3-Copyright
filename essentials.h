#ifndef ESSENTIALS_H
#define ESSENTIALS_H


/* Essentials that are needed for the mp3 handling tool.. constants, additional prototypes, .. */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <sys/param.h>
#include "editmp3.h"
#include "file_handling.h"

#define SUCCESS EXIT_SUCCESS	// POSIC EXIT_SUCCESS = 0
#define FAILURE -1				// POSIX EXIT_FAILURE = 1
#define NO_ERROR 0
#define NO_FILE_ACCESS 1

#ifdef DEBUG
#undef DEBUG
#endif
#define DEBUG 1

#ifdef VERBOSE
#undef VERBOSE
#endif
#define VERBOSE 0



/** getopt is not a C99 function (so it seems), therefore we have to provide a prototype
	and extern variables, which we need to access **/
int isprint(int c);
char *realpath(const char *path, char *resolved_path);
int getopt(int argc, char * const argv[], const char *optstring);
void printHelp(char **argv);
void printOpt(char *arg, int optopt);
void printVersion();
extern char *optarg;
extern int optind, opterr, optopt;



#endif  /* ESSENTIALS_H */
