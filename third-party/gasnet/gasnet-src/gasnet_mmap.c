/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_mmap.c $
 * Description: GASNet memory-mapping utilities
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#if GASNET_PSHM && (GASNETI_PSHM_FILE || GASNETI_PSHM_POSIX) && HAVE_FSTATVFS
  #include <sys/statvfs.h>
#endif

#if (PLATFORM_OS_LINUX || PLATFORM_OS_CNL) && !GASNETI_BUG3480_WORKAROUND // Suspect bug 3480
#define GASNETI_BUG3480_MSG "\nYour system is suspected to be impacted by bug 3480"
#else
#define GASNETI_BUG3480_MSG
#endif

#if defined(GASNETI_MMAP_OR_PSHM) && !defined(HAVE_MMAP)
 #if PLATFORM_OS_CYGWIN && (GASNETI_PSHM_POSIX || GASNETI_PSHM_FILE)
  /* Use of mmap() for PSHM over POSIX or FILE is a less-than-general case.
   * So on Cygwin define HAVE_MMAP for those cases only. */
  #define HAVE_MMAP 1
  /* And we need these, which would otherwise appear in gasnet_internal.h */
  extern void *gasneti_mmap(uintptr_t segsize);
  extern void gasneti_munmap(void *segbase, uintptr_t segsize);
 #else
  /* Ensure PSHM over SYSV never actually calls mmap() */
  #define mmap %%%ERROR__GASNet_does_not_support_mmap_in_this_configuration%%%
  #define munmap %%%ERROR__GASNet_does_not_support_munmap_in_this_configuration%%%
 #endif
#endif

#if defined(GASNETI_MMAP_OR_PSHM) && defined(GASNETI_USE_HUGETLBFS)
  #define gasneti_mmap_aligndown(sz) gasneti_mmap_aligndown_huge(sz)
  #define gasneti_mmap_alignup(sz)   gasneti_mmap_alignup_huge(sz)
  #define gasneti_mmap_pagesize()    gasneti_mmap_pagesize_huge()
#else
  #define gasneti_mmap_aligndown(sz) GASNETI_PAGE_ALIGNDOWN(sz)
  #define gasneti_mmap_alignup(sz) GASNETI_PAGE_ALIGNUP(sz)
  #define gasneti_mmap_pagesize()    GASNETI_PAGESIZE
#endif

#ifdef GASNETI_MMAP_OR_PSHM
 #if GASNET_PSHM && !defined(_POSIX_C_SOURCE) && PLATFORM_OS_SOLARIS
  #define _POSIX_C_SOURCE 200112L /* Required for shm_{open,unlink} decls */
  #include <sys/mman.h>
  #undef _POSIX_C_SOURCE
 #elif HAVE_MMAP
  #include <sys/mman.h>
 #elif !defined(MAP_FAILED)
  #define MAP_FAILED ((void*)(uintptr_t)(-1LL))
 #endif

 #if GASNET_PSHM && defined(GASNETI_PSHM_XPMEM)
  #if defined(HAVE_XPMEM_H)
   /* Cray XPMEM */
   #include <xpmem.h>
   typedef struct xpmem_addr gasneti_xpmem_addr_t;
   typedef xpmem_segid_t gasneti_xpmem_segid_t;
   typedef xpmem_apid_t gasneti_xpmem_apid_t;
   #define gasneti_xpmem_apid apid
  #elif defined(HAVE_SN_XPMEM_H)
   /* SGI XPMEM */
   #include <sn/xpmem.h>
   typedef xpmem_addr_t gasneti_xpmem_addr_t;
   typedef int64_t gasneti_xpmem_segid_t;
   typedef int64_t gasneti_xpmem_apid_t;
   #define gasneti_xpmem_apid id
  #endif
 #endif

 /* TODO:
    Below are a couple cases where we want to use the lower portion of the O/S-provided
    mmap()ed region to create our segment because one must ensure some alignment which
    is greater than the page size.  Instead of forcing the use of the bottom portion,
    we *could* change the trimming logic to use a chosen granularity other than that of
    GASNETI_PAGESIZE (such as the huge page size).
  */

 #ifdef GASNETI_USE_HUGETLBFS
  #include <hugetlbfs.h>
  /* Provide greater alignment than default: */
  static uintptr_t gasneti_mmap_pagesize_huge() {
     static long pagesz = 0;
     if (!pagesz) {
       pagesz = gethugepagesize();
       gasneti_assert_uint(pagesz ,>=, GASNETI_PAGESIZE);
       gasneti_assert_uint(pagesz % GASNETI_PAGESIZE ,==, 0);
     }
     return pagesz;
  }
  static uintptr_t gasneti_mmap_aligndown_huge(uintptr_t sz) {
     return GASNETI_ALIGNDOWN(sz, gasneti_mmap_pagesize_huge());
  }
  static uintptr_t gasneti_mmap_alignup_huge(uintptr_t sz) {
     return GASNETI_ALIGNUP(sz, gasneti_mmap_pagesize_huge());
  }
 #endif

#if !HAVE_MMAP
  /* Skip the following platform checks */
#elif PLATFORM_OS_CYGWIN && !GASNET_PSHM
  #error mmap without PSHM not supported on Cygwin - it doesnt work properly
#elif PLATFORM_ARCH_MIC
  #define GASNETI_MMAP_FLAGS (MAP_ANONYMOUS | MAP_PRIVATE | MAP_POPULATE)
#endif

#ifndef GASNETI_MMAP_FLAGS
  #ifndef GASNETI_MMAP_NORESERVE
    #if defined(MAP_NORESERVE) || defined(HAVE_MAP_NORESERVE)
      /* bug 1358: try to avoid allocating swap space, if possible */
      #define GASNETI_MMAP_NORESERVE  MAP_NORESERVE
    #else
      #define GASNETI_MMAP_NORESERVE  0
    #endif
  #endif
  /* find an appropriate flag for anonymous mmap */
  #if defined(MAP_ANONYMOUS) || defined(HAVE_MAP_ANONYMOUS)
    #define GASNETI_MAP_ANONYMOUS MAP_ANONYMOUS
  #elif defined(MAP_ANON) || defined(HAVE_MAP_ANON)
    #define GASNETI_MAP_ANONYMOUS MAP_ANON
  #else /* assume no direct capability exists, fall back on mapping /dev/zero */
    #define GASNETI_MAP_ANONYMOUS 0
    #ifndef GASNETI_MMAP_FILE
    #define GASNETI_MMAP_FILE "/dev/zero"
    #endif
  #endif
  #define GASNETI_MMAP_FLAGS (GASNETI_MAP_ANONYMOUS | MAP_PRIVATE | GASNETI_MMAP_NORESERVE)
#endif

#ifndef GASNETI_MMAP_FIXED_FLAG
  #define GASNETI_MMAP_FIXED_FLAG MAP_FIXED
#endif
#ifndef GASNETI_MMAP_NOTFIXED_FLAG
  #define GASNETI_MMAP_NOTFIXED_FLAG 0
#endif

/* Flag, if any, to prevent write-back of file-based mappings */
#ifdef MAP_NOSYNC
#define GASNETI_MMAP_NOSYNC_FLAG MAP_NOSYNC
#endif
#ifndef GASNETI_MMAP_NOSYNC_FLAG
  #define GASNETI_MMAP_NOSYNC_FLAG 0
#endif

#if GASNET_PSHM && PLATFORM_OS_CYGWIN
  /* CYGWIN: may not honor the address passed to shmat() */
  #define GASNETI_PSHM_MAP_FIXED_IGNORED 1
#endif

#if HAVE_MMAP
/* ------------------------------------------------------------------------------------ */
static void *gasneti_mmap_internal(void *segbase, uintptr_t segsize, int fixed_mayfail) {
  static int gasneti_mmapfd = -1;
  gasneti_tick_t t1, t2;
  int mmap_errno;
  void	*ptr;

  #ifdef GASNETI_MMAP_FILE
    if (gasneti_mmapfd == -1) {
      gasneti_mmapfd = open(GASNETI_MMAP_FILE, O_RDWR);
      if (gasneti_mmapfd == -1) 
        gasneti_fatalerror("failed to open "GASNETI_MMAP_FILE" for mmap : %s\n",strerror(errno));
    }
  #endif

  t1 = gasneti_ticks_now();
#if defined(GASNETI_USE_HUGETLBFS)
  ptr = gasneti_huge_mmap(segbase, segsize);
#else
  ptr = mmap(segbase, segsize, (PROT_READ|PROT_WRITE), 
      (GASNETI_MMAP_FLAGS | (segbase==NULL?GASNETI_MMAP_NOTFIXED_FLAG:GASNETI_MMAP_FIXED_FLAG)), 
      gasneti_mmapfd, 0);
#endif
  mmap_errno = errno;
  t2 = gasneti_ticks_now();

  #ifdef GASNETI_MMAP_FILE
    /* close(gasneti_mmapfd);  NOPE: we reuse one static fd */
  #endif

  GASNETI_TRACE_PRINTF(C, 
      ("mmap %s("GASNETI_LADDRFMT", %"PRIuPTR"): %.3fus => "GASNETI_LADDRFMT"%s%s\n", 
        (segbase == NULL?"":"fixed"),
        GASNETI_LADDRSTR(segbase), segsize,
        gasneti_ticks_to_ns(t2-t1)/1000.0,
        GASNETI_LADDRSTR(ptr),
        (ptr == MAP_FAILED?"  MAP_FAILED: ":""),
        (ptr == MAP_FAILED?strerror(mmap_errno):"")));

  if (ptr == MAP_FAILED && mmap_errno != ENOMEM) {
    #if PLATFORM_OS_CYGWIN
      if (mmap_errno != EACCES) /* Cygwin stupidly returns EACCES for insuff mem */
    #elif PLATFORM_OS_SOLARIS
      if (mmap_errno != EAGAIN) /* Solaris stupidly returns EAGAIN for insuff mem */
    #endif
    gasneti_fatalerror("unexpected error in mmap%s for size %"PRIuPTR": %s\n", 
                       (segbase == NULL?"":" fixed"),
                       segsize, strerror(mmap_errno));
  }

  if ((ptr != (void*)GASNETI_PAGE_ALIGNDOWN(ptr)) && (ptr != MAP_FAILED)) {
      gasneti_fatalerror("mmap result "GASNETI_LADDRFMT" is not aligned to GASNET_PAGESIZE %lu (0x%lx)",
              GASNETI_LADDRSTR(ptr), (unsigned long)GASNET_PAGESIZE, (unsigned long)GASNET_PAGESIZE);
  }
  if (segbase && ptr == MAP_FAILED) {
      if (fixed_mayfail) {
         errno = mmap_errno;
         return MAP_FAILED;
      }
      gasneti_fatalerror("mmap fixed failed at "GASNETI_LADDRFMT" for size %"PRIuPTR": %s" GASNETI_BUG3480_MSG,
	      GASNETI_LADDRSTR(segbase), segsize, strerror(mmap_errno));
  }
  if (segbase && segbase != ptr) {
    gasneti_fatalerror("mmap fixed moved from "GASNETI_LADDRFMT" to "GASNETI_LADDRFMT" for size %"PRIuPTR"\n",
	    GASNETI_LADDRSTR(segbase), GASNETI_LADDRSTR(ptr), segsize);
  }
  return ptr;
}
extern void *gasneti_mmap_fixed(void *segbase, uintptr_t segsize, int mayfail) {
  return gasneti_mmap_internal(segbase, segsize, mayfail);
}
extern void *gasneti_mmap(uintptr_t segsize) {
  return gasneti_mmap_internal(NULL, segsize, 0);
}
#endif /* HAVE_MMAP */

#if GASNET_PSHM

/* an array of filenames/keys with length 1+gasneti_pshm_nodes, the +1 is for AMs */
#if defined(GASNETI_PSHM_SYSV)
  #include <sys/shm.h>
  static key_t *gasneti_pshm_sysvkeys = NULL;
#elif defined(GASNETI_PSHM_FILE) || defined(GASNETI_PSHM_POSIX)
  static char **gasneti_pshmname = NULL;
#elif defined(GASNETI_PSHM_XPMEM)
  static gasneti_xpmem_segid_t *gasneti_pshm_segids = NULL;
  static gasneti_xpmem_apid_t *gasneti_pshm_apids = NULL;
#endif

static char *gasneti_pshm_tmpfile_ = NULL;
static char *gasneti_pshm_tmpfile = NULL;
#define GASNETI_PSHM_PREFIX_LEN1  6  /* "/GASNT" */
#define GASNETI_PSHM_PREFIX_LEN   (GASNETI_PSHM_PREFIX_LEN1 + GASNETI_PSHM_UNIQUE_LEN)

static int gasneti_pshm_mkstemp(const char *prefix, const char *tmpdir) {
  int tmpfd;

  if (!tmpdir || !strlen(tmpdir)) {
    errno = ENOTDIR;
    return -1;
  }
  gasneti_pshm_tmpfile_ = gasneti_realloc(gasneti_pshm_tmpfile_, strlen(tmpdir) + GASNETI_PSHM_PREFIX_LEN + 1);
  strcpy(gasneti_pshm_tmpfile_, tmpdir);
  strcat(gasneti_pshm_tmpfile_, prefix);

  /* Now try to create a unique file in the given directory */
  tmpfd = mkstemp(gasneti_pshm_tmpfile_);
  if (tmpfd >= 0) {
    gasneti_local_wmb();
    gasneti_pshm_tmpfile = gasneti_pshm_tmpfile_;
    close(tmpfd);
    return 0;
  } else {
    return -1;
  }
}

