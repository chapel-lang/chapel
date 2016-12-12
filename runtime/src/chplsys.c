/*
 * Copyright 2004-2016 Cray Inc.
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

// This #define needs to be before the other #includes
// since it affects included files
#ifdef __linux__
#define _GNU_SOURCE
#endif

#include "chplrt.h"
#include "chplsys.h"

// Other Chapel Header
#include "chpl-align.h"
#include "chpl-comm.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"
#include "chpltypes.h"
#include "error.h"

// System headers
#include <errno.h>

#ifdef __linux__
#include <pthread.h>
#include <sched.h>
#endif

#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/utsname.h>

#if defined(__APPLE__) || defined(__NetBSD__)
#include <sys/sysctl.h>
#endif

#ifdef __linux__
#include <sys/sysinfo.h>
#endif

#if defined _AIX
#include <sys/systemcfg.h>
#endif

#include <unistd.h>

#if defined __CYGWIN__
#include <windows.h>
#endif


size_t chpl_getSysPageSize(void) {
  static size_t pageSize = 0;

  if (pageSize == 0) {
    long int ps;
#if defined _SC_PAGESIZE
    ps = sysconf(_SC_PAGESIZE);
#elif defined _SC_PAGE_SIZE
    ps = sysconf(_SC_PAGE_SIZE);
#else
    chpl_internal_error("cannot determine page size");
#endif
    if (ps <= 0L)
      chpl_internal_error("system page size must be positive");
    pageSize = (size_t) ps;
  }

  return pageSize;
}

static size_t heapPageSize = 0;

size_t chpl_getHeapPageSize(void) {
  return heapPageSize;
}

static volatile unsigned char* check_page_size_ptr;
static unsigned char* check_page_size_base;
static size_t check_page_size_guess;
static unsigned char check_page_size_fault;
static struct sigaction check_page_size_oldact;

static
void check_page_size_segv_handler(int signum, siginfo_t* sig, void* ctx)
{
  if (sig->si_addr == (void*) check_page_size_ptr) {
    check_page_size_fault = true;
    // Re-enable read+write access so the program continues
    // if the load/store is restarted
    mprotect((void*)check_page_size_base,
             check_page_size_guess,
             PROT_READ | PROT_WRITE);
  } else {
    // Run the old handler
    if ((check_page_size_oldact.sa_flags & SA_SIGINFO))
      check_page_size_oldact.sa_sigaction(signum, sig, ctx);
    else
      check_page_size_oldact.sa_handler(signum);
  }
}

// Given a guess at a page size, use mprotect and a signal handler
// to determine if the page size guess is too small.
//
// Returns 0 if the real page size is <= page_size_guess
// Returns 1 if the real page size is > page_size_guess
// Returns -1 if the call to mprotect failed
// Returns -2 if the signal handler could not be installed
static
int check_page_size(size_t page_size_guess, size_t max_page_size,
                     unsigned char* ptr,
                     void* heap_start, size_t heap_size)
{
  int rc;
  int mprotect_failed = 0;
  const int debug = 0; // set to 1 for debug prints

  if (debug)
    printf("check_page_size(%li, %li, %p, %p, %li)\n",
           (long) page_size_guess, (long) max_page_size,
           ptr, heap_start, (long) heap_size);

  // set signal handler variables
  check_page_size_ptr = ptr + page_size_guess;
  check_page_size_base = ptr;
  check_page_size_guess = page_size_guess;
  check_page_size_fault = 0;

  // install signal handler
  {
    struct sigaction act = { .sa_sigaction = check_page_size_segv_handler,
                             .sa_flags = SA_SIGINFO };
    // Note: if this code is to work on Mac OS X, we will also need
    // to install the signal handler for SIGBUS.
    rc = sigaction(SIGSEGV, &act, &check_page_size_oldact);
    if (rc!=0)
      return -2;
  }

  // Store 0 to avoid false positives:
  // make sure the page exists
  *check_page_size_ptr = 0;

  // Call mprotect
  // Note that mprotect with huge pages seems to have extra requirements
  // on linux. In particular, on linux, the len argument needs to also
  // be a multiple of the huge page size. Mac OS X does not seem to have
  // this requirement.
  rc = mprotect((void*)check_page_size_base, check_page_size_guess, PROT_NONE);
  if (rc!=0) {
    if (debug) printf("mprotect failed\n");
    mprotect_failed = 1;
  } else {
    if (debug) printf("mprotect success\n");
    // This store operation might cause a SEGV that check_page_size_segv_handler
    // wil handle
    *check_page_size_ptr = 99;

    // Put the memory protection back
    rc = mprotect((void*) check_page_size_base, check_page_size_guess,
                  PROT_READ | PROT_WRITE);
    if (rc!=0)
      chpl_internal_error("check_page_size failed in final mprotect call");

    if (debug) printf("mprotect restore success\n");
  }

  // un-install the signal handler
  rc = sigaction(SIGSEGV, &check_page_size_oldact, NULL);
  if (rc!=0)
    chpl_internal_error("check_page_size failed in final sigaction call");

  if (debug)
    printf("check_page_size_fault is %i\n", (int) check_page_size_fault);

  // set the value to 0 for two reasons:
  // 1: 0 is the normal default for uninitialized memory
  // 2: if there is any error when writing to it after this
  //    experiment, we want to know that now
  *check_page_size_ptr = 0;

  if (mprotect_failed) return -1;
  else return check_page_size_fault;
}

// Given a best-guess page size, sets the global variable
// heapPageSize by doing experiments with mprotect.
// In the case that mprotect does not work on that memory region,
// page_size_in will be used as the best guess.
static void computeHeapPageSizeByGuessing(size_t page_size_in)
{
  // check desired shared heap page size

  void* heap_start = NULL;
  size_t heap_size = 0;

  // If nothing else in this function works, at least we should
  // start with page_size_in as the guess for the size of pages
  // in the heap.
  heapPageSize = page_size_in;

  chpl_comm_desired_shared_heap(&heap_start, &heap_size);

  if (heap_start != NULL && heap_size != 0) {

    size_t page_size = page_size_in;
    unsigned char* start = heap_start;
    unsigned char* ptr = start;
    unsigned char* best_ptr = ptr;
    size_t max_page_size = page_size;
    int small;

    // Compute the most aligned page fully enclosed within
    // start and size. We compute the "most aligned page"
    // because we don't know what the actual page size is.
    // In particular, page_size_in might be smaller than the
    // real page size. At the same time, the mprotect call
    // in check_page_size won't work unless the pointer argument
    // is aligned to the real page size. Which we don't know yet.
    while(1) {
      ptr = round_up_to_mask_ptr(ptr, page_size - 1);
      // make sure underflow has not occurred
      if (ptr - start < 0)
        break;
      // make sure that the new page size fits into
      // the bounds of the heap, including the 1st byte
      // of the next page
      if (ptr - start + 1 + page_size > heap_size)
        break;

      best_ptr = ptr;
      max_page_size = page_size;

      page_size = 2*page_size;
    }

    page_size = page_size_in;
    while(1) {
      small = check_page_size(page_size, max_page_size,
                              best_ptr, heap_start, heap_size);

      // Stop if real page size <= page_size, or if
      // a signal handler could not be installed.
      if (small == 0 || small == -2) {
        heapPageSize = page_size;
        break;
      }
      // Stop if we have reached the maximum size
      if (page_size >= max_page_size) {
        heapPageSize = page_size_in;
        break;
      }
      // Otherwise continue the search
      // if small == 1, we know we are wrong
      // if small == -1, mprotect failed, but that might be because
      // we haven't yet found a page size big enough, so keep trying.
      page_size *= 2;
    }
  }
}

void chpl_computeHeapPageSize(void) {
  size_t pageSize = 0;

#if defined __linux__
  char* ev;
  if ((ev = getenv("HUGETLB_DEFAULT_PAGE_SIZE")) == NULL)
    pageSize = chpl_getSysPageSize();
  else {

    size_t tmpPageSize;
    int  num_scanned;
    char units;

    if ((num_scanned = sscanf(ev, "%zi%c", &tmpPageSize, &units)) != 1) {
      if (num_scanned == 2 && strchr("kKmMgG", units) != NULL) {
        switch (units) {
        case 'k': case 'K': tmpPageSize <<= 10; break;
        case 'm': case 'M': tmpPageSize <<= 20; break;
        case 'g': case 'G': tmpPageSize <<= 30; break;
        }
      }
      else {
        chpl_internal_error("unexpected HUGETLB_DEFAULT_PAGE_SIZE syntax");
      }
    }

    pageSize = tmpPageSize;
  }
#else
  pageSize = chpl_getSysPageSize();
#endif

  if (pageSize == chpl_getSysPageSize()) {
    // if the page size is the system page size, check that
    // the heap isn't on hugepages when nobody told us.
    //
    // note: sets heapPageSize
    computeHeapPageSizeByGuessing(pageSize);
  } else {
    // if the page size was configured by the user to be huge,
    // just trust it.
    heapPageSize = pageSize;
  }
}


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
  pageSize = chpl_getSysPageSize();
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

  //
  // If f is NULL, we should have exited by now, but Coverity doesn't
  // seem to be catching this (anymore), so I'm adding an assertion
  // to try and help it out.
  //
  assert(f != NULL);

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
#if defined __NetBSD__
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
#if defined __NetBSD__
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


//
// Move to the last available hardware thread.  Tasking layers use
// this to get predictable placement for comm layer polling threads,
// in order to help manage execution resources.
//
void chpl_moveToLastCPU(void) {
  //
  // This is currently a no-op except on Linux.
  //
#if defined __linux__
  {
    cpu_set_t mask;
    int i, cnt;

    if (pthread_getaffinity_np(pthread_self(), sizeof(mask), &mask) < 0)
      chpl_internal_error("sched_getaffinity() failed");

    for (i = cnt = 0; !CPU_ISSET(i, &mask) || ++cnt < CPU_COUNT(&mask); i++)
      ;

    CPU_ZERO(&mask);
    CPU_SET(i, &mask);
    if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) < 0)
      chpl_internal_error("sched_setaffinity() failed");
  }
#endif

  return;
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
                                 CHPL_RT_MD_LOCALE_NAME_BUF, 0, 0);
    strcpy(namespace, utsinfo.nodename);
  }
  return namespace;
}
