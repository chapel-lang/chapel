#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#ifdef __APPLE__
#include <sys/sysctl.h>
#endif
#include "chplrt.h"
#include "chplsys.h"
#include "error.h"

#ifndef chplGetPageSize
#define chplGetPageSize() sysconf(_SC_PAGE_SIZE)
#endif

uint64_t _bytesPerLocale(void) {
#ifdef NO_BYTES_PER_LOCALE
  _printInternalError("sorry- bytesPerLocale not supported on this platform");
  return 0;
#elif defined __APPLE__
  uint64_t membytes;
  size_t len = sizeof(membytes);
  if (sysctlbyname("hw.memsize", &membytes, &len, NULL, 0)) 
    _printInternalError("query of physical memory failed");
  return membytes;
#elif defined __MTA__
  int mib[2] = {CTL_HW, HW_PHYSMEM}, membytes;
  size_t len = sizeof(membytes);
  sysctl(mib, 2, &membytes, &len, NULL, 0);
  return (uint64_t)membytes;
#else
  uint64_t numPages = sysconf(_SC_PHYS_PAGES);
  uint64_t pageSize = chplGetPageSize();
  uint64_t total = numPages * pageSize;
  return total;
#endif
}


int32_t _coresPerLocale(void) {
#ifdef NO_CORES_PER_LOCALE
  return 1;
#elif defined __APPLE__
  uint64_t numcores = 0;
  size_t len = sizeof(numcores);
  if (sysctlbyname("hw.physicalcpu", &numcores, &len, NULL, 0))
    _printInternalError("query of number of cores failed");
  // If the call to sysctlbyname() above failed to modify numcores for some reason,
  // return just 1 to be safe.
  return numcores ? (int32_t)numcores : 1;
#elif defined __MTA__
  int mib[2] = {CTL_HW, HW_NCPU}, numcores;
  size_t len = sizeof(numcores);
  sysctl(mib, 2, &numcores, &len, NULL, 0);
  return (int32_t)numcores;
#else
  int32_t numcores = (int32_t)sysconf(_SC_NPROCESSORS_ONLN);
  return numcores;
#endif
}


int32_t _maxThreads(void) {
#ifdef __MTA__
  return _coresPerLocale() * 100;
#else
  int32_t maxThreads = _coresPerLocale() - 1;
  return maxThreads <= 3 ? 3 : maxThreads;
#endif
}