#ifdef GASNETI_PSHM_SYSV
static int gasneti_pshm_settemp(const char *unique, const char *prefix, const char *tmpdir) {
  int tmpfd;
  int len;

  if (gasneti_pshm_tmpfile) return 0;

  if (!tmpdir || !strlen(tmpdir)) {
    errno = ENOTDIR;
    return -1;
  }
  gasneti_pshm_tmpfile_ = gasneti_realloc(gasneti_pshm_tmpfile_, strlen(tmpdir) + GASNETI_PSHM_PREFIX_LEN + 1);
  strcpy(gasneti_pshm_tmpfile_, tmpdir);
  strcat(gasneti_pshm_tmpfile_, prefix);

  /* Note: 'unique' might not be NUL terminated */
  len = strlen(gasneti_pshm_tmpfile_);
  memcpy(gasneti_pshm_tmpfile_ + len - GASNETI_PSHM_UNIQUE_LEN, unique, GASNETI_PSHM_UNIQUE_LEN);

  /* Now try to verify the file exists */
  tmpfd = open(gasneti_pshm_tmpfile_, O_RDWR);
  if (tmpfd >= 0) {
    gasneti_local_wmb();
    gasneti_pshm_tmpfile = gasneti_pshm_tmpfile_;
    close(tmpfd);
    return 0;
  } else {
    return -1;
  }
}
#endif

