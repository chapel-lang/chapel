/*
 * Copyright © 2009-2011 inria.  All rights reserved.
 * Copyright © 2009-2010 Université Bordeaux 1
 * Copyright © 2009-2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <private/private.h>
#include <hwloc.h>

#include "misc.h"

#include <unistd.h>

void usage(const char *callname __hwloc_attribute_unused, FILE *where)
{
  fprintf(where, "Usage: hwloc-distances [options] number\n");
  fprintf (where, "\nFormatting options:\n");
  fprintf (where, "  -l --logical          Display hwloc logical object indexes\n");
  fprintf (where, "                        (default for console output)\n");
  fprintf (where, "  -p --physical         Display physical object indexes\n");
  fprintf (where, "                        (default for graphical output)\n");
  fprintf(where, "Input topology options:\n");
  fprintf(where, "  --restrict <set> Restrict the topology to processors listed in <set>\n");
  hwloc_utils_input_format_usage(where, 0);
  fprintf(where, "Miscellaneous options:\n");
  fprintf(where, "  -v               Show verbose messages\n");
  fprintf(where, "  --version        Report version and exit\n");
}

int main(int argc, char *argv[])
{
  char *callname;
  char *input = NULL;
  enum hwloc_utils_input_format input_format = HWLOC_UTILS_INPUT_DEFAULT;
  char *restrictstring = NULL;
  hwloc_topology_t topology;
  unsigned i, depth;
  int logical = 1;
  int verbose = 0;
  int opt;
  int err;

  /* enable verbose backends */
  putenv("HWLOC_XML_VERBOSE=1");
  putenv("HWLOC_SYNTHETIC_VERBOSE=1");

  hwloc_topology_init(&topology);

  /* skip argv[0], handle options */
  callname = argv[0];
  argv++;
  argc--;

  while (argc >= 1) {
    if (!strcmp(argv[0], "--")) {
      argc--;
      argv++;
      break;
    }

    if (*argv[0] == '-') {
      if (!strcmp(argv[0], "--help")) {
	usage(callname, stdout);
	return EXIT_SUCCESS;
      }
      if (!strcmp(argv[0], "-v")) {
	verbose++;
	goto next;
      }
      if (!strcmp (argv[0], "-l") || !strcmp (argv[0], "--logical")) {
	logical = 1;
	goto next;
      }
      if (!strcmp (argv[0], "-p") || !strcmp (argv[0], "--physical")) {
	logical = 0;
	goto next;
      }
      if (hwloc_utils_lookup_input_option(argv, argc, &opt,
					  &input, &input_format,
					  callname)) {
	argv += opt;
	argc -= opt;
	goto next;
      }
      else if (!strcmp (argv[0], "--restrict")) {
	if (argc <= 2) {
	  usage (callname, stdout);
	  exit(EXIT_FAILURE);
	}
	restrictstring = strdup(argv[1]);
	argc--;
	argv++;
	goto next;
      }
      else if (!strcmp (argv[0], "--version")) {
          printf("%s %s\n", callname, VERSION);
          exit(EXIT_SUCCESS);
      }

      fprintf (stderr, "Unrecognized option: %s\n", argv[0]);
      usage(callname, stderr);
      return EXIT_FAILURE;
    }

  next:
    argc--;
    argv++;
  }

  if (input) {
    err = hwloc_utils_enable_input_format(topology, input, input_format, verbose, callname);
    if (err)
      return err;
  }
  hwloc_topology_load(topology);

  if (restrictstring) {
    hwloc_bitmap_t restrictset = hwloc_bitmap_alloc();
    hwloc_bitmap_sscanf(restrictset, restrictstring);
    err = hwloc_topology_restrict (topology, restrictset, 0);
    if (err) {
      perror("Restricting the topology");
      /* fallthrough */
    }
    hwloc_bitmap_free(restrictset);
    free(restrictstring);
  }

  depth = hwloc_topology_get_depth(topology);
  for(i=0; i<depth; i++) {
    unsigned nbobjs, j;
    nbobjs = hwloc_get_nbobjs_by_depth(topology, i);
    for(j=0; j<nbobjs; j++) {
      hwloc_obj_t obj = hwloc_get_obj_by_depth(topology, i, j);
      unsigned k;
      char roottypestring[32];
      hwloc_obj_type_snprintf (roottypestring, sizeof(roottypestring), obj, 0);
      for(k=0; k<obj->distances_count; k++) {
	struct hwloc_distances_s *distances = obj->distances[k];
	if (!distances->latency)
	  continue;
	printf("Latency matrix between %u %ss (depth %u) by %s indexes (below %s%s%u):\n",
	       distances->nbobjs,
	       hwloc_obj_type_string(hwloc_get_depth_type(topology, i+distances->relative_depth)),
	       i+distances->relative_depth,
	       logical ? "logical" : "physical",
	       roottypestring,
	       logical ? " L#" :  " P#",
	       logical ? obj->logical_index : obj->os_index);
	hwloc_utils_print_distance_matrix(topology, obj, distances->nbobjs, distances->relative_depth, distances->latency, logical);
      }
    }
  }

  hwloc_topology_destroy(topology);

  return EXIT_SUCCESS;
}
