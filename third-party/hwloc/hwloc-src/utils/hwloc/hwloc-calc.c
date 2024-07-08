/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2022 Inria.  All rights reserved.
 * Copyright © 2009-2011 Université Bordeaux
 * Copyright © 2009-2010 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "private/autogen/config.h"
#include "hwloc-calc.h"
#include "hwloc.h"
#include "misc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void usage(const char *callname __hwloc_attribute_unused, FILE *where)
{
  fprintf(where, "Usage: hwloc-calc [topology options] [options] <location> ...\n");
  fprintf(where, " <location> may be a space-separated list of cpusets or objects\n");
  fprintf(where, "            as supported by the hwloc-bind utility, e.g:\n");
  hwloc_calc_locations_usage(where);
  fprintf(where, "Input topology options (must be at the beginning):\n");
  fprintf(where, "  --no-smt                  Only keep a single PU per core\n");
  fprintf(where, "  --cpukind <n>             Only keep PUs in the CPU kind <n>\n");
  fprintf(where, "  --cpukind <name>=<value>  Only keep PUs whose CPU kind match info <name>=<value>\n");
  fprintf(where, "  --restrict [nodeset=]<bitmap>\n");
  fprintf(where, "                            Restrict the topology to some processors or NUMA nodes.\n");
  fprintf(where, "  --restrict-flags <n>      Set the flags to be used during restrict\n");
  fprintf(where, "  --disallowed              Include objects disallowed by administrative limitations\n");
  hwloc_utils_input_format_usage(where, 10);
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
  fprintf(where, "  --local-memory            Report the memory nodes that are local to the CPU set\n");
  fprintf(where, "  --local-memory flags <x>  Change flags for selecting local memory nodes\n");
  fprintf(where, "  --best-memattr <attr>     Only report the best memory node among the local ones\n");
  fprintf(where, "Formatting options:\n");
  fprintf(where, "  -l --logical              Use logical object indexes (default)\n");
  fprintf(where, "  -p --physical             Use physical object indexes\n");
  fprintf(where, "  --li --logical-input      Use logical indexes for input (default)\n");
  fprintf(where, "  --lo --logical-output     Use logical indexes for output (default)\n");
  fprintf(where, "  --pi --physical-input     Use physical indexes for input\n");
  fprintf(where, "  --po --physical-output    Use physical indexes for output\n");
  fprintf(where, "  -n --nodeset              Manipulate nodesets instead of cpusets\n");
  fprintf(where, "  --ni --nodeset-input      Manipulate nodesets instead of cpusets for inputs\n");
  fprintf(where, "  --no --nodeset-output     Manipulate nodesets instead of cpusets for outputs\n");
  fprintf(where, "  --sep <sep>               Use separator <sep> in the output\n");
  fprintf(where, "  --taskset                 Use taskset-specific format when displaying cpuset strings\n");
  fprintf(where, "  --single                  Singlify the output to a single CPU\n");
  fprintf(where, "Miscellaneous options:\n");
  fprintf(where, "  -q --quiet                Hide non-fatal error messages\n");
  fprintf(where, "  -v --verbose              Show verbose messages\n");
  fprintf(where, "  --version                 Report version and exit\n");
  fprintf(where, "  -h --help                 Show this usage\n");
}

static int verbose = 0;
static int logicali = 1;
static int logicalo = 1;
static int nodeseti = 0;
static int nodeseto = 0;
static int numberofdepth = -1;
static union hwloc_obj_attr_u numberofattr;
static int intersectdepth = -1;
static union hwloc_obj_attr_u intersectattr;
static int hiernblevels = 0;
static int *hierdepth = NULL;
static int local_numanodes = 0;
static unsigned long local_numanode_flags = HWLOC_LOCAL_NUMANODE_FLAG_SMALLER_LOCALITY | HWLOC_LOCAL_NUMANODE_FLAG_LARGER_LOCALITY;
static hwloc_memattr_id_t best_memattr_id = (hwloc_memattr_id_t) -1;
static int showobjs = 0;
static int no_smt = -1;
static int singlify = 0;
static int taskset = 0;
static hwloc_bitmap_t cpukind_cpuset = NULL;

