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
  _printInternalError("sorry- bytesPerLocale not supported on this platform");
  return 0;
#else
#ifdef APPLE
  _uint64 membytes;
  size_t len = sizeof(membytes);
  if (sysctlbyname("hw.memsize", &membytes, &len, NULL, 0)) 
    _printInternalError("query of physical memory failed");
  return (membytes);
#else
  _uint64 numPages = sysconf(_SC_PHYS_PAGES);
  _uint64 pageSize = chplGetPageSize();
  _uint64 total = numPages * pageSize;
  return total;
#endif
#endif
}


_int32 _coresPerLocale(void) {
#ifdef NO_CORES_PER_LOCALE
  return 1;
#else
#ifdef APPLE
  _uint64 numcores;
  size_t len = sizeof(numcores);
  if (sysctlbyname("hw.physicalcpu", &numcores, &len, NULL, 0))
    _printInternalError("query of number of cores failed");
  return (_int32)numcores;
#else
  _int32 numcores = (_int32)sysconf(_SC_NPROCESSORS_ONLN);
  return numcores;
#endif
#endif
}
