/*
 * Copyright © 2009-2023 Inria.  All rights reserved.
 * Copyright © 2009-2012 Université Bordeaux
 * Copyright © 2009-2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "private/autogen/config.h"
#include "hwloc.h"
#ifdef HWLOC_LINUX_SYS
#include "hwloc/linux.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
#include <fcntl.h>

#include "common-ps.h"
#include "misc.h"

int hwloc_ps_read_process(hwloc_topology_t topology, hwloc_const_bitmap_t topocpuset,
			  struct hwloc_ps_process *proc,
			  unsigned long flags)
{
#ifdef HAVE_DIRENT_H
  hwloc_pid_t realpid;
  hwloc_bitmap_t cpuset;
  unsigned pathlen;
  char *path;
  int fd;
  ssize_t n;

  if (hwloc_pid_from_number(&realpid, proc->pid, 0, 0 /* ignore failures */) < 0)
    return -1;

  cpuset = hwloc_bitmap_alloc();
  if (!cpuset)
    return -1;

  pathlen = 6 + 21 + 1 + 7 + 1; /* enough for /proc/%ld/cmdline /proc/%ld/comm and /proc/%ld/stat */
  path = malloc(pathlen);

  snprintf(path, pathlen, "/proc/%ld/cmdline", proc->pid);
  fd = open(path, O_RDONLY);
  if (fd < 0) {
    free(path);
    goto out;
  }
  n = read(fd, proc->name, sizeof(proc->name) - 1);
  close(fd);
  if (n <= 0) {
    /* Ignore kernel threads and errors */
    free(path);
    goto out;
  }
  proc->name[n] = '\0';

  if (flags & HWLOC_PS_FLAG_SHORTNAME) {
    /* try to get a small name from comm */
    char comm[16] = "";
    snprintf(path, pathlen, "/proc/%ld/comm", proc->pid);
    fd = open(path, O_RDONLY);
    if (fd >= 0) {
      n = read(fd, comm, sizeof(comm) - 1);
      close(fd);
      if (n > 0) {
	comm[n] = '\0';
	if (n > 1 && comm[n-1] == '\n')
	  comm[n-1] = '\0';
      }

    } else {
      /* Old kernel, have to look at old file */
      char stats[32];
      char *parenl = NULL, *parenr;

      snprintf(path, pathlen, "/proc/%ld/stat", proc->pid);
      fd = open(path, O_RDONLY);
      if (fd >= 0) {
	/* "pid (comm) ..." */
	n = read(fd, stats, sizeof(stats) - 1);
	close(fd);
	if (n > 0) {
	  stats[n] = '\0';
	  parenl = strchr(stats, '(');
	  parenr = strchr(stats, ')');
	  if (!parenr)
	    parenr = &stats[sizeof(stats)-1];
	  *parenr = '\0';
	  if (parenl)
	    snprintf(comm, sizeof(comm), "%s", parenl+1);
	}
      }
    }

    if (*comm)
      snprintf(proc->name, sizeof(proc->name), "%s", comm);
  }

  free(path);

  proc->string[0] = '\0'; /* might be set later if hwloc_ps_pidcmd is called */

  if (flags & HWLOC_PS_FLAG_UID) {
    proc->uid = HWLOC_PS_ALL_UIDS;
#ifdef HWLOC_LINUX_SYS
    pathlen = 6 + 21 + 1 + 6 + 1;
    path = malloc(pathlen);
    snprintf(path, pathlen, "/proc/%ld/status", proc->pid);
    fd = open(path, O_RDONLY);
    if (fd >= 0) {
      char status[1024];
      char *uid;
      if (read(fd, &status, sizeof(status)) > 0) {
        status[1023] = '\0';
        uid = strstr(status, "Uid:");
        if (uid)
          proc->uid = strtoul(uid+4, NULL, 0);
        close(fd);
      }
    }
    free(path);
#endif
    /* On *BSD, parse the end of the single-line in /proc/pid/status
     * (but the format is different between FreeBSD and NetBSD).
     * It may be a good time to switch to a portable library for gathering this info.
     */
  }

  if (flags & HWLOC_PS_FLAG_THREADS) {
#ifdef HWLOC_LINUX_SYS
    /* check if some threads must be displayed */
    DIR *taskdir;

    pathlen = 6 + 21 + 1 + 4 + 1;
    path = malloc(pathlen);
    snprintf(path, pathlen, "/proc/%ld/task", proc->pid);
    taskdir = opendir(path);
    if (taskdir) {
      struct dirent *taskdirent;
      long tid;
      char *end;
      unsigned nbth = 0;
      /* count threads */
      while ((taskdirent = readdir(taskdir))) {
	tid = strtol(taskdirent->d_name, &end, 10);
	if (*end)
	  /* Not a number */
	  continue;
	nbth++;
      }
      if (nbth > 1) {
	/* if there's more than one thread, see if some are bound */
	proc->threads = calloc(nbth, sizeof(*proc->threads));
	if (proc->threads) {
	  /* reread the directory but gather info now */
	  rewinddir(taskdir);
	  unsigned i = 0;
	  while ((taskdirent = readdir(taskdir))) {
	    char *path2;
	    unsigned path2len;

	    tid = strtol(taskdirent->d_name, &end, 10);
	    if (*end)
	      /* Not a number */
	      continue;

	    proc->threads[i].tid = tid;

	    path2len = pathlen + 1 + 21 + 1 + 4 + 1;
	    path2 = malloc(path2len);
	    if (path2) {
	      int commfd;
	      snprintf(path2, path2len, "%s/%ld/comm", path, tid);
	      commfd = open(path2, O_RDWR);
	      if (commfd >= 0) {
		n = read(commfd, proc->threads[i].name, sizeof(proc->threads[i].name));
		close(commfd);
		if (n <= 0)
		  proc->threads[i].name[0] = '\0';
		else if ((size_t)n < sizeof(proc->threads[i].name))
		  proc->threads[i].name[n] = '\0';
		proc->threads[i].name[sizeof(proc->threads[i].name)-1] = '\0';
		end = strchr(proc->threads[i].name, '\n');
		if (end)
		  *end = '\0';
	      }
	      free(path2);
	    }

	    if (flags & HWLOC_PS_FLAG_LASTCPULOCATION) {
	      if (hwloc_linux_get_tid_last_cpu_location(topology, tid, cpuset))
		goto next;
	    } else {
	      if (hwloc_linux_get_tid_cpubind(topology, tid, cpuset))
		goto next;
	    }
	    hwloc_bitmap_and(cpuset, cpuset, topocpuset);
	    if (hwloc_bitmap_iszero(cpuset))
	      goto next;

	    proc->threads[i].cpuset = hwloc_bitmap_dup(cpuset);
	    if (!hwloc_bitmap_isequal(cpuset, topocpuset)) {
	      proc->threads[i].bound = 1;
	      proc->nboundthreads++;
	    }

	  next:
	    i++;
	    proc->nthreads++;
	    if (i == nbth)
	      /* ignore the lastly created threads, I'm too lazy to reallocate */
	      break;
	  }
	} else {
	  /* failed to alloc, behave as if there were no threads */
	}
      }
      closedir(taskdir);
    }
    free(path);
#endif /* HWLOC_LINUX_SYS */
  }

  if (flags & HWLOC_PS_FLAG_LASTCPULOCATION) {
    if (hwloc_get_proc_last_cpu_location(topology, realpid, cpuset, 0))
      goto out;
  } else {
    if (hwloc_get_proc_cpubind(topology, realpid, cpuset, 0))
      goto out;
  }

  hwloc_bitmap_and(cpuset, cpuset, topocpuset);
  if (hwloc_bitmap_iszero(cpuset))
    goto out;

  proc->bound = !hwloc_bitmap_isequal(cpuset, topocpuset);
  proc->cpuset = cpuset;
  return 0;

 out:
  hwloc_bitmap_free(cpuset);
