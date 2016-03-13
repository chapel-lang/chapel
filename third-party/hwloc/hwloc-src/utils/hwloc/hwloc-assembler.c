/*
 * Copyright © 2011-2012 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <private/autogen/config.h>
#include <hwloc.h>

#include "misc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void usage(const char *name, FILE *where)
{
  fprintf (where, "Usage: %s [options] <output>.xml [-n <name1] <input1>.xml [-n name2] <input2>.xml ...\n", name);
  fprintf (where, "Options:\n");
  fprintf (where, "  -v --verbose      Show verbose messages\n");
  fprintf (where, "  -f --force        Ignore errors while reading input files\n");
  fprintf (where, "  -n --name <name>  Set the name of the next input topology\n");
}

int main(int argc, char *argv[])
{
  hwloc_topology_t topology;
  char *callname;
  char *output;
  int verbose = 0;
  int force = 0;
  int opt;
  int i, j;

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
    if (!strcmp(argv[0], "-v") || !strcmp(argv[0], "--verbose")) {
      verbose++;
    } else if (!strcmp(argv[0], "-f") || !strcmp(argv[0], "--force")) {
      force = 1;
    } else if (!strcmp(argv[0], "-h") || !strcmp(argv[0], "--help")) {
      usage(callname, stdout);
      exit(EXIT_SUCCESS);
    } else if (!strcmp(argv[0], "--")) {
      argc--;
      argv++;
      break;
    } else if (*argv[0] == '-') {
      fprintf(stderr, "Unrecognized option: %s\n", argv[0]);
      usage(callname, stderr);
      exit(EXIT_FAILURE);
    } else
      break;
    argc -= opt+1;
    argv += opt+1;
  }

  if (!argc) {
    fprintf(stderr, "Missing output file name\n");
    usage(callname, stderr);
    exit(EXIT_FAILURE);
  }
  output = argv[0];
  argc--;
  argv++;

  hwloc_topology_init(&topology);
  hwloc_topology_set_flags(topology, HWLOC_TOPOLOGY_FLAG_WHOLE_IO|HWLOC_TOPOLOGY_FLAG_ICACHES);
  hwloc_topology_set_custom(topology);

  for(i=0, j=0; i<argc; i++, j++) {
    hwloc_topology_t input;
    hwloc_obj_t root;
    char idx[10];
    char *name = NULL;

    if (!strcmp(argv[i], "-n") || !strcmp(argv[i], "--name")) {
      if (i+2 >= argc) {
	usage(callname, stderr);
	exit(EXIT_FAILURE);
      }
      name = argv[i+1];
      i+=2;
    }

    if (verbose) {
      if (name)
	printf("Importing XML topology %s with name %s ...\n", argv[i], name);
      else
	printf("Importing XML topology %s ...\n", argv[i]);
    }

    hwloc_topology_init(&input);
    hwloc_topology_set_flags(input, HWLOC_TOPOLOGY_FLAG_WHOLE_IO|HWLOC_TOPOLOGY_FLAG_ICACHES);
    if (hwloc_topology_set_xml(input, argv[i])) {
      fprintf(stderr, "Failed to set source XML file %s (%s)\n", argv[i], strerror(errno));
      hwloc_topology_destroy(input);
      if (force)
	continue;
      else
	return EXIT_FAILURE;
    }
    hwloc_topology_load(input);

    root = hwloc_get_root_obj(input);
    hwloc_obj_add_info(root, "AssemblerName", name ? name : argv[i]);
    snprintf(idx, sizeof(idx), "%d", j);
    hwloc_obj_add_info(root, "AssemblerIndex", idx);

    hwloc_custom_insert_topology(topology, hwloc_get_root_obj(topology), input, NULL);
    hwloc_topology_destroy(input);
  }

  if (verbose)
    printf("Assembling global topology...\n");
  hwloc_topology_load(topology);
  if (hwloc_topology_export_xml(topology, output) < 0) {
    fprintf(stderr, "Failed to export XML to %s (%s)\n", output, strerror(errno));
    return EXIT_FAILURE;
  }
  hwloc_topology_destroy(topology);
  if (verbose)
    printf("Exported topology to XML file %s\n", output);
  return 0;
}
