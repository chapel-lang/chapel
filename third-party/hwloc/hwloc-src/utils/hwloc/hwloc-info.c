/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2025 Inria.  All rights reserved.
 * Copyright © 2009-2012 Université Bordeaux
 * Copyright © 2009-2011 Cisco Systems, Inc.  All rights reserved.
 * Copyright © 2023 Université de Reims Champagne-Ardenne.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "private/autogen/config.h"
#include "hwloc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <assert.h>

#include "private/debug.h" /* for HWLOC_BUILD_ASSERT() */
#include "misc.h"
#include "hwloc-calc.h"

enum kind_e {
  KIND_ALL,
  KIND_NORMAL,
  KIND_CACHE,
  KIND_CPU,
  KIND_MEMORY,
  KIND_IO,
  KIND_NONE
};

static enum kind_e parse_kind(const char *name)
{
  if (!strcmp(name, "all"))
    return KIND_ALL;
  if (!strcmp(name, "normal"))
    return KIND_NORMAL;
  if (!strcmp(name, "cache"))
    return KIND_CACHE;
  if (!strcmp(name, "cpu"))
    return KIND_CPU;
  if (!strcmp(name, "memory"))
    return KIND_MEMORY;
  if (!strcmp(name, "io"))
    return KIND_IO;
  return KIND_NONE;
}

static int match_kind(hwloc_obj_t obj, enum kind_e kind)
{
  switch (kind) {
  case KIND_ALL:
    return 1;
  case KIND_NORMAL:
    return hwloc_obj_type_is_normal(obj->type);
  case KIND_CACHE:
    return hwloc_obj_type_is_cache(obj->type);
  case KIND_CPU:
    return hwloc_obj_type_is_normal(obj->type) && !hwloc_obj_type_is_cache(obj->type);
  case KIND_MEMORY:
    return hwloc_obj_type_is_memory(obj->type);
  case KIND_IO:
    return hwloc_obj_type_is_io(obj->type);
  case KIND_NONE:
    abort();
  }
  return 0;
}

static int pid_number = -1;
static hwloc_pid_t pid;
static int verbose_mode = 0;
static int logical = 1;
static int show_ancestors = 0;
static int show_ancestor_depth = HWLOC_TYPE_DEPTH_UNKNOWN;
static int show_ancestor_kind = KIND_NONE;
static int show_children = 0;
static int show_descendants_depth = HWLOC_TYPE_DEPTH_UNKNOWN;
static int show_descendants_kind = KIND_NONE;
static int show_index_prefix = 0;
static int show_first_only = 0;
static int show_local_memory = 0;
static int show_local_memory_flags = HWLOC_LOCAL_NUMANODE_FLAG_SMALLER_LOCALITY | HWLOC_LOCAL_NUMANODE_FLAG_LARGER_LOCALITY;
static hwloc_memattr_id_t best_memattr_id = (hwloc_memattr_id_t) -1;
static int show_default_memory = 0;
static unsigned long best_node_flags = 0;
static unsigned current_obj;
static const char *only_attr_name = NULL;

void usage(const char *name, FILE *where)
{
  fprintf (where, "Usage: %s [ options ] [ object | root | levels | topology | support ... ]\n", name);
  fprintf (where, "\nOutput options:\n");
  fprintf (where, "  --objects             Report information about specific objects\n");
  fprintf (where, "  --topology            Report information the topology\n");
  fprintf (where, "  --support             Report information about supported features\n");
  fprintf (where, "  -v --verbose          Include additional details\n");
  fprintf (where, "  -q --quiet -s         Reduce the amount of details to show\n");
  fprintf (where, "  --get-attr <name>     Only show the attribute line with name <name>\n");
  fprintf (where, "  --ancestors           Display the chain of ancestor objects up to the root\n");
  fprintf (where, "  --ancestor <type>     Only display the ancestor of the given type\n");
  fprintf (where, "  --children            Display all children\n");
  fprintf (where, "  --descendants <type>  Only display descendants of the given type\n");
  fprintf (where, "  --local-memory        Only display the local memory nodes\n");
  fprintf (where, "  --local-memory-flags <x>   Change flags for selecting local memory nodes\n");
  fprintf (where, "  --best-memattr <attr> Only display the best target among the local nodes\n");
  fprintf (where, "  --default-nodes       Only display the default local nodes\n");
  fprintf (where, "  --first               Only report the first matching object\n");
  fprintf (where, "  -n                    Prefix each line with the index of the considered object\n");
  fprintf (where, "Object filtering options:\n");
  fprintf (where, "  --restrict [nodeset=]<bitmap>\n");
  fprintf (where, "                        Restrict the topology to some processors or NUMA nodes.\n");
  fprintf (where, "  --restrict binding    Restrict the topology to the current process binding\n");
  fprintf (where, "  --restrict-flags <n>  Set the flags to be used during restrict\n");
  fprintf (where, "  --filter <type>:<knd> Filter objects of the given type, or all.\n");
  fprintf (where, "     <knd> may be `all' (keep all), `none' (remove all), `structure' or `important'\n");
  fprintf (where, "  --no-icaches          Do not show instruction caches\n");
  fprintf (where, "  --no-io               Do not show any I/O device or bridge\n");
  fprintf (where, "  --no-bridges          Do not any I/O bridge except hostbridges\n");
  fprintf (where, "  --whole-io            Show all I/O devices and bridges\n");
  fprintf (where, "Input options:\n");
  hwloc_utils_input_format_usage(where, 6);
  fprintf (where, "  --thissystem          Assume that the input topology provides the topology\n"
		  "                        for the system on which we are running\n");
  fprintf (where, "  --pid <pid>           Detect topology as seen by process <pid>\n");
  fprintf (where, "  --disallowed          Include objects disallowed by administrative limitations\n");
  fprintf (where, "  -l --logical          Use logical object indexes for input (default)\n");
  fprintf (where, "  -p --physical         Use physical object indexes for input\n");
  fprintf (where, "Miscellaneous options:\n");
  fprintf (where, "  --version             Report version and exit\n");
  fprintf (where, "  -h --help             Show this usage\n");
}

