#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#define SUCCESS EXIT_SUCCESS		// POSIC EXIT_SUCCESS = 0
#define FAILURE 		-1			// POSIX EXIT_FAILURE = 1
#define MAXFILENUM		10			// The maximum amount of files per directory without dynamically allocating memory
#define BLOCKSIZE 		64000		// Blocksize to read a file

typedef struct {
  char *fileName;
  char **fullFileName;
} file;

/** This backups some file specified by mp3file **/
int backupFile(FILE *mp3file, char *file);
int getFilesDirectory(const char *directory, char ***fileList);

#endif
