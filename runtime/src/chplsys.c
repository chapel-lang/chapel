#if defined __APPLE__
#include <sys/sysctl.h>
#endif
#if defined _AIX
#include <sys/systemcfg.h>
#endif
#ifdef __linux__
#define _GNU_SOURCE
#endif
#include <sys/utsname.h>
#include "chplrt.h"

#include "chpl-mem.h"
#include "chplsys.h"
#include "chpl-tasks.h"
#include "chpl-comm.h"
#include "error.h"

#ifdef __linux__
#include <sched.h>
#endif
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#ifdef __linux__
#include <sys/sysinfo.h>
#endif
#include <errno.h>
#include <unistd.h>


#ifndef chplGetPageSize
#define chplGetPageSize() sysconf(_SC_PAGE_SIZE)
#endif

uint64_t chpl_bytesPerLocale(void) {
#ifdef NO_BYTES_PER_LOCALE
  chpl_internal_error("sorry- bytesPerLocale not supported on this platform");
  return 0;
#elif defined __APPLE__
  uint64_t membytes;
  size_t len = sizeof(membytes);
  if (sysctlbyname("hw.memsize", &membytes, &len, NULL, 0)) 
    chpl_internal_error("query of physical memory failed");
  return membytes;
#elif defined _AIX
  return _system_configuration.physmem;
#elif defined __NetBSD__
  uint64_t membytes;
  size_t len = sizeof(membytes);
  if (sysctlbyname("hw.usermem", &membytes, &len, NULL, 0))
    chpl_internal_error("query of physical memory failed");
  return membytes;
#else
  long int numPages, pageSize;
  numPages = sysconf(_SC_PHYS_PAGES);
  if (numPages < 0)
    chpl_internal_error("query of physical memory failed");
  pageSize = chplGetPageSize();
  if (pageSize < 0)
    chpl_internal_error("query of physical memory failed");
  return (uint64_t)numPages * (uint64_t)pageSize;
#endif
}


size_t chpl_bytesAvailOnThisLocale(void) {
#if defined __APPLE__
  int membytes;
  size_t len = sizeof(membytes);
  if (sysctlbyname("hw.usermem", &membytes, &len, NULL, 0)) 
    chpl_internal_error("query of physical memory failed");
  return (size_t) membytes;
#elif defined __NetBSD__
  int64_t membytes;
  size_t len = sizeof(membytes);
  if (sysctlbyname("hw.usermem64", &membytes, &len, NULL, 0))
    chpl_internal_error("query of hw.usermem64 failed");
  return (size_t) membytes;
#elif defined(__linux__)
  struct sysinfo s;

  if (sysinfo(&s) != 0)
    chpl_internal_error("sysinfo() failed");
  return (size_t) s.freeram;
#else
  chpl_internal_error("bytesAvailOnThisLocale not supported on this platform");
  return 0;
#endif
}


#ifdef __linux__
static int getPUsPerCore(void) {
  FILE* f;
  char buf[100];
  int cpuCores = 0;
  int siblings = 0;

  if ((f = fopen("/proc/cpuinfo", "r")) == NULL)
    chpl_internal_error("Cannot open /proc/cpuinfo");

  while (!feof(f) && fgets(buf, sizeof(buf), f) != NULL) {
    size_t buf_len = strlen(buf);
    int cpuCoresTmp;
    int siblingsTmp;

    //
    // Currently this only supports homogeneous compute nodes, with
    // the same number of cores and siblings on every physical CPU.
    // It will probably need to become more complicated in the future.
    //
    if (sscanf(buf, "cpu cores : %i", &cpuCoresTmp) == 1) {
      if (cpuCores == 0)
        cpuCores = cpuCoresTmp;
      else if (cpuCoresTmp != cpuCores)
        chpl_internal_error("varying number of cpu cores");
    }
    else if (sscanf(buf, "siblings : %i", &siblingsTmp) == 1) {
      if (siblings == 0)
        siblings = siblingsTmp;
      else if (siblingsTmp != siblings)
        chpl_internal_error("varying number of siblings");
    }

    while (buf[buf_len - 1] != '\n' && fgets(buf, sizeof(buf), f) != NULL)
      buf_len = strlen(buf);
  }

  fclose(f);

  if (cpuCores == 0)
    cpuCores = 1;
  if (siblings == 0)
    siblings = 1;
  return siblings / cpuCores;
}
#endif


int chpl_getNumPUsOnThisNode(void) {
#ifdef NO_CORES_PER_LOCALE
  return 1;
#elif defined __APPLE__
  //
  // Apple
  //
  static int32_t numPUs = 0;
  if (numPUs == 0) {
    size_t len = sizeof(numPUs);
    if (sysctlbyname("hw.logicalcpu", &numPUs, &len, NULL, 0))
      chpl_internal_error("query of number of PUs failed");
  }
  return (int) numPUs;
#elif defined __CYGWIN__
  //
  // Cygwin
  //
  // WARNING: This has not yet been tested.
  //
  static int numPUs = 0;
  if (numPUs == 0)
    numPUs = sysconf(_SC_NPROCESSORS_ONLN);
  return numPUs;
#elif defined __linux__
  //
  // Linux
  //
  static int numPUs = 0;
  if (numPUs == 0) {
    cpu_set_t m;
    if (sched_getaffinity(getpid(), sizeof(cpu_set_t), &m) != 0)
      chpl_internal_error("sched_getaffinity() failed");
    if ((numPUs = CPU_COUNT(&m)) == 0)
      numPUs = 1;
  }
  return numPUs;
#else
#warning "Target architecture is not yet supported."
  return 1;
#endif
}


int chpl_getNumCoresOnThisNode(void) {
#ifdef NO_CORES_PER_LOCALE
  return 1;
#elif defined __APPLE__
  //
  // Apple
  //
  static int32_t numCores = 0;
  if (numCores == 0) {
    size_t len = sizeof(numCores);
    if (sysctlbyname("hw.physicalcpu", &numCores, &len, NULL, 0))
      chpl_internal_error("query of number of cores failed");
  }
  return (int) numCores;
#elif defined __CYGWIN__
  //
  // Cygwin
  //
  static int numCores = 0;
  if (numCores == 0)
    numCores = chpl_getNumPUsOnThisNode();
  return numCores;
#elif defined __linux__
  //
  // Linux
  //
  static int numCores = 0;
  if (numCores == 0)
    numCores = chpl_getNumPUsOnThisNode() / getPUsPerCore();
  return numCores;
#else
#warning "Target architecture is not yet supported."
  return 1;
#endif
}


c_string chpl_nodeName(void) {
  static char* namespace = NULL;
  static int namelen = 0;
  struct utsname utsinfo;
  int newnamelen;
  uname(&utsinfo);
  newnamelen = strlen(utsinfo.nodename)+1;
  if (newnamelen > namelen) {
    namelen = newnamelen;
    namespace = chpl_mem_realloc(namespace, newnamelen * sizeof(char), 
                                 CHPL_RT_MD_LOCALE_NAME_BUFFER, 0, NULL);
  }
  strcpy(namespace, utsinfo.nodename);
  return namespace;
}
