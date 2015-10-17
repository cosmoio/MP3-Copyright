#include "editmp3.h"


unsigned int shifts[] = { 0, 11, 13, 15, 16, 20, 22, 23, 24, 26, 28, 29, 30 };
unsigned int masks[] = { 0x7FF, 0x1800, 0x6000, 0x8000, 0xF0000, 0x300000,
        0x400000, 0x800000, 0x3000000, 0xC000000, 0x10000000, 0x20000000,
        0xC0000000 };


/** Set the bits by using the masks specified above **/
void setvalue( unsigned int *mp3header, mp3t_t tag, unsigned int value )
{
        *mp3header &= ~masks[tag];
        *mp3header |= (( masks[tag]) & (value<<shifts[tag]));
}

void setCopyright( unsigned char *mp3header)
{	
		mp3header[3] |= 0x08;
		mp3header[3] |= 0x04;
}

/** Y XOR Y = 0, get rid of data **/
void resetvalue( unsigned int *mp3header )
{
			*mp3header ^= *mp3header;
}

void printMP3Header(unsigned int *mp3header)
{
	printf("MP3 Header 0-31 (32 total): "); showbits_ui( *mp3header );
}

int isID3(const unsigned char *p, const unsigned char *e) {
  /* do we have enough room to see a 4 byte header? */
  if (p > e) return 0;
  if (e - p < 10) return 0;
  /* do we have a sync pattern? */
  if (p[0] == 'I' && p[1] == 'D' && p[2] == '3') {
    if (p[3] == 0xff || p[4] == 0xff) return 0; /* illegal version */
    if (p[6] & 0x80 || p[7] & 0x80 ||
        p[8] & 0x80) return 0; /* bad length encoding */
    /* looks like an id3 header */
    return 1;
  }
  return 0;
}

int isMP3Header(const unsigned char *p, const unsigned char *e)
{
  /* do we have enough room to see a 4 byte header? */
  if (p > e) return 0;
  if (e - p < 4) return 0;
  /* do we have a sync pattern? */
  if (p[0] == 0xff && (p[1]&0xe0) == 0xe0) {
    /* do we have any illegal field values? */
    if (((p[1] & 0x06) >> 1) == 0) return 0; /* no layer 4 */
    if (((p[2] & 0xf0) >> 4) == 15) return 0; /* bitrate can't be 1111 */
    if (((p[2] & 0x0c) >> 2) == 3) return 0; /* samplerate can't be 11 */
    /* looks like a sync'd header */ 
   return 1;
  }
  return 0;
}

int parse(const unsigned char *p, mp3header *header)
{
  const int bitrates[16] = 
	{0,  32000,  40000,  48000,  56000,  64000,  80000,  96000,
         112000, 128000, 160000, 192000, 224000, 256000, 320000, 0};
  const int samplerates[4] = {44100, 48000, 32000};

  header->version = (p[1] & 0x08) >> 3;
  header->layer = 4 - ((p[1] & 0x06) >> 1);
  header->errp = (p[1] & 0x01);

  header->bitrate = bitrates[(p[2] & 0xf0) >> 4];
  header->freq = samplerates[(p[2] & 0x0c) >> 2];
  header->pad = (p[2] & 0x02) >> 1;
  header->priv = (p[2] & 0x01);

  header->mode = (p[3] & 0xc0) >> 6;
  header->modex = (p[3] & 0x30) >> 4;
  header->copyright = (p[3] & 0x08) >> 3;
  header->original = (p[3] & 0x04) >> 2;
  header->emphasis = (p[3] & 0x03);

  return 0;
}

/* calculate the size of an mp3 frame from its header */
int framesize(mp3header *header)
{
  int size;
  int scale;

  if (header->layer == 1) scale = 48;
  else scale = 144;
  
  size = header->bitrate * scale / header->freq;
  /* divide by an extra factor of 2 for MPEG-2? */

  if (header->pad) size += 1;

  return size;
}

int dump_header(mp3header *header)
{
  printf("  version %d layer %d", header->version, header->layer);
  if (header->version == 1 && header->layer == 1)
    printf(" (MPEG-1 layer 2)");
  else if (header->version == 1 && header->layer == 2)
    printf(" (MPEG-1 layer 2)");
  else if (header->version == 1 && header->layer == 3)
    printf(" (MPEG-1 layer 3)");
  printf(" %d kbps %d Hz", header->bitrate/1000, header->freq);
  printf(" %d byte frame", framesize(header));
  printf(" %d copyright", header->copyright);

  printf("\n");

  return 0;
}

