#ifndef EDITMP3_H
#define EDITMP3_H

/** Functions to edit mp3 **/

#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include "bit_handling.h"
#include "essentials.h"


/*
bits  0-10 (11 total): 11111111111000000000000000000000
bits 11-12 ( 2 total): 00000000000110000000000000000000
bits 13-14 ( 2 total): 00000000000001100000000000000000
bits 15    ( 1 total): 00000000000000010000000000000000
bits 16-19 ( 4 total): 00000000000000001111000000000000
bits 20-21 ( 2 total): 00000000000000000000110000000000
bits 22    ( 1 total): 00000000000000000000001000000000
bits 23    ( 1 total): 00000000000000000000000100000000
bits 24-25 ( 2 total): 00000000000000000000000011000000
bits 26-27 ( 2 total): 00000000000000000000000000110000
bits 28    ( 1 total): 00000000000000000000000000001000
bits 29    ( 1 total): 00000000000000000000000000000100
bits 30-31 ( 2 total): 00000000000000000000000000000011

An example:
Set tag1, value of 2
00000000000010000000000000000000
Set tag0, value of 300
00110100100010000000000000000000
Set tag0, value of 1
10000000000010000000000000000000
*/

typedef enum mp3h_tags { MP3H_FRAMESYNC, MP3HM_PEGID, MP3H_LAYER,
        MP3H_PROTECTBIT, MP3H_BITRATEINDX, MP3H_SAMPLEFREQ, MP3H_PADDINGBIT,
        MP3H_PRIVATEBIT, MP3H_CHANNEL, MP3H_MODEEXT, MP3H_COPYRIGHT,
        MP3H_ORIGINAL, MP3H_EMPHASIS } mp3t_t;

typedef struct {
  int version;
  int layer;
  int errp;
  int bitrate;
  int freq;
  int pad;
  int priv;
  int mode;
  int modex;
  int copyright;
  int original;
  int emphasis;
} mp3header;


extern unsigned int shifts[];
extern unsigned int masks[];


void setvalue( unsigned int *mp3header, mp3t_t tag, unsigned int value );
void resetvalue( unsigned int *mp3header );
void printMP3Header(unsigned int *mp3header);
int seekMP3Header(char *file);
int isMp3Header(const char *bytes);
int isMP3Header(const unsigned char *p, const unsigned char *e);

// For testing purposes
void testHeaderManipulation();

#endif