#if defined(GASNETI_PSHM_FILE) || defined(GASNETI_PSHM_SYSV) || defined(GASNETI_PSHM_POSIX)
static const char *gasneti_pshm_makeunique(const char *unique) {
  static char prefix[] = "/GASNTXXXXXX";
#if defined(GASNETI_PSHM_FILE) || defined(GASNETI_PSHM_SYSV)
  const char *tmpdir = gasneti_tmpdir();
#endif
  size_t tmpdir_len = 0;
#if !defined(GASNETI_PSHM_SYSV)
  size_t base_len;
  char *allnames;
#endif
  int i;

  gasneti_assert_uint(strlen(prefix) ,==, GASNETI_PSHM_PREFIX_LEN);

#if defined(GASNETI_PSHM_FILE) && defined(GASNETI_USE_HUGETLBFS)
  { const char *hugedir = hugetlbfs_find_path();
    if (hugedir && !access(hugedir, R_OK|W_OK|X_OK)) {
      tmpdir = hugedir;
    }
  }
#endif
#if defined(GASNETI_PSHM_FILE) || defined(GASNETI_PSHM_SYSV)
  tmpdir_len = strlen(tmpdir);
#endif

  if (!unique) { /* We get to pick the unique bits */
#if defined(GASNETI_PSHM_FILE) || defined(GASNETI_PSHM_SYSV)
    if (gasneti_pshm_mkstemp(prefix, tmpdir)) {
      gasneti_fatalerror("mkstemp() failed to find a unique prefix: %s", strerror(errno));
    }
#else
    /* We do NOT honor $TMPDIR, since setting it to a job-specific
     * value would interfere with our purpose here of finding a
     * name that is unique per-NODE. */
    if (   gasneti_pshm_mkstemp(prefix, "/tmp")
        && gasneti_pshm_mkstemp(prefix, "/var/tmp")
        && gasneti_pshm_mkstemp(prefix, "/usr/tmp")
  #if PLATFORM_OS_LINUX
        && gasneti_pshm_mkstemp(prefix, "/dev/shm")
  #endif
       ) {
      /* We'll HOPE that our pid's low 24 bits are good enough for node-scope uniqueness */
      static const char pattern[] = "/GASNT%06x";
      gasneti_pshm_tmpfile = gasneti_realloc(gasneti_pshm_tmpfile, sizeof(prefix)); /* inc. \0 */
      snprintf(gasneti_pshm_tmpfile, sizeof(prefix), pattern, (0xFFFFFFU & (unsigned int)getpid()));
    }
#endif
    /* Don't unlink() it until we no longer require uniqueness */

    /* Strip away the tmpdir to yield a unique prefix */
    unique = strrchr(gasneti_pshm_tmpfile, '/');
    gasneti_assert_uint(strlen(unique) ,==, GASNETI_PSHM_PREFIX_LEN);
    unique += GASNETI_PSHM_PREFIX_LEN1;
  }

#if defined(GASNETI_PSHM_SYSV)
  gasneti_pshm_settemp(unique, prefix, tmpdir);
  key_t *keys = (key_t *)gasneti_malloc((gasneti_pshm_nodes+1)*sizeof(key_t));;
  for (i = 0; i <= gasneti_pshm_nodes; ++i) {
    key_t key = ftok(gasneti_pshm_tmpfile, i + 1);
    if (key == (key_t)-1){
        gasneti_fatalerror("failed to produce a unique SYSV key value for %s and rank %d, from ftok: %s",
                           gasneti_pshm_tmpfile, i, strerror(errno));
    }
  #if GASNETI_PSHM_MAX_NODES > 255
    else { /* ftok() is documented (on many systems) as using only low 8 bits - so verify */
      int j;
      for (j = 0; j < i; ++j) {
        if_pf (key == keys[j]) {
          key = (key_t)-1;
          gasneti_fatalerror("failed to produce a unique SYSV key value for %s and rank %d, dup of %d",
                             gasneti_pshm_tmpfile, i, j);
        }
      }
    }
  #endif
    keys[i] = key;
  }
  gasneti_local_wmb();
  gasneti_pshm_sysvkeys = keys;
#else
  /* Three base-36 "digits" provide 46,656 unique names, even if case-insensitive. */
 #if GASNETI_PSHM_MAX_NODES > 255
  gasneti_assert_always_uint(gasneti_pshm_nodes ,<, (36*36*36));
 #endif

  /* Note: 'unique' might not be NUL terminated */
  memcpy(prefix + GASNETI_PSHM_PREFIX_LEN1, unique, GASNETI_PSHM_UNIQUE_LEN);

  char **names = (char **)gasneti_malloc((gasneti_pshm_nodes+1)*sizeof(char*));
  base_len = tmpdir_len + GASNETI_PSHM_PREFIX_LEN;
  allnames = (char *)gasneti_malloc((gasneti_pshm_nodes+1)*(base_len + 4));

  for (i = 0; i <= gasneti_pshm_nodes; ++i) {
    const char tbl[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const unsigned int namelen = (base_len + 4);
    char *filename = allnames + (i * namelen);
    const int base = 36;
    int digit = i;

 #ifdef GASNETI_PSHM_FILE
    memcpy(filename, tmpdir, tmpdir_len);
 #endif
    memcpy(filename + tmpdir_len, prefix, GASNETI_PSHM_PREFIX_LEN);

    filename[base_len + 2] = tbl[digit % base]; digit /= base;
    filename[base_len + 1] = tbl[digit % base]; digit /= base;
    filename[base_len + 0] = tbl[digit];
    filename[base_len + 3] = '\0';

    names[i] = filename;
  }
  gasneti_local_wmb();
  gasneti_pshmname = names;
#endif

  return unique;
}
#endif
#endif /* GASNET_PSHM */

#if defined(GASNETI_USE_HUGETLBFS)

/* Apply the default hugepage size for mapping of the requested size.
 * The given size is adjusted for proper alignment and returned.
 */
static uintptr_t huge_pagesz(void *addr, uintptr_t size) {
  static long pagesz = 0;
  if (!pagesz) pagesz = gethugepagesize();
  gasneti_assert_uint((uintptr_t)addr % pagesz ,==, 0); /* alignment check */
  return GASNETI_ALIGNUP(size, pagesz);
}

extern void *gasneti_huge_mmap(void *addr, uintptr_t size) {
  int fd = hugetlbfs_unlinked_fd();
  const int mmap_flags = MAP_SHARED | (addr ? GASNETI_MMAP_FIXED_FLAG : GASNETI_MMAP_NOTFIXED_FLAG);
  void *ptr = mmap(addr, huge_pagesz(addr, size), (PROT_READ|PROT_WRITE), mmap_flags, fd, 0);

  int save_errno = errno;
  (void) close(fd);
  errno = save_errno;

  return ptr;
}

extern void gasneti_huge_munmap(void *addr, uintptr_t size) {
  if (munmap(addr, huge_pagesz(addr, size)) != 0)
    gasneti_fatalerror("munmap("GASNETI_LADDRFMT",%"PRIuPTR") failed: %s\n",
                       GASNETI_LADDRSTR(addr), size, strerror(errno));
}

#endif /* defined(GASNETI_USE_HUGETLBFS) */

#if GASNET_PSHM

static void gasneti_pshm_unlink(int pshm_rank);

/* create the object/region/segment and return its address */
static void * gasneti_pshm_mmap(int pshm_rank, void *segbase, size_t segsize) {
  const int create = (pshm_rank == gasneti_pshm_mynode) ||
                     ((pshm_rank == gasneti_pshm_nodes) && !gasneti_pshm_mynode);
  void * ptr = MAP_FAILED;

#if defined(GASNETI_PSHM_SYSV)
  const int flags = S_IRUSR | S_IWUSR | (create ? ( IPC_CREAT | IPC_EXCL ) : 0);

  /* Create and size in 1 step */
  int id = shmget(gasneti_pshm_sysvkeys[pshm_rank], segsize, flags);
  if (id == -1) {
    #if PLATFORM_OS_CYGWIN
    if (errno == ENOSYS) {
      gasneti_fatalerror("Cygwin's SystemV shared memory support is not enabled.");
    }
    #endif
    if (errno == EINVAL) {
      // shmget() returns EINVAL for (size > SHMMAX), among other causes
      // TODO: can we confirm that is the actual cause?
      errno = ENOMEM;
    }
    if (errno == ENOSPC) {
      // shmget() returns ENOSPC for (total_size > SHMALL), among other causes
      // TODO: can we confirm that is the actual cause?
      errno = ENOMEM;
    }
    return MAP_FAILED;
  }

  /* map */
  ptr = shmat(id, segbase, 0);

  if (ptr == (void *)(intptr_t)(-1)) ptr = MAP_FAILED;
#elif defined(GASNETI_PSHM_FILE) || defined(GASNETI_PSHM_POSIX)
  const int flags = O_RDWR | (create ? ( O_CREAT | O_EXCL ) : 0);
  const char *filename = gasneti_pshmname[pshm_rank];
  int fd = -1;

  /* create or open */
  #if defined(GASNETI_PSHM_FILE)
   #if defined(GASNETI_USE_HUGETLBFS)
    segsize = huge_pagesz(segbase, segsize);
   #endif
    fd = open(filename, flags, S_IRUSR | S_IWUSR);
  #elif defined(GASNETI_PSHM_POSIX)
    fd = shm_open(filename, flags, S_IRUSR | S_IWUSR);
    #if PLATFORM_OS_DARWIN
      if ((fd == -1) && (errno == EEXIST)) {
        /* Work around Darwin stupidity observed by Filip */
        int retries_remain = 32;
        do {
          gasneti_sched_yield();
          fd = shm_open(filename, flags, S_IRUSR | S_IWUSR);
        } while ((fd == -1) && (errno == EEXIST) && retries_remain--);
      }
    #endif
  #else
    #error
  #endif
  if (fd == -1) return MAP_FAILED;

  /* size on create only */
  if (create && ftruncate(fd, segsize)) {
    /* resize failed - fall through */
  } else {
    /* map */
    const int mmap_flags = MAP_SHARED | GASNETI_MMAP_NOSYNC_FLAG |
                           (segbase ? GASNETI_MMAP_FIXED_FLAG : GASNETI_MMAP_NOTFIXED_FLAG);
    ptr = mmap(segbase, segsize, (PROT_READ|PROT_WRITE), mmap_flags, fd, 0);
  }

  {
    const int save_errno = errno;
    (void) close(fd);
    errno = save_errno;
  }
#elif defined(GASNETI_PSHM_XPMEM)
  if (create) {
  #if GASNETI_USE_HUGETLBFS
    ptr = gasneti_huge_mmap(segbase, segsize);
  #else
    const int mmap_flags = GASNETI_MAP_ANONYMOUS | MAP_SHARED | \
                           (segbase ? GASNETI_MMAP_FIXED_FLAG : GASNETI_MMAP_NOTFIXED_FLAG);
    ptr = mmap(segbase, segsize, (PROT_READ|PROT_WRITE), mmap_flags, 0, 0);
  #endif
  } else {
    gasneti_xpmem_apid_t apid;
    // Bounded retry on xpmem_get() failure to tolerate non-fatal signals (see Bug 3815)
    for (int trial = 1; trial <= 5; ++trial) {
    #if HAVE_XPMEM_MAKE_2
      apid =  xpmem_get_2(gasneti_pshm_segids[pshm_rank], XPMEM_RDWR, XPMEM_PERMIT_MODE, NULL);
    #else
      apid =    xpmem_get(gasneti_pshm_segids[pshm_rank], XPMEM_RDWR, XPMEM_PERMIT_MODE, NULL);
    #endif
      if (apid != (gasneti_xpmem_apid_t)-1) {
        break; // Success
      }
      // Failure with errno == EFAULT has been seen where EINTR was
      // probably intended.  So, we retry w/o regard to actual errno.
      int save_errno = errno;
      GASNETI_TRACE_PRINTF(I, ("xpmem_get() trial %d failed %d(%s)\n",
                               trial, save_errno, strerror(save_errno)));
      errno = save_errno;
    }

    if (apid != (gasneti_xpmem_apid_t)-1) {
    #if HAVE_XPMEM_MAKE_2
      ptr = xpmem_attach_2(apid, 0, segsize, segbase);
    #else
      gasneti_xpmem_addr_t xa;
      xa.offset = 0;
      xa.gasneti_xpmem_apid = apid;
      ptr = xpmem_attach(xa, segsize, segbase);
    #endif
    }
    gasneti_pshm_apids[pshm_rank] = apid;
  }
#else
  #error
#endif

  if_pf (create && (ptr == MAP_FAILED)) {
    const int save_errno = errno;
    gasneti_pshm_unlink(pshm_rank);
    errno = save_errno;
  }
  return ptr;
}

/* Helper: remove local object's mapping from address space */
static void gasneti_pshm_munmap(void *segbase, uintptr_t segsize) {
  gasneti_assert(segsize > 0);
#if defined(GASNETI_PSHM_SYSV)
  if (shmdt(segbase) != 0) {
      gasneti_fatalerror("shmdt("GASNETI_LADDRFMT") failed: %s\n",
	      GASNETI_LADDRSTR(segbase), strerror(errno));
  }
#elif defined(GASNETI_PSHM_FILE) || defined(GASNETI_PSHM_POSIX) || defined(GASNETI_PSHM_XPMEM)
  gasneti_munmap(segbase, segsize);
#else
  #error
#endif
  gasneti_pshm_unlink(gasneti_pshm_mynode);
}

static void gasneti_munmap_remote(gex_Rank_t pshm_rank, void *segbase, uintptr_t segsize) {
  gasneti_assert(segsize > 0);

#if defined(GASNETI_PSHM_SYSV)
  if (shmdt(segbase) != 0) {
      gasneti_fatalerror("shmdt("GASNETI_LADDRFMT") failed: %s\n",
	      GASNETI_LADDRSTR(segbase), strerror(errno));
  }
#elif defined(GASNETI_PSHM_FILE) || defined(GASNETI_PSHM_POSIX)
  gasneti_munmap(segbase, segsize);
#elif defined(GASNETI_PSHM_XPMEM)
 #if HAVE_XPMEM_MAKE_2
  xpmem_detach_2(segbase, segsize);
  xpmem_release_2(gasneti_pshm_apids[pshm_rank]);
 #else
  xpmem_detach(segbase);
  xpmem_release(gasneti_pshm_apids[pshm_rank]);
 #endif
#else
  #error
#endif
}

/* Called collectively */
GASNETI_INLINE(gasneti_publish_segment)
void gasneti_publish_segment(gasnet_seginfo_t segment) {
  void *segbase = segment.addr;
  uintptr_t segsize = segment.size;
#if defined(GASNETI_PSHM_XPMEM)
  /* Create and supernode-exchange xpmem segment ids */
  gasneti_xpmem_segid_t segid =
  #if HAVE_XPMEM_MAKE_2
          xpmem_make_2(segbase, segsize, XPMEM_PERMIT_MODE, (void *)(uintptr_t)0600);
  #else
            xpmem_make(segbase, segsize, XPMEM_PERMIT_MODE, (void *)(uintptr_t)0600);
  #endif
  if_pf (segid == (gasneti_xpmem_segid_t)(-1)) {
    fprintf(stderr, "xpmem_make() failed:%s\n", strerror(errno));
  }
  gasneti_pshmnet_bootstrapExchange(gasneti_request_pshmnet, &segid, sizeof(segid), gasneti_pshm_segids);
#else
  /* empty */
#endif
}

/* Helper: destroy the "attach point" */
static void gasneti_pshm_unlink(int pshm_rank){
#if defined(GASNETI_PSHM_SYSV)
  int shmget_id = shmget(gasneti_pshm_sysvkeys[pshm_rank], 0, 0);
  (void)shmctl(shmget_id, IPC_RMID, NULL);
#elif defined(GASNETI_PSHM_FILE)
  const char *filename = gasneti_pshmname[pshm_rank];
  (void)unlink(filename);
#elif defined(GASNETI_PSHM_POSIX)
  const char *filename = gasneti_pshmname[pshm_rank];
  (void)shm_unlink(filename);
#elif defined(GASNETI_PSHM_XPMEM)
  /* Nothing to do */
#else
  #error
#endif
}

/* gasneti_pshm_unlink() so the shared memory will disappear upon exit.
 * This must be called collectively, because barriers are
 * used to prevent races against shm_open() before or after.
 */
static void gasneti_unlink_segments(void) {
  gasneti_pshmnet_bootstrapBarrier();
#if defined(GASNETI_PSHM_XPMEM)
  /* Nothing to do because gasneti_pshm_unlink() is a no-op */
#else
  gasneti_pshm_unlink(gasneti_pshm_mynode);
  gasneti_pshmnet_bootstrapBarrier();
#endif
}

/* Try to unlink everything we can, ignoring errors */
static void gasneti_cleanup_shm(void) {
#ifdef GASNETI_PSHM_SYSV
  /* Unlink the segments and vnet */
  if (gasneti_pshm_sysvkeys) {
    gasneti_local_rmb();
    for (int i=0; i<gasneti_pshm_nodes+1; ++i) {
      gasneti_pshm_unlink(i);
    }
  #if 0 // TODO-EX: can't create multiple segments with these, but don't yet mark as leaked
    gasneti_free(gasneti_pshm_sysvkeys);
    gasneti_pshm_sysvkeys = NULL;
  #endif
  }
#elif defined(GASNETI_PSHM_FILE) || defined(GASNETI_PSHM_POSIX)
  if (gasneti_pshmname) {
    gasneti_local_rmb();
    /* Unlink the segments and vnet, and free the filenames */
    int i;
    for (i=0; i<gasneti_pshm_nodes+1; ++i) {
      gasneti_pshm_unlink(i);
    }
  #if 0 // TODO-EX: can't create multiple segments with these, but don't yet mark as leaked
    gasneti_free(gasneti_pshmname[0]);
    gasneti_free(gasneti_pshmname);
    gasneti_pshmname = NULL;
  #endif
  }
#elif defined(GASNETI_PSHM_XPMEM)
  #if 0 // TODO-EX: can't create multiple segments with these, but don't yet mark as leaked
    gasneti_free(gasneti_pshm_segids);
    gasneti_pshm_segids = NULL;
    gasneti_free(gasneti_pshm_apids);
    gasneti_pshm_apids = NULL;
  #endif
#else
  #error
#endif

  /* Remove the tmpfile that ensures uniqueness of our filenames */
  if (gasneti_pshm_tmpfile) {
    gasneti_local_rmb();
    (void)unlink(gasneti_pshm_tmpfile);
    gasneti_free(gasneti_pshm_tmpfile);
    gasneti_pshm_tmpfile = NULL;
  }
}

static void *gasneti_mmap_shared_internal(int pshmnode, void *segbase, uintptr_t segsize,
                                          int may_fail) {
  int mmap_errno;
  gasneti_tick_t t1, t2;
  void	*ptr;

  /* 0-byte failure modes can vary by implemenation */
  if (!segsize) {
    if (!may_fail) {
      gasneti_cleanup_shm();
      gasneti_fatalerror("failed to setup 0-byte shared memory file/segment for node %d", pshmnode);
    }
    return MAP_FAILED;
  }

  t1 = gasneti_ticks_now();
  ptr = gasneti_pshm_mmap(pshmnode, segbase, segsize);
  mmap_errno = errno;
  t2 = gasneti_ticks_now();

  GASNETI_TRACE_PRINTF(C, 
      ("mmap %s("GASNETI_LADDRFMT", %"PRIuPTR"): %.3fus => "GASNETI_LADDRFMT"%s%s\n", 
        (segbase == NULL?"":"fixed"),
        GASNETI_LADDRSTR(segbase), segsize,
        gasneti_ticks_to_ns(t2-t1)/1000.0,
        GASNETI_LADDRSTR(ptr),
        (ptr == MAP_FAILED?"  MAP_FAILED: ":""),
        (ptr == MAP_FAILED?strerror(mmap_errno):"")));

  if ((ptr == MAP_FAILED) && !may_fail) {
    gasneti_cleanup_shm();

    if (mmap_errno != ENOMEM) {
      #if PLATFORM_OS_CYGWIN
        if (mmap_errno != EACCES) /* Cygwin stupidly returns EACCES for insuff mem */
      #elif PLATFORM_OS_SOLARIS
        if (mmap_errno != EAGAIN) /* Solaris stupidly returns EAGAIN for insuff mem */
      #endif
      gasneti_fatalerror("unexpected error in mmap%s for size %"PRIuPTR": %s\n", 
                         (segbase == NULL?"":" fixed"),
                         segsize, strerror(mmap_errno));
    }

    if (!segbase) {
      gasneti_fatalerror("mmap failed for size %"PRIuPTR": %s", segsize, strerror(mmap_errno));
    } else {
      gasneti_fatalerror("mmap fixed failed at "GASNETI_LADDRFMT" for size %"PRIuPTR": %s" GASNETI_BUG3480_MSG,
              GASNETI_LADDRSTR(segbase), segsize, strerror(mmap_errno));
    }
  }

  if ((ptr != (void*)GASNETI_PAGE_ALIGNDOWN(ptr)) && (ptr != MAP_FAILED)) {
    gasneti_cleanup_shm();
    gasneti_fatalerror("mmap result "GASNETI_LADDRFMT" is not aligned to GASNET_PAGESIZE %lu (0x%lx)",
              GASNETI_LADDRSTR(ptr), (unsigned long)GASNET_PAGESIZE, (unsigned long)GASNET_PAGESIZE);
  }
#if !GASNETI_PSHM_MAP_FIXED_IGNORED
  if (segbase && (segbase != ptr) && (ptr != MAP_FAILED)) {
    gasneti_cleanup_shm();
    gasneti_fatalerror("mmap fixed moved from "GASNETI_LADDRFMT" to "GASNETI_LADDRFMT" for size %"PRIuPTR,
            GASNETI_LADDRSTR(segbase), GASNETI_LADDRSTR(ptr), segsize);
  }
#endif

  return ptr;
}

static void *gasneti_mmap_remote_shared(void *segbase, uintptr_t segsize, gex_Rank_t pshmnode) {
  gasneti_assert_uint(pshmnode ,<, gasneti_pshm_nodes);
  return gasneti_mmap_shared_internal(pshmnode, segbase, segsize, 0);
}
extern void *gasneti_mmap_shared_fixed(void *segbase, uintptr_t segsize, int mayfail) {
  return gasneti_mmap_shared_internal(gasneti_pshm_mynode, segbase, segsize, mayfail);
}
extern void *gasneti_mmap_shared(uintptr_t segsize) {
  return gasneti_mmap_shared_internal(gasneti_pshm_mynode, NULL, segsize, 1);
}

extern void *gasneti_mmap_vnet(uintptr_t size, gasneti_bootstrapBroadcastfn_t snodebcastfn) {
  void *ptr = MAP_FAILED;
  int save_errno = 0;

  #if defined(GASNETI_PSHM_SYSV) && PLATFORM_OS_CYGWIN
  /* Cygwin may raise SIGSYS when SysV support is absent.
     This will yield more informative error messages. */
  gasneti_sighandlerfn_t prev_handler = gasneti_reghandler(SIGSYS, SIG_IGN);
  #endif

  #if defined(GASNETI_PSHM_FILE) || defined(GASNETI_PSHM_SYSV) || defined(GASNETI_PSHM_POSIX)
  {
    char unique[GASNETI_PSHM_UNIQUE_LEN];

    /* First in each supernode creates the segment plus the names/keys and returns the unique identifier */
    if (gasneti_pshm_mynode == 0) {
      const char *tmp = gasneti_pshm_makeunique(NULL);
      memcpy(unique, tmp, GASNETI_PSHM_UNIQUE_LEN);
      ptr = gasneti_mmap_shared_internal(gasneti_pshm_nodes, NULL, size, 1);
      save_errno = errno;
    }

    /* Supernode-scoped bcast communicates the names/keys generated by the firsts */
    (*snodebcastfn)(unique, GASNETI_PSHM_UNIQUE_LEN, unique, gasneti_pshm_firstnode);
  
    /* Non-first nodes attach */
    if (gasneti_pshm_mynode != 0) {
      (void)gasneti_pshm_makeunique(unique);
      ptr = gasneti_mmap_shared_internal(gasneti_pshm_nodes, NULL, size, 1);
      save_errno = errno;
    }
  }
  #elif defined(GASNETI_PSHM_XPMEM)
  {
    gasneti_xpmem_segid_t segid = (gasneti_xpmem_segid_t)(-1);

    /* Initialization */
    gasneti_pshm_segids = gasneti_malloc(sizeof(gasneti_xpmem_segid_t) * (gasneti_pshm_nodes + 1));
    gasneti_pshm_apids = gasneti_malloc(sizeof(gasneti_xpmem_apid_t) * (gasneti_pshm_nodes + 1));

    /* First in each supernode creates the segment */
    if (gasneti_pshm_mynode == 0) {
      ptr = gasneti_mmap_shared_internal(gasneti_pshm_nodes, NULL, size, 1);
      save_errno = errno;
      if (ptr != MAP_FAILED) {
      #if HAVE_XPMEM_MAKE_2
        segid = xpmem_make_2(ptr, size, XPMEM_PERMIT_MODE, (void *)(uintptr_t)0600);
      #else
        segid =   xpmem_make(ptr, size, XPMEM_PERMIT_MODE, (void *)(uintptr_t)0600);
      #endif
        save_errno = errno;
        if_pf (segid == (gasneti_xpmem_segid_t)(-1)) {
          fprintf(stderr, "xpmem_make() failed:%s\n", strerror(errno));
        }
      }
    }
    
    /* Supernode-scoped bcast communicates the segment identifier generated by the firsts */
    (*snodebcastfn)(&segid, sizeof(segid),
                    &gasneti_pshm_segids[gasneti_pshm_nodes],
                    gasneti_pshm_firstnode);

    /* Non-first nodes attach */
    if (gasneti_pshm_mynode != 0) {
      ptr = gasneti_mmap_shared_internal(gasneti_pshm_nodes, NULL, size, 1);
      save_errno = errno;
    }
  }
  #else
    #error
  #endif

  #if defined(GASNETI_PSHM_SYSV) && PLATFORM_OS_CYGWIN
  gasneti_reghandler(SIGSYS, prev_handler);
  #endif

  /* restore the pertinent errno, if any */
  errno = save_errno;

  return (ptr == MAP_FAILED) ? NULL : ptr;
}
extern void gasneti_unlink_vnet(void) {
  gasneti_pshm_unlink(gasneti_pshm_nodes);
}
#endif /* GASNET_PSHM */

/* ------------------------------------------------------------------------------------ */
#if HAVE_MMAP
extern void gasneti_munmap(void *segbase, uintptr_t segsize) {
  gasneti_tick_t t1, t2;
  gasneti_assert(segsize > 0);
  t1 = gasneti_ticks_now();
  #if defined(GASNETI_USE_HUGETLBFS)
    gasneti_huge_munmap(segbase, segsize);
  #else
    if (munmap(segbase, segsize) != 0) 
      gasneti_fatalerror("munmap("GASNETI_LADDRFMT",%"PRIuPTR") failed: %s\n",
	      GASNETI_LADDRSTR(segbase), segsize, strerror(errno));
  #endif
  t2 = gasneti_ticks_now();

  GASNETI_TRACE_PRINTF(D,("munmap("GASNETI_LADDRFMT", %"PRIuPTR"): %.3fus\n", 
     GASNETI_LADDRSTR(segbase), segsize,
     gasneti_ticks_to_ns(t2-t1)/1000.0) );
}
#endif
/* ------------------------------------------------------------------------------------ */

#if GASNET_PSHM
  #define gasneti_do_mmap       gasneti_mmap_shared
  #define gasneti_do_mmap_fixed gasneti_mmap_shared_fixed
  #define gasneti_do_munmap     gasneti_pshm_munmap
#else
  #define gasneti_do_mmap       gasneti_mmap
  #define gasneti_do_mmap_fixed gasneti_mmap_fixed
  #define gasneti_do_munmap     gasneti_munmap
#endif

#if GASNETI_BUG3480_WORKAROUND
// Bounded retry on FIXED mappings
static void *gasneti_mmap_fixed_with_retry(void *segbase, uintptr_t segsize, int mayfail) {
  const uint64_t max_delay = 2e7;
  uint64_t delay = 1e3;
  const int max_retries = 100;
  int retries = 0;

  void *ptr = gasneti_do_mmap_fixed(segbase, segsize, 1);

  while ((ptr == MAP_FAILED) && (errno == ENOMEM) && (retries++ < max_retries)) {
    GASNETI_TRACE_PRINTF(I, ("Bug 3480: retry #%d delay %gs\n", retries, 1e-9*delay));
    (void) gasneti_nsleep(delay);
    delay = MIN(max_delay, delay * 2);
    ptr = gasneti_do_mmap_fixed(segbase, segsize, 1);
  }

  if (ptr == MAP_FAILED && !mayfail) {
    int save_errno = errno;
  #if GASNET_PSHM
    gasneti_cleanup_shm();
  #endif
    gasneti_fatalerror("mmap fixed failed at "GASNETI_LADDRFMT" for size %"PRIuPTR": %s",
            GASNETI_LADDRSTR(segbase), segsize, strerror(save_errno));
  }

  return ptr;
}
#undef gasneti_do_mmap_fixed
#define gasneti_do_mmap_fixed gasneti_mmap_fixed_with_retry
#endif // GASNETI_BUG3480_WORKAROUND

/* binary search for segment - returns location, not mmaped */
static gasnet_seginfo_t gasneti_mmap_binary_segsrch(uintptr_t lowsz, uintptr_t highsz) {
  gasnet_seginfo_t si;

  if (highsz - lowsz <= GASNETI_MMAP_GRANULARITY) {
    si.size = 0;
    si.addr = NULL;
    return si;
  }

  si.size = gasneti_mmap_aligndown((lowsz + (highsz - lowsz) / 2));
  gasneti_assert(si.size > 0);

  si.addr = gasneti_do_mmap(si.size);

  if (si.addr == MAP_FAILED) 
    return gasneti_mmap_binary_segsrch(lowsz, si.size);
  else {
    gasnet_seginfo_t si_temp;
    gasneti_do_munmap(si.addr, si.size);

    si_temp = gasneti_mmap_binary_segsrch(si.size, highsz);
    if (si_temp.size) return si_temp;
    else return si;
  }
}
/* descending linear search for segment - returns location mmaped */
static gasnet_seginfo_t gasneti_mmap_lineardesc_segsrch(uintptr_t highsz) {
  const uintptr_t pagesize = gasneti_mmap_pagesize();
  gasnet_seginfo_t si;
  si.addr = MAP_FAILED;
  si.size = highsz;
  while (si.addr == MAP_FAILED && si.size > pagesize) {
    si.size -= pagesize;
    si.addr = gasneti_do_mmap(si.size);
  }
  if (si.addr == MAP_FAILED) {
    si.addr = NULL;
    si.size = 0;
  }
  return si;
}
/* ascending linear search for segment - returns location, not mmaped */
static gasnet_seginfo_t gasneti_mmap_linearasc_segsrch(uintptr_t highsz) {
  const uintptr_t pagesize = gasneti_mmap_pagesize();
  gasnet_seginfo_t si;
  gasnet_seginfo_t last_si = { NULL, 0 };
  si.size = pagesize;
  si.addr = gasneti_do_mmap(si.size);

  while (si.addr != MAP_FAILED && si.size <= highsz) {
    last_si = si;
    gasneti_do_munmap(last_si.addr, last_si.size);
    si.size += pagesize;
    si.addr = gasneti_do_mmap(si.size);
  }
  if (si.addr == MAP_FAILED) return last_si;
  else {
    gasneti_do_munmap(si.addr, si.size);
    return si;
  }
}

/* Differs from gasneti_mmap_segment_search() in that:
 * + maxsz must already be page aligned and non-zero
 * + zero-length result is not an error
 */
static gasnet_seginfo_t _gasneti_mmap_segment_search_inner(uintptr_t maxsz) {
  gasnet_seginfo_t si;
  uintptr_t prevsz = 0;
  int mmaped = 0;

again:
  gasneti_assert(maxsz);
  gasneti_assert_uint(maxsz ,==, gasneti_mmap_aligndown(maxsz));

  si.addr = gasneti_do_mmap(maxsz);
  if (si.addr != MAP_FAILED) { /* succeeded at max value - done */
    si.size = maxsz;
    mmaped = 1;
  } else { /* use a search to find largest possible */
    #if 0
      /* linear descending search best on systems with 
         fast mmap-failed and very slow unmap and/or mmap-succeed */
      si = gasneti_mmap_lineardesc_segsrch(maxsz);
      mmaped = 1;
    #elif 0
      /* linear ascending search best on systems with 
         fast mmap-succeed and fast unmap but very slow mmap-failed */
      si = gasneti_mmap_linearasc_segsrch(maxsz);
      mmaped = 0;
    #else
      /* binary search best for systems with 
         well-balanced mmap performance */
      si = gasneti_mmap_binary_segsrch(0, maxsz);
      mmaped = 0;
    #endif
  }

  if (si.addr == NULL) {
    si.size = 0;
    return si;
  }

  gasneti_assert(si.addr != NULL && si.addr != MAP_FAILED && si.size > 0);
  gasneti_assert_uint(si.size % GASNET_PAGESIZE ,==, 0);
  if (mmaped && ((uintptr_t)si.addr) % GASNET_PAGESIZE == 0) {
    /* aligned and mmaped - nothing to do */
  } else { /* need to page-align base */
    if (mmaped) gasneti_do_munmap(si.addr, si.size); 
    // ensure alignment of base and size to expected granularity, even if system's page size is smaller
    { uintptr_t begin = (uintptr_t)si.addr;
      uintptr_t end = (uintptr_t)si.addr + si.size;
      begin = GASNETI_PAGE_ALIGNUP(begin);
      end = GASNETI_PAGE_ALIGNDOWN(end);
      si.addr = (void *)begin;
      si.size = end - begin;
      if (si.size == 0) goto fail;
    }
    // perform a *fixed* mapping to preserve alignment achieved above
    void *ptr = gasneti_do_mmap_fixed(si.addr, si.size, 1);
    if (ptr == MAP_FAILED && errno == ENOMEM) {
      // Failed to mmap a size at which the probe had once succeeded (bug 3812).
      // Attempt bounded linear-descending retries.
      const uintptr_t pagesize = gasneti_mmap_pagesize();
      for (int i = 1; i <= 4; ++i) {
        if (si.size <= pagesize) goto fail;
        si.size -= pagesize;
        ptr = gasneti_do_mmap_fixed(si.addr, si.size, 1);
        if (ptr != MAP_FAILED) {
          GASNETI_TRACE_PRINTF(I, ("Segment search final mmap trimmed by %"PRIuPTR, i * pagesize));
          break;
        } else if (errno != ENOMEM) {
          // unexpected failure - skip to "second chance" or give up
          break;
        }
      }
    }
    if (ptr == MAP_FAILED) {
      if (!prevsz) {
        // "Second chance" (used at most once)
        // Restart using the most recent failed size as the new maximum
        GASNETI_TRACE_PRINTF(I, ("Segment search final mmap failed.  Starting second-chance search."));
        prevsz = maxsz = si.size;
        goto again;
      } else {
        goto fail;
      }
    }
#if GASNETI_PSHM_MAP_FIXED_IGNORED
    si.addr = ptr;
#else
    gasneti_assert_ptr(si.addr ,==, ptr);
#endif
  }

  if (prevsz) {
    GASNETI_TRACE_PRINTF(I, ("Segment search second-chance reduced size by %"PRIuPTR, prevsz - si.size));
  }
  GASNETI_TRACE_PRINTF(I, ("Segment search final size: %"PRIuPTR, si.size));

  gasneti_assert(si.addr != NULL && si.addr != MAP_FAILED && si.size > 0);
  gasneti_assert_uint(((uintptr_t)si.addr) % GASNET_PAGESIZE ,==, 0);
  gasneti_assert_uint(si.size % GASNET_PAGESIZE ,==, 0);
  return si;

fail:
  GASNETI_TRACE_PRINTF(I, ("Segment search failed (found max size of 0)"));
  si.addr = NULL;
  si.size = 0;
  return si;
}

/* gasneti_mmap_segment_search allocates the largest possible page-aligned mmap 
 * with sz <= maxsz and returns the base address and size
 */
extern gasnet_seginfo_t gasneti_mmap_segment_search(uintptr_t maxsz) {
  gasnet_seginfo_t si;

  maxsz = gasneti_mmap_aligndown(maxsz);
  if (maxsz == 0) {
    si.size = 0;
    si.addr = NULL;
    return si;
  }

  si = _gasneti_mmap_segment_search_inner(maxsz);

  if (si.addr == NULL) 
    gasneti_fatalerror("Unable to find an adequate mmap segment.");

  return si;
}

/* ------------------------------------------------------------------------------------ */
#endif /* GASNETI_MMAP_OR_PSHM */

GASNETI_IDENT(gasneti_IdentString_DefaultMaxSegsizeStr, 
              "$GASNetDefaultMaxSegsizeStr: " GASNETI_MAX_SEGSIZE_CONFIGURE " $");

/* return user-selected limit for the max segment size, as gleaned from several sources */
const char *gasnet_max_segsize_str; // intentional tentative definition, to allow client override
uint64_t gasnet_max_segsize;        // DEPRECATED: intentional tentative definition, to allow client override 
uintptr_t gasneti_max_segsize() {
  static uintptr_t result = 0;
  uint64_t tmp;
  if (!result) {
    uintptr_t auxsegsz = gasneti_auxseg_preinit();
    uint64_t pph = gasneti_myhost.node_count;
    gasneti_assert(pph > 0);
    /* start with the configure-selected default */
    const char *dflt = GASNETI_MAX_SEGSIZE_CONFIGURE;
    /* next, check the compile-time overrides */
    if (gasnet_max_segsize) { // lower-priority deprecated override, interpreted as /p
      static char tmp[80];
      snprintf(tmp,sizeof(tmp),"%"PRIu64"/p",gasnet_max_segsize);
      dflt = tmp;
    }
    if (gasnet_max_segsize_str) { // higher-priority string override
      dflt = gasnet_max_segsize_str;
    }

    #if PLATFORM_ARCH_32 && !defined(GASNETI_ALLOW_HUGE_32BIT_SEGMENT)
      /* need to be careful about overflow on 32-bit:
         can't use a full 4 GB due to sign bit problems 
         on the int argument to mmap() for some 32-bit systems
         so use 2GB - pagesz 
      */
      uint64_t hardmax = (((uint64_t)1)<<31) - GASNET_PAGESIZE;
    #else
      uint64_t hardmax = (uint64_t)-1; // unlimited
    #endif

    // finally, check the environment override, parse the result and factor in min/max/auxseg
    uint64_t val = gasneti_getenv_memsize_withdefault("GASNET_MAX_SEGSIZE", dflt,
                                                GASNET_PAGESIZE, hardmax,
                                                gasneti_getPhysMemSz(1), pph,
                                                0);

    // round UP to nearest huge page, if needed, to ensure we don't truncate client's MAX_SEGSIZE request
    val = gasneti_mmap_alignup(val);

    gasneti_assert_uint(val ,==, GASNETI_PAGE_ALIGNDOWN(val));
    gasneti_assert_uint(val ,>=, GASNET_PAGESIZE);
    gasneti_assert_uint(val ,<=, hardmax);
    result = (uintptr_t)val;
    gasneti_assert_uint(result ,==, val); // overflow check
  }
  return result;
}

#if GASNETI_BUG3480_WORKAROUND
  // Barrier used between unmap and re-map, via 1-byte exchange (a.k.a. GatherAll).
  // This is a bit of a hack, but is the most expedient way to get a barrier
  // with compute-node scope, since gasneti_pshmnet_bootstrapBarrier() may
  // have a narrower scope when env var GASNET_SUPERNODE_MAXSIZE is set.
  static void gasneti_bug3480_fence(gasneti_bootstrapExchangefn_t exchangefn) {
    char a = 0; char *b = gasneti_malloc(gasneti_nodes);
    (*exchangefn)(&a, sizeof(char), b);
    gasneti_free(b);
  }
#else
  #define gasneti_bug3480_fence(_e) ((void)0)
#endif

// gasneti_sharedLimit()
//
// Returns the per-host shared memory limit ("the limit") imposed by the active
// PSHM implementation and current resource utilization.  If PSHM is not
// enabled, or if there are no limits specific to the implementation, returns
// gasneti_getPhysMemSz() or (uint64_t)-1 if that call should fail.
//
// Limits by PSHM implementation:
//   FILE:  free space reported by fstatvfs() if available
//   POSIX: free space reported by fstatvfs() if available
//   SYSV:  UNIMPLEMENTED (but probably not needed)
//          We are not yet checking for the "shmall" or "shmmaxpgs" limits,
//          from which we'd need to subtract the current segments as one
//          might see with `ipcs`.  HOWEVER, unlike the filesystem limits
//          for FILE and POSIX, we believe that the segmentLimit() probe
//          will observe the actual free resources, and NOT overcommit.
//   XPMEM: NONE - just windows into other proc's address space
//
uint64_t gasneti_sharedLimit(void) {
  uint64_t sharedLimit;
  
  // Start with appropriate system-dependent memory limits
  {
    const uint64_t hostmem = gasneti_getPhysMemSz(0);
    sharedLimit = hostmem ? hostmem : (uint64_t)-1;
  }


#if GASNET_PSHM && (GASNETI_PSHM_FILE || GASNETI_PSHM_POSIX) && HAVE_FSTATVFS
  {
    // Apply limits appropriate to filesystem-backed allocation
    const int flags = O_RDWR | O_CREAT | O_EXCL;
    const mode_t mode = S_IRUSR | S_IWUSR;
    const char *filename = gasneti_pshmname[gasneti_pshm_mynode];
    struct statvfs buf;

  #if GASNETI_PSHM_POSIX
    int fd = shm_open(filename, flags, mode);
  #else
    int fd = open(filename, flags, mode);
  #endif

    if (fd >= 0) {
      // TODO: for now we ignore any errors here
      if (0 == fstatvfs(fd, &buf)) {
        uint64_t free_space = buf.f_bsize * buf.f_bavail;
        if (free_space) {
          sharedLimit = MIN(sharedLimit, free_space);
        }
      }
      (void) close(fd);
    #if GASNETI_PSHM_POSIX
      (void) shm_unlink(filename);
    #else
      (void) unlink(filename);
    #endif
    }
  }
#endif

  return sharedLimit;
}

#ifdef GASNETI_MMAP_OR_PSHM
/* perform a coordinated mmap probe to determine the max memory
    that can be mmap()ed while considering multiple GASNet nodes
    per shared memory node
   localLimit is an optional conduit-specific upper limit per GASNet node
   sharedLimit is an optional upper limit per host
   requires an exchangefn callback function that can be used to exchange data
   and a barrierfn callback to perform a barrier
   returns a value suitable for use as localSegmentLimit in a call
    to gasneti_segmentInit()
   
   for exchangefn and barrierfn: the implementations are only required to
    perform their functions with respect the peers on a host
    (though exchangefn does require a "full" third argument).
    however, global implementations are acceptible
 */
uintptr_t gasneti_segmentLimit(uintptr_t localLimit, uint64_t sharedLimit,
                            gasneti_bootstrapExchangefn_t exchangefn,
                            gasneti_bootstrapBarrierfn_t barrierfn) {
  int i;
  const gex_Rank_t local_count = gasneti_myhost.node_count;

#if GASNET_PSHM
  gasneti_pshm_cs_enter(&gasneti_cleanup_shm);
#endif

  // This is assumed implictly
  gasneti_assert_uint(gasneti_mmap_pagesize() ,>=, GASNETI_PAGESIZE);

  gasneti_assert(exchangefn);
  gasneti_assert(barrierfn); /* No longer optional */
  gasneti_assert(gasneti_nodemap);

  /* Apply intial limits, even if not sharing nodes */
  uintptr_t auxsegsz = gasneti_auxseg_preinit();
  uintptr_t maxsz = MAX(GASNETI_MMAP_LIMIT, auxsegsz);
  maxsz = gasneti_mmap_alignup(maxsz);
  if ((uint64_t)localLimit > sharedLimit) localLimit = sharedLimit;
  maxsz = MIN(maxsz, localLimit);

  /* Coordinate the search IFF there are any shared nodes. */
  if (gasneti_myhost.grp_count != gasneti_nodes) {
    uintptr_t *sz_exchg = gasneti_calloc(gasneti_nodes, sizeof(uintptr_t));
    gasnet_seginfo_t se = {0,0};

    /* Ensure our probe will not collectively exceed the shareLimit, if any. */
    if ((sharedLimit != (uint64_t)-1) && (local_count > 1)) {
      uint64_t tmp = sharedLimit / local_count;
      tmp = MIN(tmp,(uintptr_t)-1); // avoid overflowing 32-bit maxsz on ILP32
      maxsz = MIN(maxsz,tmp);
    }

    /* Allow each node in a given host to probe SEQUENTIALLY, and then collect the results */
    maxsz = gasneti_mmap_aligndown(maxsz);
#if GASNET_PSHM
    if (maxsz && (gasneti_myhost.grp_count == gasneti_mysupernode.grp_count)) { /* host==supernode */
      for (i = 0; i < gasneti_nodemap_local_count; ++i) {
        if (i == gasneti_nodemap_local_rank) {
          se = _gasneti_mmap_segment_search_inner(maxsz);
          maxsz = se.size;
        }
        /* Bcast because we can use "declining expectations" to potentially speed later probes */
        gasneti_pshmnet_bootstrapBroadcast(gasneti_request_pshmnet, &maxsz, sizeof(uintptr_t), &maxsz, i);
        sz_exchg[gasneti_nodemap_local[i]] = maxsz;
        if (!maxsz) break;
      }
    } else
#endif
    if (maxsz) {
      /* Find widest host */
      gex_Rank_t rounds = 0;
      {
        const gex_Rank_t num_hosts = gasneti_myhost.grp_count;
        gex_Rank_t *tmp = gasneti_calloc(num_hosts, sizeof(gex_Rank_t));
        for (i = 0; i < gasneti_nodes; ++i) {
          const gex_Rank_t host = gasneti_nodeinfo[i].host;
          gasneti_assert_uint(host ,<, num_hosts);
          tmp[host] += 1;
          rounds = MAX(rounds, tmp[host]);
        }
        gasneti_free(tmp);
      }

      for (i = 0; i < rounds; ++i) {
        if (i == gasneti_myhost.node_rank) {
          se = _gasneti_mmap_segment_search_inner(maxsz);
        }
        (*barrierfn)();
      }
    }
    (*exchangefn)(&se.size, sizeof(uintptr_t), sz_exchg);

    /* Compute the host-local mean */
    { uint64_t sum = 0;
      for (i = 0; i < local_count; ++i) {
        sum += sz_exchg[gasneti_myhost.nodes[i]];
      }
      maxsz = gasneti_mmap_aligndown(sum / local_count);

#if GASNET_PSHM
      /* The probe completed has determined how much memory we can map.
       * However, we now need to be sure that multiple segments will fit.
       * The limit could be, for instance, due to
       * + RLIMIT_AS or RLIMIT_VMEM
       * + size of the holes in the address space (especially on 32-bit arch)
       * NOTE: must use pshm's view of supernode, which may be less than nodemap's.
       */
      if (se.size) gasneti_do_munmap(se.addr, se.size);
      gasneti_unlink_segments(); /* Includes supernode-scoped barrier to complete munmap()s */
      if (gasneti_myhost.grp_count != gasneti_mysupernode.grp_count) {
        // num_hosts != num_supernodes (multiple supernodes on at least one node)
        // Lacking a node-scoped barrier, we require a full barrier to complete munmap()s
        (*barrierfn)();
      }
      se.size = 0;

      if (gasneti_pshm_mynode == 0 && maxsz) {
        gasnet_seginfo_t *tmp_se = gasneti_calloc(gasneti_pshm_nodes,sizeof(gasnet_seginfo_t));
	int done;
	/* Iterate until we find a size for which N segments fit.
	 * Ideally the first pass finds that the size probed above works.
	 * If rlimit or total address space limit the mapping, then the first
	 *   iteration determines the right size and the second verifies it.
	 * If we are limited by the size of the holes in the address space
	 *   then it may take several iterations to work out what fits.
	 * TODO: If we reach a 3rd pass perhaps we could try something else?
	 */
	do {
          sum = 0; done = 1;
          for (i = 0; i < gasneti_pshm_nodes; ++i) {
            tmp_se[i] = _gasneti_mmap_segment_search_inner(maxsz);
            gasneti_pshm_unlink(gasneti_pshm_mynode);
            sum += tmp_se[i].size;
	    if (tmp_se[i].size != maxsz) {
	      done = 0;
	      if (tmp_se[i].size < GASNETI_MMAP_GRANULARITY) break;
            }
          }
          for (i = 0; i < gasneti_pshm_nodes; ++i) {
            if (tmp_se[i].size) gasneti_do_munmap(tmp_se[i].addr, tmp_se[i].size);
            tmp_se[i].size = 0;
          }
          maxsz = gasneti_mmap_aligndown(sum / gasneti_pshm_nodes);
        } while (!done && maxsz);
        gasneti_free(tmp_se);
      }

      /* supernode-local communication of the maxsz results */
      gasneti_pshmnet_bootstrapBroadcast(gasneti_request_pshmnet, &maxsz, sizeof(uintptr_t), &maxsz, 0);

      /* Unlink the shared segments to prevent leaks (they are recreated in segmentInit) */
      gasneti_unlink_segments();
#endif
    }

    /* Free held resources */
    gasneti_free(sz_exchg);
    if (se.size) gasneti_do_munmap(se.addr, se.size);
    (*barrierfn)(); /* Ensures munmap()s complete on-node before return */
  }

#if GASNET_PSHM
  gasneti_pshm_cs_leave();
#endif
  return maxsz;
}
#endif /* GASNETI_MMAP_OR_PSHM */

// State between segmentInit() and segmentAttach()
#ifdef GASNETI_MMAP_OR_PSHM
static gasnet_seginfo_t gasneti_presegment = {0,0};
#endif
#if GASNET_ALIGNED_SEGMENTS
static uintptr_t gasneti_maxbase;
#endif

/* do the work necessary for initing a standard segment map in arbitrary memory 
     uses mmap if available, or malloc otherwise
   requires an exchange callback function that can be used to exchange data
   sets max local & global segment size
   localSegmentLimit provides an optional conduit-specific limit on max segment sz
    (for example, to limit size based on physical memory availability)
    pass (uintptr_t)-1 for unlimited
    Use of gasneti_segmentLimit() can help determine the right value to pass here
   keeps internal state for attach
 */
void gasneti_segmentInit(uintptr_t localSegmentLimit,
                         gasneti_bootstrapExchangefn_t exchangefn,
                         gex_Flags_t flags)
{
  const int legacy_mode = flags & GASNETI_FLAG_INIT_LEGACY;
#if GASNET_PSHM
  gasneti_pshm_cs_enter(&gasneti_cleanup_shm);
#endif

  gasneti_assert_uint(gasneti_MaxLocalSegmentSize ,==, 0);
  gasneti_assert_uint(gasneti_MaxGlobalSegmentSize ,==, 0);
  gasneti_assert(exchangefn);
  gasneti_assert_uint(gasneti_nodes ,>, 0);
  gasneti_assert_uint(gasneti_mynode ,<, gasneti_nodes);

#if GASNET_SEGMENT_EVERYTHING

  // Initialize global data
  gasneti_leak(gasneti_seginfo    = gasneti_malloc(gasneti_nodes*sizeof(gasnet_seginfo_t)));
  for (gex_Rank_t i = 0; i < gasneti_nodes; i++) {
    gasneti_seginfo[i].addr = NULL;
    gasneti_seginfo[i].size = (uintptr_t)-1;
  }
  gasneti_MaxLocalSegmentSize = (uintptr_t)-1;
  gasneti_MaxGlobalSegmentSize = (uintptr_t)-1;

#else // GASNET_SEGMENT_EVERYTHING

  // PART 0: allocate (and zero-initialize) global data

  gasneti_leak(gasneti_seginfo    = gasneti_calloc(gasneti_nodes, sizeof(gasnet_seginfo_t)));

  // PART I: allocate "pre-segment"

#ifdef GASNETI_MMAP_OR_PSHM
  localSegmentLimit = MIN(localSegmentLimit, GASNETI_MMAP_LIMIT);
#endif
  localSegmentLimit = gasneti_mmap_aligndown(localSegmentLimit);

  #ifdef GASNETI_MMAP_OR_PSHM
    // NOTE: If the conduit did not derive localSegmentLimit from a call to
    // gasneti_segmentLimit(), then this call might lead to unexpected failures
    // (such as bug 651) due to it's lack of coordination among processes.
    gasneti_presegment = gasneti_mmap_segment_search(localSegmentLimit);

    GASNETI_TRACE_PRINTF(C, ("My segment: addr="GASNETI_LADDRFMT"  sz=%"PRIuPTR,
      GASNETI_LADDRSTR(gasneti_presegment.addr), gasneti_presegment.size));
  #else
    #if GASNET_ALIGNED_SEGMENTS && !GASNET_CONDUIT_SMP
      #error bad config: dont know how to provide GASNET_ALIGNED_SEGMENTS when !HAVE_MMAP
    #endif
  #endif

#if GASNET_PSHM
  gasneti_unlink_segments();
  gasneti_pshm_cs_leave();
#endif

  // PART II: Exchange segment info if needed for later stage
  // TODO-EX: Non-scalable exchange where a reduce is probably sufficient

  #ifdef GASNETI_MMAP_OR_PSHM
  gasnet_seginfo_t *gasneti_segexch = NULL;
  if (1) { // TODO-EX: in anticipation of alignment and Max*SegmentSize being runtime choices
    /* gather the mmap segment location */
    gasneti_segexch = gasneti_malloc(gasneti_nodes*sizeof(gasnet_seginfo_t));
    (*exchangefn)(&gasneti_presegment, sizeof(gasnet_seginfo_t), gasneti_segexch);
  }
  #endif

  // PART III: Optionally align segments

  #if GASNET_ALIGNED_SEGMENTS && defined(GASNETI_MMAP_OR_PSHM)
  if (legacy_mode) { // Aligned segments only available to GASNet-1 clients
      if (gasneti_nodes > 1) {
        /* bug 2067 - detect if the compute nodes are using Linux's 'intentional VM space randomization'
         * security feature, which is known to break GASNET_ALIGNED_SEGMENTS, esp at large scale
         */
         FILE *fp = fopen("/proc/sys/kernel/randomize_va_space", "r");
         if (fp) {
           int VMrand = fgetc(fp);
           if (VMrand != EOF && VMrand != '0') {
             const char *wmsg = "WARNING: It appears your compute nodes are using a Linux security feature "
                                "which intentionally randomizes the virtual address space, "
                                "but GASNet was configured to optimize for congruent address spaces. "
                                "You probably need to re-configure with --disable-aligned-segments to avoid "
                                "errors at job startup (especially for runs with large node count or shared segment size).";
             GASNETI_TRACE_MSG(I, wmsg);
             if (!gasneti_getenv_yesno_withdefault("GASNET_QUIET",0)) {
               fprintf(stderr, "%s\n", wmsg);
               fflush(stderr);
             }
           }
           fclose(fp);
         }
      }

    // Find highest start and lowest end among all segments
    uintptr_t maxbase = (uintptr_t)gasneti_segexch[0].addr;
    uintptr_t minend  = (uintptr_t)gasneti_segexch[0].addr + gasneti_segexch[0].size;
    for (int i = 1; i < gasneti_nodes; i++) {
      maxbase = MAX(maxbase, (uintptr_t)gasneti_segexch[i].addr);
      minend  = MIN(minend,  (uintptr_t)gasneti_segexch[i].addr + gasneti_segexch[i].size);
    }

    char alignstats[255];
    snprintf(alignstats, sizeof(alignstats),
        "Segment alignment stats: "
        "maxbase = "GASNETI_LADDRFMT"   "
        "minend = "GASNETI_LADDRFMT"   ",
        GASNETI_LADDRSTR(maxbase), GASNETI_LADDRSTR(minend));
    alignstats[sizeof(alignstats)-1] = '\0';
    GASNETI_TRACE_MSG(C, alignstats);

    if (maxbase >= minend) { /* no overlap - maybe should be a fatal error... */
      const char *wmsg = "WARNING: unable to locate overlapping mmap segments: "
                         "perhaps you need to re-configure with --disable-aligned-segments";
      GASNETI_TRACE_MSG(I, wmsg);
      if (!gasneti_mynode && !gasneti_getenv_yesno_withdefault("GASNET_QUIET",0)) {
        fprintf(stderr, "%s\n%s\n", wmsg, alignstats);
        for (int i = 0; i < gasneti_nodes; i++) {
          fprintf(stderr, " %i: seg=["GASNETI_LADDRFMT","GASNETI_LADDRFMT"]"
                          " size=%"PRIuPTR"\n", i,
                  GASNETI_LADDRSTR(gasneti_segexch[i].addr),
                  GASNETI_LADDRSTR(((uintptr_t)gasneti_segexch[i].addr)+gasneti_segexch[i].size),
                  gasneti_segexch[i].size);
          fflush(stderr);
        }
      }

      // Zero my size (in two place) to yield Max{Local,Global}SegmentSize == 0
      gasneti_presegment.size = 0;
      gasneti_segexch[gasneti_mynode].size = 0;
    } else {
      // Trim sizes to reflect begining at maxbase (for finding MaxGlobalSegmentSize)
      for (int i = 0; i < gasneti_nodes; i++) {
        gasneti_segexch[i].size -= (maxbase - (uintptr_t)gasneti_segexch[i].addr);
      }
    }
    gasneti_maxbase = maxbase;
  }
  #endif

  // PART IV: Compute Max{Local,Remote}SegmentSize

  if (1) { // TODO-EX: in anticipation of Max{Local,Global}SegmentSize becoming optional
  #ifdef GASNETI_MMAP_OR_PSHM
    /* compute min and max sizes across nodes */
    uintptr_t maxsize = gasneti_segexch[0].size;
    uintptr_t minsize = gasneti_segexch[0].size;
    for (int i = 1; i < gasneti_nodes; i++) {
      maxsize = MAX(maxsize, gasneti_segexch[i].size);
      minsize = MIN(minsize, gasneti_segexch[i].size);
    }

    #if GASNET_TRACE
      char segstats[255];
      snprintf(segstats, sizeof(segstats),
          "Segment stats: "
          "maxsize = %"PRIuPTR"   "
          "minsize = %"PRIuPTR"   ",
          maxsize, minsize);
      segstats[sizeof(segstats)-1] = '\0';
      GASNETI_TRACE_MSG(C, segstats);
    #endif

    gasneti_MaxLocalSegmentSize = gasneti_presegment.size;
    gasneti_MaxGlobalSegmentSize = minsize;
  #else /* !GASNETI_MMAP_OR_PSHM */
    /* some systems don't support mmap - 
       TODO: safe mechanism to determine a true max seg sz, 
       for now just trust gasneti_max_segsize */
    gasneti_MaxLocalSegmentSize = GASNETI_PAGE_ALIGNDOWN(MIN(localSegmentLimit, gasneti_max_segsize()));
    gasneti_MaxGlobalSegmentSize = gasneti_MaxLocalSegmentSize;
  #endif

    GASNETI_TRACE_PRINTF(C, ("MaxLocalSegmentSize = %"PRIuPTR"   MaxGlobalSegmentSize = %"PRIuPTR,
                           gasneti_MaxLocalSegmentSize, gasneti_MaxGlobalSegmentSize));

    gasneti_assert_uint(gasneti_MaxLocalSegmentSize % GASNET_PAGESIZE ,==, 0);
    gasneti_assert_uint(gasneti_MaxGlobalSegmentSize % GASNET_PAGESIZE ,==, 0);
    gasneti_assert_uint(gasneti_MaxGlobalSegmentSize ,<=, gasneti_MaxLocalSegmentSize);
    gasneti_assert_uint(gasneti_MaxLocalSegmentSize ,<=, localSegmentLimit);
  }

  // PART V: Discard the pre-segment if not supporting GASNet-1's attach
#ifdef GASNETI_MMAP_OR_PSHM
  if (!legacy_mode) {
    if (gasneti_presegment.addr) gasneti_do_munmap(gasneti_presegment.addr, gasneti_presegment.size);
    gasneti_presegment.addr = NULL;
    gasneti_presegment.size = 0;
  }
#endif

#ifdef GASNETI_MMAP_OR_PSHM
  gasneti_free(gasneti_segexch);
#endif
#endif // GASNET_SEGMENT_EVERYTHING
}

/* ------------------------------------------------------------------------------------ */

static // TODO-EX: static for now, at least
void gasneti_segmentAttachLocal(gasnet_seginfo_t *segment_p, uintptr_t segsize,
                                gasneti_bootstrapExchangefn_t exchangefn)
{
  void *segbase = NULL;

  #ifdef GASNETI_MMAP_OR_PSHM
  {
    if (segsize == 0) { /* no segment */
      if (segment_p->addr && segment_p->size) {
        gasneti_do_munmap(segment_p->addr, segment_p->size);
      }
      segbase = NULL; 
    } else if (segment_p->addr) { /* a pre-segment exists */
    #if GASNET_ALIGNED_SEGMENTS
      segbase = (void*)gasneti_maxbase;
    #else
      segbase = segment_p->addr;
    #endif
      gasneti_assert_uint(segsize ,<=, segment_p->size);
      if (GASNET_PSHM || (segment_p->size != segsize) || (segment_p->addr != segbase)) {
        gasneti_do_munmap(segment_p->addr, segment_p->size);
        gasneti_bug3480_fence(exchangefn);
#if GASNETI_PSHM_MAP_FIXED_IGNORED
        segbase =
#endif
        gasneti_do_mmap_fixed(segbase, segsize, 0);
      } else {
        gasneti_bug3480_fence(exchangefn);
      }
    } else { /* need segment from scratch */
      segbase = gasneti_do_mmap(segsize);
      if (MAP_FAILED == segbase) {
        // TODO-EX: improve error handling here (e.g. ENOMEM might be returned to caller)
        int mmap_errno = errno;
      #if GASNET_PSHM
        gasneti_cleanup_shm();
      #endif
        gasneti_fatalerror("mmap failed for segment of size %"PRIuPTR": %s",
                           segsize, strerror(mmap_errno));
      }
    }
  }
  #else /* !GASNETI_MMAP_OR_PSHM */
    /* for the T3E, and other platforms which don't support mmap */
    // TODO_EX: if we think non-mmap support remains important:
    //    + move to segmentInit and use presegment
    //    + perform full binary search
    //    + use hidden base (as in gasneti_malloc_aligned) to support free
    segbase = gasneti_malloc_allowfail(segsize + GASNET_PAGESIZE);
    while (!segbase) {
      segsize = GASNETI_PAGE_ALIGNDOWN(segsize/2);
      if (segsize == 0) break; 
      segbase = gasneti_malloc_allowfail(segsize + GASNET_PAGESIZE);
    }
    if (segbase) {
      segbase = (void *)GASNETI_PAGE_ALIGNUP(segbase);
    }
  #endif /* GASNETI_MMAP_OR_PSHM */
  gasneti_assert_uint(((uintptr_t)segbase) % GASNET_PAGESIZE ,==, 0);
  gasneti_assert_uint(segsize % GASNET_PAGESIZE ,==, 0);
  GASNETI_TRACE_PRINTF(C, ("Final segment: segbase="GASNETI_LADDRFMT"  segsize=%"PRIuPTR,
    GASNETI_LADDRSTR(segbase), segsize));

  segment_p->addr = segbase;
  segment_p->size = segsize;
}

#if GASNET_PSHM
/* Map the remote shared segments */
// TODO-EX: need scalable data structure in place of gasneti_nodeinfo
static // TODO-EX: static for now, at least
void gasneti_segmentAttachRemote(gasnet_seginfo_t *seginfo)
{
    gasneti_nodeinfo[gasneti_mynode].offset = 0;
    gasneti_pshm_rank_t local_rank = 0;

    gasneti_publish_segment(seginfo[gasneti_mynode]);

    // Note that we try to avoid iteration over all nodes.
    // For the case of supernode peers with contiguous ranks we examine no extra nodes
    for (gex_Rank_t node = gasneti_pshm_firstnode; local_rank < gasneti_pshm_nodes; node++) {
        if (! gasneti_pshm_jobrank_in_supernode(node)) continue;
        gasneti_assert_uint(local_rank ,==, gasneti_pshm_jobrank_to_local_rank(node));
        if (node != gasneti_mynode) {

            const uintptr_t size = seginfo[node].size;
            void *segbase = gasneti_mmap_remote_shared(NULL, size, local_rank);

            gasneti_assert_uint(((uintptr_t)segbase) % GASNET_PAGESIZE ,==, 0);
            gasneti_assert_uint(size % GASNET_PAGESIZE ,==, 0);

            // TODO-EX: single global gasneti_nodeinfo is a problem (eg for aux vs client)
            gasneti_nodeinfo[node].offset = (uintptr_t)segbase - (uintptr_t)seginfo[node].addr;

            GASNETI_TRACE_PRINTF(C, ("Remote segment %d: segbase="GASNETI_LADDRFMT"  segsize=%"PRIuPTR,
                                     (int)node, GASNETI_LADDRSTR(segbase), size));
        }
        ++local_rank;
    }

    /* Barrier #1 ensures all attaches complete before unlinking */
    gasneti_pshmnet_bootstrapBarrier();
    gasneti_cleanup_shm();
    /* Barrier #2 ensures unlinking completes before return, so crashes cannot leak segments */
    gasneti_pshmnet_bootstrapBarrier();
}
#endif /* GASNET_PSHM */

static gasnet_seginfo_t
gasneti_do_attach_segment(
                           uintptr_t segsize,
                           gasnet_seginfo_t *all_segments,
                           gasneti_bootstrapExchangefn_t exchangefn,
                           gex_Flags_t flags)
{
  gasneti_assert(all_segments);
  gasneti_assert(exchangefn);

#if GASNET_PSHM
  /* Avoid leaking shared memory files in case of non-collective exit between init/attach */
  gasneti_pshm_cs_enter(&gasneti_cleanup_shm);
  gasneti_pshmnet_bootstrapBarrier();
#endif

  gasnet_seginfo_t local_segment = {0,0};
#ifdef GASNETI_MMAP_OR_PSHM
  /* in "legacy_mode" we consume the presegment, otherwise working from scratch */
  if (flags & GASNETI_FLAG_INIT_LEGACY) local_segment = gasneti_presegment;
#endif
  
  gasneti_segmentAttachLocal(&local_segment, segsize, exchangefn);

  /*  gather segment information */   // TODO-EX: need scalable replacement
  (*exchangefn)(&local_segment, sizeof(gasnet_seginfo_t), all_segments);

#if GASNET_PSHM
  gasneti_segmentAttachRemote(all_segments);
  gasneti_pshm_cs_leave();
#endif

  return local_segment;
}

/* ------------------------------------------------------------------------------------ */
gasnet_seginfo_t gasneti_segmentAttach(
                gex_Segment_t                 *segment_p,
                size_t                        allocsz,
                gex_TM_t                      tm,
                uintptr_t                     segsize,
                gasneti_bootstrapExchangefn_t exchangefn,
                gex_Flags_t                   flags)
{
#if GASNET_DEBUG
  // TODO-EX: detection of multiple calls until we support them
  static int called = 0;
  gasneti_assert(!called);
  called = 1;
#endif

  /* ------------------------------------------------------------------------------------ */
  /*  register segment  */

  gasnet_seginfo_t myseg = gasneti_do_attach_segment(segsize, gasneti_seginfo, exchangefn, flags);

  void *segbase = myseg.addr;
  segsize = myseg.size;

  gasneti_assert_uint(((uintptr_t)segbase) % GASNET_PAGESIZE ,==, 0);
  gasneti_assert_uint(segsize % GASNET_PAGESIZE ,==, 0);

  gasneti_EP_t ep = gasneti_import_tm(tm)->_ep;
  ep->_segment = gasneti_alloc_segment(ep->_client, segbase, segsize, flags, allocsz);
  gasneti_legacy_segment_attach_hook(ep);
  *segment_p = gasneti_export_segment(ep->_segment);
  
  // After local segment is attached, call optional client-provided hook
  if (gasnet_client_attach_hook) {
    gasnet_client_attach_hook(segbase, segsize);
  }

  // sanity check (but should be true "by construction")
  gasneti_assert_ptr(gasneti_seginfo[gasneti_mynode].addr ,==, segbase);
  gasneti_assert_uint(gasneti_seginfo[gasneti_mynode].size ,==, segsize);

  return myseg;
}
/* ------------------------------------------------------------------------------------ */

/* Used to pass the nodemap information to the client
 * Similar to gasneti_getSegmentInfo(). 
 * */
extern int gasneti_getNodeInfo(gasnet_nodeinfo_t *nodeinfo_table, int numentries) {
  GASNETI_CHECKINIT();

  if_pf (numentries <= 0) {
    if (numentries == 0) return GASNET_OK;
    else GASNETI_RETURN_ERR(BAD_ARG);
  }
  gasneti_assert(nodeinfo_table);
  if_pf (numentries > gasneti_nodes) numentries = gasneti_nodes;

  if (gasneti_nodeinfo) {
    memcpy(nodeinfo_table, gasneti_nodeinfo, numentries*sizeof(gasnet_nodeinfo_t));
  } else {
    gex_Rank_t i;

    for (i=0; i < numentries; i++) {
      nodeinfo_table[i].host = i;
      nodeinfo_table[i].supernode = i;
    #if GASNET_PSHM
      nodeinfo_table[i].offset = 0;
    #endif
    }
  }

  return GASNET_OK;
}

// Provides information about supernode peers (the caller's "neighborhood")
//
// TODO: Could improve "safety" by not exposing addr of critical internal data
// TODO: Could reduce memory use if array lived in shared memory
extern void gex_System_QueryNbrhdInfo(
            gex_RankInfo_t         **info_p,
            gex_Rank_t             *info_count_p,
            gex_Rank_t             *my_info_index_p)
{
  GASNETI_CHECKINIT();
#if GASNET_PSHM
  if (info_p) {
    gasneti_static_assert(sizeof(gex_RankInfo_t) == sizeof(gex_Rank_t));
    *info_p = (gex_RankInfo_t *) gasneti_mysupernode.nodes;
  }
  if (info_count_p) {
    *info_count_p = gasneti_mysupernode.node_count;
  }
  if (my_info_index_p) {
    *my_info_index_p = gasneti_mysupernode.node_rank;
  }
#else
  if (info_p) {
    gasneti_static_assert(sizeof(gex_RankInfo_t) == sizeof(gex_Rank_t));
    *info_p = (gex_RankInfo_t *) &gasneti_mynode;
  }
  if (info_count_p) {
    *info_count_p = 1;
  }
  if (my_info_index_p) {
    *my_info_index_p = 0;
  }
#endif
}

// Provides information about compute-node peers (same O/S image, files system, etc.)
//
// TODO: Could improve "safety" by not exposing addr of critical internal data
// TODO: Could reduce memory use if array lived in shared memory
extern void gex_System_QueryHostInfo(
            gex_RankInfo_t         **info_p,
            gex_Rank_t             *info_count_p,
            gex_Rank_t             *my_info_index_p)
{
  GASNETI_CHECKINIT();
  if (info_p) {
    gasneti_static_assert(sizeof(gex_RankInfo_t) == sizeof(gex_Rank_t));
    *info_p = (gex_RankInfo_t *) gasneti_myhost.nodes;
  }
  if (info_count_p) {
    *info_count_p = gasneti_myhost.node_count;
  }
  if (my_info_index_p) {
    *my_info_index_p = gasneti_myhost.node_rank;
  }
}

extern void gex_System_QueryMyPosition(
            gex_Rank_t *nbrhd_set_size_p,
            gex_Rank_t *nbrhd_set_rank_p,
            gex_Rank_t *host_set_size_p,
            gex_Rank_t *host_set_rank_p)
{
  GASNETI_CHECKINIT();
  if (nbrhd_set_size_p) {
    *nbrhd_set_size_p = gasneti_mysupernode.grp_count;
  }
  if (nbrhd_set_rank_p) {
    *nbrhd_set_rank_p = gasneti_mysupernode.grp_rank;
  }
  if (host_set_size_p) {
    *host_set_size_p = gasneti_myhost.grp_count;
  }
  if (host_set_rank_p) {
    *host_set_rank_p = gasneti_myhost.grp_rank;
  }
}

/* ------------------------------------------------------------------------------------ */
/* seginfo initialization and manipulation */
extern int gasneti_getSegmentInfo(gasnet_seginfo_t *seginfo_table, int numentries) {
  GASNETI_CHECKATTACH();
  gasneti_memcheck(gasneti_seginfo_aux);
  gasneti_memcheck(gasneti_seginfo);
  #if GASNET_DEBUG /*  sanity checks */
    #if GASNET_SEGMENT_EVERYTHING
    { int i; /*  sanity check seg-everything condition */
      for (i=0; i < gasneti_nodes; i++) {
        gasneti_assert(gasneti_seginfo[i].addr == 0 && gasneti_seginfo[i].size == (uintptr_t)-1);
      }
    }
    #else
    { int i; 
      #if GASNET_ALIGNED_SEGMENTS
      void *segbase = NULL;
      #endif
      for (i=0; i < gasneti_nodes; i++) {
        if (gasneti_seginfo_aux[i].size == 0) {
          gasneti_assert(gasneti_seginfo_aux[i].addr == 0);
          gasneti_assert(gasneti_seginfo[i].addr == 0);
        } else {
          #if GASNET_ALIGNED_SEGMENTS
          if (gasneti_maxbase) { // non-zero IFF gasneti_segmentInit() passed GASNETI_FLAG_INIT_LEGACY
            /*  sanity check that segments are aligned */
            if (!segbase) segbase = gasneti_seginfo[i].addr;
            else if (gasneti_seginfo[i].addr != segbase)
              gasneti_fatalerror("Failed sanity check for aligned segments with GASNET_ALIGNED_SEGMENTS");
          }
          #endif
          /*  sanity check that client and aux segments do not overlap */
          if (gasneti_seginfo[i].size > 0) {
            uintptr_t seg_ub = (uintptr_t)gasneti_seginfo[i].addr + gasneti_seginfo[i].size;
            uintptr_t aux_ub = (uintptr_t)gasneti_seginfo_aux[i].addr + gasneti_seginfo_aux[i].size;
            gasneti_assert((seg_ub <= (uintptr_t)gasneti_seginfo_aux[i].addr) ||
                           (aux_ub <= (uintptr_t)gasneti_seginfo[i].addr));
          }
          /* misc segment sanity checks */
          gasneti_assert_uint(((uintptr_t)gasneti_seginfo_aux[i].addr) % GASNET_PAGESIZE ,==, 0);
          gasneti_assert_uint(((uintptr_t)gasneti_seginfo[i].addr) % GASNET_PAGESIZE ,==, 0);
        }
      }
    }
    #endif
  #endif
  if_pf (numentries <= 0) {
    if (numentries == 0) return GASNET_OK;
    else GASNETI_RETURN_ERR(BAD_ARG);
  }
  gasneti_assert(seginfo_table);
  if_pf (numentries > gasneti_nodes) numentries = gasneti_nodes;
  memcpy(seginfo_table, gasneti_seginfo, numentries*sizeof(gasnet_seginfo_t));
  return GASNET_OK;
}

int gasneti_Segment_QueryBound(
                        gex_TM_t tm,
                        gex_Rank_t rank,
                        void **owneraddr_p,
                        void **localaddr_p,
                        uintptr_t *size_p)
{
  // Trivial implementation using legacy data structures and assumptions.
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);

  // TODO-EX: cannot yet tell no segment from zero-length segment
  gasneti_assert(gasneti_seginfo);
  if (!gasneti_seginfo[jobrank].addr) return 1; // No (bound) segment

  if (owneraddr_p) {
    *owneraddr_p = gasneti_seginfo[jobrank].addr;
  }

  if (size_p){
    *size_p = gasneti_seginfo[jobrank].size;
  }

  if (localaddr_p) {
    if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    #if GASNET_PSHM
      gasneti_assert(gasneti_nodeinfo);
      *localaddr_p = (void*)((uintptr_t)gasneti_seginfo[jobrank].addr + gasneti_nodeinfo[jobrank].offset);
    #else
      *localaddr_p = gasneti_seginfo[jobrank].addr;
    #endif
    } else {
      *localaddr_p = NULL;
    }
  }

  return 0;
}

