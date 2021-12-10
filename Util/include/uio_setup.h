/*
 * $Id: 
 *-
 *-   Purpose and Methods: A generic interface to find and open a userspace I/O device.
 *-
 *-   Inputs  : The name of the UIO device to open
 *-   Outputs : The address of the data block, or null if the open failed
 *-   Controls:
 *-
 *-   Created  14-APR-2021   John D. Hobbs
 *- $Revision: 1.3 $
 *-
*/
#ifdef __cplusplus
extern "C" {
#endif

  void* uio_setup(const char *udevname);

#ifdef __cplusplus
}
#endif
