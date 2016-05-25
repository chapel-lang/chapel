/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/blcr/gasnet_blcr.c $
 * Description: implementation of GASNet+BLCR support routines
 * Copyright 2016, Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_blcr.h>
#include <libcr.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

uint64_t gasneti_checkpoint_guid = 0;
uint32_t gasneti_checkpoint_sequence = 0;

char *gasneti_checkpoint_jobdir = NULL;

/* Routine for default guid construction
 *
 * Concatenate2
 * - All 32 bits of hostid
 * - 16 "inner" bits of time (advance every 34min, wrap every 4yr)
 * - lower 16 bits of pid
 */
extern uint64_t gasneti_checkpoint_mkguid(void) {
    uint32_t now = time(NULL);
    uint32_t mypid = getpid();
    uint32_t myhostid = gasneti_gethostid();
    return GASNETI_MAKEWORD(myhostid, ((now << 5) & 0xffff0000) | (mypid & 0x0000ffff));
}

extern void gasneti_checkpoint_init(gasneti_bootstrapBroadcastfn_t bcast_fn) {
  /* Initialize the GUID if the conduit has not already set a non-zero value */
  if (!gasneti_checkpoint_guid) {
    if (! gasneti_mynode) {
      gasneti_checkpoint_guid = gasneti_checkpoint_mkguid();
    }
    bcast_fn(&gasneti_checkpoint_guid, sizeof(uint64_t), &gasneti_checkpoint_guid, 0);
  }

  /* Enforce use of absolute paths */
  {
    char *val;
    if (NULL != (val = gasneti_getenv("GASNET_CHECKPOINT_JOBDIR"))) {
      if ('/' != val[0]) {
        gasneti_fatalerror("Environment variable GASNET_CHECKPOINT_JOBDIR='%s' is not an absolute path", val);
      } else {
        gasneti_checkpoint_jobdir = val;
      }
    } else {
      char *dir;
      size_t len;
      if (NULL != (val = gasneti_getenv("GASNET_CHECKPOINT_BASEDIR"))) {
        if ('/' != val[0]) {
          gasneti_fatalerror("Environment variable GASNET_CHECKPOINT_BASEDIR='%s' is not an absolute path", val);
        } else {
          dir = val;
        }
      } else {
        if (NULL != (val = gasneti_getenv("HOME"))) {
          if ('/' != val[0]) {
            gasneti_fatalerror("Environment variable HOME='%s' is not an absolute path", val);
          } else {
            const char *rest = "/gasnet-checkpoint";
            len = strlen(val) + strlen(rest) + 1;
            dir = gasneti_malloc(len);
            strcpy(dir, val);
            strcat(dir, rest);
          }
        } else {
          gasneti_fatalerror("Environment variable HOME is not set");
        }
      }
      len = strlen(dir) + 19; /* 19 = 16 digits, '/' , '.' and '\0' */
      gasneti_checkpoint_jobdir = gasneti_malloc(len);
      gasneti_leak(gasneti_checkpoint_jobdir);
      snprintf(gasneti_checkpoint_jobdir, len, "%s/%08x.%08x", dir,
               GASNETI_HIWORD(gasneti_checkpoint_guid),
               GASNETI_LOWORD(gasneti_checkpoint_guid));
      if (dir != val) gasneti_free(dir);
    }
  }
}

/* Prepare the checkpoint directory, optionally choosing it as well.
 *
 * If the 'dir' argument is non-NULL, it is used as the checkpoint directory and
 * is also the return value.  Otherwise the default directory name is chosen (as
 * described below) and its name (in malloced memory) is returned.
 *
 * Regardles of whether the directory name is caller-provided or default, the
 * directory is prepared as follows:
 * + The directory and all parents are created if they did not already exist.
 * + A 'metadata' file is created, storing info useful at restart time.
 *
 * Default checkpoint directory names are of the form
 *     [jobdir]/[sequence]
 * where
 *   [jobdir] is one of the following (the first w/o an undefined variable):
 *     1. ${GASNET_CHECKPOINT_JOBDIR}
 *     2. ${GASNET_CHECKPOINT_BASEDIR}/[guid_hi].[guid_lo]
 *     3. ${HOME}/gasnet-checkpoint/[guid_hi].[guid_lo]
 * and
 *   [sequence] is a decimal integer (increased on each call).
 *
 * NOT thread-safe (but neither is checkpoint initiation).
 */
