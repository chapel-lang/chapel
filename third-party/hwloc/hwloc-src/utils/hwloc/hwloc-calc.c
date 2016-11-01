/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2016 Inria.  All rights reserved.
 * Copyright © 2009-2011 Université Bordeaux
 * Copyright © 2009-2010 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <private/private.h>
#include <hwloc-calc.h>
#include <hwloc.h>

#include "misc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void usage(const char *callname __hwloc_attribute_unused, FILE *where)
{
  fprintf(where, "Usage: hwloc-calc [options] <location> ...\n");
  fprintf(where, " <location> may be a space-separated list of cpusets or objects\n");
  fprintf(where, "            as supported by the hwloc-bind utility, e.g:\n");
  hwloc_calc_locations_usage(where);
  fprintf(where, "Conversion options:\n");
  fprintf(where, "  [default]                 Report the combined input locations as a CPU set\n");
  fprintf(where, "  --number-of <type|depth>\n"
                 "  -N <type|depth>           Report the number of objects intersecting the CPU set\n");
  fprintf(where, "  --intersect <type|depth>\n"
		 "  -I <type|depth>           Report the indexes of object intersecting the CPU set\n");
  fprintf(where, "  --hierarchical <type1>.<type2>...\n"
		 "  -H <type1>.<type2>...     Find the list of objects intersecting the CPU set and\n"
		 "                            display them as hierarchical combinations such as\n"
		 "                            type1:index1.type2:index2...\n");
  fprintf(where, "  --largest                 Report the list of largest objects in the CPU set\n");
  fprintf(where, "Formatting options:\n");
  fprintf(where, "  -l --logical              Use logical object indexes (default)\n");
  fprintf(where, "  -p --physical             Use physical object indexes\n");
  fprintf(where, "  --li --logical-input      Use logical indexes for input (default)\n");
  fprintf(where, "  --lo --logical-output     Use logical indexes for output (default)\n");
  fprintf(where, "  --pi --physical-input     Use physical indexes for input\n");
  fprintf(where, "  --po --physical-output    Use physical indexes for output\n");
  fprintf(where, "  --sep <sep>               Use separator <sep> in the output\n");
  fprintf(where, "  --taskset                 Use taskset-specific format when displaying cpuset strings\n");
  fprintf(where, "  --single                  Singlify the output to a single CPU\n");
  fprintf(where, "Input topology options:\n");
  fprintf(where, "  --restrict <cpuset>       Restrict the topology to processors listed in <cpuset>\n");
  fprintf(where, "  --whole-system            Do not consider administration limitations\n");
  hwloc_utils_input_format_usage(where, 10);
  fprintf(where, "Miscellaneous options:\n");
  fprintf(where, "  -q --quiet                Hide non-fatal error messages\n");
  fprintf(where, "  -v --verbose              Show verbose messages\n");
  fprintf(where, "  --version                 Report version and exit\n");
}

static int verbose = 0;
static int logicali = 1;
static int logicalo = 1;
static int numberofdepth = -1;
static int intersectdepth = -1;
static int hiernblevels = 0;
static int *hierdepth = NULL;
static int showobjs = 0;
static int singlify = 0;
static int taskset = 0;

static void
hwloc_calc_hierarch_output(hwloc_topology_t topology, const char *prefix, const char *sep, hwloc_obj_t root, hwloc_bitmap_t set, int level)
{
  hwloc_obj_t obj, prev = NULL;
  unsigned logi = 0;
  int first = 1;
  while ((obj = hwloc_get_next_obj_covering_cpuset_by_depth(topology, root->cpuset, hierdepth[level], prev)) != NULL) {
    char string[256];
    char type[32];
    if (!hwloc_bitmap_intersects(set, obj->cpuset))
     goto next;
    hwloc_obj_type_snprintf(type, sizeof(type), obj, 1);
    snprintf(string, sizeof(string), "%s%s%s:%u", prefix, level ? "." : "", type, logicalo ? logi : obj->os_index);
    if (!first)
      printf("%s", sep);
    first = 0;
    if (level != hiernblevels - 1) {
      hwloc_bitmap_t new = hwloc_bitmap_dup(set);
      hwloc_bitmap_and(new, new, obj->cpuset);
      hwloc_calc_hierarch_output(topology, string, sep, obj, new, level+1);
      hwloc_bitmap_free(new);
    } else {
      printf("%s", string);
    }
next:
    prev = obj;
    logi++;
  }
}

