/*
 * Copyright © 2009-2010 inria.  All rights reserved.
 * Copyright © 2009-2011 Université Bordeaux 1
 * Copyright © 2009-2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <private/autogen/config.h>
#include <hwloc.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>

static void usage(char *name, FILE *where)
{
  fprintf (where, "Usage: %s [ options ] ...\n", name);
  fprintf (where, "Options:\n");
  fprintf (where, "  -a             Show all processes, including those that are not bound\n");
  fprintf (where, "  -l --logical   Use logical object indexes (default)\n");
  fprintf (where, "  -p --physical  Use physical object indexes\n");
  fprintf (where, "  -c --cpuset    Show cpuset instead of objects\n");
  fprintf (where, "  --whole-system Do not consider administration limitations\n");
}

int main(int argc, char *argv[])
{
  const struct hwloc_topology_support *support;
  hwloc_topology_t topology;
  hwloc_obj_t root;
  hwloc_bitmap_t cpuset;
  unsigned long flags = 0;
  int logical = 1;
  int show_cpuset = 0;
  DIR *dir;
  struct dirent *dirent;
  int show_all = 0;
  char *callname;
  int err;
  int opt;

  callname = strrchr(argv[0], '/');
  if (!callname)
    callname = argv[0];
  else
    callname++;

  while (argc >= 2) {
    opt = 0;
    if (!strcmp(argv[1], "-a"))
      show_all = 1;
    else if (!strcmp(argv[1], "-l") || !strcmp(argv[1], "--logical")) {
      logical = 1;
    } else if (!strcmp(argv[1], "-p") || !strcmp(argv[1], "--physical")) {
      logical = 0;
    } else if (!strcmp(argv[1], "-c") || !strcmp(argv[1], "--cpuset")) {
      show_cpuset = 1;
    } else if (!strcmp (argv[1], "--whole-system")) {
      flags |= HWLOC_TOPOLOGY_FLAG_WHOLE_SYSTEM;
    } else {
      fprintf (stderr, "Unrecognized option: %s\n", argv[1]);
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

  root = hwloc_get_root_obj(topology);

  support = hwloc_topology_get_support(topology);

  if (!support->cpubind->get_thisproc_cpubind)
    goto out_with_topology;

  dir  = opendir("/proc");
  if (!dir)
    goto out_with_topology;

  cpuset = hwloc_bitmap_alloc();
  if (!cpuset)
    goto out_with_dir;

  while ((dirent = readdir(dir))) {
    long pid;
    char *end;
    char name[64] = "";
    char *cpuset_str = NULL;

    pid = strtol(dirent->d_name, &end, 10);
    if (*end)
      /* Not a number */
      continue;

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

    if (hwloc_get_proc_cpubind(topology, pid, cpuset, 0))
      continue;

    hwloc_bitmap_and(cpuset, cpuset, hwloc_topology_get_topology_cpuset(topology));
    if (hwloc_bitmap_iszero(cpuset))
      continue;

    if (hwloc_bitmap_isequal(cpuset, root->cpuset) && !show_all)
      continue;

    printf("%ld\t", pid);

    if (show_cpuset) {
      hwloc_bitmap_asprintf(&cpuset_str, cpuset);
      printf("%s", cpuset_str);
    } else {
      hwloc_bitmap_t remaining = hwloc_bitmap_dup(cpuset);
      int first = 1;
      while (!hwloc_bitmap_iszero(remaining)) {
        char type[64];
        unsigned idx;
        hwloc_obj_t obj = hwloc_get_first_largest_obj_inside_cpuset(topology, remaining);
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

    printf("\t\t%s\n", name);
    free(cpuset_str);
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