/* ------------------------------------------------------------------------------------ */
/* Aux-seg support */

#include <gasnet_core_internal.h> /* for GASNETC_AUXSEG_{FNS,DECLS} */

/* lists of internal functions that want auxseg (with trailing commas) */
/* conduit-specific auxseg fns */
#ifndef GASNETC_AUXSEG_FNS
#define GASNETC_AUXSEG_FNS() 
#endif
/* extended-ref auxseg fns */
#ifndef GASNETE_AUXSEG_FNS
#define GASNETE_AUXSEG_FNS() 
#endif
/* extended-ref collectives auxseg fns */
#ifndef GASNETE_COLL_AUXSEG_FNS
#define GASNETE_COLL_AUXSEG_FNS() 
#endif

gasneti_auxseg_request_t gasneti_auxseg_dummy(gasnet_seginfo_t *auxseg_info);
#ifdef GASNETC_AUXSEG_DECLS
  GASNETC_AUXSEG_DECLS
#endif
#ifdef GASNETE_AUXSEG_DECLS
  GASNETE_AUXSEG_DECLS
#endif
#ifdef GASNETE_COLL_AUXSEG_DECLS
  GASNETE_COLL_AUXSEG_DECLS
#endif

gasneti_auxsegregfn_t gasneti_auxsegfns[] = {
  GASNETC_AUXSEG_FNS()
  GASNETE_AUXSEG_FNS()
  GASNETE_COLL_AUXSEG_FNS()
  #if GASNET_DEBUG
    gasneti_auxseg_dummy, 
  #endif
  NULL
};

