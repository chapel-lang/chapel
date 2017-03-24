/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2017 Inria.  All rights reserved.
 * Copyright © 2009-2012 Université Bordeaux
 * Copyright © 2009-2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <private/autogen/config.h>
#include <hwloc.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>

#include "misc.h"
#include "hwloc-calc.h"

static int pid_number = -1;
static hwloc_pid_t pid;
static int verbose_mode = 0;
static int logical = 1;
static int show_ancestors = 0;
static hwloc_obj_type_t show_ancestor_type = (hwloc_obj_type_t) -1;
static int show_ancestor_attrdepth = -1;
static hwloc_obj_cache_type_t show_ancestor_attrcachetype = (hwloc_obj_cache_type_t) -1;
static int show_index_prefix = 0;
static int current_obj;

void usage(const char *name, FILE *where)
{
  fprintf (where, "Usage: %s [ options ] [ locations ]\n", name);
  fprintf (where, "\nOutput options:\n");
  fprintf (where, "  --objects             Report information about specific objects\n");
  fprintf (where, "  --topology            Report information the topology\n");
  fprintf (where, "  --support             Report information about supported features\n");
  fprintf (where, "  -v --verbose          Include additional details\n");
  fprintf (where, "  -s --silent           Reduce the amount of details to show\n");
  fprintf (where, "  --ancestors           Display the chain of ancestor objects up to the root\n");
  fprintf (where, "  --ancestor <type>     Only display the ancestor of the given type\n");
  fprintf (where, "  -n                    Prefix each line with the index of the considered object\n");
  fprintf (where, "Object filtering options:\n");
  fprintf (where, "  --restrict <cpuset>   Restrict the topology to processors listed in <cpuset>\n");
  fprintf (where, "  --restrict binding    Restrict the topology to the current process binding\n");
  fprintf (where, "  --no-icaches          Do not show instruction caches\n");
  fprintf (where, "  --no-io               Do not show any I/O device or bridge\n");
  fprintf (where, "  --no-bridges          Do not any I/O bridge except hostbridges\n");
  fprintf (where, "  --whole-io            Show all I/O devices and bridges\n");
  fprintf (where, "Input options:\n");
  hwloc_utils_input_format_usage(where, 6);
  fprintf (where, "  --thissystem          Assume that the input topology provides the topology\n"
		  "                        for the system on which we are running\n");
  fprintf (where, "  --pid <pid>           Detect topology as seen by process <pid>\n");
  fprintf (where, "  --whole-system        Do not consider administration limitations\n");
  fprintf (where, "  -l --logical          Use logical object indexes for input (default)\n");
  fprintf (where, "  -p --physical         Use physical object indexes for input\n");
  fprintf (where, "Miscellaneous options:\n");
  fprintf (where, "  --version             Report version and exit\n");
}

