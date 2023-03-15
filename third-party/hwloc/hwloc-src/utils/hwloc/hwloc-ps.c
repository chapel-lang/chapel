/*
 * Copyright © 2009-2021 Inria.  All rights reserved.
 * Copyright © 2009-2012 Université Bordeaux
 * Copyright © 2009-2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "private/autogen/config.h"
#include "hwloc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
#include <fcntl.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "common-ps.h"
#include "misc.h"

#define JSON_PORT 8888

static int show_all = 0;
static int show_threads = 0;
static char *only_name = NULL;
static int show_cpuset = 0;
static int logical = 1;
static int single_ancestor = 0;
#define NO_ONLY_PID -1
static long only_pid = NO_ONLY_PID;
static long only_uid;
static int json_server = 0;
static int json_port = JSON_PORT;
static FILE *json_output = NULL;
static int verbose = 0;

void usage(const char *name, FILE *where)
{
  fprintf (where, "Usage: %s [ options ] ...\n", name);
  fprintf (where, "Options:\n");
  fprintf (where, "  -a                 Show all processes, including those that are not bound\n");
  fprintf (where, "  --pid <pid>        Only show process of pid number <pid>\n");
  fprintf (where, "  --name <name>      Only show processes whose name contains <name>\n");
#ifdef HWLOC_LINUX_SYS
  fprintf (where, "  --uid <uid>        Only show processes of the user with the given uid\n");
  fprintf (where, "  --uid all          Show processes of all users\n");
#endif
  fprintf (where, "  -l --logical       Use logical object indexes (default)\n");
  fprintf (where, "  -p --physical      Use physical object indexes\n");
  fprintf (where, "  -c --cpuset        Show cpuset instead of objects\n");
  fprintf (where, "  --single-ancestor  Show a single ancestor containing the binding\n");
#ifdef HWLOC_LINUX_SYS
  fprintf (where, "  -t --threads       Show threads\n");
#endif
  fprintf (where, "  -e --get-last-cpu-location\n");
  fprintf (where, "                     Retrieve the last processors where the tasks ran\n");
  fprintf (where, "  --pid-cmd <cmd>    Append the output of <cmd> <pid> to each PID line\n");
  fprintf (where, "  --short-name       Show only the process short name instead of the path\n");
  fprintf (where, "  --disallowed       Include objects disallowed by administrative limitations\n");
  fprintf (where, "  --json-server      Run as a JSON server\n");
  fprintf (where, "  --json-port <n>    Use port <n> for JSON server (default is %d)\n", JSON_PORT);
  fprintf (where, "  -v --verbose       Increase verbosity\n");
  fprintf (where, "  --version          Report version and exit\n");
  fprintf (where, "  -h --help          Show this usage\n");
}

static void print_task(hwloc_topology_t topology,
		       long pid, const char *name, hwloc_bitmap_t cpuset,
		       char *pidoutput,
		       int thread)
{
  printf("%s%ld\t", thread ? " " : "", pid);

  if (show_cpuset) {
    char *cpuset_str = NULL;
    hwloc_bitmap_asprintf(&cpuset_str, cpuset);
    printf("%s", cpuset_str);
    free(cpuset_str);
  } else {
    hwloc_bitmap_t remaining = hwloc_bitmap_dup(cpuset);
    int first = 1;
    char type[64];
    unsigned idx;
    if (single_ancestor) {
      hwloc_obj_t obj = hwloc_get_obj_covering_cpuset(topology, cpuset);
      while (obj->parent && hwloc_bitmap_isequal(obj->cpuset, obj->parent->cpuset) && !hwloc_obj_type_is_cache(obj->parent->type) )
        obj = obj->parent;

      hwloc_obj_type_snprintf(type, sizeof(type), obj, 1);
      idx = logical ? obj->logical_index : obj->os_index;
      if (idx == (unsigned) -1)
        printf("%s", type);
      else
        printf("%s:%u", type, idx);
    } else {
     while (!hwloc_bitmap_iszero(remaining)) {
      hwloc_obj_t obj = hwloc_get_first_largest_obj_inside_cpuset(topology, remaining);
      /* don't show a cache if there's something equivalent and nicer */
      while (hwloc_obj_type_is_cache(obj->type) && obj->arity == 1)
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
    }
    hwloc_bitmap_free(remaining);
  }

  printf("\t\t%s%s%s\n", name, pidoutput ? "\t" : "", pidoutput ? pidoutput : "");
}

static void print_process(hwloc_topology_t topology,
			  struct hwloc_ps_process *proc)
{
  unsigned i;

  print_task(topology, proc->pid, proc->name, proc->cpuset, proc->string[0] == '\0' ? NULL : proc->string, 0);
  if (proc->nthreads)
    for(i=0; i<proc->nthreads; i++)
      if (proc->threads[i].cpuset)
	print_task(topology, proc->threads[i].tid, proc->threads[i].name, proc->threads[i].cpuset, NULL, 1);
}

static void print_process_json(hwloc_topology_t topology,
			       struct hwloc_ps_process *proc)
{
  hwloc_obj_t obj;
  char type[64];