/* page-aligned size of auxseg */
static gasneti_auxseg_request_t gasneti_auxseg_total_alignedsz = { 0, 0 };
static gasneti_auxseg_request_t *gasneti_auxseg_alignedsz = NULL;

#if GASNET_DEBUG
  #define GASNETI_AUXSEG_DUMMY_SZ    463

  gasneti_auxseg_request_t gasneti_auxseg_dummy(gasnet_seginfo_t *auxseg_info) {
    gasneti_auxseg_request_t retval;
    static gasnet_seginfo_t *auxseg_save = NULL;
    int i, selftest=0;
    retval.minsz = 213;
    retval.optimalsz = GASNETI_AUXSEG_DUMMY_SZ;
    if (auxseg_info == NULL) return retval; /* initial query */
    if (auxseg_info == (void*)(uintptr_t)-1) { /* self test */
      selftest = 1;
      gasneti_assert(auxseg_save);
    } else { /* auxseg granted */
      gasneti_assert(!auxseg_save);
      auxseg_save = gasneti_malloc(gasneti_nodes*sizeof(gasnet_seginfo_t));
      memcpy(auxseg_save, auxseg_info, gasneti_nodes*sizeof(gasnet_seginfo_t));
      gasneti_leak(auxseg_save); /* Needed by self test, if any */
    }
    for (i=0; i < gasneti_nodes; i++) {
      gasneti_assert(auxseg_save[i].addr);
      gasneti_assert_uint(((uintptr_t)auxseg_save[i].addr) % GASNETI_CACHE_LINE_BYTES ,==, 0);
      gasneti_assert_uint(((uintptr_t)auxseg_save[i].addr) % 8 ,==, 0);
      gasneti_assert_uint(auxseg_save[i].size ,>=, retval.minsz);
      gasneti_assert_uint(auxseg_save[i].size ,<=, retval.optimalsz);
    }
    for (i=0; i < auxseg_save[gasneti_mynode].size; i++) {
      uint8_t *p = (uint8_t *)auxseg_save[gasneti_mynode].addr;
      #define AUXSEG_TESTVAL(i) ((uint8_t)(8|((i+0x3F)^(i>>8))))
      if (selftest) gasneti_assert_uint(p[i] ,==, AUXSEG_TESTVAL(i));
      else p[i] = AUXSEG_TESTVAL(i);
      #undef AUXSEG_TESTVAL
    }
    return retval;
  }