static void
hwloc_info_show_attr(const char *prefix, const char *name, const char *value)
{
  if (only_attr_name) {
    if (!strcmp(only_attr_name, name))
      printf("%s\n", value);
  } else {
    printf("%s %s = %s\n", prefix, name, value);
  }
}

static void
hwloc_info_show_obj(hwloc_topology_t topology, hwloc_obj_t obj, const char *type, const char *prefix, int verbose)
{
  char name[512], value[512];
  unsigned i;
  if (verbose < 0)
    return;

  hwloc_info_show_attr(prefix, "type", hwloc_obj_type_string(obj->type));
  hwloc_info_show_attr(prefix, "full type", type);
  if (obj->subtype)
    hwloc_info_show_attr(prefix, "subtype", obj->subtype);

  snprintf(value, sizeof(value), "%u", obj->logical_index);
  hwloc_info_show_attr(prefix, "logical index", value);
  if (obj->os_index != (unsigned) -1) {
    snprintf(value, sizeof(value), "%u", obj->os_index);
    hwloc_info_show_attr(prefix, "os index", value);
  }
  snprintf(value, sizeof(value), "%llu", (unsigned long long) obj->gp_index);
  hwloc_info_show_attr(prefix, "gp index", value);

  if (obj->name)
    hwloc_info_show_attr(prefix, "name", obj->name);

  snprintf(value, sizeof(value), "%d", obj->depth);
  hwloc_info_show_attr(prefix, "depth", value);
  snprintf(value, sizeof(value), "%u", obj->sibling_rank);
  hwloc_info_show_attr(prefix, "sibling rank", value);
  snprintf(value, sizeof(value), "%u", obj->arity);
  hwloc_info_show_attr(prefix, "children", value);
  snprintf(value, sizeof(value), "%u", obj->memory_arity);
  hwloc_info_show_attr(prefix, "memory children", value);
  snprintf(value, sizeof(value), "%u", obj->io_arity);
  hwloc_info_show_attr(prefix, "i/o children", value);
  snprintf(value, sizeof(value), "%u", obj->misc_arity);
  hwloc_info_show_attr(prefix, "misc children", value);

  if (obj->type == HWLOC_OBJ_NUMANODE) {
    snprintf(value, sizeof(value), "%llu", (unsigned long long) obj->attr->numanode.local_memory);
    hwloc_info_show_attr(prefix, "local memory", value);
  }
  if (obj->total_memory) {
    snprintf(value, sizeof(value), "%llu", (unsigned long long) obj->total_memory);
    hwloc_info_show_attr(prefix, "total memory", value);
  }

  if (obj->cpuset) {
    hwloc_bitmap_snprintf(value, sizeof(value), obj->cpuset);
    hwloc_info_show_attr(prefix, "cpuset", value);

    hwloc_bitmap_snprintf(value, sizeof(value), obj->complete_cpuset);
    hwloc_info_show_attr(prefix, "complete cpuset", value);

    {
      hwloc_bitmap_t allowed_cpuset = hwloc_bitmap_dup(obj->cpuset);
      hwloc_bitmap_and(allowed_cpuset, allowed_cpuset, hwloc_topology_get_allowed_cpuset(topology));
      hwloc_bitmap_snprintf(value, sizeof(value), allowed_cpuset);
      hwloc_bitmap_free(allowed_cpuset);
      hwloc_info_show_attr(prefix, "allowed cpuset", value);
    }

    hwloc_bitmap_snprintf(value, sizeof(value), obj->nodeset);
    hwloc_info_show_attr(prefix, "nodeset", value);

    hwloc_bitmap_snprintf(value, sizeof(value), obj->complete_nodeset);
    hwloc_info_show_attr(prefix, "complete nodeset", value);

    {
      hwloc_bitmap_t allowed_nodeset = hwloc_bitmap_dup(obj->nodeset);
      hwloc_bitmap_and(allowed_nodeset, allowed_nodeset, hwloc_topology_get_allowed_nodeset(topology));
      hwloc_bitmap_snprintf(value, sizeof(value), allowed_nodeset);
      hwloc_bitmap_free(allowed_nodeset);
      hwloc_info_show_attr(prefix, "allowed nodeset", value);
    }
  }

  switch (obj->type) {
  case HWLOC_OBJ_L1CACHE:
  case HWLOC_OBJ_L2CACHE:
  case HWLOC_OBJ_L3CACHE:
  case HWLOC_OBJ_L4CACHE:
  case HWLOC_OBJ_L5CACHE:
  case HWLOC_OBJ_L1ICACHE:
  case HWLOC_OBJ_L2ICACHE:
  case HWLOC_OBJ_L3ICACHE:
  case HWLOC_OBJ_MEMCACHE:
    snprintf(value, sizeof(value), "%u", obj->attr->cache.depth);
    hwloc_info_show_attr(prefix, "attr cache depth", value);
    switch (obj->attr->cache.type) {
    case HWLOC_OBJ_CACHE_UNIFIED: hwloc_info_show_attr(prefix, "attr cache type", "Unified"); break;
    case HWLOC_OBJ_CACHE_DATA: hwloc_info_show_attr(prefix, "attr cache type", "Data"); break;
    case HWLOC_OBJ_CACHE_INSTRUCTION: hwloc_info_show_attr(prefix, "attr cache type", "Instruction"); break;
    }
    snprintf(value, sizeof(value), "%llu", (unsigned long long) obj->attr->cache.size);
    hwloc_info_show_attr(prefix, "attr cache size", value);
    snprintf(value, sizeof(value), "%u", obj->attr->cache.linesize);
    hwloc_info_show_attr(prefix, "attr cache line size", value);
    if (obj->attr->cache.associativity == -1) {
      hwloc_info_show_attr(prefix, "attr cache line ways", "Fully-associative");
    } else if (obj->attr->cache.associativity != 0) {
      snprintf(value, sizeof(value), "%d", obj->attr->cache.associativity);
      hwloc_info_show_attr(prefix, "attr cache line ways", value);
    }
    break;
  case HWLOC_OBJ_GROUP:
    snprintf(value, sizeof(value), "%u", obj->attr->group.depth);
    hwloc_info_show_attr(prefix, "attr group depth", value);
    break;
  case HWLOC_OBJ_BRIDGE:
    switch (obj->attr->bridge.upstream_type) {
    case HWLOC_OBJ_BRIDGE_HOST:
      hwloc_info_show_attr(prefix, "attr bridge upstream type", "Host");
      break;
    case HWLOC_OBJ_BRIDGE_PCI:
      hwloc_info_show_attr(prefix, "attr bridge upstream type", "PCI");
      snprintf(value, sizeof(value), "%04x:%02x:%02x.%01x",
               obj->attr->pcidev.domain, obj->attr->pcidev.bus, obj->attr->pcidev.dev, obj->attr->pcidev.func);
      hwloc_info_show_attr(prefix, "attr PCI bus id", value);
      snprintf(value, sizeof(value), "%04x", obj->attr->pcidev.class_id);
      hwloc_info_show_attr(prefix, "attr PCI class", value);
      snprintf(value, sizeof(value), "%04x:%04x", obj->attr->pcidev.vendor_id, obj->attr->pcidev.device_id);
      hwloc_info_show_attr(prefix, "attr PCI id", value);
      if (obj->attr->pcidev.linkspeed) {
	snprintf(value, sizeof(value), "%f GB/s\n", obj->attr->pcidev.linkspeed);
        hwloc_info_show_attr(prefix, "attr PCI linkspeed", value);
      }
      break;
    }
    switch (obj->attr->bridge.downstream_type) {
    case HWLOC_OBJ_BRIDGE_HOST:
      assert(0);
    case HWLOC_OBJ_BRIDGE_PCI:
      hwloc_info_show_attr(prefix, "attr bridge downstream type", "PCI");
      snprintf(value, sizeof(value), "%02x", obj->attr->bridge.downstream.pci.secondary_bus);
      hwloc_info_show_attr(prefix, "attr PCI secondary bus bus", value);
      snprintf(value, sizeof(value), "%02x", obj->attr->bridge.downstream.pci.subordinate_bus);
      hwloc_info_show_attr(prefix, "attr PCI subordinate bus bus", value);
      break;
    }
    break;
  case HWLOC_OBJ_PCI_DEVICE:
    snprintf(value, sizeof(value), "%04x:%02x:%02x.%01x",
             obj->attr->pcidev.domain, obj->attr->pcidev.bus, obj->attr->pcidev.dev, obj->attr->pcidev.func);
    hwloc_info_show_attr(prefix, "attr PCI bus id", value);
    snprintf(value, sizeof(value), "%04x", obj->attr->pcidev.class_id);
    hwloc_info_show_attr(prefix, "attr PCI class", value);
    snprintf(value, sizeof(value), "%04x:%04x", obj->attr->pcidev.vendor_id, obj->attr->pcidev.device_id);
    hwloc_info_show_attr(prefix, "attr PCI id", value);
    if (obj->attr->pcidev.linkspeed) {
      snprintf(value, sizeof(value), "%f GB/s\n", obj->attr->pcidev.linkspeed);
      hwloc_info_show_attr(prefix, "attr PCI linkspeed", value);
    }
    break;
  case HWLOC_OBJ_OS_DEVICE:
    hwloc_info_show_attr(prefix, "attr osdev type", type);
    break;
  default:
    /* nothing to show */
    break;
  }

  snprintf(value, sizeof(value), "%d", obj->symmetric_subtree);
  hwloc_info_show_attr(prefix, "symmetric subtree", value);

  for(i=0; i<obj->infos_count; i++) {
    struct hwloc_info_s *info = &obj->infos[i];
    snprintf(name, sizeof(name), "info %s", info->name);
    hwloc_info_show_attr(prefix, name, info->value);
  }

  if (hwloc_obj_type_is_normal(obj->type)) {
    unsigned nr = hwloc_cpukinds_get_nr(topology, 0);
    hwloc_bitmap_t cpuset = hwloc_bitmap_alloc();
    for(i=0; i<nr; i++) {
      int efficiency;
      struct hwloc_info_s *infos;
      unsigned nr_infos, j;
      int partial;
      hwloc_cpukinds_get_info(topology, i, cpuset, &efficiency, &nr_infos, &infos, 0);
      if (hwloc_bitmap_isincluded(obj->cpuset, cpuset))
        partial = 0;
      else if (hwloc_bitmap_intersects(obj->cpuset, cpuset))
        partial = 1;
      else
        continue;
      snprintf(value, sizeof(value), "%u%s", i, partial ? " (partially)" : "");
      hwloc_info_show_attr(prefix, "cpukind", value);
      if (efficiency != -1) {
        snprintf(value, sizeof(value), "%d", efficiency);
        hwloc_info_show_attr(prefix, "cpukind efficiency", value);
      }
      for(j=0; j<nr_infos; j++) {
        snprintf(name, sizeof(name), "cpukind info %s", infos[j].name);
        hwloc_info_show_attr(prefix, name, infos[j].value);
      }
    }
    hwloc_bitmap_free(cpuset);
  }

  if (obj->type == HWLOC_OBJ_NUMANODE) {
    /* FIXME display for non-NUMA too.
     * but that's rare so maybe detect in advance whether it's needed?
     */
    unsigned id;
    for(id=0; ; id++) {
      const char *mname;
      unsigned long flags;
      int err;

      err = hwloc_memattr_get_name(topology, id, &mname);
      if (err < 0)
        break;
      err = hwloc_memattr_get_flags(topology, id, &flags);
      assert(!err);

      if (!(flags & HWLOC_MEMATTR_FLAG_NEED_INITIATOR)) {
        hwloc_uint64_t mvalue;
        err = hwloc_memattr_get_value(topology, id, obj, NULL, 0, &mvalue);
        if (!err) {
          snprintf(name, sizeof(name), "memory attribute %s", mname);
          snprintf(value, sizeof(value), "%llu", (unsigned long long) mvalue);
          hwloc_info_show_attr(prefix, name, value);
        }
      } else {
        unsigned nr_initiators = 0;
        err = hwloc_memattr_get_initiators(topology, id, obj, 0, &nr_initiators, NULL, NULL);
        if (!err) {
          struct hwloc_location *initiators = malloc(nr_initiators * sizeof(*initiators));
          hwloc_uint64_t *values = malloc(nr_initiators * sizeof(*values));
          if (initiators && values) {
            err = hwloc_memattr_get_initiators(topology, id, obj, 0, &nr_initiators, initiators, values);
            if (!err) {
              unsigned j;
              for(j=0; j<nr_initiators; j++) {
                char *inits, _inits[256];
                if (initiators[j].type == HWLOC_LOCATION_TYPE_CPUSET) {
                  hwloc_bitmap_asprintf(&inits, initiators[j].location.cpuset);
                } else if (initiators[j].type == HWLOC_LOCATION_TYPE_OBJECT) {
                  char types[64];
                  hwloc_obj_type_snprintf(types, sizeof(types), initiators[j].location.object, 1);
                  if (initiators[j].location.object->os_index != (unsigned)-1)
                    snprintf(_inits, sizeof(_inits), "%s L#%u P#%u", types, initiators[j].location.object->logical_index, initiators[j].location.object->os_index);
                  else
                    snprintf(_inits, sizeof(_inits), "%s L#%u", types, initiators[j].location.object->logical_index);
                  inits = _inits;
                } else {
                  assert(0);
                }
                snprintf(name, sizeof(name), "memory attribute %s from initiator %s", mname, inits);
                snprintf(value, sizeof(value), "%llu", (unsigned long long) values[j]);
                hwloc_info_show_attr(prefix, name, value);
                if (inits != _inits)
                  free(inits);
              }
            }
          }
          free(initiators);
          free(values);
        }
      }
    }
  }
}