  assert(json_output);

  if (verbose > 1)
    printf("  sending process PID %ld name %s\n", proc->pid, proc->name);

  /* process */
  obj = hwloc_get_obj_covering_cpuset(topology, proc->cpuset);
  while (obj->parent && hwloc_bitmap_isequal(obj->cpuset, obj->parent->cpuset))
    obj = obj->parent;
  hwloc_obj_type_snprintf(type, sizeof(type), obj, 0);
  fprintf(json_output,
	  "{\n"
	  "  \"PID\": %ld,\n"
	  "  \"name\": \"%s\",\n"
	  "  \"object\": \"%s:%u\"%s\n",
	  proc->pid,
	  proc->name,
	  type, obj->logical_index,
	  proc->nthreads ? "," : "");

  /* threads */
  if (proc->nthreads) {
    unsigned i;
    fprintf(json_output, "  \"threads\": [\n");
    for(i=0; i<proc->nthreads; i++) {
      struct hwloc_ps_thread *thread = &proc->threads[i];
      if (thread->cpuset) {
	obj = hwloc_get_obj_covering_cpuset(topology, thread->cpuset);
	while (obj->parent && hwloc_bitmap_isequal(obj->cpuset, obj->parent->cpuset))
	  obj = obj->parent;
	hwloc_obj_type_snprintf(type, sizeof(type), obj, 0);

	fprintf(json_output,
		"    {\n"
		"      \"PID\": %ld,\n"
		"      \"name\": \"%s\",\n"
		"      \"object\": \"%s:%u\"\n"
		"    }%s\n",
		thread->tid,
		thread->name,
		type, obj->logical_index,
		i < proc->nthreads-1 ? "," : "");
      }
    }
    fprintf(json_output, "  ]\n");
  }

  /* close the process */
  fprintf(json_output, "},\n");
}

static void foreach_process_cb(hwloc_topology_t topology,
			       struct hwloc_ps_process *proc,
			       void *cbdata)
{
  const char *pidcmd = cbdata;

  /* don't print anything if the process isn't bound and if no threads are bound and if not showing all */
  if (!proc->bound && (!proc->nthreads || !proc->nboundthreads) && !show_all && !only_name)
    return;

  if (pidcmd)
    hwloc_ps_pidcmd(proc, pidcmd);

  if (json_output)
    print_process_json(topology, proc);
  else
    print_process(topology, proc);
}

static int run(hwloc_topology_t topology, hwloc_const_bitmap_t topocpuset,
	       unsigned long psflags, char *pidcmd)
{
  if (only_pid == NO_ONLY_PID) {
    /* show all */
    return hwloc_ps_foreach_process(topology, topocpuset, foreach_process_cb, pidcmd, psflags, only_name, only_uid);

  } else {
    /* show only one */
    struct hwloc_ps_process proc;
    int ret;

    proc.pid = only_pid;
    proc.uid = only_uid;
    proc.cpuset = NULL;
    proc.nthreads = 0;
    proc.nboundthreads = 0;
    proc.threads = NULL;
    ret = hwloc_ps_read_process(topology, topocpuset, &proc, psflags);
    if (ret >= 0) {

      if (pidcmd)
        hwloc_ps_pidcmd(&proc, pidcmd);

      if (json_output)
	print_process_json(topology, &proc);
      else
	print_process(topology, &proc);
    }
    hwloc_ps_free_process(&proc);
    return ret;
  }
}

#define JSON_REQLENMAX 100

static int
run_json_server(hwloc_topology_t topology, hwloc_const_bitmap_t topocpuset)
{
  int server_socket;
  struct sockaddr_in server_addr;
  int err;

  /* open the socket */
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    perror("json-server: socket");
    return -1;
  }

  /* bind it to port */
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(json_port);
  err = bind(server_socket, (struct sockaddr *)&server_addr , sizeof server_addr);
  if (err < 0) {
    perror("json-server: bind");
    close(server_socket);
    return -1;
  }

  /* listen to connections, up to 1 awaiting */
  err = listen(server_socket, 1);
  if (err < 0) {
    perror("json-server: listen");
    close(server_socket);
    return -1;
  }

  printf("server running on port %d...\n", json_port);

  while (1) {
    int client_socket;
    int ret;

    /* wait for a new client connection */
    client_socket = accept(server_socket, NULL, NULL);
    if (client_socket < 0) {
      perror("json-server: accept");
      continue;
    }
    printf("client connected\n");
    json_output = fdopen(client_socket, "r+");
    if (!json_output) {
      perror("json-server: fdopen");
      close(client_socket);
      continue;
    }

    while (1) {
      char req[JSON_REQLENMAX+1];
      char *end, *current;
      unsigned long psflags = HWLOC_PS_FLAG_SHORTNAME;

      /* read the client request */
      ret = read(client_socket, req, sizeof(req)-1);
      if (ret <= 0)
	break;
      req[ret] = '\0';
      end = strchr(req, '\n');
      if (end)
	*end = '\0';

      if (verbose > 0)
	printf(" received request `%s'\n", req);

      only_name = NULL;
      only_pid = NO_ONLY_PID;
      current = req;
      while (*current) {
	if (!strncmp(current, "lastcpulocation ", 16)) {
	  psflags |= HWLOC_PS_FLAG_LASTCPULOCATION;
	  current += 16;
	  continue;
	} else if (!strncmp(current, "threads ", 8)) {
	  psflags |= HWLOC_PS_FLAG_THREADS;
	  current += 8;
	  continue;
	} else if (!strcmp(current, "all")) {
	  show_all = 1;
	  break;
	} else if (!strcmp(current, "bound")) {
	  show_all = 0;
	  break;
	} else if (!strncmp(current, "pid=", 4)) {
	  only_pid = atoi(current+4);
	  psflags |= HWLOC_PS_FLAG_THREADS;
	  show_all = 1;
	  break;
	} else if (!strncmp(current, "name=", 5)) {
	  only_name = current+5;
	  show_all = 1;
	  break;
	}
      }

      fprintf(json_output, "[ ");
      run(topology, topocpuset, psflags, NULL);
      fprintf(json_output, "{ } ]\n");
      fflush(json_output);
    }

    if (ret == 0) {
      printf("disconnected\n");
    } else if (ret < -1) {
      perror("json-server: read");
    }
    fclose(json_output);
    json_output = NULL;
    close(client_socket);
  }

  close(server_socket);
  return 0;
}