#endif

// collect and return optimal auxseg size sum, padded to page size
// may be called multiple times, subsequent calls return cached value
uintptr_t gasneti_auxseg_preinit(void) {
  static uintptr_t gasneti_auxseg_sz = 0;
  if (gasneti_auxseg_sz) return gasneti_auxseg_sz; // only the first call computes requirements

  const int numfns = (sizeof(gasneti_auxsegfns)/sizeof(gasneti_auxsegregfn_t))-1;
  gasneti_assert(gasneti_auxsegfns[numfns] == NULL);
  if (numfns > 0) {
    gasneti_auxseg_alignedsz = gasneti_calloc(numfns,sizeof(gasneti_auxseg_request_t));
    gasneti_leak(gasneti_auxseg_alignedsz);
  }

  /* collect requests */
  for (int i = 0; i < numfns; i++) {
    gasneti_auxseg_alignedsz[i] = (gasneti_auxsegfns[i])(NULL);
    gasneti_auxseg_total_alignedsz.minsz += 
      GASNETI_ALIGNUP(gasneti_auxseg_alignedsz[i].minsz,GASNETI_CACHE_LINE_BYTES);
    gasneti_auxseg_total_alignedsz.optimalsz += 
      GASNETI_ALIGNUP(gasneti_auxseg_alignedsz[i].optimalsz,GASNETI_CACHE_LINE_BYTES);
  }
  gasneti_auxseg_total_alignedsz.minsz = 
    gasneti_mmap_alignup(gasneti_auxseg_total_alignedsz.minsz);
  gasneti_auxseg_total_alignedsz.optimalsz = 
    gasneti_mmap_alignup(gasneti_auxseg_total_alignedsz.optimalsz);

  gasneti_auxseg_sz = gasneti_auxseg_total_alignedsz.optimalsz;
  GASNETI_TRACE_PRINTF(C, ("gasneti_auxseg_preinit(): gasneti_auxseg_sz = %"PRIuPTR, gasneti_auxseg_sz));
  gasneti_assert_uint(gasneti_auxseg_sz % gasneti_mmap_pagesize() ,==, 0);
  return gasneti_auxseg_sz;
}

