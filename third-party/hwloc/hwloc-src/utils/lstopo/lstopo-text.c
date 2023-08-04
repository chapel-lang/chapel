/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2022 Inria.  All rights reserved.
 * Copyright © 2009-2012 Université Bordeaux
 * Copyright © 2009-2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "private/autogen/config.h"
#include "hwloc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef HWLOC_WIN_SYS
#include <hwloc/windows.h>
#endif

#include "lstopo.h"
#include "misc.h"

#define indent(output, i) \
  fprintf (output, "%*s", (int) i, "");

/*
 * Console fashion text output
 */

static void
output_console_obj (struct lstopo_output *loutput, hwloc_obj_t l, int collapse)
{
  FILE *output = loutput->file;
  enum lstopo_index_type_e index_type = loutput->index_type;
  int verbose_mode = loutput->verbose_mode;
  char pidxstr[16];
  char lidxstr[32];
  char busidstr[32];

  if (collapse > 1 && l->type == HWLOC_OBJ_PCI_DEVICE) {
    strcpy(pidxstr, "P#[collapsed]"); /* shouldn't be used, os_index should be -1 except if importing old XMLs */
    snprintf(lidxstr, sizeof(lidxstr), "%s%u-%u", loutput->logical_index_prefix, l->logical_index, l->logical_index+collapse-1);
  } else {
    snprintf(pidxstr, sizeof(pidxstr), "%s%u", loutput->os_index_prefix, l->os_index);
    snprintf(lidxstr, sizeof(lidxstr), "%s%u", loutput->logical_index_prefix, l->logical_index);
  }
  if (l->type == HWLOC_OBJ_PCI_DEVICE)
    lstopo_busid_snprintf(loutput, busidstr, sizeof(busidstr), l, collapse, loutput->need_pci_domain);

  if (loutput->show_cpuset < 2) {
    char type[64], *attr, phys[32] = "";
    int len;
    hwloc_obj_type_snprintf (type, sizeof(type), l, verbose_mode-1);
    if (l->subtype)
      fprintf(output, "%s(%s)", type, l->subtype);
    else
      fprintf(output, "%s", type);
    if (l->depth != 0 && (verbose_mode >= 2 || (hwloc_obj_type_is_normal(l->type) || hwloc_obj_type_is_memory(l->type)))) {
      if (index_type != LSTOPO_INDEX_TYPE_PHYSICAL)
	/* print logical index in logical and default case */
	fprintf(output, "%s", lidxstr);
      else if (index_type == LSTOPO_INDEX_TYPE_PHYSICAL && l->os_index != HWLOC_UNKNOWN_INDEX)
	/* print physical index in physical case */
	fprintf(output, "%s", pidxstr);
    }
    if (l->name && (l->type == HWLOC_OBJ_MISC || l->type == HWLOC_OBJ_GROUP))
      fprintf(output, " %s", l->name);
    if (index_type == LSTOPO_INDEX_TYPE_DEFAULT
	&& l->os_index != HWLOC_UNKNOWN_INDEX
	&& (verbose_mode >= 2 || l->type == HWLOC_OBJ_PU || l->type == HWLOC_OBJ_NUMANODE))
      /* print physical index too if default index */
      snprintf(phys, sizeof(phys),
               "%s",
               pidxstr[0] == ' ' ? pidxstr+1 : pidxstr /* skip the starting space if any */
        );
    if (l->type == HWLOC_OBJ_PCI_DEVICE && verbose_mode <= 1)
      fprintf(output, " %s (%s)",
	      busidstr, hwloc_pci_class_string(l->attr->pcidev.class_id));
    /* display attributes */
    len = hwloc_obj_attr_snprintf (NULL, 0, l, " ", verbose_mode-1);
    attr = malloc(len+1);
    *attr = '\0';
    hwloc_obj_attr_snprintf (attr, len+1, l, " ", verbose_mode-1);
    if (*phys || *attr) {
      fprintf(output, " (");
      if (*phys)
	fprintf(output, "%s", phys);
      if (*phys && *attr)
	fprintf(output, " ");
      if (*attr) {
	if (collapse > 1 && l->type == HWLOC_OBJ_PCI_DEVICE) {
	  assert(!strncmp(attr, "busid=", 6));
	  assert(!strncmp(attr+18, " id=", 4));
	  fprintf(output, "busid=%s%s", busidstr, attr+18);
	} else
	  fprintf(output, "%s", attr);
      }
      fprintf(output, ")");
    }
    free(attr);
    /* display the root total_memory if not verbose (already shown)
     * (cannot be local_memory since root cannot be a NUMA node) */
    if (verbose_mode == 1 && !l->parent && l->total_memory)
      fprintf(output, " (%lu%s total)",
	      (unsigned long) hwloc_memory_size_printf_value(l->total_memory, 0),
	      hwloc_memory_size_printf_unit(l->total_memory, 0));
    /* append the name */
    if (l->name && (l->type == HWLOC_OBJ_OS_DEVICE || verbose_mode >= 2)
	&& l->type != HWLOC_OBJ_MISC && l->type != HWLOC_OBJ_GROUP)
      fprintf(output, " \"%s\"", l->name);
  }
  if (!l->cpuset)
    return;
  if (loutput->show_cpuset == 1)
    fprintf(output, " cpuset=");
  if (loutput->show_cpuset) {
    char *cpusetstr;
    if (loutput->show_taskset)
      hwloc_bitmap_taskset_asprintf(&cpusetstr, l->cpuset);
    else
      hwloc_bitmap_asprintf(&cpusetstr, l->cpuset);
    fprintf(output, "%s", cpusetstr);
    free(cpusetstr);
  }

  /* annotate if the PU/NUMA is disallowed/binding */
  if (verbose_mode >= 2) {
    if (l->type == HWLOC_OBJ_PU) {
      if (lstopo_pu_disallowed(loutput, l))
	fprintf(output, " (disallowed)");
      else if (lstopo_pu_binding(loutput, l))
	fprintf(output, " (binding)");
    } else if (l->type == HWLOC_OBJ_NUMANODE) {
      if (lstopo_numa_disallowed(loutput, l))
	fprintf(output, " (disallowed)");
      else if (lstopo_numa_binding(loutput, l))
	fprintf(output, " (binding)");
    }
  }
}

