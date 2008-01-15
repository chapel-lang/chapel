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
#elif defined APPLE
  _uint64 membytes;
  size_t len = sizeof(membytes);
  if (sysctlbyname("hw.memsize", &membytes, &len, NULL, 0)) 
    _printInternalError("query of physical memory failed");
  return membytes;
#elif defined __MTA__  // The following code may cause the MTA (to seem) to hang!
  int mib[2] = {CTL_HW, HW_PHYSMEM}, membytes;
  size_t len = sizeof(membytes);
  sysctl(mib, 2, &membytes, &len, NULL, 0);
  return (_uint64)membytes;
#else
  _uint64 numPages = sysconf(_SC_PHYS_PAGES);
  _uint64 pageSize = chplGetPageSize();
  _uint64 total = numPages * pageSize;
  return total;
#endif
}


_int32 _coresPerLocale(void) {
#ifdef NO_CORES_PER_LOCALE
  return 1;
#elif defined APPLE
  _uint64 numcores;
  size_t len = sizeof(numcores);
  if (sysctlbyname("hw.physicalcpu", &numcores, &len, NULL, 0))
    _printInternalError("query of number of cores failed");
  return (_int32)numcores;
#elif defined __MTA__  // The following code may cause the MTA (to seem) to hang!
  int mib[2] = {CTL_HW, HW_NCPU}, numcores;
  size_t len = sizeof(numcores);
  sysctl(mib, 2, &numcores, &len, NULL, 0);
  return (_int32)numcores;
#else
  _int32 numcores = (_int32)sysconf(_SC_NPROCESSORS_ONLN);
  return numcores;
#endif
}


_int32 _maxThreads(void) {
#ifdef __MTA__
  return _coresPerLocale * 100;
#else
  _int32 maxThreads = _coresPerLocale() - 1;
  return maxThreads <= 3 ? 3 : maxThreads;
#endif
}
