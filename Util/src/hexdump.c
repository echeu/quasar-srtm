/*
*-
*-   Purpose and Methods: Write the contents of buffer to stdout in a
*-     vax-style hex dump.
*-
*-   Inputs  :
*-   Outputs :
*-   Controls:
*-
*-   Created  10-Mar-1994   John D. Hobbs
*-
*/

#include <stdio.h>
#include <math.h>
#include <errno.h>
#include "hexdump.h"

/* #include "xtypes.h" */

char Isprint( letter ) 
register char letter;
{
  if( (letter>126) || (letter<32) ) return( 0 );
  else return( letter );        
}

void hexdump( const u8 *buff, int len) {
  u16  i,j;
  u8   temp;
  int     loc;

  for( i=0 ; i<(len/16) ; i++ ) {
    printf("%04x:  ",16*i );
    for( j=0 ; j<16 ; j++ ) {
      if( j%4 == 0 ) printf(" ");
      temp = buff[16*i + j] ;
      printf( "%02x",temp );
    }
    printf("   ");
    for( j=0 ; j<16 ; j++ ) {
      loc = 16*i + j;
      if( buff[loc]>126 || buff[loc]<32 ) printf(".");                      
      else printf( "%c",buff[loc] );
    }
    printf("\n");
  }
  if( (len%16) == 0 ) {
    printf("\n\n");                
    return;
  }

  /* Print any remaining elements */
  printf("%04x:  ",16*i );
  for( j=0 ; j<(len%16) ; j++ ) {
    if( j%4 == 0 ) printf(" ");
    temp = buff[16*i + j] ;
    printf( "%02x",temp );
  }
  for( j=0 ; j<16 ; j++ ) {
    if( j%4 == 0 ) printf(" ");
    printf("  ");
  }
  printf("   ");
  for( j=0 ; j<(len%16) ; j++ ) {
    if( (Isprint( buff[16*i + j] ) )==0 ) printf(".");                      
    else printf( "%c",buff[16*i + j] );
  }
  printf("\n");
}

