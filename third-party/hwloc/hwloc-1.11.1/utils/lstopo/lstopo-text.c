/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2015 Inria.  All rights reserved.
 * Copyright © 2009-2012 Université Bordeaux
 * Copyright © 2009-2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <private/autogen/config.h>
#include <private/private.h>
#include <hwloc.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "lstopo.h"
#include "misc.h"

#define indent(output, i) \
  fprintf (output, "%*s", (int) i, "");

/*
 * Console fashion text output
 */

static void
output_console_obj (hwloc_topology_t topology, hwloc_obj_t l, FILE *output, int logical, int verbose_mode, int collapsed)
{
  unsigned idx = logical ? l->logical_index : l->os_index;
  const char *value;
  char pidxstr[16] = "P#[collapsed]";
  char lidxstr[16] = "L#[collapsed]";

  if (!collapsed || l->type != HWLOC_OBJ_PCI_DEVICE) {
    snprintf(pidxstr, sizeof(pidxstr), "P#%u", l->os_index);
    snprintf(lidxstr, sizeof(lidxstr), "L#%u", l->logical_index);
  }

  if (lstopo_show_cpuset < 2) {
    char type[64], *attr, phys[32] = "";
    int len;
    value = hwloc_obj_get_info_by_name(l, "Type");
    hwloc_obj_type_snprintf (type, sizeof(type), l, verbose_mode-1);
    if (value)
      fprintf(output, "%s(%s)", type, value);
    else
      fprintf(output, "%s", type);
    if (l->depth != 0 && idx != (unsigned)-1
        && l->type != HWLOC_OBJ_MISC
        && l->type != HWLOC_OBJ_PCI_DEVICE
        && (l->type != HWLOC_OBJ_BRIDGE || l->attr->bridge.upstream_type == HWLOC_OBJ_BRIDGE_HOST))
      fprintf(output, " %s", logical ? lidxstr : pidxstr);
    if (l->name && (l->type == HWLOC_OBJ_MISC || l->type == HWLOC_OBJ_GROUP))
      fprintf(output, " %s", l->name);
    if (logical && l->os_index != (unsigned) -1 &&
	(verbose_mode >= 2 || l->type == HWLOC_OBJ_PU || l->type == HWLOC_OBJ_NUMANODE))
      snprintf(phys, sizeof(phys), "%s", pidxstr);
    /* display attributes */
    len = hwloc_obj_attr_snprintf (NULL, 0, l, " ", verbose_mode-1);
    attr = malloc(len+1);
    *attr = '\0';
    hwloc_obj_attr_snprintf (attr, len+1, l, " ", verbose_mode-1);
    if (*phys || *attr) {
      const char *separator = *phys != '\0' && *attr!= '\0' ? " " : "";
      fprintf(output, " (%s%s%s)",
	      phys, separator, attr);
    }
    free(attr);
    /* display the root total_memory if not verbose (already shown)
     * and different from the local_memory (already shown) */
    if (verbose_mode == 1 && !l->parent && l->memory.total_memory > l->memory.local_memory)
      fprintf(output, " (%lu%s total)",
	      (unsigned long) hwloc_memory_size_printf_value(l->memory.total_memory, 0),
	      hwloc_memory_size_printf_unit(l->memory.total_memory, 0));
    /* append the name */
    if (l->name && (l->type == HWLOC_OBJ_OS_DEVICE || verbose_mode >= 2)
	&& l->type != HWLOC_OBJ_MISC && l->type != HWLOC_OBJ_GROUP)
      fprintf(output, " \"%s\"", l->name);
  }
  if (!l->cpuset)
    return;
  if (lstopo_show_cpuset == 1)
    fprintf(output, " cpuset=");
  if (lstopo_show_cpuset) {
    char *cpusetstr;
    if (lstopo_show_taskset)
      hwloc_bitmap_taskset_asprintf(&cpusetstr, l->cpuset);
    else
      hwloc_bitmap_asprintf(&cpusetstr, l->cpuset);
    fprintf(output, "%s", cpusetstr);
    free(cpusetstr);
  }

  /* annotate if the PU is forbidden/offline/running */
  if (l->type == HWLOC_OBJ_PU && verbose_mode >= 2) {
    if (lstopo_pu_offline(l))
      fprintf(output, " (offline)");
    else if (lstopo_pu_forbidden(l))
      fprintf(output, " (forbidden)");
    else if (lstopo_pu_running(topology, l))
      fprintf(output, " (running)");
  }
}

