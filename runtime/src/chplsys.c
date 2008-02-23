#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#if defined __APPLE__ || defined __MTA__
#include <sys/sysctl.h>
#endif
#include "chplrt.h"
#include "chplsys.h"
#include "chplthreads.h"
#include "chplcomm.h"
#include "error.h"

#ifndef chplGetPageSize
#define chplGetPageSize() sysconf(_SC_PAGE_SIZE)
#endif

uint64_t _bytesPerLocale(void) {
#ifdef NO_BYTES_PER_LOCALE
  _chpl_internal_error("sorry- bytesPerLocale not supported on this platform");
  return 0;
#elif defined __APPLE__
  uint64_t membytes;
  size_t len = sizeof(membytes);
  if (sysctlbyname("hw.memsize", &membytes, &len, NULL, 0)) 
    _chpl_internal_error("query of physical memory failed");
  return membytes;
#elif defined __MTA__
  int mib[2] = {CTL_HW, HW_PHYSMEM}, membytes;
  size_t len = sizeof(membytes);
  sysctl(mib, 2, &membytes, &len, NULL, 0);
  return membytes;
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
    _chpl_internal_error("query of number of cores failed");
  // If the call to sysctlbyname() above failed to modify numcores for some reason,
  // return just 1 to be safe.
  return numcores ? numcores : 1;
#elif defined __MTA__
  int mib[2] = {CTL_HW, HW_NCPU}, numcores;
  size_t len = sizeof(numcores);
  sysctl(mib, 2, &numcores, &len, NULL, 0);
  return numcores;
#else
  int32_t numcores = (int32_t)sysconf(_SC_NPROCESSORS_ONLN);
  return numcores;
#endif
}


//
// returns default values of max threads set according to the
// communication and threading runtimes; if neither set a max, then 0
// is returned; otherwise the minimum max is returned.
//
int32_t _maxThreads(void) {
  int32_t comm_max = _chpl_comm_getMaxThreads();
  int32_t threads_max = _chpl_threads_getMaxThreads();

  if (comm_max == 0)
    return threads_max;
  else if (threads_max == 0)
    return comm_max;
  else if (comm_max < threads_max)
    return comm_max;
  else
    return threads_max;
}
