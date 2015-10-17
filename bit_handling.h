#ifndef BIT_HANDLING_H
#define BIT_HANDLING_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int endian_swap(unsigned int *x);
void showbits_ui( unsigned int bits );
void showbits_uc( unsigned char bits );
void showbits_ucv( const unsigned char *bits, unsigned int length );
int extractBit(char byte, int pos);
char extractBitRange(char byte, int startingPos, int offset);

#endif