/* Recursively output topology in a console fashion */
static void
output_topology (struct lstopo_output *loutput, hwloc_obj_t l, hwloc_obj_t parent, int i)
{
  FILE *output = loutput->file;
  int verbose_mode = loutput->verbose_mode;
  hwloc_obj_t child;
  int group_identical = (verbose_mode <= 1) && !loutput->show_cpuset;
  int collapse = loutput->pci_collapse_enabled ? ((struct lstopo_obj_userdata *) l->userdata)->pci_collapsed : 0;

  if (l->type == HWLOC_OBJ_PCI_DEVICE && collapse == -1)
    return;

  if (group_identical
      && parent && parent->arity == 1
      && !parent->memory_arity && !parent->io_arity && !parent->misc_arity
      && l->cpuset && parent->cpuset && hwloc_bitmap_isequal(l->cpuset, parent->cpuset)) {
    /* in non-verbose mode, merge objects with their parent is they are exactly identical */
    fprintf(output, " + ");
  } else {
    if (parent)
      fprintf(output, "\n");
    indent (output, 2*i);
    i++;
  }

  if (collapse > 1)
    fprintf(output, "%d x { ", collapse);
  output_console_obj(loutput, l, collapse);
  if (collapse > 1)
    fprintf(output, " }");

  for_each_memory_child(child, l)
    if (child->type != HWLOC_OBJ_PU || !loutput->ignore_numanodes)
      output_topology (loutput, child, l, i);
  for_each_child(child, l)
    if (child->type != HWLOC_OBJ_PU || !loutput->ignore_pus)
      output_topology (loutput, child, l, i);
  for_each_io_child(child, l)
    output_topology (loutput, child, l, i);
  for_each_misc_child(child, l)
    output_topology (loutput, child, l, i);
}

