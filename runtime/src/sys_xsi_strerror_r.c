/*
 * Since GNU environments like to define their own
 * non-portable strerror_r, we need a separate
 * source file that does not define _GNU_SOURCE
 * in order to get the portable one.
 */

#define _POSIX_C_SOURCE 20112L
#define _XOPEN_SOURCE 600
#undef _GNU_SOURCE

#include <string.h>
#include <errno.h>

#ifdef __MTA__
// no strerror_r
#else
#define HAS_STRERROR_R
#endif

int sys_xsi_strerror_r(int errnum, char* buf, size_t buflen);


int sys_xsi_strerror_r(int errnum, char* buf, size_t buflen)
{
#ifdef HAS_STRERROR_R
  return strerror_r(errnum, buf, buflen);
#else
  char* msg = strerror(errnum);
  if( strlen(msg) + 1 < buflen ) {
   strcpy(buf, msg);
   return 0;
  }
  return ERANGE;
#endif
}

#if 0
// Squelch the warning about not including the chplrt; not necessary here
#include "chplrt.h"
#endif