#endif /* HAVE_DIRENT_H */
  return -1;
}

#if defined(HWLOC_WIN_SYS) && !defined(__CYGWIN__)
/* hwloc-ps isn't built for Windows, but lstopo is */
#define popen _popen
#define pclose _pclose
#endif

static int
hwloc_ps_pidcmd__from_env(struct hwloc_ps_process *proc,
                          const char *envname, size_t envnamelen,
                          const char *buffer)
{
  const char *cur = buffer;
  while (*cur) {
    size_t len;
    if (!strncmp(cur, envname, envnamelen)) {
      /* copy the entire "name=value" instead of only the value
       * so that users know which variable was used.
       */
      strncpy(proc->string, cur, sizeof(proc->string));
      proc->string[sizeof(proc->string)-1] = '\0';
      return 0;
    }
    len = strlen(cur);
    cur += len + 1;
  }
  return -1;
}

static void
hwloc_ps_pidcmd_from_env(struct hwloc_ps_process *proc,
                         unsigned nr_env, const char *env[])
{
  char path[64]; /* enough for "/proc/%ld/environ" */
  char buffer[65536]; /* should be enough for the vast majority of cases */
  FILE *file;
  size_t len;
  unsigned i;

  snprintf(path, sizeof(path), "/proc/%ld/environ", proc->pid);
  file = fopen(path, "r");
  if (!file)
    return;
  len = fread(buffer, 1, sizeof(buffer)-2, file);
  fclose(file);
  if (!len)
    return;

  /* we must end with 2 '\0', one for the current variable if truncated, and one for an empty variable ending the environment array */
  buffer[len] = '\0';
  buffer[len+1] = '\0';

  for(i=0; i<nr_env; i++)
    if (hwloc_ps_pidcmd__from_env(proc, env[i], strlen(env[i]), buffer) == 0)
      return;
}