/* provide auxseg to GASNet components and init secondary segment arrays 
   requires input seginfo has been initialized to the correct values
 */
void gasneti_auxseg_attach(gasnet_seginfo_t *auxseg_info) {
  const int numfns = (sizeof(gasneti_auxsegfns)/sizeof(gasneti_auxsegregfn_t))-1;
  gasneti_assert(gasneti_auxsegfns[numfns] == NULL);

  if (numfns > 0) {
    gasnet_seginfo_t *si = gasneti_malloc(gasneti_nodes*sizeof(gasnet_seginfo_t));
    memcpy(si, auxseg_info, gasneti_nodes*sizeof(gasnet_seginfo_t));

    uintptr_t auxseg_sz = auxseg_info[gasneti_mynode].size;

    for (int j = 0; j < gasneti_nodes; j++) {
      gasneti_assert_uint((uintptr_t)si[j].addr % GASNET_PAGESIZE ,==, 0);
      gasneti_assert_uint((uintptr_t)si[j].addr % GASNETI_CACHE_LINE_BYTES ,==, 0);
      gasneti_assert_uint((uintptr_t)si[j].size ,==, auxseg_sz);
      si[j].size = gasneti_auxseg_alignedsz[0].optimalsz;
    }

    for (int i = 0; i < numfns; i++) {
      GASNETI_TRACE_PRINTF(C,("gasneti_auxseg_attach() fn[%i] => ("GASNETI_LADDRFMT".."GASNETI_LADDRFMT") (%"PRIuPTR" bytes)",
                      i, GASNETI_LADDRSTR(si[gasneti_mynode].addr), 
                      GASNETI_LADDRSTR(((uintptr_t)si[gasneti_mynode].addr)+si[gasneti_mynode].size),
                      si[gasneti_mynode].size));
      (gasneti_auxsegfns[i])(si);
      if (i+1 < numfns) {
        for (int j = 0; j < gasneti_nodes; j++) {
          si[j].addr = (void *)(((uintptr_t)si[j].addr) + gasneti_auxseg_alignedsz[i].optimalsz);
          si[j].addr = (void *)GASNETI_ALIGNUP(si[j].addr,GASNETI_CACHE_LINE_BYTES);
          si[j].size = gasneti_auxseg_alignedsz[i+1].optimalsz;
        }
      }
    }
    gasneti_free(si);
  }

#if GASNET_PSHM // TODO-EX: this is a hack until AttachRemote can set the right offset array
  for (int i = 0; i < gasneti_pshm_nodes; i++){
    const gex_Rank_t node = gasneti_nodemap_local[i];
    gasneti_nodeinfo[node].auxoffset = gasneti_nodeinfo[node].offset;
  }
#endif
}