static int
hwloc_calc_output(hwloc_topology_t topology, const char *sep, hwloc_bitmap_t set)
{
  if (singlify)
    hwloc_bitmap_singlify(set);

  if (showobjs) {
    hwloc_bitmap_t remaining = hwloc_bitmap_dup(set);
    int first = 1;
    if (!sep)
      sep = " ";
    while (!hwloc_bitmap_iszero(remaining)) {
      char type[64];
      unsigned idx;
      hwloc_obj_t obj = hwloc_get_first_largest_obj_inside_cpuset(topology, remaining);
      if (!obj) {
        hwloc_bitmap_free(remaining);
        fprintf(stderr, "No object included in this cpuset\n");
        return EXIT_FAILURE;
      }
      hwloc_obj_type_snprintf(type, sizeof(type), obj, 1);
      idx = logicalo ? obj->logical_index : obj->os_index;
      if (idx == (unsigned) -1)
        printf("%s%s", first ? (const char *) "" : sep, type);
      else
        printf("%s%s:%u", first ? (const char *) "" : sep, type, idx);
      hwloc_bitmap_andnot(remaining, remaining, obj->cpuset);
      first = 0;
    }
    printf("\n");
    hwloc_bitmap_free(remaining);
  } else if (numberofdepth != -1) {
    unsigned nb = 0;
    hwloc_obj_t obj = NULL;
    while ((obj = hwloc_get_next_obj_covering_cpuset_by_depth(topology, set, numberofdepth, obj)) != NULL)
      nb++;
    printf("%u\n", nb);
  } else if (intersectdepth != -1) {
    hwloc_obj_t proc, prev = NULL;
    if (!sep)
      sep = ",";
    while ((proc = hwloc_get_next_obj_covering_cpuset_by_depth(topology, set, intersectdepth, prev)) != NULL) {
      if (prev)
	printf("%s", sep);
      printf("%u", logicalo ? proc->logical_index : proc->os_index);
      prev = proc;
    }
    printf("\n");
  } else if (hiernblevels) {
    if (!sep)
      sep = " ";
    hwloc_calc_hierarch_output(topology, "", sep, hwloc_get_root_obj(topology), set, 0);
    printf("\n");
  } else {
    char *string = NULL;
    if (taskset)
      hwloc_bitmap_taskset_asprintf(&string, set);
    else
      hwloc_bitmap_asprintf(&string, set);
    printf("%s\n", string);
    free(string);
  }
  return EXIT_SUCCESS;
}

static int hwloc_calc_type_depth(const char *string, hwloc_obj_type_t *typep, int *depthp)
{
  hwloc_obj_type_t type = (hwloc_obj_type_t) -1; /* in case we match a depth */
  int depth = -1;
  if (hwloc_obj_type_sscanf(string, &type, NULL, NULL, 0) < 0) {
    char *endptr;
    depth = strtoul(string, &endptr, 0);
    if (*endptr)
      return -1;
  }
  *depthp = depth;
  *typep = type;
  return 0;
}

static int hwloc_calc_check_type_depth(hwloc_topology_t topology, hwloc_obj_type_t type, int *depthp, const char *caller)
{
  if (type != (hwloc_obj_type_t) -1) {
    int depth = hwloc_get_type_depth(topology, type);
    if (depth == HWLOC_TYPE_DEPTH_UNKNOWN) {
      fprintf(stderr, "unavailable %s type %s\n", caller, hwloc_obj_type_string(type));
      return -1;
    } else  if (depth == HWLOC_TYPE_DEPTH_MULTIPLE) {
      fprintf(stderr, "cannot use %s type %s with multiple depth, please use the relevant depth directly\n", caller, hwloc_obj_type_string(type));
      return -1;
    }
    *depthp = depth;
  }
  return 0;
}

