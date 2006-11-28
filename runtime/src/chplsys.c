#include <stdio.h>
#include <unistd.h>
#include "chplrt.h"
#include "chplsys.h"

#ifndef chplGetPageSize
#define chplGetPageSize() sysconf(_SC_PAGE_SIZE)
#endif

_uint64 _bytesPerLocale(void) {
  _uint64 numPages = sysconf(_SC_PHYS_PAGES);
  _uint64 pageSize = chplGetPageSize();
  _uint64 total = numPages * pageSize;
  return total;
}