static int
hwloc_calc_intersects_set(hwloc_bitmap_t set, int use_nodeset, hwloc_obj_t obj)
{
  while (!hwloc_obj_type_is_normal(obj->type))
    obj = obj->parent;
  if (use_nodeset)
    return hwloc_bitmap_intersects(set, obj->nodeset);
  else
    return hwloc_bitmap_intersects(set, obj->cpuset);
}

/* generalization of hwloc_get_next_obj_covering_cpuset_by_depth() which may also use nodeset instead of cpuset */
static hwloc_obj_t
hwloc_calc_get_next_obj_covering_set_by_depth(hwloc_topology_t topology,
					      hwloc_bitmap_t set, int use_nodeset,
					      int depth,
					      hwloc_obj_t prev)
{
  hwloc_obj_t next = hwloc_get_next_obj_by_depth(topology, depth, prev);
  if (!next)
    return NULL;
  while (next && !hwloc_calc_intersects_set(set, use_nodeset, next))
    next = next->next_cousin;
  return next;
}

static void
hwloc_calc_hierarch_output(hwloc_topology_t topology, const char *prefix, const char *sep, hwloc_obj_t root, hwloc_bitmap_t set, int level)
{
  hwloc_obj_t obj, prev = NULL;
  unsigned logi = 0;
  int first = 1;
  while ((obj = hwloc_get_next_obj_covering_cpuset_by_depth(topology, root->cpuset, hierdepth[level], prev)) != NULL) {
    char string[256];
    char type[32];
    unsigned idx = logicalo ? logi : obj->os_index;
    if (!hwloc_bitmap_intersects(set, obj->cpuset))
     goto next;
    hwloc_obj_type_snprintf(type, sizeof(type), obj, 1);
    if (idx == (unsigned)-1)
      snprintf(string, sizeof(string), "%s%s%s:-1", prefix, level ? "." : "", type);
    else
      snprintf(string, sizeof(string), "%s%s%s:%u", prefix, level ? "." : "", type, idx);
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
  if (cpukind_cpuset)
    hwloc_bitmap_and(set, set, cpukind_cpuset);

  if (no_smt != -1 && !nodeseto) {
    if (hwloc_get_type_depth(topology, HWLOC_OBJ_CORE) == HWLOC_TYPE_DEPTH_UNKNOWN) {
      fprintf(stderr, "Topology has no Core object, ignoring --no-smt\n");
    } else {
      hwloc_bitmap_singlify_per_core(topology, set, no_smt);
    }
  }

  if (singlify)
    hwloc_bitmap_singlify(set);

  if (showobjs) {
    hwloc_bitmap_t remaining = hwloc_bitmap_dup(set);
    int first = 1;
    assert(!nodeseto); /* disabled for now, not very useful since the hierarchy of nodes isn't complex */
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
    while ((obj = hwloc_calc_get_next_obj_covering_set_by_depth(topology, set, nodeseto, numberofdepth, obj)) != NULL) {
      if (numberofdepth == HWLOC_TYPE_DEPTH_OS_DEVICE
          && numberofattr.osdev.type != (hwloc_obj_osdev_type_t) -1
          && numberofattr.osdev.type != obj->attr->osdev.type)
        continue;
      nb++;
    }
    printf("%u\n", nb);
  } else if (intersectdepth != -1) {
    hwloc_obj_t obj = NULL;
    int first = 1;
    if (!sep)
      sep = ",";
    while ((obj = hwloc_calc_get_next_obj_covering_set_by_depth(topology, set, nodeseto, intersectdepth, obj)) != NULL) {
      unsigned idx;
      if (intersectdepth == HWLOC_TYPE_DEPTH_OS_DEVICE
          && intersectattr.osdev.type != (hwloc_obj_osdev_type_t) -1
          && intersectattr.osdev.type != obj->attr->osdev.type)
        continue;
      if (!first)
	printf("%s", sep);
      idx = logicalo ? obj->logical_index : obj->os_index;
      if (idx == (unsigned)-1)
        printf("-1");
      else
        printf("%u", idx);
      first = 0;
    }
    printf("\n");
  } else if (hiernblevels) {
    if (!sep)
      sep = " ";
    hwloc_calc_hierarch_output(topology, "", sep, hwloc_get_root_obj(topology), set, 0);
    printf("\n");

  } else if (local_numanodes) {
    unsigned nrnodes;
    hwloc_obj_t *nodes;
    nrnodes = hwloc_bitmap_weight(hwloc_topology_get_topology_nodeset(topology));
    nodes = malloc(nrnodes * sizeof(*nodes));
    if (nodes) {
      int err;
      struct hwloc_location loc;
      loc.type = HWLOC_LOCATION_TYPE_CPUSET;
      loc.location.cpuset = set;
      err = hwloc_get_local_numanode_objs(topology, &loc, &nrnodes, nodes, local_numanode_flags);
      if (!err) {
        unsigned i;
        if (best_memattr_id != (hwloc_memattr_id_t) -1) {
          int best = hwloc_utils_get_best_node_in_array_by_memattr(topology, best_memattr_id, nrnodes, nodes, &loc);
          if (best == -1) {
            /* no perf info found, report nothing */
            nrnodes = 0;
          } else {
            /* only report the best nodes */
            nodes[0] = nodes[best];
            nrnodes = 1;
          }
        }
        if (!sep)
          sep = ",";
        for(i=0; i<nrnodes; i++) {
          char type[64];
          unsigned idx;
          hwloc_obj_type_snprintf(type, sizeof(type), nodes[i], 1);
          idx = logicalo ? nodes[i]->logical_index : nodes[i]->os_index;
          printf("%s%u", i==0 ? (const char *) "" : sep, idx);
        }
      }
      free(nodes);
    }
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

static int hwloc_calc_type_depth(hwloc_topology_t topology, const char *string, int *depthp, union hwloc_obj_attr_u *attrp, const char *caller)
{
  union hwloc_obj_attr_u attr;
  hwloc_obj_type_t type;
  int depth;
  int err;

  /* similar to hwloc_type_sscanf_as_depth() but we want to get attr as well */

  err = hwloc_type_sscanf(string, &type, &attr, sizeof(attr));
  if (err < 0) {
    char *endptr;
    depth = strtoul(string, &endptr, 0);
    if (*endptr) {
      fprintf(stderr, "unrecognized %s type or depth %s\n", caller, string);
      return -1;
    }

    *depthp = depth;
    return 0;
  }

  depth = hwloc_get_type_depth(topology, type);
  if (type == HWLOC_OBJ_GROUP
      && depth == HWLOC_TYPE_DEPTH_MULTIPLE
      && attr.group.depth != (unsigned)-1) {
    unsigned l;
    depth = HWLOC_TYPE_DEPTH_UNKNOWN;
    for(l=0; l<(unsigned) hwloc_topology_get_depth(topology); l++) {
      hwloc_obj_t tmp = hwloc_get_obj_by_depth(topology, l, 0);
      if (tmp->type == HWLOC_OBJ_GROUP && tmp->attr->group.depth == attr.group.depth) {
	depth = (int)l;
	break;
      }
    }
  }

  if (depth == HWLOC_TYPE_DEPTH_UNKNOWN) {
    fprintf(stderr, "unavailable %s type %s\n", caller, hwloc_obj_type_string(type));
    return -1;
  } else  if (depth == HWLOC_TYPE_DEPTH_MULTIPLE) {
    fprintf(stderr, "cannot use %s type %s with multiple depth, please use the relevant depth\n", caller, hwloc_obj_type_string(type));
    return -1;
  }

  if (attrp)
    memcpy(attrp, &attr, sizeof(attr));
  *depthp = depth;
  return 0;
}

int main(int argc, char *argv[])
{
  hwloc_topology_t topology;
  unsigned long flags = HWLOC_TOPOLOGY_FLAG_IMPORT_SUPPORT;
  unsigned long restrict_flags = 0;
  char *input = NULL;
  enum hwloc_utils_input_format input_format = HWLOC_UTILS_INPUT_DEFAULT;
  int depth = 0;
  hwloc_bitmap_t set;
  int cmdline_args = 0;
  const char * numberoftype = NULL;
  const char * intersecttype = NULL;
  char *restrictstring = NULL;
  char * hiertype = NULL;
  char * best_memattr_str = NULL;
  char *callname;
  char *outsep = NULL;
  int cpukind_index = -1;
  char *cpukind_infoname = NULL;
  char *cpukind_infovalue = NULL;
  int opt;
  int i;
  int err;
  int ret = EXIT_SUCCESS;
  struct hwloc_calc_location_context_s lcontext;
  struct hwloc_calc_set_context_s scontext;

  callname = strrchr(argv[0], '/');
  if (!callname)
    callname = argv[0];
  else
    callname++;

  /* skip argv[0], handle options */
  argv++;
  argc--;

  hwloc_utils_check_api_version(callname);

  /* enable verbose backends */
  if (!getenv("HWLOC_XML_VERBOSE"))
    putenv((char *) "HWLOC_XML_VERBOSE=1");
  if (!getenv("HWLOC_SYNTHETIC_VERBOSE"))
    putenv((char *) "HWLOC_SYNTHETIC_VERBOSE=1");

  set = hwloc_bitmap_alloc();

  while (argc >= 1) {
    opt = 0;

    if (!strcmp (argv[0], "--disallowed") || !strcmp (argv[0], "--whole-system")) {
      flags |= HWLOC_TOPOLOGY_FLAG_INCLUDE_DISALLOWED;
      goto next_config;
    }
    if (!strcmp (argv[0], "--restrict")) {
      if (argc < 2) {
	usage (callname, stderr);
	exit(EXIT_FAILURE);
      }
      if(strncmp(argv[1], "nodeset=", 7))
        restrictstring = strdup(argv[1]);
      else {
        restrictstring = strdup(argv[1]+8);
        restrict_flags |= HWLOC_RESTRICT_FLAG_BYNODESET;
      }
      opt = 1;
      goto next_config;
    }
    if (!strcmp (argv[0], "--restrict-flags")) {
      if (argc < 2) {
	usage (callname, stderr);
	exit(EXIT_FAILURE);
      }
      restrict_flags = hwloc_utils_parse_restrict_flags(argv[1]);
      opt = 1;
      goto next_config;
    }
    if (!strcmp(argv[0], "--cpukind")) {
      char *equal;
      if (argc < 2) {
        usage(callname, stderr);
        return EXIT_FAILURE;
      }
      equal = strchr(argv[1], '=');
      if (equal) {
        cpukind_infoname = argv[1];
        cpukind_infovalue = equal+1;
        *equal = 0;
      } else {
        cpukind_index = atoi(argv[1]);
      }
      opt = 1;
      goto next_config;
    }
    if (hwloc_utils_lookup_input_option(argv, argc, &opt,
					  &input, &input_format,
					  callname)) {
      opt = 1;
      goto next_config;
    }

    break;

  next_config:
    argc -= opt+1;
    argv += opt+1;
  }

  hwloc_topology_init(&topology);
  hwloc_topology_set_all_types_filter(topology, HWLOC_TYPE_FILTER_KEEP_ALL);
  hwloc_topology_set_flags(topology, flags);
  if (input) {
    err = hwloc_utils_enable_input_format(topology, flags, input, &input_format, verbose, callname);
    if (err) return EXIT_FAILURE;
  }
  err = hwloc_topology_load(topology);
  if (restrictstring) {
    hwloc_bitmap_t restrictset = hwloc_bitmap_alloc();
    hwloc_bitmap_sscanf(restrictset, restrictstring);
    if (hwloc_topology_restrict (topology, restrictset, restrict_flags)) {
      perror("Restricting the topology");
      /* FALLTHRU */
    }
    hwloc_bitmap_free(restrictset);
    free(restrictstring);
  }
  if (cpukind_index >= 0) {
    cpukind_cpuset = hwloc_bitmap_alloc();
    err = hwloc_cpukinds_get_info(topology, cpukind_index, cpukind_cpuset, NULL, NULL, NULL, 0);
    if (err < 0) {
      fprintf(stderr, "Couldn't find CPU kind #%d, keeping no PU.\n", cpukind_index);
      /* FALLTHRU */
    }
  } else if (cpukind_infoname && cpukind_infovalue) {
    hwloc_bitmap_t cpuset = hwloc_bitmap_alloc();
    int nr = hwloc_cpukinds_get_nr(topology, 0);
    cpukind_cpuset = hwloc_bitmap_alloc();
    for(i=0; i<nr; i++) {
      struct hwloc_info_s *infos;
      unsigned nr_infos, j;
      hwloc_cpukinds_get_info(topology, i, cpuset, NULL, &nr_infos, &infos, 0);
      for(j=0; j<nr_infos; j++)
        if (!strcmp(infos[j].name, cpukind_infoname) && !strcmp(infos[j].value, cpukind_infovalue)) {
          hwloc_bitmap_or(cpukind_cpuset, cpukind_cpuset, cpuset);
          break;
        }
    }
    hwloc_bitmap_free(cpuset);
    if (hwloc_bitmap_iszero(cpukind_cpuset)) {
      fprintf(stderr, "Couldn't find any CPU kind matching %s=%s, keeping no PU.\n", cpukind_infoname, cpukind_infovalue);
      /* FALLTHRU */
    }
  }

  while (argc >= 1) {
    opt = 0;

    if (*argv[0] == '-') {
      if (!strcmp (argv[0], "-h") || !strcmp (argv[0], "--help")) {
        usage(callname, stdout);
        exit(EXIT_SUCCESS);
      }
      if (!strcmp(argv[0], "-v") || !strcmp(argv[0], "--verbose")) {
        verbose++;
        goto next;
      }
      if (!strcmp(argv[0], "-q") || !strcmp(argv[0], "--quiet")) {
        verbose--;
        goto next;
      }
      if (!strcmp (argv[0], "--disallowed") || !strcmp (argv[0], "--whole-system")) {
        fprintf(stderr, "Input option %s disallowed after options using the topology\n", argv[0]);
	exit(EXIT_FAILURE);
      }
      if (!strcmp (argv[0], "--no-smt")) {
	no_smt = 0;
	goto next;
      }
      if (!strncmp(argv[0], "--no-smt=", 9)) {
	no_smt = atoi(argv[0] + 9);
	goto next;
      }
      if (!strcmp(argv[0], "--number-of") || !strcmp(argv[0], "-N")) {
	if (argc < 2) {
	  usage(callname, stderr);
	  return EXIT_FAILURE;
	}
	numberoftype = argv[1];
	opt = 1;
	goto next;
      }
      if (!strcmp(argv[0], "--intersect") || !strcmp(argv[0], "-I")) {
	if (argc < 2) {
	  usage(callname, stderr);
	  return EXIT_FAILURE;
	}
	intersecttype = argv[1];
	opt = 1;
	goto next;
      }
      if (!strcmp(argv[0], "--hierarchical") || !strcmp(argv[0], "-H")) {
	if (argc < 2) {
	  usage(callname, stderr);
	  return EXIT_FAILURE;
	}
	hiertype = argv[1];
	opt = 1;
	goto next;
      }
      if (!strcmp(argv[0], "--local-memory")) {
        local_numanodes = 1;
        goto next;
      }
      if (!strcmp(argv[0], "--local-memory-flags")) {
        if (argc < 2) {
          usage(callname, stderr);
          return EXIT_FAILURE;
        }
        local_numanodes = 1;
        local_numanode_flags = hwloc_utils_parse_local_numanode_flags(argv[1]);
        opt = 1;
        goto next;
      }
      if (!strcmp(argv[0], "--best-memattr")) {
        if (argc < 2) {
          usage(callname, stderr);
          return EXIT_FAILURE;
        }
        local_numanodes = 1;
        best_memattr_str = argv[1];
        opt = 1;
        goto next;
      }
      if (!strcasecmp(argv[0], "--pulist") || !strcmp(argv[0], "--proclist")) {
	/* backward compat with 1.0 */
	intersecttype = "pu";
        goto next;
      }
      if (!strcmp(argv[0], "--nodelist")) {
	/* backward compat with 1.0 */
	intersecttype = "numa";
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
      if (!strcmp(argv[0], "-n") || !strcmp(argv[0], "--nodeset")) {
	nodeseti = 1;
	nodeseto = 1;
	goto next;
      }
      if (!strcmp(argv[0], "--ni") || !strcmp(argv[0], "--nodeset-input")) {
	nodeseti = 1;
	goto next;
      }
      if (!strcmp(argv[0], "--no") || !strcmp(argv[0], "--nodeset-output")) {
	nodeseto = 1;
	goto next;
      }
      if (!strcmp(argv[0], "--sep")) {
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
	outsep = argv[1];
	opt = 1;
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

      fprintf (stderr, "Unrecognized option: %s\n", argv[0]);
      usage(callname, stderr);
      return EXIT_FAILURE;
    }

    cmdline_args++;
    lcontext.topology = topology;
    lcontext.topodepth = depth;
    lcontext.only_hbm = -1;
    lcontext.logical = logicali;
    lcontext.verbose = verbose;
    scontext.output_set = set;
    scontext.nodeset_input = nodeseti;
    scontext.nodeset_output = nodeseto;
    if (hwloc_calc_process_location_as_set(&lcontext, &scontext, argv[0]) < 0)
      fprintf(stderr, "ignored unrecognized argument %s\n", argv[0]);


    if (showobjs && nodeseto) {
      fprintf(stderr, "ignoring --nodeset-output when --largest output is enabled\n");
      nodeseto = 0;
    }

  next:
    argc -= opt+1;
    argv += opt+1;
  }

  if (numberoftype && hwloc_calc_type_depth(topology, numberoftype, &numberofdepth, &numberofattr, "--number-of") < 0)
    goto out;

  if (intersecttype && hwloc_calc_type_depth(topology, intersecttype, &intersectdepth, &intersectattr, "--intersect") < 0)
    goto out;

  if (hiertype) {
    char *tmp, *next;
    hiernblevels = 1;
    tmp = hiertype;
    while (1) {
      tmp = strchr(tmp, '.');
      if (!tmp)
	break;
      tmp++;
      hiernblevels++;
    }
    hierdepth = malloc(hiernblevels * sizeof(int));
    tmp = hiertype;
    for(i=0; i<hiernblevels; i++) {
      next = strchr(tmp, '.');
      if (next)
	*next = '\0';
      if (hwloc_calc_type_depth(topology, tmp, &hierdepth[i], NULL, "--hierarchical") < 0)
	goto out;
      if (hierdepth[i] < 0 && hierdepth[i] != HWLOC_TYPE_DEPTH_NUMANODE) {
	fprintf(stderr, "unsupported (non-normal) --hierarchical type %s\n", tmp);
	goto out;
      }
      tmp = next+1;
    }
  }

  if (best_memattr_str) {
    best_memattr_id = hwloc_utils_parse_memattr_name(topology, best_memattr_str);
    if (best_memattr_id == (hwloc_memattr_id_t) -1) {
      fprintf(stderr, "unrecognized memattr %s\n", best_memattr_str);
      return EXIT_FAILURE;
    }
  }

  if (cmdline_args) {
    /* process command-line arguments */
    ret = hwloc_calc_output(topology, outsep, set);

  } else {
    /* process stdin arguments line-by-line */
#define HWLOC_CALC_LINE_LEN 64
    size_t len = HWLOC_CALC_LINE_LEN;
    char * line = malloc(len);
    printf("Waiting for locations to process on stdin...\n");
    while (1) {
      char *current, *tmpline;

      /* stop if line is empty */
      if (!fgets(line, (int)len, stdin)){
        break;
      }
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
	lcontext.topology = topology;
	lcontext.topodepth = depth;
	lcontext.only_hbm = -1;
	lcontext.logical = logicali;
	lcontext.verbose = verbose;
	scontext.output_set = set;
	scontext.nodeset_input = nodeseti;
	scontext.nodeset_output = nodeseto;
	if (hwloc_calc_process_location_as_set(&lcontext, &scontext, token) < 0)
	  fprintf(stderr, "ignored unrecognized argument %s\n", token);
      }
      hwloc_calc_output(topology, outsep, set);
    }
    free(line);
  }

 out:
  hwloc_topology_destroy(topology);

  hwloc_bitmap_free(set);
  hwloc_bitmap_free(cpukind_cpuset);

  free(hierdepth);

  return ret;
}