static void
hwloc_info_show_obj(hwloc_obj_t obj, const char *type, const char *prefix, int verbose)
{
  char s[128];
  unsigned i;
  if (verbose < 0)
    return;
  printf("%s type = %s\n", prefix, hwloc_obj_type_string(obj->type));
  printf("%s full type = %s\n", prefix, type);
  printf("%s logical index = %u\n", prefix, obj->logical_index);
  if (obj->os_index != (unsigned) -1)
    printf("%s os index = %u\n", prefix, obj->os_index);
  if (obj->name)
    printf("%s name = %s\n", prefix, obj->name);
  if (obj->depth != (unsigned) -1)
    printf("%s depth = %d\n", prefix, (int) obj->depth); /* special levels have negative values */
  printf("%s sibling rank = %u\n", prefix, obj->sibling_rank);
  printf("%s children = %u\n", prefix, obj->arity);
  if (obj->memory.local_memory)
    printf("%s local memory = %llu\n", prefix, (unsigned long long) obj->memory.local_memory);
  if (obj->memory.total_memory)
    printf("%s total memory = %llu\n", prefix, (unsigned long long) obj->memory.total_memory);

  if (obj->cpuset) {
    hwloc_bitmap_snprintf(s, sizeof(s), obj->cpuset);
    printf("%s cpuset = %s\n", prefix, s);
  }
  if (obj->complete_cpuset) {
    hwloc_bitmap_snprintf(s, sizeof(s), obj->complete_cpuset);
    printf("%s complete cpuset = %s\n", prefix, s);
  }
  if (obj->online_cpuset) {
    hwloc_bitmap_snprintf(s, sizeof(s), obj->online_cpuset);
    printf("%s online cpuset = %s\n", prefix, s);
  }
  if (obj->allowed_cpuset) {
    hwloc_bitmap_snprintf(s, sizeof(s), obj->allowed_cpuset);
    printf("%s allowed cpuset = %s\n", prefix, s);
  }

  if (obj->nodeset) {
    hwloc_bitmap_snprintf(s, sizeof(s), obj->nodeset);
    printf("%s nodeset = %s\n", prefix, s);
  }
  if (obj->complete_nodeset) {
    hwloc_bitmap_snprintf(s, sizeof(s), obj->complete_nodeset);
    printf("%s complete nodeset = %s\n", prefix, s);
  }
  if (obj->allowed_nodeset) {
    hwloc_bitmap_snprintf(s, sizeof(s), obj->allowed_nodeset);
    printf("%s allowed nodeset = %s\n", prefix, s);
  }

  switch (obj->type) {
  case HWLOC_OBJ_CACHE:
    printf("%s attr cache depth = %u\n", prefix, obj->attr->cache.depth);
    switch (obj->attr->cache.type) {
    case HWLOC_OBJ_CACHE_UNIFIED: printf("%s attr cache type = Unified\n", prefix); break;
    case HWLOC_OBJ_CACHE_DATA: printf("%s attr cache type = Data\n", prefix); break;
    case HWLOC_OBJ_CACHE_INSTRUCTION: printf("%s attr cache type = Instruction\n", prefix); break;
    }
    printf("%s attr cache size = %llu\n", prefix, (unsigned long long) obj->attr->cache.size);
    printf("%s attr cache line size = %u\n", prefix, obj->attr->cache.linesize);
    if (obj->attr->cache.associativity == -1)
      printf("%s attr cache ways = Fully-associative\n", prefix);
    else if (obj->attr->cache.associativity != 0)
      printf("%s attr cache ways = %d\n", prefix, obj->attr->cache.associativity);
    break;
  case HWLOC_OBJ_GROUP:
    printf("%s attr group depth = %u\n", prefix, obj->attr->group.depth);
    break;
  case HWLOC_OBJ_BRIDGE:
    switch (obj->attr->bridge.upstream_type) {
    case HWLOC_OBJ_BRIDGE_HOST:
      printf("%s attr bridge upstream type = Host\n", prefix);
      break;
    case HWLOC_OBJ_BRIDGE_PCI:
      printf("%s attr bridge upstream type = PCI\n", prefix);
      printf("%s attr PCI bus id = %04x:%02x:%02x.%01x\n",
	     prefix, obj->attr->pcidev.domain, obj->attr->pcidev.bus, obj->attr->pcidev.dev, obj->attr->pcidev.func);
      printf("%s attr PCI class = %04x\n",
	     prefix, obj->attr->pcidev.class_id);
      printf("%s attr PCI id = %04x:%04x\n",
	     prefix, obj->attr->pcidev.vendor_id, obj->attr->pcidev.device_id);
      if (obj->attr->pcidev.linkspeed)
	printf("%s attr PCI linkspeed = %f GB/s\n", prefix, obj->attr->pcidev.linkspeed);
      break;
    }
    switch (obj->attr->bridge.downstream_type) {
    case HWLOC_OBJ_BRIDGE_HOST:
      assert(0);
    case HWLOC_OBJ_BRIDGE_PCI:
      printf("%s attr bridge downstream type = PCI\n", prefix);
      printf("%s attr PCI secondary bus = %02x\n",
	     prefix, obj->attr->bridge.downstream.pci.secondary_bus);
      printf("%s attr PCI subordinate bus = %02x\n",
	     prefix, obj->attr->bridge.downstream.pci.subordinate_bus);
      break;
    }
    break;
  case HWLOC_OBJ_PCI_DEVICE:
    printf("%s attr PCI bus id = %04x:%02x:%02x.%01x\n",
	   prefix, obj->attr->pcidev.domain, obj->attr->pcidev.bus, obj->attr->pcidev.dev, obj->attr->pcidev.func);
    printf("%s attr PCI class = %04x\n",
	   prefix, obj->attr->pcidev.class_id);
    printf("%s attr PCI id = %04x:%04x\n",
	   prefix, obj->attr->pcidev.vendor_id, obj->attr->pcidev.device_id);
    if (obj->attr->pcidev.linkspeed)
      printf("%s attr PCI linkspeed = %f GB/s\n", prefix, obj->attr->pcidev.linkspeed);
    break;
  case HWLOC_OBJ_OS_DEVICE:
    printf("%s attr osdev type = %s\n", prefix, type);
    break;
  default:
    /* nothing to show */
    break;
  }
  for(i=0; i<obj->infos_count; i++) {
    printf("%s info %s = %s\n", prefix, obj->infos[i].name, obj->infos[i].value);
  }
}