int main(int argc, char *argv[])
{
  hwloc_topology_t topology;
  unsigned long flags = HWLOC_TOPOLOGY_FLAG_WHOLE_IO|HWLOC_TOPOLOGY_FLAG_ICACHES;
  char *input = NULL;
  enum hwloc_utils_input_format input_format = HWLOC_UTILS_INPUT_DEFAULT;
  int input_changed = 0;
  unsigned depth;
  hwloc_bitmap_t set;
  int cmdline_args = 0;
  hwloc_obj_type_t numberoftype = (hwloc_obj_type_t) -1;
  hwloc_obj_type_t intersecttype = (hwloc_obj_type_t) -1;
  hwloc_obj_type_t *hiertype = NULL;
  char *callname;
  char *outsep = NULL;
  int opt;
  int i;
  int err;
  int ret = EXIT_SUCCESS;

  /* enable verbose backends */
  putenv("HWLOC_XML_VERBOSE=1");
  putenv("HWLOC_SYNTHETIC_VERBOSE=1");

  set = hwloc_bitmap_alloc();

  hwloc_topology_init(&topology);
  hwloc_topology_set_flags(topology, flags);
  hwloc_topology_load(topology);
  depth = hwloc_topology_get_depth(topology);

  callname = argv[0];
  /* skip argv[0], handle options */
  argv++;
  argc--;

  while (argc >= 1) {
    if (*argv[0] == '-') {
      if (!strcmp(argv[0], "-v") || !strcmp(argv[0], "--verbose")) {
        verbose++;
        goto next;
      }
      if (!strcmp(argv[0], "-q") || !strcmp(argv[0], "--quiet")) {
        verbose--;
        goto next;
      }
      if (!strcmp (argv[0], "--whole-system")) {
	flags |= HWLOC_TOPOLOGY_FLAG_WHOLE_SYSTEM;
	input_changed = 1;
	goto next;
      }
      if (!strcmp(argv[0], "--help")) {
	usage(callname, stdout);
	return EXIT_SUCCESS;
      }
      if (!strcmp (argv[0], "--restrict")) {
	hwloc_bitmap_t restrictset;
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
	restrictset = hwloc_bitmap_alloc();
	hwloc_bitmap_sscanf(restrictset, argv[1]);
	err = hwloc_topology_restrict (topology, restrictset, 0);
	if (err) {
	  perror("Restricting the topology");
	  /* fallthrough */
	}
	hwloc_bitmap_free(restrictset);
	argv++;
	argc--;
	goto next;
      }
      if (!strcmp(argv[0], "--number-of") || !strcmp(argv[0], "-N")) {
	if (argc < 2) {
	  usage(callname, stderr);
	  return EXIT_FAILURE;
	}
	if (hwloc_calc_type_depth(argv[1], &numberoftype, &numberofdepth) < 0) {
	  fprintf(stderr, "unrecognized --number-of type or depth %s\n", argv[1]);
	  usage(callname, stderr);
	  return EXIT_FAILURE;
	}
	argv++;
	argc--;
	goto next;
      }
      if (!strcmp(argv[0], "--intersect") || !strcmp(argv[0], "-I")) {
	if (argc < 2) {
	  usage(callname, stderr);
	  return EXIT_FAILURE;
	}
	if (hwloc_calc_type_depth(argv[1], &intersecttype, &intersectdepth) < 0) {
	  fprintf(stderr, "unrecognized --intersect type or depth %s\n", argv[1]);
	  usage(callname, stderr);
	  return EXIT_FAILURE;
	}
	argv++;
	argc--;
	goto next;
      }
      if (!strcmp(argv[0], "--hierarchical") || !strcmp(argv[0], "-H")) {
	char *tmp, *next;
	if (argc < 2) {
	  usage(callname, stderr);
	  return EXIT_FAILURE;
	}
	hiernblevels = 1;
	tmp = argv[1];
        while (1) {
	  tmp = strchr(tmp, '.');
	  if (!tmp)
	    break;
	  tmp++;
	  hiernblevels++;
        }
	hiertype = malloc(hiernblevels * sizeof(hwloc_obj_type_t));
	hierdepth = malloc(hiernblevels * sizeof(int));
	tmp = argv[1];
	for(i=0; i<hiernblevels; i++) {
	  next = strchr(tmp, '.');
	  if (next)
	    *next = '\0';
	  if (hwloc_calc_type_depth(tmp, &hiertype[i], &hierdepth[i]) < 0) {
	    fprintf(stderr, "unrecognized --hierarchical type or depth %s\n", tmp);
	    usage(callname, stderr);
	    return EXIT_FAILURE;
	  }
	  tmp = next+1;
	}
	argv++;
	argc--;
	goto next;
      }
      if (!strcasecmp(argv[0], "--pulist") || !strcmp(argv[0], "--proclist")) {
	/* backward compat with 1.0 */
	intersecttype = HWLOC_OBJ_PU;
        goto next;
      }
      if (!strcmp(argv[0], "--nodelist")) {
	/* backward compat with 1.0 */
	intersecttype = HWLOC_OBJ_NUMANODE;
        goto next;
      }
      if (!strcmp(argv[0], "--largest")  || !strcmp(argv[0], "--objects") /* backward compat with 1.0 */) {
	showobjs = 1;
        goto next;
      }
      if (!strcmp(argv[0], "--version")) {
        printf("%s %s\n", callname, HWLOC_VERSION);
        exit(EXIT_SUCCESS);
      }
      if (!strcmp(argv[0], "-l") || !strcmp(argv[0], "--logical")) {
	logicali = 1;
	logicalo = 1;
	goto next;
      }
      if (!strcmp(argv[0], "--li") || !strcmp(argv[0], "--logical-input")) {
	logicali = 1;
	goto next;
      }
      if (!strcmp(argv[0], "--lo") || !strcmp(argv[0], "--logical-output")) {
	logicalo = 1;
	goto next;
      }
      if (!strcmp(argv[0], "-p") || !strcmp(argv[0], "--physical")) {
	logicali = 0;
	logicalo = 0;
	goto next;
      }
      if (!strcmp(argv[0], "--pi") || !strcmp(argv[0], "--physical-input")) {
	logicali = 0;
	goto next;
      }
      if (!strcmp(argv[0], "--po") || !strcmp(argv[0], "--physical-output")) {
	logicalo = 0;
	goto next;
      }
      if (!strcmp(argv[0], "--sep")) {
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
	outsep = argv[1];
	argv++;
	argc--;
	goto next;
      }
      if (!strcmp(argv[0], "--single")) {
	singlify = 1;
	goto next;
      }
      if (!strcmp(argv[0], "--taskset")) {
	taskset = 1;
	goto next;
      }
      if (hwloc_utils_lookup_input_option(argv, argc, &opt,
					  &input, &input_format,
					  callname)) {
	argv += opt;
	argc -= opt;
	input_changed = 1;
	goto next;
      }

      fprintf (stderr, "Unrecognized option: %s\n", argv[0]);
      usage(callname, stderr);
      return EXIT_FAILURE;
    }

    if (input_changed) {
      /* flags or input was changed */
      hwloc_topology_destroy(topology);
      hwloc_topology_init(&topology);
      hwloc_topology_set_flags(topology, flags);
      if (input) {
	/* only update the input when actually using it */
	err = hwloc_utils_enable_input_format(topology, input, &input_format, verbose, callname);
	if (err)
	  return err;
      }
      hwloc_topology_load(topology);
      depth = hwloc_topology_get_depth(topology);
      input_changed = 0;
    }

    cmdline_args++;
    if (hwloc_calc_process_arg(topology, depth, argv[0], logicali, set, 0, 0, verbose) < 0)
      fprintf(stderr, "ignored unrecognized argument %s\n", argv[0]);

 next:
    argc--;
    argv++;
  }

  if (hwloc_calc_check_type_depth(topology, numberoftype, &numberofdepth, "--number-of") < 0)
    goto out;

  if (hwloc_calc_check_type_depth(topology, intersecttype, &intersectdepth, "--intersect") < 0)
    goto out;

  for(i=0; i<hiernblevels; i++)
    if (hwloc_calc_check_type_depth(topology, hiertype[i], &hierdepth[i], "--hierarchical") < 0)
      goto out;

  if (cmdline_args) {
    /* process command-line arguments */
    ret = hwloc_calc_output(topology, outsep, set);

  } else {
    /* process stdin arguments line-by-line */
#define HWLOC_CALC_LINE_LEN 64
    size_t len = HWLOC_CALC_LINE_LEN;
    char * line = malloc(len);

    if (input_changed) {
      /* flags or input was changed */
      hwloc_topology_destroy(topology);
      hwloc_topology_init(&topology);
      hwloc_topology_set_flags(topology, flags);
      if (input) {
	/* only update the input when actually using it */
	err = hwloc_utils_enable_input_format(topology, input, &input_format, verbose, callname);
	if (err)
	  return err;
      }
      hwloc_topology_load(topology);
      depth = hwloc_topology_get_depth(topology);
      input_changed = 0;
    }

    while (1) {
      char *current, *tmpline;

      /* stop if line is empty */
      if (!fgets(line, (int)len, stdin))
	break;

      /* keep reading until we get EOL */
      tmpline = line;
      while (!strchr(tmpline, '\n')) {
	char *tmp;
	tmp = realloc(line, len*2);
	if (!tmp) {
	  /* failed to allocate, ignore that line */
	  fprintf(stderr, "Failed to allocate line buffer, line ignored.\n");
	  free(line);
	  goto out;
	}
	line = tmp;
	tmpline = line + len-1;
	if (!fgets(tmpline, (int)(len+1), stdin))
	  break;
	len *= 2;
      }

      /* parse now that we got everything */
      current = line;
      hwloc_bitmap_zero(set);
      while (1) {
	char *token = strtok(current, " \n");
	if (!token)
	  break;
	current = NULL;
	if (hwloc_calc_process_arg(topology, depth, token, logicali, set, 0, 0, verbose) < 0)
	  fprintf(stderr, "ignored unrecognized argument %s\n", token);
      }
      hwloc_calc_output(topology, outsep, set);
    }
    free(line);
  }

 out:
  hwloc_topology_destroy(topology);

  hwloc_bitmap_free(set);

  free(hierdepth);
  free(hiertype);

  return ret;
}
