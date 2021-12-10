/*
  This is a replacement for the old xil_types.h which defines shorthand for 
  various types which we have all over the code from the legacy of xil_kernel.
 */
/**************************** Type Definitions *******************************
 * These probably should be somewhere else, e.g. the old xiltypes.h          */
typedef unsigned char   Xuint8;
typedef char            Xint8;      /**< signed 8-bit */
typedef unsigned short  Xuint16;    /**< unsigned 16-bit */
typedef short           Xint16;     /**< signed 16-bit */
typedef unsigned long   Xuint32;    /**< unsigned 32-bit */
typedef long            Xint32;     /**< signed 32-bit */
typedef float           Xfloat32;   /**< 32-bit floating point */
typedef double          Xfloat64;   /**< 64-bit double precision floating point */
typedef unsigned long   Xboolean;   /**< boolean (XTRUE or XFALSE) */

/* And these are the old xil_types.h equivalents */
typedef unsigned char   u8;
typedef char            s8;      /**< signed 8-bit */
//typedef char            i8;      /**< signed 8-bit */
typedef unsigned short  u16;    /**< unsigned 16-bit */
typedef short           s16;     /**< signed 16-bit */
//typedef short           i16;     /**< signed 16-bit */
typedef unsigned int    u32;    /**< unsigned 32-bit */
typedef int             s32;     /**< signed 32-bit */
//typedef int             i32;     /**< signed 32-bit */

#define TRUE 1
#define FALSE 0