static void
hwloc_calc_process_arg_info_cb(void *_data __hwloc_attribute_unused,
			       hwloc_obj_t obj,
			       int verbose)
{
  char prefix[32];
  char objs[128];

  prefix[0] = '\0';
  if (show_index_prefix)
    snprintf(prefix, sizeof(prefix), "%u: ", current_obj);

  hwloc_obj_type_snprintf(objs, sizeof(objs), obj, 1);

  if (show_ancestors) {
    char parents[128];
    unsigned level = 0;
    hwloc_obj_t parent = obj;
    while (parent) {
      if (show_index_prefix)
	snprintf(prefix, sizeof(prefix), "%u.%u: ", current_obj, level);
      hwloc_obj_type_snprintf(parents, sizeof(parents), parent, 1);
      if (verbose < 0)
	printf("%s%s:%u\n", prefix, parents, parent->logical_index);
      else if (level)
	printf("%s%s L#%u = parent #%u of %s L#%u\n",
	       prefix, parents, parent->logical_index, level, objs, obj->logical_index);
      else
	printf("%s%s L#%u\n", prefix, parents, parent->logical_index);
      hwloc_info_show_obj(parent, parents, prefix, verbose);
      parent = parent->parent;
      level++;
    }
  } else if (show_ancestor_type != (hwloc_obj_type_t) -1) {
    char parents[128];
    hwloc_obj_t parent = obj;
    while (parent) {
      if (parent->type == show_ancestor_type) {
	if (parent->type == HWLOC_OBJ_GROUP
	    && show_ancestor_attrdepth != -1
	    && show_ancestor_attrdepth != (int) parent->attr->group.depth)
	  goto next;
	if (parent->type == HWLOC_OBJ_CACHE
	    && show_ancestor_attrdepth != -1
	    && show_ancestor_attrdepth != (int) parent->attr->cache.depth)
	  goto next;
	if (parent->type == HWLOC_OBJ_CACHE
	    && show_ancestor_attrcachetype != (hwloc_obj_cache_type_t) -1
	    && parent->attr->cache.type != HWLOC_OBJ_CACHE_UNIFIED
	    && show_ancestor_attrcachetype != parent->attr->cache.type)
	  goto next;
	hwloc_obj_type_snprintf(parents, sizeof(parents), parent, 1);
	if (verbose < 0)
	  printf("%s%s:%u\n", prefix, parents, parent->logical_index);
	else
	  printf("%s%s L#%u = parent of %s L#%u\n",
		 prefix, parents, parent->logical_index, objs, obj->logical_index);
	hwloc_info_show_obj(parent, parents, prefix, verbose);
      }
next:
      parent = parent->parent;
    }
  } else {
    if (verbose < 0)
      printf("%s%s:%u\n", prefix, objs, obj->logical_index);
    else
      printf("%s%s L#%u\n", prefix, objs, obj->logical_index);
    hwloc_info_show_obj(obj, objs, prefix, verbose);
  }

  current_obj++;
}