int main(int argc, char *argv[])
{
  const struct hwloc_topology_support *support;
  hwloc_topology_t topology;
  hwloc_const_bitmap_t topocpuset;
  unsigned long flags = 0;
  unsigned long psflags = 0;
  int get_last_cpu_location = 0;
  char *pidcmd = NULL;
  char *callname;
  int err;
  int opt;

  only_uid = getuid();

  callname = strrchr(argv[0], '/');
  if (!callname)
    callname = argv[0];
  else
    callname++;
  /* skip argv[0], handle options */
  argc--;
  argv++;

  hwloc_utils_check_api_version(callname);

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
    } else if (!strcmp(argv[0], "--single-ancestor")) {
      single_ancestor = 1;
    } else if (!strcmp(argv[0], "--pid")) {
      if (argc < 2) {
	usage(callname, stderr);
	exit(EXIT_FAILURE);
      }
      only_pid = strtol(argv[1], NULL, 10);
      opt = 1;
    } else if (!strcmp(argv[0], "--name")) {
      if (argc < 2) {
	usage(callname, stderr);
	exit(EXIT_FAILURE);
      }
      only_name = argv[1];
      opt = 1;
    } else if (!strcmp(argv[0], "--uid")) {
#ifdef HWLOC_LINUX_SYS
      if (argc < 2) {
	usage(callname, stderr);
	exit(EXIT_FAILURE);
      }
      if (!strcmp(argv[1], "all"))
	only_uid = HWLOC_PS_ALL_UIDS;
      else
	only_uid = atoi(argv[1]);
      opt = 1;
#else
      fprintf (stderr, "Filtering by UID is currently only supported on Linux\n");
#endif
    } else if (!strcmp (argv[0], "--disallowed") || !strcmp (argv[0], "--whole-system")) {
      flags |= HWLOC_TOPOLOGY_FLAG_INCLUDE_DISALLOWED;
    } else if (!strcmp (argv[0], "--pid-cmd")) {
      if (argc < 2) {
	usage(callname, stderr);
	exit(EXIT_FAILURE);
      }
      pidcmd = argv[1];
      opt = 1;

    } else if (!strcmp (argv[0], "--json-server")) {
      json_server = 1;
    } else if (!strcmp (argv[0], "--json-port")) {
      if (argc < 2) {
	usage(callname, stderr);
	exit(EXIT_FAILURE);
      }
      json_port = atoi(argv[1]);
      opt = 1;

    } else if (!strcmp(argv[0], "-v") || !strcmp(argv[0], "--verbose")) {
      verbose++;

    } else if (!strcmp (argv[0], "--short-name")) {
      psflags |= HWLOC_PS_FLAG_SHORTNAME;

    } else if (!strcmp (argv[0], "--version")) {
      printf("%s %s\n", callname, HWLOC_VERSION);
      exit(EXIT_SUCCESS);

    } else if (!strcmp (argv[0], "-h") || !strcmp (argv[0], "--help")) {
      usage (callname, stderr);
      exit(EXIT_SUCCESS);
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

  if (show_threads)
    psflags |= HWLOC_PS_FLAG_THREADS;
  if (get_last_cpu_location)
    psflags |= HWLOC_PS_FLAG_LASTCPULOCATION;
  if (only_uid != HWLOC_PS_ALL_UIDS)
    psflags |= HWLOC_PS_FLAG_UID;

  if (json_server) {
    run_json_server(topology, topocpuset);
  } else {
    if (run(topology, topocpuset, psflags, pidcmd))
      goto out_with_topology;
  }

  err = 0;

 out_with_topology:
  hwloc_topology_destroy(topology);
 out:
  return err;
}
