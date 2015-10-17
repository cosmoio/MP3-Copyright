#include "bit_handling.h"

/** Prints the bits of an unsigned int **/
void showbits_ui( unsigned int bits )
{
        int x;
        for( x =0; x < sizeof bits << 3; x++ )
                printf("%d", !!(bits  & (1<<x)) );
        printf("\n");
}

/** Prints the bits of an unsigned char **/
void showbits_uc( const unsigned char bits )
{
        int x;
        for( x =0; x < sizeof bits << 3; x++ )
                printf("%d", !!(bits  & (1<<x)) );
        printf("\n");
}

void showbits_ucv( const unsigned char *bits, unsigned int length )
{
        int x;
		int y;
		for( y = 0; y < length; y++)
	        for( x =0; x < sizeof bits[y] << 3; x++ )
                printf("%d", !!(bits[y]  & (1<<x)) );
        printf("\n");
}

int endian_swap(unsigned int *x)
{
    *x = (*x>>24) | 
        ((*x<<8) & 0x00FF0000) |
        ((*x>>8) & 0x0000FF00) |
        (*x<<24);
	return *x;
}

int extractBit(char byte, int pos) 
{
    assert( (pos >= 0) && (pos < 8) );
    return ( ( byte & (1<<pos) ) >> pos);
}


char extractBitRange(char byte, int startingPos, int offset) 
{
   assert( ( (startingPos + offset) >= 0) && ( (startingPos + offset) < 8) );
   return ( byte >> startingPos ) & ~(0xff << (offset + 1));
}