void hwloc_ps_pidcmd(struct hwloc_ps_process *proc, const char *pidcmd)
{
  char *cmd;
  FILE *file;

  if (!strcmp(pidcmd, "mpirank")) {
    /* SLURM_PROCID works in srun, but not in salloc+mpirun (returns the node ID?), hence use it as a last fallback */
    const char *envs[] = { "OMPI_COMM_WORLD_RANK", "PMIX_RANK", "PMI_RANK", "SLURM_PROCID" };
    hwloc_ps_pidcmd_from_env(proc, sizeof(envs)/sizeof(*envs), envs);
    return;
  }
  if (!strncmp(pidcmd, "env=", 4)) {
    const char *env = pidcmd+4;
    hwloc_ps_pidcmd_from_env(proc, 1, &env);
    return;
  }

  cmd = malloc(strlen(pidcmd)+1+5+2+1);
  sprintf(cmd, "%s %u", pidcmd, (unsigned) proc->pid);
  file = popen(cmd, "r");
  if (file) {
    if (fgets(proc->string, sizeof(proc->string), file)) {
      char *end = strchr(proc->string, '\n');
      if (end)
        *end = '\0';
    }
    pclose(file);
  }
  free(cmd);
}

void hwloc_ps_free_process(struct hwloc_ps_process *proc)
{
  unsigned i;

  if (proc->nthreads)
    for(i=0; i<proc->nthreads; i++)
      if (proc->threads[i].cpuset)
	hwloc_bitmap_free(proc->threads[i].cpuset);
  free(proc->threads);

  hwloc_bitmap_free(proc->cpuset);
}

int hwloc_ps_foreach_process(hwloc_topology_t topology, hwloc_const_bitmap_t topocpuset,
			     void (*callback)(hwloc_topology_t topology, struct hwloc_ps_process *proc, void *cbdata),
			     void *cbdata,
			     unsigned long flags, const char *only_name, long uid)
{
#ifdef HAVE_DIRENT_H
  DIR *dir;
  struct dirent *dirent;

  dir  = opendir("/proc");
  if (!dir)
    return -1;

  while ((dirent = readdir(dir))) {
    struct hwloc_ps_process proc;
    long pid;
    char *end;

    pid = strtol(dirent->d_name, &end, 10);
    if (*end)
      /* Not a number */
      continue;

    proc.pid = pid;
    proc.cpuset = NULL;
    proc.nthreads = 0;
    proc.nboundthreads = 0;
    proc.threads = NULL;
    if (hwloc_ps_read_process(topology, topocpuset, &proc, flags) < 0)
      goto next;
    if (only_name && !strstr(proc.name, only_name))
      goto next;
    if (uid != HWLOC_PS_ALL_UIDS && proc.uid != HWLOC_PS_ALL_UIDS && proc.uid != uid)
      goto next;
    callback(topology, &proc, cbdata);
  next:
    hwloc_ps_free_process(&proc);
  }

  closedir(dir);
  return 0;
#else /* HAVE_DIRENT_H */
  return -1;
#endif /* HAVE_DIRENT_H */
}

int hwloc_ps_foreach_child(hwloc_topology_t topology, hwloc_const_bitmap_t topocpuset,
                           long pid,
                           void (*callback)(hwloc_topology_t topology, struct hwloc_ps_process *proc, void *cbdata),
                           void *cbdata,
                           unsigned long flags, const char *only_name, long uid)
{
#ifdef HAVE_DIRENT_H
  struct hwloc_ps_process proc;
  DIR *taskdir;
  char path[512];

  proc.pid = pid;
  proc.cpuset = NULL;
  proc.nthreads = 0;
  proc.nboundthreads = 0;
  proc.threads = NULL;
  if (hwloc_ps_read_process(topology, topocpuset, &proc, flags) < 0)
    goto next;
  if (only_name && !strstr(proc.name, only_name))
    goto next;
  if (uid != HWLOC_PS_ALL_UIDS && proc.uid != HWLOC_PS_ALL_UIDS && proc.uid != uid)
    goto next;
  callback(topology, &proc, cbdata);
 next:
  hwloc_ps_free_process(&proc);

  snprintf(path, sizeof(path), "/proc/%ld/task", proc.pid);
  taskdir = opendir(path); /* should be enough for the vast majority of cases */
  if (taskdir) {
    struct dirent *taskdirent;
    while ((taskdirent = readdir(taskdir))) {
      char pidline[4096];
      FILE *file;
      char *begin;
      size_t len;
      snprintf(path, sizeof(path), "/proc/%ld/task/%s/children", proc.pid, taskdirent->d_name);
      file = fopen(path, "r");
      if (!file)
        continue;
      len = fread(pidline, 1, sizeof(pidline)-1, file);
      fclose(file);
      pidline[len] = '\0';
      begin = pidline;
      while (1) {
        char *end;
        long childpid = strtoul(begin, &end, 10);
        if (end == begin)
          break;
        hwloc_ps_foreach_child(topology, topocpuset, childpid, callback, cbdata, flags, only_name, uid);
        begin = end;
      }
    }
    closedir(taskdir);
  }
  return 0;
#else /* HAVE_DIRENT_H */
  return -1;
#endif /* HAVE_DIRENT_H */
}
