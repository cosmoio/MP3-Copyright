#include "essentials.h"


/** Handles the arguments by using getopt, returns SUCCESS/FAILURE **/
int handleArguments(int argc, char **argv, int *dflag, int *rflag, int *vflag, char **dvalue)
{
      int c;
	  while ((c = getopt (argc, argv, "hd:v")) != -1)
	  {
		switch (c)
        {
		   case 'h':
			printHelp(argv);
			exit(EXIT_SUCCESS);
		   case 'd':		// Capture the 'd' option to get to the directory
		    *dflag = 1;
			 *dvalue = (char *) malloc(strlen(optarg)+1);
			 strncpy(*dvalue, optarg, strlen(optarg)+1);
			return SUCCESS;
		   case 'v':		// Capture the 'v' option to later print the version of the tool
		     printVersion();
			 exit(EXIT_SUCCESS);
		   case '?':
		   	if (optopt == 'd')
		   		printOpt("Option -%c requires a directory.\n", optopt);
			else if (isprint (optopt))
				printOpt("Unknown option `-%c'.\n", optopt);
		    else
				printOpt("Unknown option character `\\x%x'.\n", optopt);
     	    return FAILURE;
		   default:
				printHelp(argv);
		   		exit(EXIT_SUCCESS);
		}
	}
		printHelp(argv);
  		exit(EXIT_SUCCESS);		
}

int editFile(char *file)
{
    char fullFilename[PATH_MAX]; 
    char *res = realpath(file, fullFilename);	// realpath constructs the full pathname to the specified file
	int errorlevel;	
	unsigned long *headerPositions;
	FILE *mp3file;

	if (res == NULL)
	{
        perror("Failure in realpath"); 		// problems with the file?
		errorlevel = NO_FILE_ACCESS;		
		return errorlevel;
	}

	#if (DEBUG && VERBOSE)
		printf("editFile, files: %s - %s\n",file, fullFilename);
	#endif	

	mp3file = fopen(fullFilename, "rb+"); 	// open the requested file with mode read and write in binary

    if (ferror(mp3file))
	{
        perror("Failure opening file"); 		// problems with the file?
		errorlevel = NO_FILE_ACCESS;		
		return errorlevel;
    }

	// Always create backups of files you edit
	if (backupFile(mp3file, file) == FAILURE)
	{
		perror("Failure in backupFile(mp3File)");
		exit ( EXIT_FAILURE );		
	}


	seekMP3Header(file);	

	fclose(mp3file); // Close your files 
	return NO_ERROR;
}




int main(int argc, char ** argv)
{
 	int i;       
	int dflag = 0;
	int rflag = 0;
    int vflag = 0;
    char *directory = NULL;
    int index;
	char requestedFile[10];
	char **directoryFiles;
	int numfiles = 0;
	int errorLevel;

	if (FAILURE == handleArguments(argc, argv, &dflag, &rflag, &vflag, &directory))
		return EXIT_FAILURE;
	
	#ifdef DEBUG
		printf("\nDirectory to search in: %s\n", directory);
	#endif
	
	if ((numfiles = getFilesDirectory(directory, &directoryFiles)) == FAILURE)
		exit(EXIT_FAILURE);

	if(dflag)
	{
		for (int i = 0; i < numfiles; i++)
		{
			errorLevel = editFile(directoryFiles[i]);
			
			switch(errorLevel)
			{
				case NO_ERROR:
					fprintf(stdout, "Edited File: %s\n\n", directoryFiles[i]);
					break;
				case NO_FILE_ACCESS:
					fprintf(stdout, "Not able to alter file: %s\n\n", directoryFiles[i]);
					break;
				default:
					fprintf(stderr, "Abnormal program termination\n\n");
					exit(EXIT_FAILURE);
			}
			free(directoryFiles[i]);			
		}
	}

	return EXIT_SUCCESS;
}