static void
hwloc_info_show_ancestor(hwloc_topology_t topology, hwloc_obj_t ancestor,
                         hwloc_obj_t obj, const char *objs,
                         int level, const char *prefix, int verbose)
{
  char ancestors[128];
  hwloc_obj_type_snprintf(ancestors, sizeof(ancestors), ancestor, 1);
  if (!only_attr_name) {
    if (verbose < 0)
      printf("%s%s:%u\n", prefix, ancestors, ancestor->logical_index);
    else if (level > 0)
      printf("%s%s L#%u = parent #%u of %s L#%u\n",
             prefix, ancestors, ancestor->logical_index, level, objs, obj->logical_index);
    else if (level == 0) /* the object itself, don't show it twice */
      printf("%s%s L#%u\n",
             prefix, ancestors, ancestor->logical_index);
    else /* single ancestor */
      printf("%s%s L#%u = parent of %s L#%u\n",
             prefix, ancestors, ancestor->logical_index, objs, obj->logical_index);
  }
  hwloc_info_show_obj(topology, ancestor, ancestors, prefix, verbose);
}

static void
hwloc_info_show_descendant(hwloc_topology_t topology, hwloc_obj_t descendant,
                           hwloc_obj_t obj, const char *objs,
                           int number, const char *prefix, int verbose)
{
  char descendants[128];
  hwloc_obj_type_snprintf(descendants, sizeof(descendants), descendant, 1);
  if (!only_attr_name) {
    if (verbose < 0)
      printf("%s%s:%u\n", prefix, descendants, descendant->logical_index);
    else
      printf("%s%s L#%u = descendant #%u of %s L#%u\n",
             prefix, descendants, descendant->logical_index, number, objs, obj->logical_index);
  }
  hwloc_info_show_obj(topology, descendant, descendants, prefix, verbose);
}