int
main (int argc, char *argv[])
{
  int err;
  hwloc_topology_t topology;
  unsigned topodepth;
  unsigned long flags = HWLOC_TOPOLOGY_FLAG_IO_DEVICES | HWLOC_TOPOLOGY_FLAG_IO_BRIDGES | HWLOC_TOPOLOGY_FLAG_ICACHES;
  char * callname;
  char * input = NULL;
  enum hwloc_utils_input_format input_format = HWLOC_UTILS_INPUT_DEFAULT;
  char *restrictstring = NULL;
  size_t typelen;
  int opt;
  enum hwloc_info_mode { HWLOC_INFO_MODE_UNKNOWN, HWLOC_INFO_MODE_TOPOLOGY, HWLOC_INFO_MODE_OBJECTS, HWLOC_INFO_MODE_SUPPORT } mode = HWLOC_INFO_MODE_UNKNOWN;

  /* enable verbose backends */
  putenv("HWLOC_XML_VERBOSE=1");
  putenv("HWLOC_SYNTHETIC_VERBOSE=1");

  callname = strrchr(argv[0], '/');
  if (!callname)
    callname = argv[0];
  else
    callname++;
  /* skip argv[0], handle options */
  argc--;
  argv++;

  err = hwloc_topology_init (&topology);
  if (err)
    return EXIT_FAILURE;

  hwloc_topology_set_flags(topology, flags);

  while (argc >= 1) {
    opt = 0;
    if (*argv[0] == '-') {
      if (!strcmp (argv[0], "--objects"))
	mode = HWLOC_INFO_MODE_OBJECTS;
      else if (!strcmp (argv[0], "--topology"))
	mode = HWLOC_INFO_MODE_TOPOLOGY;
      else if (!strcmp (argv[0], "--support"))
	mode = HWLOC_INFO_MODE_SUPPORT;
      else if (!strcmp (argv[0], "-v") || !strcmp (argv[0], "--verbose"))
        verbose_mode++;
      else if (!strcmp (argv[0], "-s") || !strcmp (argv[0], "--silent"))
        verbose_mode--;
      else if (!strcmp (argv[0], "-h") || !strcmp (argv[0], "--help")) {
	usage(callname, stdout);
        exit(EXIT_SUCCESS);
      }
      else if (!strcmp (argv[0], "-n"))
	show_index_prefix = 1;
      else if (!strcmp (argv[0], "--ancestors"))
	show_ancestors = 1;
      else if (!strcmp (argv[0], "--ancestor")) {
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
	err = hwloc_obj_type_sscanf(argv[1], &show_ancestor_type, &show_ancestor_attrdepth, &show_ancestor_attrcachetype, sizeof(show_ancestor_attrcachetype));
        if (err < 0) {
          fprintf(stderr, "unrecognized --ancestor type %s\n", argv[1]);
          usage(callname, stderr);
          return EXIT_FAILURE;
        }
	opt = 1;
      }
      else if (!strcmp (argv[0], "--no-icaches"))
	flags &= ~HWLOC_TOPOLOGY_FLAG_ICACHES;
      else if (!strcmp (argv[0], "--whole-system"))
	flags |= HWLOC_TOPOLOGY_FLAG_WHOLE_SYSTEM;
      else if (!strcmp (argv[0], "--no-io"))
	flags &= ~(HWLOC_TOPOLOGY_FLAG_IO_DEVICES | HWLOC_TOPOLOGY_FLAG_IO_BRIDGES);
      else if (!strcmp (argv[0], "--no-bridges"))
	flags &= ~(HWLOC_TOPOLOGY_FLAG_IO_BRIDGES);
      else if (!strcmp (argv[0], "--whole-io"))
	flags |= HWLOC_TOPOLOGY_FLAG_WHOLE_IO;
      else if (!strcmp (argv[0], "--thissystem"))
	flags |= HWLOC_TOPOLOGY_FLAG_IS_THISSYSTEM;
      else if (!strcmp (argv[0], "--restrict")) {
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
	restrictstring = strdup(argv[1]);
	opt = 1;
      }

      else if (hwloc_utils_lookup_input_option(argv, argc, &opt,
					       &input, &input_format,
					       callname)) {
	/* we'll enable later */
      }
      else if (!strcmp (argv[0], "--pid")) {
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
	pid_number = atoi(argv[1]); opt = 1;
      }
      else if (!strcmp(argv[0], "-l") || !strcmp(argv[0], "--logical"))
	logical = 1;
      else if (!strcmp(argv[0], "-p") || !strcmp(argv[0], "--physical"))
	logical = 0;
      else if (!strcmp (argv[0], "--version")) {
        printf("%s %s\n", callname, HWLOC_VERSION);
        exit(EXIT_SUCCESS);
      }
      else {
	fprintf (stderr, "Unrecognized option: %s\n", argv[0]);
	usage(callname, stderr);
	return EXIT_FAILURE;
      }
      argc -= opt+1;
      argv += opt+1;
    } else {
      /* not an option */
      break;
    }
  }

  hwloc_topology_set_flags(topology, flags);

  if (input) {
    err = hwloc_utils_enable_input_format(topology, input, &input_format, verbose_mode, callname);
    if (err)
      return err;
  }

  if (pid_number > 0) {
    pid = hwloc_pid_from_number(pid_number, 0);
    if (hwloc_topology_set_pid(topology, pid)) {
      perror("Setting target pid");
      return EXIT_FAILURE;
    }
  }

  err = hwloc_topology_load (topology);
  if (err)
    return EXIT_FAILURE;

  topodepth = hwloc_topology_get_depth(topology);

  if (restrictstring) {
    hwloc_bitmap_t restrictset = hwloc_bitmap_alloc();
    if (!strcmp (restrictstring, "binding")) {
      if (pid_number > 0)
	hwloc_get_proc_cpubind(topology, pid, restrictset, HWLOC_CPUBIND_PROCESS);
      else
	hwloc_get_cpubind(topology, restrictset, HWLOC_CPUBIND_PROCESS);
    } else {
      hwloc_bitmap_sscanf(restrictset, restrictstring);
    }
    err = hwloc_topology_restrict (topology, restrictset, 0);
    if (err) {
      perror("Restricting the topology");
      /* fallthrough */
    }
    hwloc_bitmap_free(restrictset);
    free(restrictstring);
  }

  if (mode == HWLOC_INFO_MODE_UNKNOWN) {
    if (argc)
      mode = HWLOC_INFO_MODE_OBJECTS;
    else
      mode = HWLOC_INFO_MODE_TOPOLOGY;
  }

  if (mode == HWLOC_INFO_MODE_TOPOLOGY) {
    hwloc_lstopo_show_summary(stdout, topology);

  } else if (mode == HWLOC_INFO_MODE_SUPPORT) {
    const struct hwloc_topology_support *support = hwloc_topology_get_support(topology);
#define DO(x,y) printf(#x ":" #y " = %u\n", (unsigned char) support->x->y);
    DO(discovery, pu);

    DO(cpubind, set_thisproc_cpubind);
    DO(cpubind, get_thisproc_cpubind);
    DO(cpubind, set_proc_cpubind);
    DO(cpubind, get_proc_cpubind);
    DO(cpubind, set_thisthread_cpubind);
    DO(cpubind, get_thisthread_cpubind);
    DO(cpubind, set_thread_cpubind);
    DO(cpubind, get_thread_cpubind);
    DO(cpubind, get_thisproc_last_cpu_location);
    DO(cpubind, get_proc_last_cpu_location);
    DO(cpubind, get_thisthread_cpubind);

    DO(membind, set_thisproc_membind);
    DO(membind, get_thisproc_membind);
    DO(membind, set_proc_membind);
    DO(membind, get_proc_membind);
    DO(membind, set_thisthread_membind);
    DO(membind, get_thisthread_membind);
    DO(membind, set_area_membind);
    DO(membind, get_area_membind);
    DO(membind, alloc_membind);
    DO(membind, firsttouch_membind);
    DO(membind, bind_membind);
    DO(membind, interleave_membind);
    DO(membind, nexttouch_membind);
    DO(membind, migrate_membind);
    DO(membind, get_area_memlocation);

  } else if (mode == HWLOC_INFO_MODE_OBJECTS) {
    current_obj = 0;
    while (argc >= 1) {
      if (!strcmp(argv[0], "all") || !strcmp(argv[0], "root")) {
	hwloc_calc_process_arg_info_cb(NULL, hwloc_get_root_obj(topology), verbose_mode);
      } else {
	/* try to match a type/depth followed by a special character */
	typelen = strspn(argv[0], "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
	if (typelen && (argv[0][typelen] == ':' || argv[0][typelen] == '=' || argv[0][typelen] == '[')) {
	  err = hwloc_calc_process_type_arg(topology, topodepth, argv[0], typelen, logical,
					    hwloc_calc_process_arg_info_cb, NULL,
					    verbose_mode);
	}
      }
      argc--; argv++;
    }

  } else assert(0);

  hwloc_topology_destroy (topology);

  return EXIT_SUCCESS;
}
