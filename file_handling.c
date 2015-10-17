#include "file_handling.h"

/** This backups our file handed over by mp3file **/
int backupFile(FILE *mp3file, char *file)
{
	size_t n,m;
	unsigned int buffer[BLOCKSIZE];
	FILE *backupfile;
	char *backupFile = malloc(sizeof(char) * (strlen(file) + 5));
	strncpy(backupFile, file, strlen(file)+1);
	strncat(backupFile, ".bak", 4);

	#ifdef DEBUG
		printf("Backing up file: %s \n",file);
	#endif
	
	backupfile = fopen(backupFile, "wb+"); 	// Open the requested file with mode create, read, and write in binary
    if (ferror(mp3file) || ferror(backupfile)){
        perror("Failure in fopen()"); 				// Can't open => abort
		return FAILURE;
    }

    fseek(mp3file,0,SEEK_SET);
    fseek(backupfile,0,SEEK_SET);

	while((n = fread(buffer, sizeof(unsigned int), 100, mp3file)) > 0)
	{

		if ((m = fwrite(buffer, sizeof(unsigned int), n, backupfile)) < 0)
		{
			perror("Failure in write()");
			return FAILURE;					
		}
	//		printf("n = %d, m = %d\n",n,m);
	}
	free(backupFile);
	fclose(backupfile);
	return SUCCESS;
}

/** Retrieve all the files from a given directory and store it in a list (filelist). 
	Return the number of found files that are mp3 files. A file is an mp3 file if the file
	extension is .mp3 **/

int getFilesDirectory(const char *directory, char ***fileList)
{
	int numFiles = 0;
    DIR *dp;
    struct dirent *file;
	*fileList = (char **) malloc(sizeof(char *) * MAXFILENUM);    	

	int currentNumFiles = MAXFILENUM;

       dp = opendir (directory);
       if (dp != NULL)
         {
           while (file = readdir (dp))
           {
				char *ext = strrchr(file->d_name, '.');
				if (ext && strcmp(ext, ".mp3") == 0)
				{
					(*fileList)[numFiles] = (char *) malloc(sizeof(char)*(strlen(file->d_name)+strlen(directory))+1);
					strncpy((*fileList)[numFiles], directory, strlen(directory)+1);
					strncat((*fileList)[numFiles], file->d_name, strlen(file->d_name)+1);
					#if DEBUG && VERBOSE
						printf("%s %d %d\n",(*fileList)[numFiles], strlen((*fileList)[numFiles]),sizeof(char)*(strlen(file->d_name)+strlen(directory))+1);
					#endif
					numFiles++;
					if(numFiles >= currentNumFiles)
					{
						#if DEBUG && VERBOSE
							printf("1 pointer: %p\n",*fileList);
							printf("2 pointer: %p %s\n",(*fileList)[0], (*fileList)[0]);
							printf("3 pointer: %p %s\n",(*fileList)[numFiles-1], (*fileList)[numFiles-1]);
						#endif
						*fileList = realloc(*fileList, sizeof(char *) * (currentNumFiles + MAXFILENUM));
						#if DEBUG && VERBOSE
							printf("1 pointer: %p\n",*fileList);
							printf("2 pointer: %p %s\n",(*fileList)[0], (*fileList)[0]);
							printf("3 pointer: %p %s\n",(*fileList)[numFiles-1], (*fileList)[numFiles-1]);
						#endif
						fflush(stdout);

						if(fileList == NULL)
						{
							perror("Not enough memory");
							exit(EXIT_FAILURE);
						}
						currentNumFiles += MAXFILENUM;					
					}					
				}	
			}
           closedir (dp);
		}
		else
		{
			perror ("Couldn't open the directory");
			return FAILURE;
		}

	return numFiles;
}