static void
hwloc_info_show_child(hwloc_topology_t topology, hwloc_obj_t child,
                      hwloc_obj_t obj, const char *objs,
                      int number, const char *prefix, int verbose)
{
  char childs[128];
  hwloc_obj_type_snprintf(childs, sizeof(childs), child, 1);
  if (!only_attr_name) {
    if (verbose < 0)
      printf("%s%s:%u\n", prefix, childs, child->logical_index);
    else
      printf("%s%s L#%u = child #%u of %s L#%u\n",
             prefix, childs, child->logical_index, number, objs, obj->logical_index);
  }
  hwloc_info_show_obj(topology, child, childs, prefix, verbose);
}

static void
hwloc_info_show_local_memory(hwloc_topology_t topology, hwloc_obj_t node,
                             hwloc_obj_t obj, const char *objs,
                             int number, const char *prefix, int verbose)
{
  char nodes[128];
  hwloc_obj_type_snprintf(nodes, sizeof(nodes), node, 1);
  if (!only_attr_name) {
    if (verbose < 0)
      printf("%s%s:%u\n", prefix, nodes, node->logical_index);
    else
      printf("%s%s L#%u = local memory #%u of %s L#%u\n",
             prefix, nodes, node->logical_index, number, objs, obj->logical_index);
  }
  hwloc_info_show_obj(topology, node, nodes, prefix, verbose);
}

static void
hwloc_info_show_default_memory(hwloc_topology_t topology, hwloc_obj_t node,
                               hwloc_obj_t obj, const char *objs,
                               int number, const char *prefix, int verbose)
{
  char nodes[128];
  hwloc_obj_type_snprintf(nodes, sizeof(nodes), node, 1);
  if (!only_attr_name) {
    if (verbose < 0)
      printf("%s%s:%u\n", prefix, nodes, node->logical_index);
    else
      printf("%s%s L#%u = default node #%u of %s L#%u\n",
             prefix, nodes, node->logical_index, number, objs, obj->logical_index);
  }
  hwloc_info_show_obj(topology, node, nodes, prefix, verbose);
}

static void
hwloc_info_show_single_obj(hwloc_topology_t topology,
                           hwloc_obj_t obj, const char *objs,
                           const char *prefix, int verbose)
{
  if (!only_attr_name) {
    if (verbose < 0)
      printf("%s%s:%u\n", prefix, objs, obj->logical_index);
    else
      printf("%s%s L#%u\n", prefix, objs, obj->logical_index);
  }
  hwloc_info_show_obj(topology, obj, objs, prefix, verbose);
}