/* Recursively output topology in a console fashion */
static void
output_topology (hwloc_topology_t topology, hwloc_obj_t l, hwloc_obj_t parent, FILE *output, int i, int logical, int verbose_mode)
{
  unsigned x;
  int group_identical = (verbose_mode <= 1) && !lstopo_show_cpuset;
  unsigned collapse = 1;

  if (l->type == HWLOC_OBJ_PCI_DEVICE) {
    const char *collapsestr = hwloc_obj_get_info_by_name(l, "lstopoCollapse");
    if (collapsestr)
      collapse = atoi(collapsestr);
    if (!collapse)
      return;
  }

  if (group_identical
      && parent && parent->arity == 1
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
    fprintf(output, "%u x { ", collapse);
  output_console_obj(topology, l, output, logical, verbose_mode, collapse > 1);
  if (collapse > 1)
    fprintf(output, " }");

  for (x=0; x<l->arity; x++)
    if (l->children[x]->type != HWLOC_OBJ_PU || !lstopo_ignore_pus)
      output_topology (topology, l->children[x], l, output, i, logical, verbose_mode);
}

/* Recursive so that multiple depth types are properly shown */
static void
output_only (hwloc_topology_t topology, hwloc_obj_t l, FILE *output, int logical, int verbose_mode)
{
  unsigned x;
  if (lstopo_show_only == l->type) {
    output_console_obj (topology, l, output, logical, verbose_mode, 0);
    fprintf (output, "\n");
  }
  for (x=0; x<l->arity; x++)
    output_only (topology, l->children[x], output, logical, verbose_mode);
}

void output_console(struct lstopo_output *loutput, const char *filename)
{
  hwloc_topology_t topology = loutput->topology;
  unsigned topodepth;
  int verbose_mode = loutput->verbose_mode;
  int logical = loutput->logical;
  FILE *output;

  output = open_output(filename, loutput->overwrite);
  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    return;
  }

  topodepth = hwloc_topology_get_depth(topology);

  /*
   * if verbose_mode == 0, only print the summary.
   * if verbose_mode == 1, only print the topology tree.
   * if verbose_mode > 1, print both.
   */

  if (lstopo_show_only != (hwloc_obj_type_t)-1) {
    if (verbose_mode > 1)
      fprintf(output, "Only showing %s objects\n", hwloc_obj_type_string(lstopo_show_only));
    output_only (topology, hwloc_get_root_obj(topology), output, logical, verbose_mode);
  } else if (verbose_mode >= 1) {
    output_topology (topology, hwloc_get_root_obj(topology), NULL, output, 0, logical, verbose_mode);
    fprintf(output, "\n");
  }

  if ((verbose_mode > 1 || !verbose_mode) && lstopo_show_only == (hwloc_obj_type_t)-1) {
    hwloc_lstopo_show_summary(output, topology);
 }

  if (verbose_mode > 1 && lstopo_show_only == (hwloc_obj_type_t)-1) {
    const struct hwloc_distances_s * distances;
    unsigned depth;

    for (depth = 0; depth < topodepth; depth++) {
      distances = hwloc_get_whole_distance_matrix_by_depth(topology, depth);
      if (!distances || !distances->latency)
        continue;
      fprintf(output, "relative latency matrix between %ss (depth %u) by %s indexes:\n",
	      hwloc_obj_type_string(hwloc_get_depth_type(topology, depth)),
	      depth,
	      logical ? "logical" : "physical");
      hwloc_utils_print_distance_matrix(output, topology, hwloc_get_root_obj(topology), distances->nbobjs, depth, distances->latency, logical);
    }
  }

  if (verbose_mode > 1 && lstopo_show_only == (hwloc_obj_type_t)-1) {
    hwloc_const_bitmap_t complete = hwloc_topology_get_complete_cpuset(topology);
    hwloc_const_bitmap_t topo = hwloc_topology_get_topology_cpuset(topology);
    hwloc_const_bitmap_t online = hwloc_topology_get_online_cpuset(topology);
    hwloc_const_bitmap_t allowed = hwloc_topology_get_allowed_cpuset(topology);

    if (complete && !hwloc_bitmap_isequal(topo, complete)) {
      hwloc_bitmap_t unknown = hwloc_bitmap_alloc();
      char *unknownstr;
      hwloc_bitmap_copy(unknown, complete);
      hwloc_bitmap_andnot(unknown, unknown, topo);
      hwloc_bitmap_asprintf(&unknownstr, unknown);
      fprintf (output, "%d processors not represented in topology: %s\n", hwloc_bitmap_weight(unknown), unknownstr);
      free(unknownstr);
      hwloc_bitmap_free(unknown);
    }
    if (complete && !hwloc_bitmap_isequal(online, complete)) {
      hwloc_bitmap_t offline = hwloc_bitmap_alloc();
      char *offlinestr;
      hwloc_bitmap_copy(offline, complete);
      hwloc_bitmap_andnot(offline, offline, online);
      hwloc_bitmap_asprintf(&offlinestr, offline);
      fprintf (output, "%d processors offline: %s\n", hwloc_bitmap_weight(offline), offlinestr);
      free(offlinestr);
      hwloc_bitmap_free(offline);
    }
    if (complete && !hwloc_bitmap_isequal(allowed, online)) {
      if (!hwloc_bitmap_isincluded(online, allowed)) {
        hwloc_bitmap_t forbidden = hwloc_bitmap_alloc();
        char *forbiddenstr;
        hwloc_bitmap_copy(forbidden, online);
        hwloc_bitmap_andnot(forbidden, forbidden, allowed);
        hwloc_bitmap_asprintf(&forbiddenstr, forbidden);
        fprintf(output, "%d processors online but not allowed: %s\n", hwloc_bitmap_weight(forbidden), forbiddenstr);
        free(forbiddenstr);
        hwloc_bitmap_free(forbidden);
      }
      if (!hwloc_bitmap_isincluded(allowed, online)) {
        hwloc_bitmap_t potential = hwloc_bitmap_alloc();
        char *potentialstr;
        hwloc_bitmap_copy(potential, allowed);
        hwloc_bitmap_andnot(potential, potential, online);
        hwloc_bitmap_asprintf(&potentialstr, potential);
        fprintf(output, "%d processors allowed but not online: %s\n", hwloc_bitmap_weight(potential), potentialstr);
        free(potentialstr);
        hwloc_bitmap_free(potential);
      }
    }
    if (!hwloc_topology_is_thissystem(topology))
      fprintf (output, "Topology not from this system\n");
  }

  if (output != stdout)
    fclose(output);
}

void output_synthetic(struct lstopo_output *loutput, const char *filename)
{
  hwloc_topology_t topology = loutput->topology;
  FILE *output;
  int length;
  char sbuffer[1024];
  char * dbuffer = NULL;

  if (!hwloc_get_root_obj(topology)->symmetric_subtree) {
    fprintf(stderr, "Cannot output assymetric topology in synthetic format.\n");
    fprintf(stderr, "Adding --no-io may help making the topology symmetric.\n");
    return;
  }

  length = hwloc_topology_export_synthetic(topology, sbuffer, sizeof(sbuffer), lstopo_export_synthetic_flags);
  if (length < 0)
    return;

  if (length >= sizeof(sbuffer)) {
    dbuffer = malloc(length+1 /* \0 */);
    if (!dbuffer)
      return;

    length = hwloc_topology_export_synthetic(topology, dbuffer, length+1, lstopo_export_synthetic_flags);
    if (length < 0)
      goto out;
  }

  output = open_output(filename, loutput->overwrite);
  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    goto out;
  }

  fprintf(output, "%s\n", dbuffer ? dbuffer : sbuffer);

  if (output != stdout)
    fclose(output);

 out:
  if (dbuffer)
    free(dbuffer);
}