extern const char *gasneti_checkpoint_dir(const char *dir) {
  char *filename;

  if (!dir) {
    size_t len = strlen(gasneti_checkpoint_jobdir) + 12; /* 12 = "/0123456789\0" */
    char *tmp = gasneti_malloc(len);
    snprintf(tmp, len, "%s/%d", gasneti_checkpoint_jobdir, gasneti_checkpoint_sequence++);
    dir = tmp;
  }

  /* Make a copy of 'dir' which we can write to as needed */
  filename = gasneti_malloc(10 + strlen(dir)); /* 10 = "/metadata\0" */
  strcpy(filename, dir);

  { /* The following implements "mkdir -p" (and needs to write to filename) */
    char *p = filename;
    int rc;
    do {
      p = strchr(p+1, '/');
      if (p) { *p = '\0'; }
      rc = mkdir(filename, S_IRWXU);
      if ((rc < 0) && (errno != EEXIST)) {
        gasneti_fatalerror("Failed to mkdir('%s') %d:%s\n", filename, errno, strerror(errno));
        /* BLCR-TODO: error recovery */
      }
      if (p) { *p = '/'; }
    } while (p);
  }

  /* Create metadata file.
   * Since every process tries, we get one copy per filesystem.
   */
  strcat(filename, "/metadata");
  { /* Want O_EXCL, but not available directly via fopen() */
    int fd = open(filename, O_CREAT|O_EXCL|O_WRONLY, S_IRUSR);
    if (fd >= 0) {
      FILE *md = fdopen(fd, "w");
      if (md) {
        /* BLCR-TODO: error detection or silent failure for fwrite() or fclose()? */
        fprintf(md, "argv0:\t%s\n", gasneti_exename);
        fprintf(md, "nproc:\t%d\n", gasneti_nodes);
        fprintf(md, "guid:\t%08x.%08x\n",
                    GASNETI_HIWORD(gasneti_checkpoint_guid),
                    GASNETI_LOWORD(gasneti_checkpoint_guid));
        fprintf(md, "time:\t%lu\n", (unsigned long)time(NULL));
        fclose(md);
      } else {
        gasneti_fatalerror("Failed to fdopen file '%s' %d:%s\n", filename, errno, strerror(errno));
      }
    } else if (errno != EEXIST) {
      gasneti_fatalerror("Failed to create file '%s' %d:%s\n", filename, errno, strerror(errno));
    }
  }

  gasneti_free(filename);
  return dir;
}

/* Create the caller's context file */
extern int gasneti_checkpoint_create(const char *dir) {
  const int flags = O_WRONLY|O_APPEND|O_CREAT|O_EXCL|O_LARGEFILE|O_TRUNC;
  const int mode = S_IRUSR;
  char *filename;
  size_t len;
  int fd;

  gasneti_assert(NULL != dir);

  len = strlen(dir) + 19; /* 19 = "/context.123456789\0" */
  filename = gasneti_malloc(len);
  snprintf(filename, len, "%s/context.%d", dir, gasneti_mynode);
  fd = open(filename, flags, mode);
  gasneti_free(filename);

  if (fd < 0) { /* BLCR-TODO: error checking/recovery */
    gasneti_fatalerror("Failed to create '%s' errno=%d(%s)\n", filename, errno, strerror(errno));
  }

  return fd;
}
  

/* Write process state to the provided context file */
extern int gasneti_checkpoint_write(int fd) {
  cr_checkpoint_handle_t cr_handle;
  int rc;

  {   /* Request the checkpoint */
    cr_checkpoint_args_t cr_args;
    cr_initialize_checkpoint_args_t(&cr_args);
    cr_args.cr_scope  = CR_SCOPE_PROC;
    cr_args.cr_flags  = CR_CHKPT_ASYNC_ERR; /* defers reporting of most errors to "reap" */
    cr_args.cr_target = 0; /* self */
    cr_args.cr_fd = fd;

    rc = cr_request_checkpoint(&cr_args, &cr_handle);
    /* BLCR-TODO: error checking for cr_request_checkpoint() */
  }

  do { /* This loop is necessary because checkpointing self causes EINTR */
    rc = cr_wait_checkpoint(&cr_handle, NULL);
    /* BLCR-TODO: error checking for cr_wait_checkpoint() */
  } while ((rc < 0) && (errno == EINTR));

  rc = cr_reap_checkpoint(&cr_handle);
  if (rc >= 0) {
    /* Continue case */
    return 0;
  } else if (errno == CR_ERESTARTED) {
    /* Restart case */
    return 1;
  } else { /* Error case */
    return -1;
  }
}

/* Create/write/close context file containing caller's process state */
extern int gasneti_checkpoint_self(const char *dir) {
  int fd, rc;

  gasneti_assert(NULL != dir);

  fd = gasneti_checkpoint_create(dir);
  /* BLCR-TODO: error handling (curently _create() dies on error) */

  rc = gasneti_checkpoint_write(fd);
  /* BLCR-TODO: error handling/recovery */

  if (!rc) { /* Continue case */
    (void)close(fd);
  }

  return rc;
}