static void
hwloc_info_recurse_descendants(hwloc_topology_t topology,
                               hwloc_obj_t obj, const char *objs, /* where we started */
                               hwloc_obj_t root, /* current recursion root */
                               int *number,
                               int verbose)
{
  /* process the current object before recursing, so that we traverse special objects
   * as hwloc_list_special_objects() lists them on special levels.
   */
  hwloc_obj_t child;

  if (show_first_only && *number)
    return;

  if (root != obj /* don't show ourself */
      && match_kind(root, show_descendants_kind)) {
    char prefix[32] = "";
    if (show_index_prefix)
      snprintf(prefix, sizeof(prefix), "%u.%u: ", current_obj, *number);
    hwloc_info_show_descendant(topology, root, obj, objs, *number, prefix, verbose);
    if (show_first_only)
      return;
    (*number)++;
  }

  child = NULL;
  while ((child = hwloc_get_next_child(topology, root, child)) != NULL)
    hwloc_info_recurse_descendants(topology, obj, objs, child, number, verbose);
}

static void
hwloc_calc_process_location_info_cb(struct hwloc_calc_location_context_s *lcontext,
				    void *_data __hwloc_attribute_unused,
				    hwloc_obj_t obj)
{
  hwloc_topology_t topology = lcontext->topology;
  int verbose = lcontext->verbose;
  char prefix[32];
  char objs[128];

  prefix[0] = '\0';
  if (show_index_prefix)
    snprintf(prefix, sizeof(prefix), "%u: ", current_obj);

  hwloc_obj_type_snprintf(objs, sizeof(objs), obj, 1);

  if (show_ancestors) {
    unsigned level = 0;
    hwloc_obj_t parent = obj;
    while (parent) {
      if (show_index_prefix)
        snprintf(prefix, sizeof(prefix), "%u.%u: ", current_obj, level);
      hwloc_info_show_ancestor(topology, parent, obj, objs, level, prefix, verbose);
      if (show_first_only)
        break;
      parent = parent->parent;
      level++;
    }
  } else if (show_ancestor_depth != HWLOC_TYPE_DEPTH_UNKNOWN) {
    hwloc_obj_t parent = obj;
    while (parent) {
      if (parent->depth == show_ancestor_depth) {
        hwloc_info_show_ancestor(topology, parent, obj, objs, -1, prefix, verbose);
	break;
      }
      parent = parent->parent;
    }
  } else if (show_ancestor_kind != KIND_NONE) {
    hwloc_obj_t parent = obj->parent; /* don't show ourself */
    unsigned level = 0;
    while (parent) {
      if (match_kind(parent, show_ancestor_kind)) {
        if (show_index_prefix)
          snprintf(prefix, sizeof(prefix), "%u.%u: ", current_obj, level);
        hwloc_info_show_ancestor(topology, parent, obj, objs, level, prefix, verbose);
        if (show_first_only)
          break;
        level++;
      }
      parent = parent->parent;
    }
  } else if (show_children) {
    unsigned i = 0;
    hwloc_obj_t child = NULL;
    while ((child = hwloc_get_next_child(topology, obj, child)) != NULL) {
      if (show_index_prefix)
	snprintf(prefix, sizeof(prefix), "%u.%u: ", current_obj, i);
      hwloc_info_show_child(topology, child, obj, objs, i, prefix, verbose);
      if (show_first_only)
        break;
      i++;
    }
  } else if (show_descendants_depth != HWLOC_TYPE_DEPTH_UNKNOWN) {
    if (show_descendants_depth >= 0) {
      /* normal level */
      struct hwloc_calc_level level;
      unsigned i = 0, n;
      level.type = HWLOC_OBJ_TYPE_NONE;
      level.depth = show_descendants_depth;
      level.only_hbm = -1;
      level.subtype[0] = '\0';
      level.memory_tier = -1;
      level.pci_vendor = level.pci_device = -1;
      n = hwloc_calc_get_nbobjs_inside_sets_by_depth(lcontext, obj->cpuset, obj->nodeset, &level);
      for(i=0; i<n; i++) {
	hwloc_obj_t child = hwloc_calc_get_obj_inside_sets_by_depth(lcontext, obj->cpuset, obj->nodeset, &level, i);
	if (show_index_prefix)
	  snprintf(prefix, sizeof(prefix), "%u.%u: ", current_obj, i);
        hwloc_info_show_descendant(topology, child, obj, objs, i, prefix, verbose);
        if (show_first_only)
          break;
      }
    } else {
      /* custom level */
      unsigned i = 0;
      hwloc_obj_t child = NULL;
      while ((child = hwloc_get_next_obj_by_depth(topology, show_descendants_depth, child)) != NULL) {
	hwloc_obj_t parent = child->parent;
	if (obj->cpuset) {
	  while (parent && !parent->cpuset)
	    parent = parent->parent;
	  if (!parent)
	    continue;
	  if (!hwloc_bitmap_isincluded(parent->cpuset, obj->cpuset)
	      || !hwloc_bitmap_isincluded(parent->nodeset, obj->nodeset))
	    continue;
	} else {
	  while (parent && parent != obj)
	    parent = parent->parent;
	  if (!parent)
	    continue;
	}
	if (show_index_prefix)
	  snprintf(prefix, sizeof(prefix), "%u.%u: ", current_obj, i);
        hwloc_info_show_descendant(topology, child, obj, objs, i, prefix, verbose);
        if (show_first_only)
          break;
	i++;
      }
    }
  } else if (show_descendants_kind != KIND_NONE) {
    /* recurse in all children. */
    int number = 0;
    hwloc_info_recurse_descendants(topology, obj, objs, obj, &number, verbose);
  } else if (show_local_memory) {
    unsigned nrnodes;
    hwloc_obj_t *nodes;
    hwloc_nodeset_t nodeset = hwloc_bitmap_alloc_full(); /* show all nodes by default */
    nrnodes = hwloc_bitmap_weight(hwloc_topology_get_topology_nodeset(topology));
    nodes = malloc(nrnodes * sizeof(*nodes));
    if (nodeset && nodes) {
      struct hwloc_location loc;
      int err;
      loc.type = HWLOC_LOCATION_TYPE_OBJECT;
      loc.location.object = obj;
      err = hwloc_get_local_numanode_objs(topology, &loc, &nrnodes, nodes, show_local_memory_flags);
      if (!err) {
        unsigned i, j;
        if (best_memattr_id != (hwloc_memattr_id_t) -1) {
          /* only keep the best ones for that memattr */

          /* won't work if obj is CPU-less: perf from I/O is likely different from perf from CPU objects */
          loc.type = HWLOC_LOCATION_TYPE_CPUSET;
          loc.location.cpuset = obj->cpuset;
          err = hwloc_utils_get_best_node_in_array_by_memattr(topology, best_memattr_id,
                                                              nrnodes, nodes, &loc, best_node_flags, nodeset, verbose);
          if (err < -1) {
            if (verbose > 0)
              fprintf(stderr, "Failed to find a best local node for memory attribute.\n");
            /* on error, nodeset is zeroed, and we report nothing below (except if default flag is set) */
          }
        }
        for(i=0, j=0; i<nrnodes; i++) {
          if (!hwloc_bitmap_isset(nodeset, nodes[i]->os_index))
            continue;
          if (show_index_prefix)
	    snprintf(prefix, sizeof(prefix), "%u.%u: ", current_obj, j);
          hwloc_info_show_local_memory(topology, nodes[i], obj, objs, j, prefix, verbose);
          if (show_first_only)
            break;
          j++;
        }
      }
    } else {
      fprintf(stderr, "Failed to allocate array of local NUMA nodes\n");
    }
    hwloc_bitmap_free(nodeset);
    free(nodes);
  } else if (show_default_memory) {
    int err;
    unsigned i, j;
    unsigned nrnodes;
    hwloc_obj_t *nodes;
    hwloc_nodeset_t default_nodeset = hwloc_bitmap_alloc();
    nrnodes = hwloc_bitmap_weight(hwloc_topology_get_topology_nodeset(topology));
    nodes = malloc(nrnodes * sizeof(*nodes));
    if (default_nodeset && nodes) {
      err = hwloc_topology_get_default_nodeset(topology, default_nodeset, 0);
      if (!err) {
        struct hwloc_location loc;
        loc.type = HWLOC_LOCATION_TYPE_OBJECT;
        loc.location.object = obj;
        err = hwloc_get_local_numanode_objs(topology, &loc, &nrnodes, nodes, show_local_memory_flags);
        if (!err) {
          for(i=0, j=0; i<nrnodes; i++) {
            if (!hwloc_bitmap_isset(default_nodeset, nodes[i]->os_index))
              continue;
            if (show_index_prefix)
              snprintf(prefix, sizeof(prefix), "%u.%u: ", current_obj, j);
            hwloc_info_show_default_memory(topology, nodes[i], obj, objs, j, prefix, verbose);
            if (show_first_only)
              break;
            j++;
          }
        }
      }
    } else {
      fprintf(stderr, "Failed to allocate array of local NUMA nodes for default\n");
    }
    hwloc_bitmap_free(default_nodeset);
    free(nodes);
  } else {
    hwloc_info_show_single_obj(topology, obj, objs, prefix, verbose);
  }

  current_obj++;
}

