/*
 * Copyright © 2009-2012 Inria.  All rights reserved.
 * Copyright © 2009-2012 Université Bordeaux
 * Copyright © 2009-2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <private/autogen/config.h>
#include <hwloc.h>
#ifdef HWLOC_LINUX_SYS
#include <hwloc/linux.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
#include <fcntl.h>

#include "misc.h"

static int show_cpuset = 0;
static int logical = 1;

void usage(const char *name, FILE *where)
{
  fprintf (where, "Usage: %s [ options ] ...\n", name);
  fprintf (where, "Options:\n");
  fprintf (where, "  -a               Show all processes, including those that are not bound\n");
  fprintf (where, "  -l --logical     Use logical object indexes (default)\n");
  fprintf (where, "  -p --physical    Use physical object indexes\n");
  fprintf (where, "  -c --cpuset      Show cpuset instead of objects\n");
  fprintf (where, "  -t --threads     Show threads\n");
  fprintf (where, "  -e --get-last-cpu-location\n");
  fprintf (where, "                   Retrieve the last processors where the tasks ran\n");
  fprintf (where, "  --pid-cmd <cmd>  Append the output of <cmd> <pid> to each PID line\n");
  fprintf (where, "  --whole-system   Do not consider administration limitations\n");
}

static void print_task(hwloc_topology_t topology,
		       long pid_number, const char *name, hwloc_bitmap_t cpuset,
		       char *pidoutput,
		       int thread)
{
  printf("%s%ld\t", thread ? " " : "", pid_number);

  if (show_cpuset) {
    char *cpuset_str = NULL;
    hwloc_bitmap_asprintf(&cpuset_str, cpuset);
    printf("%s", cpuset_str);
    free(cpuset_str);
  } else {
    hwloc_bitmap_t remaining = hwloc_bitmap_dup(cpuset);
    int first = 1;
    while (!hwloc_bitmap_iszero(remaining)) {
      char type[64];
      unsigned idx;
      hwloc_obj_t obj = hwloc_get_first_largest_obj_inside_cpuset(topology, remaining);
      /* don't show a cache if there's something equivalent and nicer */
      while (obj->type == HWLOC_OBJ_CACHE && obj->arity == 1)
	obj = obj->first_child;
      hwloc_obj_type_snprintf(type, sizeof(type), obj, 1);
      idx = logical ? obj->logical_index : obj->os_index;
      if (idx == (unsigned) -1)
        printf("%s%s", first ? "" : " ", type);
      else
        printf("%s%s:%u", first ? "" : " ", type, idx);
      hwloc_bitmap_andnot(remaining, remaining, obj->cpuset);
      first = 0;
    }
    hwloc_bitmap_free(remaining);
  }

  printf("\t\t%s%s%s\n", name, pidoutput ? "\t" : "", pidoutput ? pidoutput : "");
}

