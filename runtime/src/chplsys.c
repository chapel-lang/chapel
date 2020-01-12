/*
 * Copyright 2004-2020 Cray Inc.
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
#include "chpl-env.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"
#include "chpltypes.h"
#include "error.h"

// System headers

// We need this first so we can then decide based on the existence and
// perhaps the value of _POSIX_VERSION what else to #include below.
#ifdef __unix__
#include <unistd.h>
#endif

#include <errno.h>

#ifdef _POSIX_VERSION
#include <pthread.h>
#endif

#ifdef __linux__
#include <sched.h>
#endif

#include <ctype.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/utsname.h>

#if defined(__APPLE__) || defined(__NetBSD__) || defined(__FreeBSD__)
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
    printf("check_page_size(%li, %lx, %p, %p-%p)\n",
           (long) page_size_guess, (long) max_page_size,
           ptr, heap_start, ((unsigned char*) heap_start) + heap_size);

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
    if (debug) printf("mprotect failed with errno=%i\n", errno);
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


// Determine if 2 page_size-sized pages fit into the heap.
// Assumes page_size is a power of 2.
// Returns 1 if they do fit.
// Returns 0 otherwise.
static int fitsInHeap(size_t page_size, void* heap_start, size_t heap_size)
{
  uintptr_t start = (uintptr_t) heap_start;
  uintptr_t end = start + (uintptr_t) heap_size;

  start = round_up_to_mask(start, page_size-1);
  end   = round_down_to_mask(end, page_size-1);
  
  if (2*page_size <= end - start)
    return 1;
  else
    return 0;
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

  chpl_comm_regMemHeapInfo(&heap_start, &heap_size);

  if (heap_start != NULL && heap_size != 0) {

    size_t page_size = page_size_in;
    size_t max_heap_page_size = page_size;
    size_t max_page_size = 1*1024*1024*1024; // Stop at 1GB

    // Compute a maximum page size based upon the size
    // of the heap.
    for (page_size = page_size_in;
         fitsInHeap(page_size, heap_start, heap_size);
         page_size *= 2) {
      max_heap_page_size = page_size;
    }

    if (max_heap_page_size > max_page_size)
      max_heap_page_size = max_page_size;
 
    // Now do experiments with the last 2 pages in the heap
    // to find if we have underestimated the page size.

    // Doing this at the end of the heap is important
    // for performance and to maximize the contiguous region
    // that can be handled at once.

    page_size = page_size_in;
    while(1) {
      unsigned char* start = (unsigned char*) heap_start;
      unsigned char* end = start + heap_size;
      unsigned char* last_page = NULL;
      unsigned char* last_two_pages = NULL;
      int small;

      start = round_up_to_mask_ptr(start, page_size-1);
      end   = round_down_to_mask_ptr(end, page_size-1);
      last_page = end - page_size;
      last_two_pages = last_page - page_size;

      small = check_page_size(page_size, max_heap_page_size,
                              last_two_pages, heap_start, heap_size);

      // Stop if real page size <= page_size, or if
      // a signal handler could not be installed.
      if (small == 0 || small == -2) {
        heapPageSize = page_size;
        break;
      }
      // Stop if we have reached the maximum size
      if (page_size >= max_heap_page_size) {
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

static void computeHeapPageSize(void) {
#if defined __linux__
  //
  // If we're using hugepages explicitly, just go with what it says.
  // Otherwise, try to figure it out ourselves.
  //
  {
    char* ev;

    if ((ev = getenv("HUGETLB_DEFAULT_PAGE_SIZE")) != NULL) {
      int  num_scanned;
      char units;

      if ((num_scanned = sscanf(ev, "%zi%c", &heapPageSize, &units)) != 1) {
        if (num_scanned != 2 || strchr("kKmMgG", units) == NULL) {
          chpl_internal_error("unexpected HUGETLB_DEFAULT_PAGE_SIZE syntax");
        }

        switch (units) {
        case 'k': case 'K': heapPageSize <<= 10; break;
        case 'm': case 'M': heapPageSize <<= 20; break;
        case 'g': case 'G': heapPageSize <<= 30; break;
        }
      }

      return;
    }
  }
#endif

  // note: sets heapPageSize
  computeHeapPageSizeByGuessing(chpl_getSysPageSize());
}


size_t chpl_getHeapPageSize(void) {
#ifdef _POSIX_VERSION
  {
    static pthread_once_t onceCtl = PTHREAD_ONCE_INIT;

    if (pthread_once(&onceCtl, computeHeapPageSize) != 0) {
      chpl_internal_error("pthread_once() failed");
    }

    return heapPageSize;
  }
#else
  return chpl_getSysPageSize();
#endif
}


// This function returns the amount of physical memory the
// system reports there is. This amount of memory is not generally
// allocatable, since there has to be space for the kernel. This routine
// makes no attempt to account for kernel space.
uint64_t chpl_sys_physicalMemoryBytes(void) {
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
#elif defined __FreeBSD__
  uint64_t membytes;
  size_t len = sizeof(membytes);
  if (sysctlbyname("hw.physmem", &membytes, &len, NULL, 0))
    chpl_internal_error("query of physical memory failed");
  return membytes;
#elif defined __NetBSD__
  uint64_t membytes;
  size_t len = sizeof(membytes);
  if (sysctlbyname("hw.physmem64", &membytes, &len, NULL, 0))
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
  // This code runs for linux, but probably works for FreeBSD/NetBSD.
  //
  numPages = sysconf(_SC_PHYS_PAGES);
  if (numPages < 0)
    chpl_internal_error("query of physical memory failed");
  pageSize = chpl_getSysPageSize();
  if (pageSize < 0)
    chpl_internal_error("query of physical memory failed");
  return (uint64_t)numPages * (uint64_t)pageSize;
#endif
}

#if defined(__linux__) || defined(__NetBSD__)
//
// Return information about the processors on the system.
//
static struct {
  int physId, coreId;
} cpuTab[8192];
static const int cpuTabSize = sizeof(cpuTab) / sizeof(cpuTab[0]);
static int cpuTabLen = 0;


static void
add_to_cpuTab(int physId, int coreId) {
  int i = 0;
  while (i < cpuTabLen
         && (physId != cpuTab[i].physId || coreId != cpuTab[i].coreId))
    i++;
  if (i >= cpuTabLen) {
    if (++cpuTabLen >= cpuTabSize) {
      chpl_internal_error("cpuTab[] full");
    } else {
      cpuTab[i].physId = physId;
      cpuTab[i].coreId = coreId;
    }
  }
}


static int numCores, numPUs;


static void getCpuInfo_once(void) {
  FILE* f;
  char buf[100];
  int procs = 0;
  int physId;
  int coreId;

  const char* fname = chpl_env_rt_get("PROC_CPUINFO_FNAME", "/proc/cpuinfo");
  if ((f = fopen(fname, "r")) == NULL)
    chpl_internal_error_v("Cannot open %s", fname);

  //
  // If f is NULL, we should have exited by now, but Coverity doesn't
  // seem to be catching this (anymore), so I'm adding an assertion
  // to try and help it out.
  //
  assert(f != NULL);

  physId = coreId = -1;

  while (!feof(f) && fgets(buf, sizeof(buf), f) != NULL) {
    size_t buf_len = strlen(buf);
    int procTmp;
    int physIdTmp;
    int coreIdTmp;

    if (sscanf(buf, "processor : %i", &procTmp) == 1) {
      procs++;
    }
    else if (sscanf(buf, "physical id : %i", &physIdTmp) == 1) {
      if (physId >= 0) {
        add_to_cpuTab(physId, coreId);
        coreId = -1;
      }
      physId = physIdTmp;
    }
    else if (sscanf(buf, "core id : %i", &coreIdTmp) == 1) {
      if (coreId >= 0) {
        add_to_cpuTab(physId, coreId);
        physId = -1;
      }
      coreId = coreIdTmp;
    }

    while (buf[buf_len - 1] != '\n' && fgets(buf, sizeof(buf), f) != NULL)
      buf_len = strlen(buf);
  }

  if (physId >= 0 || coreId >= 0) {
    add_to_cpuTab(physId, coreId);
  }

  fclose(f);

  if ((numPUs = procs) <= 0)
    numPUs = cpuTabLen;

  if (cpuTabLen > 0) {
    numCores = cpuTabLen;
    return;
  }

  // We have a limited-format /proc/cpuinfo.
  // See if the /sys filesystem has any more information for us.
  int threads_per_core = 0;
  if ((f = fopen("/sys/devices/system/cpu/cpu0/topology/thread_siblings", "r"))
      != NULL) {
    int c;
    while ((c = getc(f)) != EOF) {
      // The number of threads per core is the total number of bits
      // set in the hex digits of the thread_siblings map.
      //
      // The most authoritative source, kernel.org, does not dictate
      // the formatting of the mask.  However, the following document
      // indicates that it is either a hex or binary bit mask.  The code
      // below works in either case.
      //
      // https://www.ibm.com/support/knowledgecenter/en/linuxonibm/liaat/liaattunproctop.htm
      //
      // Also note that hwloc itself parses the file as a hex bitmap only.
      if (isxdigit(c)) {
        switch (tolower(c)) {
        case '0':
          break;
        case '1':
        case '2':
        case '4':
        case '8':
          threads_per_core += 1;
          break;
        case '3':
        case '5':
        case '6':
        case '9':
        case 'a':
        case 'c':
          threads_per_core += 2;
          break;
        case '7':
        case 'b':
        case 'd':
        case 'e':
          threads_per_core += 3;
          break;
        case 'f':
          threads_per_core += 4;
          break;
        }
      } else if (c != ',' && c != '\n' && tolower(c) != 'x') {
        // unknown file format -- don't use
        threads_per_core = 1;
        break;
      }
    }
    fclose(f);
  }
  if (threads_per_core == 0)
    threads_per_core = 1;
  if ((numCores = procs / threads_per_core) <= 0)
    numCores = 1;
}


static void getCpuInfo(int* p_numPhysCpus, int* p_numLogCpus) {
#ifdef _POSIX_VERSION
  {
    static pthread_once_t onceCtl = PTHREAD_ONCE_INIT;

    if (pthread_once(&onceCtl, getCpuInfo_once) != 0) {
      chpl_internal_error("pthread_once(getCpuInfo_once) failed");
    }

    *p_numPhysCpus = numCores;
    *p_numLogCpus = numPUs;
  }
#else
#error "on __linux__ or __NetBSD__, but !defined(_POSIX_VERSION)"
#endif
}
#endif


int chpl_sys_getNumCPUsPhysical(chpl_bool accessible_only) {
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
    numCpus = chpl_sys_getNumCPUsLogical(true);
  return numCpus;
#elif defined  __FreeBSD__
  //
  // FreeBSD
  //
  static int numCpus = 0;
  if (numCpus == 0)
    numCpus = chpl_sys_getNumCPUsLogical(true);
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

      if (numCpus <= 0)
        numCpus = 1;
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


int chpl_sys_getNumCPUsLogical(chpl_bool accessible_only) {
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
#elif defined __FreeBSD__
  //
  // FreeBSD
  //
  static int32_t numCpus = 0;
  if (numCpus == 0) {
    size_t len = sizeof(numCpus);
    if (sysctlbyname("hw.ncpu", &numCpus, &len, NULL, 0))
      chpl_internal_error("query of number of PUs failed");
  }
  return (int) numCpus;

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

      if (numCpus <= 0)
        numCpus = 1;
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
                                 CHPL_RT_MD_LOCALE_NAME_BUF, 0, 0);
    strcpy(namespace, utsinfo.nodename);
  }
  return namespace;
}