/* Recursive so that multiple depth types are properly shown */
static void
output_only (struct lstopo_output *loutput, hwloc_obj_t l)
{
  FILE *output = loutput->file;
  hwloc_obj_t child;
  if (loutput->show_only == l->type) {
    output_console_obj (loutput, l, 0);
    fprintf (output, "\n");
  }
  /* there can be anything below normal children */
  for_each_child(child, l)
    output_only (loutput, child);
  /* there can be only memory or Misc below memory children */
  if (hwloc_obj_type_is_memory(loutput->show_only) || loutput->show_only == HWLOC_OBJ_MISC) {
    for(child = l->memory_first_child; child; child = child->next_sibling)
      output_only (loutput, child);
  }
  /* there can be only I/O or Misc below I/O children */
  if (hwloc_obj_type_is_io(loutput->show_only) || loutput->show_only == HWLOC_OBJ_MISC) {
    for_each_io_child(child, l)
      output_only (loutput, child);
  }
  /* there can be only Misc below Misc children */
  if (loutput->show_only == HWLOC_OBJ_MISC) {
    /* Misc can only contain other Misc, no need to recurse otherwise */
    for_each_misc_child(child, l)
      output_only (loutput, child);
  }
}

static void output_distances(struct lstopo_output *loutput)
{
  hwloc_topology_t topology = loutput->topology;
  enum lstopo_index_type_e index_type = loutput->index_type;
  FILE *output = loutput->file;
  struct hwloc_distances_s **dist;
  unsigned nr = 0, j;
  int err = hwloc_distances_get(topology, &nr, NULL, 0, 0);
  if (err < 0 || !nr)
    return;
  dist = malloc(nr * sizeof(*dist));
  if (!dist)
    return;
  err = hwloc_distances_get(topology, &nr, dist, 0, 0);
  if (!err) {
    for(j=0; j<nr; j++) {
      const char *kindmeans = (dist[j]->kind & HWLOC_DISTANCES_KIND_MEANS_LATENCY) ? "latency" : (dist[j]->kind & HWLOC_DISTANCES_KIND_MEANS_BANDWIDTH) ? "bandwidth" : "distance";
      const char *name = hwloc_distances_get_name(topology, dist[j]);
      if (!name)
        name = "(null)";
      if (loutput->transform_distances != -1)
        hwloc_distances_transform(topology, dist[j], loutput->transform_distances, NULL, 0);
      if (dist[j]->kind & HWLOC_DISTANCES_KIND_HETEROGENEOUS_TYPES) {
	fprintf(output, "Relative %s matrix (name %s kind %lu) between %u heterogeneous objects by %s indexes:\n",
		kindmeans, name, dist[j]->kind,
		dist[j]->nbobjs,
		index_type != LSTOPO_INDEX_TYPE_PHYSICAL ? "logical" : "physical");
      } else {
	fprintf(output, "Relative %s matrix (name %s kind %lu) between %u %ss (depth %d) by %s indexes:\n",
		kindmeans, name, dist[j]->kind,
		dist[j]->nbobjs,
		hwloc_obj_type_string(dist[j]->objs[0]->type),
		dist[j]->objs[0]->depth,
		index_type != LSTOPO_INDEX_TYPE_PHYSICAL ? "logical" : "physical");
      }
      hwloc_utils_print_distance_matrix(output, dist[j]->nbobjs, dist[j]->objs, dist[j]->values, index_type != LSTOPO_INDEX_TYPE_PHYSICAL, dist[j]->kind & HWLOC_DISTANCES_KIND_HETEROGENEOUS_TYPES);
      hwloc_distances_release(topology, dist[j]);
    }
  }
  free(dist);
}

static void output_memattr_obj(struct lstopo_output *loutput,
                               hwloc_obj_t obj)
{
  enum lstopo_index_type_e index_type = loutput->index_type;
  unsigned idx = (index_type == LSTOPO_INDEX_TYPE_PHYSICAL ? obj->os_index : obj->logical_index);
  char objtype[16];

  hwloc_obj_type_snprintf(objtype, sizeof(objtype), obj, 0);
  if (idx == (unsigned) -1)
    printf("%s %c#-1", objtype,
           index_type == LSTOPO_INDEX_TYPE_PHYSICAL ? 'P' : 'L');
  else
    printf("%s %c#%u", objtype,
           index_type == LSTOPO_INDEX_TYPE_PHYSICAL ? 'P' : 'L',
           idx);

  if (obj->name)
    printf(" \"%s\"", obj->name);
}