static void
hwloc_info_show_levels(FILE *output, hwloc_topology_t topology)
{
  hwloc_lstopo_show_summary(output, topology);
}

static void
hwloc_info_show_topology_infos(hwloc_topology_t topology)
{
  hwloc_obj_t root = hwloc_get_root_obj(topology);
  unsigned i;
  for(i=0; i<root->infos_count; i++) {
    const char *infoname = root->infos[i].name;
    if (!strcmp(infoname, "Backend")
        || !strcmp(infoname, "SyntheticDescription")
        || !strcmp(infoname, "MemoryTiersNr")
        || !strcmp(infoname, "LinuxCgroup")
        || !strcmp(infoname, "WindowsBuildEnvironment")
        || !strcmp(infoname, "OSName")
        || !strcmp(infoname, "OSRelease")
        || !strcmp(infoname, "OSVersion")
        || !strcmp(infoname, "HostName")
        || !strcmp(infoname, "Architecture")
        || !strcmp(infoname, "hwlocVersion")
        || !strcmp(infoname, "ProcessName")) {
      if (!only_attr_name)
        printf("info %s = %s\n", infoname, root->infos[i].value);
      else {
        char name[256];
        snprintf(name, sizeof(name), "info %s", infoname);
        if (!strcmp(only_attr_name, name))
          printf("%s\n", root->infos[i].value);
      }
    }
  }
}

static void
hwloc_info_show_support(hwloc_topology_t topology)
{
  const struct hwloc_topology_support *support = hwloc_topology_get_support(topology);

#ifdef HWLOC_DEBUG
  HWLOC_BUILD_ASSERT(sizeof(struct hwloc_topology_support) == 4*sizeof(void*));
  HWLOC_BUILD_ASSERT(sizeof(struct hwloc_topology_discovery_support) == 6);
  HWLOC_BUILD_ASSERT(sizeof(struct hwloc_topology_cpubind_support) == 11);
  HWLOC_BUILD_ASSERT(sizeof(struct hwloc_topology_membind_support) == 16);
  HWLOC_BUILD_ASSERT(sizeof(struct hwloc_topology_misc_support) == 1);
#endif

#define DO(x,y) printf(#x ":" #y " = %u\n", (unsigned char) support->x->y);
  DO(discovery, pu);
  DO(discovery, disallowed_pu);
  DO(discovery, numa);
  DO(discovery, numa_memory);
  DO(discovery, disallowed_numa);
  DO(discovery, cpukind_efficiency);

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
  DO(cpubind, get_thisthread_last_cpu_location);

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
  DO(membind, weighted_interleave_membind);
  DO(membind, nexttouch_membind);
  DO(membind, migrate_membind);
  DO(membind, get_area_memlocation);

  DO(misc, imported_support);
#undef DO
}

