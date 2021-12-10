#ifndef HEXDUMP_H
#define HEXDUMP_H
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

#include "xtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

void hexdump( u8 *buff, int len );

#ifdef __cplusplus
}
#endif 

#endif