static void output_memattr_initiator(struct lstopo_output *loutput,
                                     struct hwloc_location *initiator)
{
  hwloc_topology_t topology = loutput->topology;

  if (initiator->type == HWLOC_LOCATION_TYPE_CPUSET) {
    hwloc_obj_t obj;
    char *c;

    assert(initiator->location.cpuset);
    hwloc_bitmap_asprintf(&c, initiator->location.cpuset);
    printf(" from cpuset %s", c);
    free(c);

    obj = hwloc_get_obj_covering_cpuset(topology, initiator->location.cpuset);
    if (obj && !hwloc_bitmap_isequal(obj->cpuset, initiator->location.cpuset))
      obj = NULL;
    if (obj) {
      while (obj->parent && hwloc_bitmap_isequal(obj->cpuset, obj->parent->cpuset))
        obj = obj->parent;
      printf(" (");
      output_memattr_obj(loutput, obj);
      printf(")");
    }

  } else if (initiator->type == HWLOC_LOCATION_TYPE_OBJECT) {
    printf(" from ");
    output_memattr_obj(loutput, initiator->location.object);

  } else {
    printf(" from initiator with unexpected type %d",
	   (int) initiator->type);
  }

}

static int output_memattr(struct lstopo_output *loutput, unsigned id)
{
  hwloc_topology_t topology = loutput->topology;
  int verbose_mode = loutput->verbose_mode;
  int show_all = (loutput->show_memattrs_only || (verbose_mode >= 3));
  const char *name;
  unsigned long flags;
  unsigned nr_targets;
  hwloc_obj_t *targets;
  unsigned i;
  int err;

  if (!show_all
      && (id == HWLOC_MEMATTR_ID_CAPACITY || id == HWLOC_MEMATTR_ID_LOCALITY))
    return 0;

  err = hwloc_memattr_get_name(topology, id, &name);
  if (err < 0)
    return -1;
  err = hwloc_memattr_get_flags(topology, id, &flags);
  assert(!err);

  nr_targets = 0;
  err = hwloc_memattr_get_targets(topology, id, NULL, 0, &nr_targets, NULL, NULL);
  assert(!err);

  if (!show_all && !nr_targets)
    return 0;

  printf("Memory attribute #%u name `%s' flags %lu\n", id, name, flags);

  targets = malloc(nr_targets * sizeof(*targets));
  if (!targets)
    return 0;

  err = hwloc_memattr_get_targets(topology, id, NULL, 0, &nr_targets, targets, NULL);
  assert(!err);

  for(i=0; i<nr_targets; i++) {

    if (!(flags & HWLOC_MEMATTR_FLAG_NEED_INITIATOR)) {
      hwloc_uint64_t value;
      err = hwloc_memattr_get_value(topology, id, targets[i], NULL, 0, &value);
      if (!err) {
        printf("  ");
        output_memattr_obj(loutput, targets[i]);
        printf(" = %llu\n", (unsigned long long) value);
      }

    } else {
      unsigned nr_initiators = 0;
      err = hwloc_memattr_get_initiators(topology, id, targets[i], 0, &nr_initiators, NULL, NULL);
      if (!err) {
        struct hwloc_location *initiators = malloc(nr_initiators * sizeof(*initiators));
        hwloc_uint64_t *values = malloc(nr_initiators * sizeof(*values));
        if (initiators && values) {
          err = hwloc_memattr_get_initiators(topology, id, targets[i], 0, &nr_initiators, initiators, values);
          if (!err) {
            unsigned j;
            for(j=0; j<nr_initiators; j++) {
              printf("  ");
              output_memattr_obj(loutput, targets[i]);
              printf(" = %llu",
                     (unsigned long long) values[j]);
              output_memattr_initiator(loutput, &initiators[j]);
              printf("\n");
            }
          }
        }
        free(initiators);
        free(values);
      }
    }
  }
  free(targets);

  return 0;
}