int
main (int argc, char *argv[])
{
  int err;
  hwloc_topology_t topology;
  int topodepth;
  unsigned long flags = HWLOC_TOPOLOGY_FLAG_IMPORT_SUPPORT;
  unsigned long restrict_flags = 0;
  char * callname;
  char * input = NULL;
  struct hwloc_utils_input_format_s input_format = HWLOC_UTILS_INPUT_FORMAT_DEFAULT;
  const char *show_ancestor_type = NULL;
  const char *show_descendants_type = NULL;
  char *best_memattr_str = NULL;
  char *restrictstring = NULL;
  size_t typelen;
  int opt;
  enum hwloc_info_mode { HWLOC_INFO_MODE_UNKNOWN, HWLOC_INFO_MODE_TOPOLOGY, HWLOC_INFO_MODE_OBJECTS, HWLOC_INFO_MODE_SUPPORT } mode = HWLOC_INFO_MODE_UNKNOWN;

  callname = strrchr(argv[0], '/');
  if (!callname)
    callname = argv[0];
  else
    callname++;
  /* skip argv[0], handle options */
  argc--;
  argv++;

  hwloc_utils_check_api_version(callname);

  /* enable verbose backends */
  if (!getenv("HWLOC_XML_VERBOSE"))
    putenv((char *) "HWLOC_XML_VERBOSE=1");
  if (!getenv("HWLOC_SYNTHETIC_VERBOSE"))
    putenv((char *) "HWLOC_SYNTHETIC_VERBOSE=1");

  err = hwloc_topology_init (&topology);
  if (err)
    return EXIT_FAILURE;

  hwloc_topology_set_all_types_filter(topology, HWLOC_TYPE_FILTER_KEEP_ALL);
  hwloc_topology_set_io_types_filter(topology, HWLOC_TYPE_FILTER_KEEP_IMPORTANT);

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
      else if (!strcmp (argv[0], "-q") || !strcmp (argv[0], "--quiet")
               || !strcmp (argv[0], "-s") || !strcmp (argv[0], "--silent"))
        verbose_mode--;
      else if (!strcmp (argv[0], "-h") || !strcmp (argv[0], "--help")) {
	usage(callname, stdout);
        exit(EXIT_SUCCESS);
      }
      else if (!strcmp (argv[0], "--get-attr")) {
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
	only_attr_name = argv[1];
	opt = 1;
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
	show_ancestor_type = argv[1];
	opt = 1;
      }
      else if (!strcmp (argv[0], "--children"))
	show_children = 1;
      else if (!strcmp (argv[0], "--descendants")) {
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
	show_descendants_type = argv[1];
	opt = 1;
      }
      else if (!strcmp (argv[0], "--local-memory"))
        show_local_memory = 1;
      else if (!strcmp (argv[0], "--local-memory-flags")) {
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
        show_local_memory = 1;
	show_local_memory_flags = hwloc_utils_parse_local_numanode_flags(argv[1]);
	opt = 1;
      }
      else if (!strcmp (argv[0], "--best-memattr")) {
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
        show_local_memory = 1;
        best_memattr_str = argv[1];
        opt = 1;
      }
      else if (!strcmp (argv[0], "--default-nodes")) {
        show_default_memory = 1;
      }
      else if (!strcmp (argv[0], "--first")) {
        show_first_only = 1;
      }
      else if (!strcmp (argv[0], "--filter")) {
        hwloc_obj_type_t type;
        char *colon;
        enum hwloc_type_filter_e filter = HWLOC_TYPE_FILTER_KEEP_ALL;
        int all = 0;
	int allio = 0;
	int allcaches = 0;
	int allicaches = 0;
        if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
        colon = strchr(argv[1], ':');
        if (colon) {
          *colon = '\0';
          if (!strcmp(colon+1, "none"))
            filter = HWLOC_TYPE_FILTER_KEEP_NONE;
          else if (!strcmp(colon+1, "all"))
            filter = HWLOC_TYPE_FILTER_KEEP_ALL;
          else if (!strcmp(colon+1, "structure"))
            filter = HWLOC_TYPE_FILTER_KEEP_STRUCTURE;
	  else if (!strcmp(colon+1, "important"))
	    filter = HWLOC_TYPE_FILTER_KEEP_IMPORTANT;
	  else {
	    fprintf(stderr, "Unsupported filtering kind `%s' passed to --filter.\n", colon+1);
	    usage (callname, stderr);
	    exit(EXIT_FAILURE);
	  }
        }
        if (!strcmp(argv[1], "all"))
          all = 1;
	else if (!strcmp(argv[1], "io"))
	  allio = 1;
	else if (!strcmp(argv[1], "cache"))
	  allcaches = 1;
	else if (!strcmp(argv[1], "icache"))
	  allicaches = 1;
        else if (hwloc_type_sscanf(argv[1], &type, NULL, 0) < 0) {
          fprintf(stderr, "Unsupported type `%s' passed to --filter.\n", argv[1]);
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
        }
        if (all)
          hwloc_topology_set_all_types_filter(topology, filter);
	else if (allio)
          hwloc_topology_set_io_types_filter(topology, filter);
	else if (allcaches) {
	  hwloc_topology_set_cache_types_filter(topology, filter);
	  hwloc_topology_set_type_filter(topology, HWLOC_OBJ_MEMCACHE, filter);
	} else if (allicaches)
	  hwloc_topology_set_icache_types_filter(topology, filter);
        else
          hwloc_topology_set_type_filter(topology, type, filter);
        opt = 1;
      }
      else if (!strcmp (argv[0], "--no-icaches")) {
	hwloc_topology_set_icache_types_filter(topology, HWLOC_TYPE_FILTER_KEEP_NONE);
      } else if (!strcmp (argv[0], "--disallowed") || !strcmp (argv[0], "--whole-system"))
	flags |= HWLOC_TOPOLOGY_FLAG_INCLUDE_DISALLOWED;
      else if (!strcmp (argv[0], "--no-io")) {
	hwloc_topology_set_io_types_filter(topology, HWLOC_TYPE_FILTER_KEEP_NONE);
      } else if (!strcmp (argv[0], "--no-bridges")) {
	hwloc_topology_set_type_filter(topology, HWLOC_OBJ_BRIDGE, HWLOC_TYPE_FILTER_KEEP_NONE);
      } else if (!strcmp (argv[0], "--whole-io")) {
	hwloc_topology_set_io_types_filter(topology, HWLOC_TYPE_FILTER_KEEP_ALL);
      } else if (!strcmp (argv[0], "--thissystem"))
	flags |= HWLOC_TOPOLOGY_FLAG_IS_THISSYSTEM;
      else if (!strcmp (argv[0], "--restrict")) {
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
        if(strncmp(argv[1], "nodeset=", 8)) {
          restrictstring = strdup(argv[1]);
        } else {
          restrictstring = strdup(argv[1]+8);
          restrict_flags |= HWLOC_RESTRICT_FLAG_BYNODESET;
        }
	opt = 1;
      }
      else if (!strcmp (argv[0], "--restrict-flags")) {
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
        }
	restrict_flags = hwloc_utils_parse_restrict_flags(argv[1]);
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
    err = hwloc_utils_enable_input_format(topology, flags, input, &input_format, verbose_mode, callname);
    if (err)
      return err;
  }

  if (pid_number > 0) {
    if (hwloc_pid_from_number(&pid, pid_number, 0, 1 /* verbose */) < 0
	|| hwloc_topology_set_pid(topology, pid)) {
      perror("Setting target pid");
      if (input) hwloc_utils_disable_input_format(&input_format);
      return EXIT_FAILURE;
    }
  }

  err = hwloc_topology_load (topology);
  if (err) {
    perror("hwloc_topology_load");
    if (input) hwloc_utils_disable_input_format(&input_format);
    return EXIT_FAILURE;
  }

  if (input) {
    hwloc_utils_disable_input_format(&input_format);
  }

  topodepth = hwloc_topology_get_depth(topology);

  if (show_ancestor_type) {
    if (!strncmp(show_ancestor_type, "kind=", 5))
      show_ancestor_kind = parse_kind(show_ancestor_type+5);
    else
      show_ancestor_kind = parse_kind(show_ancestor_type);
    if (show_ancestor_kind == KIND_NONE) {
      err = hwloc_type_sscanf_as_depth(show_ancestor_type, NULL, topology, &show_ancestor_depth);
      if (err < 0) {
        fprintf(stderr, "unrecognized --ancestor type %s\n", show_ancestor_type);
        usage(callname, stderr);
        return EXIT_FAILURE;
      }
      if (show_ancestor_depth == HWLOC_TYPE_DEPTH_UNKNOWN) {
        fprintf(stderr, "unavailable --ancestor type %s\n", show_ancestor_type);
        return EXIT_FAILURE;
      }
      if (show_ancestor_depth == HWLOC_TYPE_DEPTH_MULTIPLE) {
        fprintf(stderr, "multiple --ancestor type %s\n", show_ancestor_type);
        return EXIT_FAILURE;
      }
    }
  }
  if (show_descendants_type) {
    if (!strncmp(show_descendants_type, "kind=", 5))
      show_descendants_kind = parse_kind(show_descendants_type+5);
    else
      show_descendants_kind = parse_kind(show_descendants_type);
    if (show_descendants_kind == KIND_NONE) {
    err = hwloc_type_sscanf_as_depth(show_descendants_type, NULL, topology, &show_descendants_depth);
      if (err < 0) {
        fprintf(stderr, "unrecognized --descendants type %s\n", show_descendants_type);
        usage(callname, stderr);
        return EXIT_FAILURE;
      }
      if (show_descendants_depth == HWLOC_TYPE_DEPTH_UNKNOWN) {
        fprintf(stderr, "unavailable --descendants type %s\n", show_descendants_type);
        return EXIT_FAILURE;
      }
      if (show_descendants_depth == HWLOC_TYPE_DEPTH_MULTIPLE) {
        fprintf(stderr, "multiple --descendants type %s\n", show_descendants_type);
        return EXIT_FAILURE;
      }
    }
  }

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
    err = hwloc_topology_restrict (topology, restrictset, restrict_flags);
    if (err) {
      perror("Restricting the topology");
      /* FALLTHRU */
    }
    hwloc_bitmap_free(restrictset);
    free(restrictstring);
  }

  if (best_memattr_str) {
    best_node_flags = hwloc_utils_parse_best_node_flags(best_memattr_str);

    best_memattr_id = hwloc_utils_parse_memattr_name(topology, best_memattr_str);
    if (best_memattr_id == (hwloc_memattr_id_t) -1) {
      fprintf(stderr, "unrecognized memattr %s\n", best_memattr_str);
      return EXIT_FAILURE;
    }
  }

  if (mode == HWLOC_INFO_MODE_UNKNOWN) {
    if (argc)
      mode = HWLOC_INFO_MODE_OBJECTS;
    else
      mode = HWLOC_INFO_MODE_TOPOLOGY;
  }

  if (mode == HWLOC_INFO_MODE_TOPOLOGY) {
    hwloc_info_show_levels(stdout, topology);

  } else if (mode == HWLOC_INFO_MODE_SUPPORT) {
    hwloc_info_show_support(topology);

  } else if (mode == HWLOC_INFO_MODE_OBJECTS) {
    struct hwloc_calc_location_context_s lcontext;
    lcontext.topology = topology;
    lcontext.topodepth = topodepth;
    lcontext.only_hbm = -1;
    lcontext.logical = logical;
    lcontext.verbose = verbose_mode;
    current_obj = 0;
    while (argc >= 1) {
      if (!strcmp(argv[0], "levels")) {
        hwloc_info_show_levels(stdout, topology);
      } else if (!strcmp(argv[0], "topology")) {
        hwloc_info_show_topology_infos(topology);
      } else if (!strcmp(argv[0], "support")) {
        hwloc_info_show_support(topology);
      } else if (!strcmp(argv[0], "all") || !strcmp(argv[0], "root")) {
	hwloc_calc_process_location_info_cb(&lcontext, NULL, hwloc_get_root_obj(topology));
      } else if (*argv[0] == '-') {
        fprintf(stderr, "Cannot handle command-line option %s after some locations.\n", argv[0]);
        return EXIT_FAILURE;
      } else {
	/* try to match a type/depth followed by a special character */
	typelen = hwloc_calc_parse_level_size(argv[0]);
	if (typelen && (argv[0][typelen] == ':' || argv[0][typelen] == '=')) {
	  err = hwloc_calc_process_location(&lcontext, argv[0], typelen,
					    hwloc_calc_process_location_info_cb, NULL);
	}
      }
      argc--; argv++;
    }

  } else assert(0);

  hwloc_topology_destroy (topology);

  return EXIT_SUCCESS;
}