int main(int argc, char *argv[])
{
  const struct hwloc_topology_support *support;
  hwloc_topology_t topology;
  hwloc_const_bitmap_t topocpuset;
  hwloc_bitmap_t cpuset;
  unsigned long flags = 0;
  DIR *dir;
  struct dirent *dirent;
  int show_all = 0;
  int show_threads = 0;
  int get_last_cpu_location = 0;
  char *callname;
  char *pidcmd = NULL;
  int err;
  int opt;

  callname = strrchr(argv[0], '/');
  if (!callname)
    callname = argv[0];
  else
    callname++;
  /* skip argv[0], handle options */
  argc--;
  argv++;

  while (argc >= 1) {
    opt = 0;
    if (!strcmp(argv[0], "-a"))
      show_all = 1;
    else if (!strcmp(argv[0], "-l") || !strcmp(argv[0], "--logical")) {
      logical = 1;
    } else if (!strcmp(argv[0], "-p") || !strcmp(argv[0], "--physical")) {
      logical = 0;
    } else if (!strcmp(argv[0], "-c") || !strcmp(argv[0], "--cpuset")) {
      show_cpuset = 1;
    } else if (!strcmp(argv[0], "-e") || !strncmp(argv[0], "--get-last-cpu-location", 10)) {
      get_last_cpu_location = 1;
    } else if (!strcmp(argv[0], "-t") || !strcmp(argv[0], "--threads")) {
#ifdef HWLOC_LINUX_SYS
      show_threads = 1;
#else
      fprintf (stderr, "Listing threads is currently only supported on Linux\n");
#endif
    } else if (!strcmp (argv[0], "--whole-system")) {
      flags |= HWLOC_TOPOLOGY_FLAG_WHOLE_SYSTEM;
    } else if (!strcmp (argv[0], "--pid-cmd")) {
      if (argc < 2) {
	usage(callname, stdout);
	exit(EXIT_FAILURE);
      }
      pidcmd = argv[1];
      opt = 1;
    } else {
      fprintf (stderr, "Unrecognized option: %s\n", argv[0]);
      usage (callname, stderr);
      exit(EXIT_FAILURE);
    }
    argc -= opt+1;
    argv += opt+1;
  }

  err = hwloc_topology_init(&topology);
  if (err)
    goto out;

  hwloc_topology_set_flags(topology, flags);

  err = hwloc_topology_load(topology);
  if (err)
    goto out_with_topology;

  support = hwloc_topology_get_support(topology);

  if (get_last_cpu_location) {
    if (!support->cpubind->get_proc_last_cpu_location)
      goto out_with_topology;
  } else {
    if (!support->cpubind->get_proc_cpubind)
      goto out_with_topology;
  }

  topocpuset = hwloc_topology_get_topology_cpuset(topology);

  dir  = opendir("/proc");
  if (!dir)
    goto out_with_topology;

  cpuset = hwloc_bitmap_alloc();
  if (!cpuset)
    goto out_with_dir;

  while ((dirent = readdir(dir))) {
    long pid_number;
    hwloc_pid_t pid;
    char pidoutput[1024];
    char *end;
    char name[64] = "";
    /* management of threads */
    unsigned boundthreads = 0, i;
    long *tids = NULL; /* NULL if process is not threaded */
    hwloc_bitmap_t *tidcpusets = NULL;

    pid_number = strtol(dirent->d_name, &end, 10);
    if (*end)
      /* Not a number */
      continue;

    pid = hwloc_pid_from_number(pid_number, 0);

#ifdef HWLOC_LINUX_SYS
    {
      unsigned pathlen = 6 + strlen(dirent->d_name) + 1 + 7 + 1;
      char *path;
      int file;
      ssize_t n;

      path = malloc(pathlen);
      snprintf(path, pathlen, "/proc/%s/cmdline", dirent->d_name);
      file = open(path, O_RDONLY);
      free(path);

      if (file >= 0) {
        n = read(file, name, sizeof(name) - 1);
        close(file);

        if (n <= 0)
          /* Ignore kernel threads and errors */
          continue;

        name[n] = 0;
      }
    }
#endif /* HWLOC_LINUX_SYS */

    if (show_threads) {
#ifdef HWLOC_LINUX_SYS
      /* check if some threads must be displayed */
      unsigned pathlen = 6 + strlen(dirent->d_name) + 1 + 4 + 1;
      char *path;
      DIR *taskdir;

      path = malloc(pathlen);
      snprintf(path, pathlen, "/proc/%s/task", dirent->d_name);
      taskdir = opendir(path);
      if (taskdir) {
	struct dirent *taskdirent;
	long tid;
	unsigned n = 0;
	/* count threads */
	while ((taskdirent = readdir(taskdir))) {
	  tid = strtol(taskdirent->d_name, &end, 10);
	  if (*end)
	    /* Not a number */
	    continue;
	  n++;
	}
	if (n > 1) {
	  /* if there's more than one thread, see if some are bound */
	  tids = malloc(n * sizeof(*tids));
	  tidcpusets = calloc(n+1, sizeof(*tidcpusets));
	  if (tids && tidcpusets) {
	    /* reread the directory but gather info now */
	    rewinddir(taskdir);
	    i = 0;
	    while ((taskdirent = readdir(taskdir))) {
	      tid = strtol(taskdirent->d_name, &end, 10);
	      if (*end)
		/* Not a number */
		continue;
	      if (get_last_cpu_location) {
		if (hwloc_linux_get_tid_last_cpu_location(topology, tid, cpuset))
		  continue;
	      } else {
		if (hwloc_linux_get_tid_cpubind(topology, tid, cpuset))
		  continue;
	      }
	      hwloc_bitmap_and(cpuset, cpuset, topocpuset);
	      tids[i] = tid;
	      tidcpusets[i] = hwloc_bitmap_dup(cpuset);
	      i++;
	      if (hwloc_bitmap_iszero(cpuset))
		continue;
	      if (hwloc_bitmap_isequal(cpuset, topocpuset) && !show_all)
		continue;
	      boundthreads++;
	    }
	  } else {
	    /* failed to alloc, behave as if there were no threads */
	    free(tids); tids = NULL;
	    free(tidcpusets); tidcpusets = NULL;
	  }
	}
	closedir(taskdir);
      }
#endif /* HWLOC_LINUX_SYS */
    }

    if (get_last_cpu_location) {
      if (hwloc_get_proc_last_cpu_location(topology, pid, cpuset, 0))
	continue;
    } else {
      if (hwloc_get_proc_cpubind(topology, pid, cpuset, 0))
	continue;
    }

    hwloc_bitmap_and(cpuset, cpuset, topocpuset);
    if (hwloc_bitmap_iszero(cpuset))
      continue;

    /* don't print anything if the process isn't bound and if no threads are bound and if not showing all */
    if (hwloc_bitmap_isequal(cpuset, topocpuset) && (!tids || !boundthreads) && !show_all)
      continue;

    pidoutput[0] = '\0';
    if (pidcmd) {
      char *cmd;
      FILE *file;
      cmd = malloc(strlen(pidcmd)+1+5+2+1);
      sprintf(cmd, "%s %u", pidcmd, pid);
      file = popen(cmd, "r");
      if (file) {
	if (fgets(pidoutput, sizeof(pidoutput), file)) {
	  end = strchr(pidoutput, '\n');
	  if (end)
	    *end = '\0';
	}
	pclose(file);
      }
      free(cmd);
    }

    /* print the process */
    print_task(topology, pid_number, name, cpuset, pidoutput[0] == '\0' ? NULL : pidoutput, 0);
    if (tids)
      /* print each tid we found (it's tidcpuset isn't NULL anymore) */
      for(i=0; tidcpusets[i] != NULL; i++) {
	print_task(topology, tids[i], "", tidcpusets[i], NULL, 1);
	hwloc_bitmap_free(tidcpusets[i]);
      }

    /* free threads stuff */
    free(tidcpusets);
    free(tids);
  }

  err = 0;
  hwloc_bitmap_free(cpuset);

 out_with_dir:
  closedir(dir);
 out_with_topology:
  hwloc_topology_destroy(topology);
 out:
  return err;
}
