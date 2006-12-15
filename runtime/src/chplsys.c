#include <stdio.h>
#include <unistd.h>
#include "chplrt.h"
#include "chplsys.h"
#include "error.h"

#ifndef chplGetPageSize
#define chplGetPageSize() sysconf(_SC_PAGE_SIZE)
#endif

_uint64 _bytesPerLocale(void) {
#ifdef NO_BYTES_PER_LOCALE
  printInternalError("sorry- bytesPerLocale not supported on this platform");
  return 0;
#else
  _uint64 numPages = sysconf(_SC_PHYS_PAGES);
  _uint64 pageSize = chplGetPageSize();
  _uint64 total = numPages * pageSize;
  return total;
#endif
}