static void output_memattrs(struct lstopo_output *loutput)
{
  unsigned id;
  /* output in a convenient instead of the native ID order */
  output_memattr(loutput, HWLOC_MEMATTR_ID_CAPACITY);
  output_memattr(loutput, HWLOC_MEMATTR_ID_LOCALITY);
  output_memattr(loutput, HWLOC_MEMATTR_ID_BANDWIDTH);
  output_memattr(loutput, HWLOC_MEMATTR_ID_READ_BANDWIDTH);
  output_memattr(loutput, HWLOC_MEMATTR_ID_WRITE_BANDWIDTH);
  output_memattr(loutput, HWLOC_MEMATTR_ID_LATENCY);
  output_memattr(loutput, HWLOC_MEMATTR_ID_READ_LATENCY);
  output_memattr(loutput, HWLOC_MEMATTR_ID_WRITE_LATENCY);
  /* output others */
  for(id=HWLOC_MEMATTR_ID_WRITE_LATENCY+1; ; id++)
    if (output_memattr(loutput, id) < 0)
      break;
}


static void output_windows_processor_groups(struct lstopo_output *loutput __hwloc_attribute_unused,
                                            int force __hwloc_attribute_unused)
{
#ifdef HWLOC_WIN_SYS
  hwloc_topology_t topology = loutput->topology;
  int err = hwloc_windows_get_nr_processor_groups(topology, 0);
  if (err > 0) {
    unsigned nr = (unsigned) err;
    if (nr > 1 || force) {
      hwloc_bitmap_t set = hwloc_bitmap_alloc();
      if (set) {
        unsigned i;
        for(i=0; i<nr; i++) {
          err = hwloc_windows_get_processor_group_cpuset(topology, i, set, 0);
          if (!err) {
            char *s;
            hwloc_bitmap_asprintf(&s, set);
            printf("Processor Group #%u = %s\n", i, s);
            free(s);
          }
        }
        hwloc_bitmap_free(set);
      }
    }
  }
#endif
}

static void output_cpukinds(struct lstopo_output *loutput)
{
  hwloc_topology_t topology = loutput->topology;
  unsigned i, j, nr;
  hwloc_bitmap_t cpuset = hwloc_bitmap_alloc();

  nr = hwloc_cpukinds_get_nr(topology, 0);

  for(i=0; i<nr; i++) {
    int efficiency;
    struct hwloc_info_s *infos;
    unsigned nr_infos;
    int err;

    err = hwloc_cpukinds_get_info(topology, i, cpuset, &efficiency, &nr_infos, &infos, 0);
    if (!err) {
      char *cpusets;
      hwloc_bitmap_asprintf(&cpusets, cpuset);
      printf("CPU kind #%u efficiency %d cpuset %s\n", i, efficiency, cpusets);
      free(cpusets);
      for(j=0; j<nr_infos; j++)
        printf("  %s = %s\n", infos[j].name, infos[j].value);
    }
  }

  hwloc_bitmap_free(cpuset);
}

