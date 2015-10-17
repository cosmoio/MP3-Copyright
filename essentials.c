#include "essentials.h"

/** Help the user **/
void printHelp(char **argv)
{
	fprintf(stdout, "USAGE: %s -d [DIRECTORY] [-v] [-h]\n", argv[0]);
	return;
}

/** Help the user **/
void printOpt(char *arg, int optopt)
{
	fprintf(stderr, arg, optopt);
	return;
}

/** Prints the version of tool and sums up its functionality **/
void printVersion()
{
	printf("\n\t********************************************************************\n");
	printf("\t* mp3tool 0.1 supports formatting the frame header of an mp3 file. *\n");
	printf("\t*             I.e., it sets the copyright bit                      *\n");
	printf("\t* No license restriction (use it as you see fit)                   *\n");
	printf("\t*                                                                  *\n");
	printf("\t* Usage: -d [DIRECTORY]                                            *\n");
	printf("\t*        -v (Prints version)                                       *\n");
	printf("\t*        -h (Prints usage)                                         *\n");
	printf("\t*                                                                  *\n");
	printf("\t********************************************************************\n");
}
