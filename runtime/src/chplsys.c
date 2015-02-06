/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if defined __CYGWIN__
#include <windows.h>
#endif
#if defined(__APPLE__) || defined(__NetBSD__)
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
#include "chpltypes.h"
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
#elif defined __CYGWIN__
  MEMORYSTATUS status;
  status.dwLength = sizeof(status);
  GlobalMemoryStatus( &status );
  return (uint64_t)status.dwTotalPhys;
  //
  // The following general case used to work for cygwin, but no longer
  // seems to.  Now, it seems to return a very small number of pages
  // for SC_PHYS_PAGES, which I can't explain.  Found the recipe above
  // on the web and it works, so adopting it.
  //
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


#if defined(__linux__) || defined(__NetBSD__)
//
// Return information about the processors on the system.
//
static void getCpuInfo(int* p_numPhysCpus, int* p_numLogCpus) {
  //
  // Currently this is pretty limited -- it only returns the number of
  // physical and logical (hyperthread, e.g.) CPUs, only looks at
  // /proc/cpuinfo, and only supports homogeneous compute nodes with
  // the same number of cores and siblings on every physical CPU.  It
  // will probably need to become more complicated in the future.
  //
  FILE* f;
  char buf[100];
  int procs = 0;
  int cpuCores = 0;
  int siblings = 0;

  if ((f = fopen("/proc/cpuinfo", "r")) == NULL)
    chpl_internal_error("Cannot open /proc/cpuinfo");

  while (!feof(f) && fgets(buf, sizeof(buf), f) != NULL) {
    size_t buf_len = strlen(buf);
    int procTmp;
    int cpuCoresTmp;
    int siblingsTmp;

    if (sscanf(buf, "processor : %i", &procTmp) == 1) {
      procs++;
    }
    else if (sscanf(buf, "cpu cores : %i", &cpuCoresTmp) == 1) {
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
  *p_numPhysCpus = procs / (siblings / cpuCores);
  *p_numLogCpus = procs;
}
#endif


int chpl_getNumPhysicalCpus(chpl_bool accessible_only) {
  //
  // Support for the accessible_only flag here is spotty.  For non-Linux
  // systems we ignore it.  For Linux systems we obey it, but we may
  // return an inaccurate result (see the commentary there for details).
  //
#ifdef NO_CORES_PER_LOCALE
  return 1;
#elif defined __APPLE__
  //
  // Apple
  //
  static int32_t numCpus = 0;
  if (numCpus == 0) {
    size_t len = sizeof(numCpus);
    if (sysctlbyname("hw.physicalcpu", &numCpus, &len, NULL, 0))
      chpl_internal_error("query of number of cores failed");
  }
  return (int) numCpus;
#elif defined __CYGWIN__
  //
  // Cygwin
  //
  static int numCpus = 0;
  if (numCpus == 0)
    numCpus = chpl_getNumLogicalCpus(true);
  return numCpus;
#elif defined(__linux__) || defined(__NetBSD__)
  //
  // Linux
  //
  static int numPhysCpus = 0;
  static int numLogCpus = 0;
  if (numPhysCpus == 0)
    getCpuInfo(&numPhysCpus, &numLogCpus);

  if (accessible_only) {
    static int numCpus = 0;

    if (numCpus == 0) {
#if defined __MIC__
      //
      // On Intel MIC, we seem (for now at least) not to have kernel
      // scheduling affinity information.
      //
      numCpus = numPhysCpus;
#elif defined __NetBSD__
      numCpus = numPhysCpus;
#else
      //
      // The accessibility information is with respect to logical
      // CPUs.  Assume we have accessibility to the physical CPUs in
      // the same ratio that we do to the logical ones.  This may not
      // be true; if our accessibility is to one hyperthread of the
      // two on each of several cores, for example, then the ratio of
      // accessible hyperthreads is 0.5 but the ratio of accessible
      // cores is 1.0.  For now we live with this.
      //
      cpu_set_t m;
      int numLogCpusAcc;

      if (sched_getaffinity(getpid(), sizeof(cpu_set_t), &m) != 0)
        chpl_internal_error("sched_getaffinity() failed");
      numLogCpusAcc = CPU_COUNT(&m);
      if (numLogCpusAcc == 0)
        numLogCpusAcc = 1;
      numCpus = (numPhysCpus * numLogCpusAcc) / numLogCpus;
#endif
    }
    return numCpus;
  }
  else {
    static int numCpus = 0;

    if (numCpus == 0)
      numCpus = numPhysCpus;
    return numCpus;
  }
#else
#warning "Target architecture is not yet supported."
  return 1;
#endif
}


int chpl_getNumLogicalCpus(chpl_bool accessible_only) {
  //
  // Support for the accessible_only flag here is spotty -- we only obey
  // it for Linux systems.
  //
#ifdef NO_CORES_PER_LOCALE
  return 1;
#elif defined __APPLE__
  //
  // Apple
  //
  static int32_t numCpus = 0;
  if (numCpus == 0) {
    size_t len = sizeof(numCpus);
    if (sysctlbyname("hw.logicalcpu", &numCpus, &len, NULL, 0))
      chpl_internal_error("query of number of PUs failed");
  }
  return (int) numCpus;
#elif defined __CYGWIN__
  //
  // Cygwin
  //
  // WARNING: This has not yet been tested.
  //
  static int numCpus = 0;
  if (numCpus == 0)
    numCpus = sysconf(_SC_NPROCESSORS_ONLN);
  return numCpus;
#elif defined(__linux__) || defined(__NetBSD__)
  //
  // Linux
  //
  static int numPhysCpus = 0;
  static int numLogCpus = 0;
  if (numPhysCpus == 0)
    getCpuInfo(&numPhysCpus, &numLogCpus);

  if (accessible_only) {
    static int numCpus = 0;

    if (numCpus == 0) {
#if defined __MIC__
      //
      // On Intel MIC, we seem (for now at least) not to have kernel
      // scheduling affinity information.
      //
      numCpus = numLogCpus;
#elif defined __NetBSD__
      numCpus = numLogCpus;
#else
      cpu_set_t m;
      int numLogCpusAcc;

      if (sched_getaffinity(getpid(), sizeof(cpu_set_t), &m) != 0)
        chpl_internal_error("sched_getaffinity() failed");
      numLogCpusAcc = CPU_COUNT(&m);
      if (numLogCpusAcc == 0)
        numLogCpusAcc = 1;
      numCpus = (numLogCpusAcc < numLogCpus) ? numLogCpusAcc : numLogCpus;
#endif
    }
    return numCpus;
  }
  else {
    static int numCpus = 0;

    if (numCpus == 0)
      numCpus = numLogCpus;
    return numCpus;
  }
#else
#warning "Target architecture is not yet supported."
  return 1;
#endif
}


// Using a static buffer is a bad idea from the standpoint of thread-safety.
// However, since the node name is not expected to change it is OK to
// initialize it once and share the singleton string.
// There could still be a race condition concerning which thread actually gets
// to initialize the static, but since two or more should end up writing the
// same bytes, it probably just works out.
c_string chpl_nodeName(void) {
  static char* namespace = NULL;
  if (namespace == NULL)
  {
    struct utsname utsinfo;
    int namelen;

    uname(&utsinfo);
    namelen = strlen(utsinfo.nodename)+1;
    namespace = chpl_mem_realloc(namespace, namelen * sizeof(char), 
                                 CHPL_RT_MD_LOCALE_NAME_BUFFER, 0, NULL);
    strcpy(namespace, utsinfo.nodename);
  }
  return namespace;
}