int
output_console(struct lstopo_output *loutput, const char *filename)
{
  hwloc_topology_t topology = loutput->topology;
  int verbose_mode = loutput->verbose_mode;
  FILE *output;

  output = open_output(filename, loutput->overwrite);
  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    return -1;
  }
  loutput->file = output;

  if (loutput->show_distances_only) {
    output_distances(loutput);
    return 0;
  }
  if (loutput->show_memattrs_only) {
    output_memattrs(loutput);
    return 0;
  }
  if (loutput->show_cpukinds_only) {
    output_cpukinds(loutput);
    return 0;
  }
  if (loutput->show_windows_processor_groups_only) {
    output_windows_processor_groups(loutput, 1);
    return 0;
  }

  /*
   * if verbose_mode == 0, only print the summary.
   * if verbose_mode == 1, only print the topology tree.
   * if verbose_mode > 1, print both.
   */

  if (loutput->show_only != HWLOC_OBJ_TYPE_NONE) {
    if (verbose_mode > 1)
      fprintf(output, "Only showing %s objects\n", hwloc_obj_type_string(loutput->show_only));
    output_only (loutput, hwloc_get_root_obj(topology));
  } else if (verbose_mode >= 1) {
    output_topology (loutput, hwloc_get_root_obj(topology), NULL, 0);
    fprintf(output, "\n");
  }

  if ((verbose_mode > 1 || !verbose_mode) && loutput->show_only == HWLOC_OBJ_TYPE_NONE) {
    hwloc_lstopo_show_summary(output, topology);
  }

  if (verbose_mode > 1 && loutput->show_only == HWLOC_OBJ_TYPE_NONE) {
    output_distances(loutput);
    output_memattrs(loutput);
    output_cpukinds(loutput);
    output_windows_processor_groups(loutput, verbose_mode > 2);
  }

  if (verbose_mode > 1 && loutput->show_only == HWLOC_OBJ_TYPE_NONE) {
    hwloc_const_bitmap_t complete = hwloc_topology_get_complete_cpuset(topology);
    hwloc_const_bitmap_t topo = hwloc_topology_get_topology_cpuset(topology);
    hwloc_const_bitmap_t allowed = hwloc_topology_get_allowed_cpuset(topology);

    if (!hwloc_bitmap_isequal(topo, complete)) {
      hwloc_bitmap_t unknown = hwloc_bitmap_alloc();
      char *unknownstr;
      hwloc_bitmap_copy(unknown, complete);
      hwloc_bitmap_andnot(unknown, unknown, topo);
      hwloc_bitmap_asprintf(&unknownstr, unknown);
      fprintf (output, "%d processors not represented in topology: %s\n", hwloc_bitmap_weight(unknown), unknownstr);
      free(unknownstr);
      hwloc_bitmap_free(unknown);
    }
    if (!hwloc_bitmap_isequal(topo, allowed)) {
      hwloc_bitmap_t disallowed = hwloc_bitmap_alloc();
      char *disallowedstr;
      hwloc_bitmap_copy(disallowed, topo);
      hwloc_bitmap_andnot(disallowed, disallowed, allowed);
      hwloc_bitmap_asprintf(&disallowedstr, disallowed);
      fprintf(output, "%d processors represented but not allowed: %s\n", hwloc_bitmap_weight(disallowed), disallowedstr);
      free(disallowedstr);
      hwloc_bitmap_free(disallowed);
    }
    if (!hwloc_topology_is_thissystem(topology))
      fprintf (output, "Topology not from this system\n");
  }

  if (output != stdout)
    fclose(output);

  return 0;
}

int
output_synthetic(struct lstopo_output *loutput, const char *filename)
{
  hwloc_topology_t topology = loutput->topology;
  FILE *output;
  int length;
  char sbuffer[1024];
  char * dbuffer = NULL;
  unsigned nb1, nb2, nb3;

  if (!hwloc_get_root_obj(topology)->symmetric_subtree) {
    fprintf(stderr, "Cannot output assymetric topology in synthetic format.\n");
    goto out;
  }

  nb1 = hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_MISC);
  if (nb1) {
    fprintf(stderr, "# Ignoring %u Misc objects.\n", nb1);
    fprintf(stderr, "# (pass --filter Misc:none to hide this message).\n");
  }
  nb1 = hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_BRIDGE);
  nb2 = hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_PCI_DEVICE);
  nb3 = hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_OS_DEVICE);
  if (nb1 || nb2 || nb3) {
    fprintf(stderr, "# Ignoring %u Bridge, %u PCI device and %u OS device objects\n", nb1, nb2, nb3);
    fprintf(stderr, "# (pass --no-io to hide this message).\n");
  }

  length = hwloc_topology_export_synthetic(topology, sbuffer, sizeof(sbuffer), loutput->export_synthetic_flags);
  if (length < 0) {
    fprintf(stderr, "Failed to export a synthetic description (%s)\n", strerror(errno));
    goto out;
  }

  if (length >= (int) sizeof(sbuffer)) {
    dbuffer = malloc(length+1 /* \0 */);
    if (!dbuffer)
      goto out;

    length = hwloc_topology_export_synthetic(topology, dbuffer, length+1, loutput->export_synthetic_flags);
    if (length < 0)
      goto out_with_dbuffer;
  }

  output = open_output(filename, loutput->overwrite);
  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    goto out_with_dbuffer;
  }

  fprintf(output, "%s\n", dbuffer ? dbuffer : sbuffer);

  if (output != stdout)
    fclose(output);

  free(dbuffer);
  return 0;

 out_with_dbuffer:
  free(dbuffer);
 out:
  return -1;
}