int seekMP3Header(char *file)
{
	FILE *mp3file;
	mp3header header;
	int errorLevel = 0, printFrameHeaderOnce = 1;	
	unsigned int file_length, read_length, skip;
	char *buffer, *q, *pointer, *end, fullFilename[PATH_MAX]; 
    char *res = realpath(file, fullFilename);	// realpath constructs the full pathname to the specified file

	if (res == NULL)
	{
        perror("Failure in realpath"); 		// problems with the file?
		errorLevel = NO_FILE_ACCESS;		
		return errorLevel;
	}

	#ifdef DEBUG
		printf("SeekMP3Header, file: %s\n", file);
	#endif 

	mp3file = fopen(fullFilename, "r+b"); 	// open the requested file with mode read and write in binary
	fseek(mp3file,0,SEEK_END);
	file_length = ftell(mp3file);
	fseek(mp3file,0,SEEK_SET);

    if (ferror(mp3file))
	{
        perror("Failure opening file"); 		// problems with the file?
		errorLevel = NO_FILE_ACCESS;		
		return errorLevel;
    }

	// Let's get some header data from the file
	#ifdef DEBUG
		printf("SeekMP3Header, length of file: %d\n",file_length);
	#endif

	buffer = malloc(file_length);
	end = buffer + file_length -2;
		pointer = buffer;
	q = pointer;

	if ((read_length = fread(buffer,sizeof( char ),file_length,mp3file)) < file_length)
	{
		#ifdef DEBUG	
			printf("%d\n",read_length);
		#endif
		return FAILURE;
	}

	while(pointer < end)
	{
		// If we find an ID3 header we have to derive an offset
		if (isID3(pointer, end)) 
		{
		  if (pointer - q) printf("LOST SYNC ID3\n");
		  skip = 10 + (pointer[9] | (pointer[8] << 7) | (pointer[7] << 14) | (pointer[6] << 21));
	
		#ifdef DEBUG
		  printf("SeekMP3Header, id3 header at 0x%08lx (%ld bytes)\n", (long)(pointer-buffer), (long)skip);
		#endif

		  pointer += skip;
		  q = pointer;
		}
		// finally check and possibly parse an mp3 frame header (and derive the offset)
		// skip is the length of the frame
		else if (isMP3Header(pointer,end))
		{
			parse(pointer, &header);
			skip = framesize(&header);	
			if(printFrameHeaderOnce)
			{
				printf("SeekMP3Header, mp3 header at 0x%08lx (%ld bytes)\n", (long)(pointer-buffer), (long)skip);
				printFrameHeaderOnce = 0;				
			}
			#if (DEBUG && VERBOSE)
			  printf("SeekMP3Header, mp3 header at 0x%08lx (%ld bytes)\n", (long)(pointer-buffer), (long)skip);
			#endif
			setCopyright(pointer);	
			pointer += skip;
			q = pointer;
		}
		else
			pointer++;
	}

	fseek(mp3file,0,SEEK_SET);
	fwrite(buffer, sizeof(unsigned char), file_length, mp3file);
	fclose(mp3file);
	free(buffer);
	return 0;
}


/** Tests setting various header values and prints them. Example output above. **/
void testHeaderManipulation()
{
        unsigned int x = 0;
	
        printf("bits  0-10 (11 total): "); showbits_ui( masks[0] );
        printf("bits 11-12 ( 2 total): "); showbits_ui( masks[1] );
        printf("bits 13-14 ( 2 total): "); showbits_ui( masks[2] );
        printf("bits 15    ( 1 total): "); showbits_ui( masks[3] );
        printf("bits 16-19 ( 4 total): "); showbits_ui( masks[4] );
        printf("bits 20-21 ( 2 total): "); showbits_ui( masks[5] );
        printf("bits 22    ( 1 total): "); showbits_ui( masks[6] );
        printf("bits 23    ( 1 total): "); showbits_ui( masks[7] );
        printf("bits 24-25 ( 2 total): "); showbits_ui( masks[8] );
        printf("bits 26-27 ( 2 total): "); showbits_ui( masks[9] );
        printf("bits 28    ( 1 total): "); showbits_ui( masks[10] );
        printf("bits 29    ( 1 total): "); showbits_ui( masks[11] );
        printf("bits 30-31 ( 2 total): "); showbits_ui( masks[12] );
        printf("\n");

        printf("An example:\nSet tag1, value of 2\n");
        setvalue( &x, 1, 2 ); showbits_ui( x );
        printf("Set tag0, value of 300\n"); 
        setvalue( &x, 0, 300 ); showbits_ui( x );
        printf("Set tag0, value of 1\n"); 
        setvalue( &x, 0, 1 ); showbits_ui( x );
		printf("Set tag11, value of 1\n");
		setvalue( &x, 10, 1 ); showbits_ui( x ); 
}