/* common case use of gasneti_auxseg_{preinit,attach} for conduits using gasneti_segmentAttach() */
gasnet_seginfo_t
gasneti_auxsegAttach(uint64_t maxsize, gasneti_bootstrapExchangefn_t exchangefn)
{
  uintptr_t auxsize = gasneti_auxseg_preinit();
  if (auxsize > maxsize) {
    /* TODO: implement request downsizing down to minsz */
    gasneti_fatalerror("GASNet internal auxseg size (%"PRIuPTR" bytes) exceeds available limit (%"PRIu64" bytes)",
                       auxsize, maxsize);
  }
  gasneti_leak(gasneti_seginfo_aux    = gasneti_malloc(gasneti_nodes*sizeof(gasnet_seginfo_t)));
  gasnet_seginfo_t local_segment = gasneti_do_attach_segment(auxsize, gasneti_seginfo_aux, exchangefn, 0);
  gasneti_auxseg_attach(gasneti_seginfo_aux);
  gasneti_assert_uint(gasneti_seginfo_aux[gasneti_mynode].size ,==, auxsize);
  return local_segment;
}

/* ------------------------------------------------------------------------------------ */
// AM-based gasneti_bootstrapExchangefn_t

static gasneti_weakatomic32_t gasneti_exchg_rcvd[2][32]; // Implicitly zero-initialized

static uint8_t *_gasneti_exchg_data[2] = {NULL,NULL};
static uint8_t *gasneti_exchg_data(int phase, size_t elemsz) {
  uint8_t *data = _gasneti_exchg_data[phase];
  if_pf (! data) {
    static gasneti_mutex_t lock = GASNETI_MUTEX_INITIALIZER;
    gasneti_mutex_lock(&lock);
    data = _gasneti_exchg_data[phase];
    if (! data) {
      data = gasneti_malloc(elemsz * gasneti_nodes);
      _gasneti_exchg_data[phase] = data;
    }
    gasneti_mutex_unlock(&lock);
  }
  return data;
}

extern void gasnetc_exchg_reqh(gex_Token_t token, void *buf, size_t nbytes,
                               gex_AM_Arg_t arg0, gex_AM_Arg_t elemsz) {
    const int phase = arg0 & 1;
    const int step = (arg0 >> 1) & 0x1f; // Max 2^5 steps            => 2^32 nodes
    const int seq  = (arg0 >> 6);        // Max 2^26 fragments * 512 => 32GB (and max sent is elemsz*nodes/2)
    const int distance = (1 << step);
    gasneti_assert_uint(distance ,<, gasneti_nodes);
    uint8_t *data = gasneti_exchg_data(phase, elemsz);
    uint8_t *dest = data + (elemsz * distance) + (seq * gex_AM_LUBRequestMedium());
    gasneti_assert_ptr(dest + nbytes ,<=, data + elemsz * gasneti_nodes);
    memcpy(dest, buf, nbytes);
    gasneti_weakatomic32_increment(&gasneti_exchg_rcvd[phase][step], GASNETI_ATOMIC_REL);
}

extern void gasneti_defaultExchange(void *src, size_t elemsz, void *dst) {
  static int phase = 0;
  gasneti_sync_reads();

  uint8_t *data = gasneti_exchg_data(phase, elemsz);

  /* copy in local contribution */
  memcpy(data, src, elemsz);

  /* Bruck's concatenation algorithm: */
  unsigned int step, distance;
  for (step = 0, distance = 1; distance < gasneti_nodes; ++step, distance *= 2) {
    gex_Rank_t peer = (distance <= gasneti_mynode) ? gasneti_mynode - distance
                                                        : gasneti_mynode + (gasneti_nodes - distance);
    size_t nbytes = elemsz * MIN(distance, gasneti_nodes - distance);
    size_t offset = 0;
    uint32_t seq = 0;

    /* Send payload using AMMedium(s) */
    do {
      const size_t to_xfer = MIN(nbytes, gex_AM_LUBRequestMedium());
      gex_AM_RequestMedium(gasneti_THUNK_TM, peer, _hidx_gasnetc_exchg_reqh,
                               data + offset, to_xfer, GEX_EVENT_NOW, 0,
                               phase | (step << 1) | (seq << 6), (uint32_t)elemsz);
      ++seq;
      offset += to_xfer;
      nbytes -= to_xfer;
    } while (nbytes);

    /* Poll until we have received the same number of messages as we sent */
    GASNET_BLOCKUNTIL((int)gasneti_weakatomic32_read(&gasneti_exchg_rcvd[phase][step], 0) >= (int)seq);
    gasneti_assert_int((int)gasneti_weakatomic32_read(&gasneti_exchg_rcvd[phase][step], 0) ,==, (int)seq);
    gasneti_weakatomic32_set(&gasneti_exchg_rcvd[phase][step], 0, 0);
  }

#if GASNET_PSHM
  // Cannot use AMPSHM and pshm bootstrap collectives in same pshmnet barrier phase
  gasneti_pshmnet_bootstrapBarrierPoll();
#endif

  /* Copy to final destination while performing the rotation */
  const size_t a = elemsz * (gasneti_nodes - gasneti_mynode);
  const size_t b = elemsz * gasneti_mynode;
  memcpy(dst, data + a, b);
  memcpy((uint8_t*)dst + b, data, a);
  gasneti_assert(! memcmp((uint8_t*)dst + gasneti_mynode*elemsz, src, elemsz));

  gasneti_free(data);
  _gasneti_exchg_data[phase] = NULL;

  gasneti_sync_writes();
  phase ^= 1;
}

/* ------------------------------------------------------------------------------------ */
