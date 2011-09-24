#define _POSIX_C_SOURCE 20112L
#define _XOPEN_SOURCE 600
#undef _GNU_SOURCE

#include <string.h>

int sys_xsi_strerror_r(int errnum, char* buf, size_t buflen);

int sys_xsi_strerror_r(int errnum, char* buf, size_t buflen)
{
  return strerror_r(errnum, buf, buflen);
}

#ifndef SIMPLE_TEST
// Squelch the warning about not includnig chplrt.
#include "chplrt.h"
#endif

