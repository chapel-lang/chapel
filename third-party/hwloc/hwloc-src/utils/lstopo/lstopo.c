/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2024 Inria.  All rights reserved.
 * Copyright © 2009-2012, 2015, 2017 Université Bordeaux
 * Copyright © 2009-2011 Cisco Systems, Inc.  All rights reserved.
 * Copyright © 2020 Hewlett Packard Enterprise.  All rights reserved.
 * Copyright © 2023 Université de Reims Champagne-Ardenne.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "private/autogen/config.h"
#include "hwloc.h"
#ifdef HWLOC_LINUX_SYS
#include "hwloc/linux.h"
#endif /* HWLOC_LINUX_SYS */
#include "hwloc/shmem.h"

#include "private/debug.h" /* for HWLOC_BUILD_ASSERT() */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
#include <fcntl.h>
#include <sys/stat.h>
#include <assert.h>
#ifdef HAVE_TIME_H
#include <time.h>
#endif

#ifdef LSTOPO_HAVE_GRAPHICS
#ifdef HWLOC_HAVE_CAIRO
#include <cairo.h>
#endif
#endif

#ifdef HAVE_SETLOCALE
#include <locale.h>
#endif

#include "lstopo.h"
#include "common-ps.h"
#include "misc.h"

#ifdef __MINGW32__
# ifdef HAVE_CLOCK_GETTIME
#  undef HAVE_CLOCK_GETTIME
# endif
#endif

#ifdef HAVE_CLOCK_GETTIME
# ifndef CLOCK_MONOTONIC /* HP-UX doesn't have CLOCK_MONOTONIC */
#  define CLOCK_MONOTONIC CLOCK_REALTIME
# endif
#endif

#ifdef ANDROID
extern void setJNIEnv();
#endif

FILE *open_output(const char *filename, int overwrite)
{
  struct stat st;

  if (!filename || !strcmp(filename, "-"))
    return stdout;

  if (!stat(filename, &st) && !overwrite) {
    errno = EEXIST;
    return NULL;
  }

  return fopen(filename, "w");
}

static hwloc_obj_t insert_misc(hwloc_topology_t topology, hwloc_cpuset_t cpuset, const char *subtype, const char * name)
{
  hwloc_obj_t group, obj;

  hwloc_bitmap_and(cpuset, cpuset, hwloc_topology_get_topology_cpuset(topology));
  if (hwloc_bitmap_iszero(cpuset))
    return NULL;

  /* try to insert a group at exact position */
  group = hwloc_topology_alloc_group_object(topology);
  if (!group)
    return NULL;
  group->cpuset = hwloc_bitmap_dup(cpuset);
  group->attr->group.kind = (unsigned) -1;
  group = hwloc_topology_insert_group_object(topology, group);
  if (!group) {
    /* try to insert in a larger parent */
    char *s, *gs;
    hwloc_bitmap_asprintf(&s, cpuset);
    group = hwloc_get_obj_covering_cpuset(topology, cpuset);
    hwloc_bitmap_asprintf(&gs, group->cpuset);
    fprintf(stderr, "%s `%s' binding %s doesn't match any object, extended to %s before inserting the object.\n",
	    subtype, name, s, gs);
    free(s);
    free(gs);
  }
  obj = hwloc_topology_insert_misc_object(topology, group, name);
  if (!obj)
    fprintf(stderr, "Failed to insert process `%s'\n", name);
  else if (subtype)
    obj->subtype = strdup(subtype);

  return obj;
}

static void foreach_process_cb(hwloc_topology_t topology,
			       struct hwloc_ps_process *proc,
			       void *cbdata __hwloc_attribute_unused)
{
  /* sort of similar to print_process_lstopo_misc() in hwloc-ps.c */
  char name[100];
  unsigned i;

  snprintf(name, sizeof(name), "%ld", proc->pid);
  if (*proc->name)
    snprintf(name, sizeof(name), "%ld %s", proc->pid, proc->name);

  if (proc->bound)
    insert_misc(topology, proc->cpuset, "Process", name);

  if (proc->nthreads)
    for(i=0; i<proc->nthreads; i++)
      if (proc->threads[i].cpuset
	  && !hwloc_bitmap_isequal(proc->threads[i].cpuset, proc->cpuset)) {
	char task_name[150];
	if (*proc->threads[i].name)
	  snprintf(task_name, sizeof(task_name), "%s %li %s", name, proc->threads[i].tid, proc->threads[i].name);
	else
	  snprintf(task_name, sizeof(task_name), "%s %li", name, proc->threads[i].tid);

	insert_misc(topology, proc->threads[i].cpuset, "Thread", task_name);
      }
}

static void add_process_objects(hwloc_topology_t topology)
{
  const struct hwloc_topology_support *support = hwloc_topology_get_support(topology);
  hwloc_obj_t root = hwloc_get_root_obj(topology);

  if (!support->cpubind->get_proc_cpubind)
    return;

  hwloc_ps_foreach_process(topology, root->cpuset,
			   foreach_process_cb, NULL,
			   HWLOC_PS_FLAG_THREADS | HWLOC_PS_FLAG_SHORTNAME, NULL, HWLOC_PS_ALL_UIDS);
}

static void add_one_misc_object_from(hwloc_topology_t topology,
                                     char *subtype, char *name, hwloc_bitmap_t cpuset)
{
  if (!hwloc_bitmap_iszero(cpuset) && subtype && name) {
    insert_misc(topology, cpuset, subtype, name);
  } else {
    char *s;
    hwloc_bitmap_asprintf(&s, cpuset);
    fprintf(stderr, "Ignoring misc object subtype %s name %s cpuset %s\n", subtype, name, s);
    free(s);
  }
}

/* reads Misc description from the FILE*
 * entries must look like:
 * name=... (must be first)
 * cpuset=... (cannot be 0)
 * subtype=... (optional)
 */
static void add_misc_objects_from(hwloc_topology_t topology, FILE *from)
{
  char line[256];
  hwloc_bitmap_t cpuset;
  char *subtype = NULL;
  char *name = NULL;
  cpuset = hwloc_bitmap_alloc();
  if (!cpuset)
    return;

  while (fgets(line, sizeof line, from)) {
    char *end;

    /* remove ending \n */
    end = strchr(line, '\n');
    if (end)
      *end = '\0';
    /* ignoring empty lines */
    if (line[0] == '\0')
      continue;

    if (!strncmp(line, "name=", 5)) {
      /* commit (or ignore) the previous entry */
      if (name)
        add_one_misc_object_from(topology, subtype, name, cpuset);
      /* start a new entry */
      free(subtype);
      subtype = NULL;
      free(name);
      name = strdup(line+5);
      hwloc_bitmap_zero(cpuset);

    } else if (!strncmp(line, "cpuset=", 7)) {
      hwloc_bitmap_sscanf(cpuset, line+7);

    } else if (!strncmp(line, "subtype=", 8)) {
      free(subtype);
      subtype = strdup(line+8);

    } else {
      fprintf(stderr, "Unrecognized --misc-from line `%s', ignored\n", line);
    }
  }

  /* commit (or ignore) the last entry */
  if (name)
    add_one_misc_object_from(topology, subtype, name, cpuset);
  free(name);
  free(subtype);
  hwloc_bitmap_free(cpuset);
}

static __hwloc_inline void lstopo_update_factorize_bounds(unsigned min, unsigned *first, unsigned *last)
{
  switch (min) {
  case 0:
  case 1:
  case 2:
    *first = 1;
    *last = 0;
    break;
  case 3:
    *first = 1;
    *last = 1;
    break;
  default:
    *first = 2;
    *last = 1;
    break;
  }
}

static __hwloc_inline void lstopo_update_factorize_alltypes_bounds(struct lstopo_output *loutput)
{
  hwloc_obj_type_t type;
  for(type = 0; type < HWLOC_OBJ_TYPE_MAX; type++)
    lstopo_update_factorize_bounds(loutput->factorize_min[type], &loutput->factorize_first[type], &loutput->factorize_last[type]);
}

static void
lstopo_add_factorized_attributes(struct lstopo_output *loutput, hwloc_topology_t topology, hwloc_obj_t obj)
{
  hwloc_obj_t child;

  if (!obj->first_child)
    return;

  if (obj->symmetric_subtree && obj->arity > loutput->factorize_min[obj->first_child->type]){
    int may_factorize = 1;
    /* check that the object is in a single cpukind */
    if (loutput->nr_cpukind_styles) {
      int err = hwloc_cpukinds_get_by_cpuset(topology, obj->cpuset, 0);
      if (err < 0 && errno == EXDEV)
        may_factorize = 0;
    }
    if (may_factorize) {
      /* factorize those children */
      for_each_child(child, obj) {
        unsigned factorized;
        if (child->sibling_rank < loutput->factorize_first[child->type]
            || child->sibling_rank >= obj->arity - loutput->factorize_last[child->type])
          factorized = 0; /* keep first and last */
        else if (child->sibling_rank == loutput->factorize_first[child->type])
          factorized = 1; /* replace with dots */
        else
          factorized = -1; /* remove that one */
        ((struct lstopo_obj_userdata *)child->userdata)->factorized = factorized;
      }
    }
  }
  /* recurse */
  for_each_child(child, obj)
    lstopo_add_factorized_attributes(loutput, topology, child);
}

static void
lstopo_add_collapse_attributes(hwloc_topology_t topology)
{
  hwloc_obj_t obj, collapser = NULL;
  unsigned collapsed = 0;
  /* collapse identical PCI devs */
  for(obj = hwloc_get_next_pcidev(topology, NULL); obj; obj = hwloc_get_next_pcidev(topology, obj)) {
    if (collapser) {
      if (!obj->io_arity && !obj->misc_arity
	  && obj->parent == collapser->parent
	  && obj->attr->pcidev.vendor_id == collapser->attr->pcidev.vendor_id
	  && obj->attr->pcidev.device_id == collapser->attr->pcidev.device_id
	  && obj->attr->pcidev.subvendor_id == collapser->attr->pcidev.subvendor_id
	  && obj->attr->pcidev.subdevice_id == collapser->attr->pcidev.subdevice_id) {
	/* collapse another one */
	((struct lstopo_obj_userdata *)obj->userdata)->pci_collapsed = -1;
	collapsed++;
	continue;
      } else if (collapsed > 1) {
	/* end this collapsing */
	((struct lstopo_obj_userdata *)collapser->userdata)->pci_collapsed = collapsed;
	collapser = NULL;
	collapsed = 0;
      }
    }
    if (!obj->io_arity && !obj->misc_arity) {
      /* start a new collapsing */
      collapser = obj;
      collapsed = 1;
    }
  }
  if (collapsed > 1) {
    /* end this collapsing */
    ((struct lstopo_obj_userdata *)collapser->userdata)->pci_collapsed = collapsed;
  }
}

static void
lstopo_add_cpukind_style(struct lstopo_output *loutput, hwloc_topology_t topology)
{
  unsigned i, nr;
  hwloc_bitmap_t cpuset = hwloc_bitmap_alloc();
  if (!cpuset)
    return;
  nr = hwloc_cpukinds_get_nr(topology, 0);
  for(i=0; i<nr; i++) {
    hwloc_obj_t obj;
    hwloc_cpukinds_get_info(topology, i, cpuset, NULL, NULL, NULL, 0);
    obj = NULL;
    while ((obj = hwloc_get_next_obj_inside_cpuset_by_type(topology, cpuset, HWLOC_OBJ_PU, obj)) != NULL)
      ((struct lstopo_obj_userdata *)obj->userdata)->cpukind_style = i;
  }
  hwloc_bitmap_free(cpuset);

  loutput->nr_cpukind_styles = nr;
}

static int
lstopo_check_pci_domains(hwloc_topology_t topology)
{
  hwloc_obj_t obj;

  /* check PCI devices for domains.
   * they are listed by depth-first search, the order doesn't guarantee a domain at the end.
   */
  obj = NULL;
  while ((obj = hwloc_get_next_pcidev(topology, obj)) != NULL) {
    if (obj->attr->pcidev.domain)
      return 1;
  }

  /* check PCI Bridges for domains.
   * they are listed by depth-first search, the order doesn't guarantee a domain at the end.
   */
  obj = NULL;
  while ((obj = hwloc_get_next_bridge(topology, obj)) != NULL) {
    if (obj->attr->bridge.upstream_type != HWLOC_OBJ_BRIDGE_PCI)
      break;
    if (obj->attr->pcidev.domain)
      return 1;
  }

  return 0;
}

static void
lstopo_parse_children_order(char *s, unsigned *children_order_p,
                            enum lstopo_orient_e *above_force_orient_p,
                            enum lstopo_orient_e *right_force_orient_p,
                            enum lstopo_orient_e *below_force_orient_p)
{
  char *tmp, *next;
  unsigned children_order;
  enum lstopo_orient_e above_force_orient = LSTOPO_ORIENT_NONE;
  enum lstopo_orient_e right_force_orient = LSTOPO_ORIENT_NONE;
  enum lstopo_orient_e below_force_orient= LSTOPO_ORIENT_NONE;

  if (!strcmp(s, "plain")) {
    *children_order_p = LSTOPO_ORDER_PLAIN;
    return;
  }

  tmp = s;
  children_order = LSTOPO_ORDER_PLAIN;
  while (tmp && *tmp) {
    next = strchr(tmp, ',');
    if (next) {
      *next = 0;
      next++;
    }

    if (!strcmp(tmp, "memory:above") || !strcmp(tmp, "memoryabove") /* backward compat with 2.5 */) {
      children_order |= LSTOPO_ORDER_MEMORY_ABOVE;
    } else if (!strcmp(tmp, "memory:above:horiz")) {
      children_order |= LSTOPO_ORDER_MEMORY_ABOVE;
      above_force_orient = LSTOPO_ORIENT_HORIZ;
    } else if (!strcmp(tmp, "memory:above:vert")) {
      children_order |= LSTOPO_ORDER_MEMORY_ABOVE;
      above_force_orient = LSTOPO_ORIENT_VERT;
    } else if (!strcmp(tmp, "memory:above:rect")) {
      children_order |= LSTOPO_ORDER_MEMORY_ABOVE;
      above_force_orient = LSTOPO_ORIENT_RECT;

    } else if (!strcmp(tmp, "io:right")) {
      children_order |= LSTOPO_ORDER_IO_RIGHT;
    } else if (!strcmp(tmp, "io:right:horiz")) {
      children_order |= LSTOPO_ORDER_IO_RIGHT;
      right_force_orient = LSTOPO_ORIENT_HORIZ;
    } else if (!strcmp(tmp, "io:right:vert")) {
      children_order |= LSTOPO_ORDER_IO_RIGHT;
      right_force_orient = LSTOPO_ORIENT_VERT;
    } else if (!strcmp(tmp, "io:right:rect")) {
      children_order |= LSTOPO_ORDER_IO_RIGHT;
      right_force_orient = LSTOPO_ORIENT_RECT;

    } else if (!strcmp(tmp, "io:below")) {
      children_order |= LSTOPO_ORDER_IO_BELOW;
    } else if (!strcmp(tmp, "io:below:horiz")) {
      children_order |= LSTOPO_ORDER_IO_BELOW;
      below_force_orient = LSTOPO_ORIENT_HORIZ;
    } else if (!strcmp(tmp, "io:below:vert")) {
      children_order |= LSTOPO_ORDER_IO_BELOW;
      below_force_orient = LSTOPO_ORIENT_VERT;
    } else if (!strcmp(tmp, "io:below:rect")) {
      children_order |= LSTOPO_ORDER_IO_BELOW;
      below_force_orient = LSTOPO_ORIENT_RECT;

    } else if (!strcmp(tmp, "misc:right")) {
      children_order |= LSTOPO_ORDER_MISC_RIGHT;
    } else if (!strcmp(tmp, "misc:right:horiz")) {
      children_order |= LSTOPO_ORDER_MISC_RIGHT;
      right_force_orient = LSTOPO_ORIENT_HORIZ;
    } else if (!strcmp(tmp, "misc:right:vert")) {
      children_order |= LSTOPO_ORDER_MISC_RIGHT;
      right_force_orient = LSTOPO_ORIENT_VERT;
    } else if (!strcmp(tmp, "misc:right:rect")) {
      children_order |= LSTOPO_ORDER_MISC_RIGHT;
      right_force_orient = LSTOPO_ORIENT_RECT;

    } else if (!strcmp(tmp, "misc:below")) {
      children_order |= LSTOPO_ORDER_MISC_BELOW;
    } else if (!strcmp(tmp, "misc:below:horiz")) {
      children_order |= LSTOPO_ORDER_MISC_BELOW;
      below_force_orient = LSTOPO_ORIENT_HORIZ;
    } else if (!strcmp(tmp, "misc:below:vert")) {
      children_order |= LSTOPO_ORDER_MISC_BELOW;
      below_force_orient = LSTOPO_ORIENT_VERT;
    } else if (!strcmp(tmp, "misc:below:rect")) {
      children_order |= LSTOPO_ORDER_MISC_BELOW;
      below_force_orient = LSTOPO_ORIENT_RECT;

    } else if (strcmp(tmp, "plain")) {
      fprintf(stderr, "Unsupported children order `%s', ignoring.\n", tmp);
    }

    tmp = next;
  }

  *children_order_p = children_order;
  *above_force_orient_p = above_force_orient;
  *right_force_orient_p = right_force_orient;
  *below_force_orient_p = below_force_orient;
}

static void
lstopo_populate_userdata(hwloc_obj_t parent)
{
  hwloc_obj_t child;
  struct lstopo_obj_userdata *save = malloc(sizeof(*save));

  save->common.buffer = NULL; /* so that it is ignored on XML export */
  save->common.next = parent->userdata;
  save->factorized = 0;
  save->pci_collapsed = 0;
  save->cpukind_style = 0;
  parent->userdata = save;

  for_each_child(child, parent)
    lstopo_populate_userdata(child);
  for_each_memory_child(child, parent)
    lstopo_populate_userdata(child);
  for_each_io_child(child, parent)
    lstopo_populate_userdata(child);
  for_each_misc_child(child, parent)
    lstopo_populate_userdata(child);
}

static void
lstopo_destroy_userdata(hwloc_obj_t parent)
{
  hwloc_obj_t child;
  struct lstopo_obj_userdata *save = parent->userdata;

  if (save) {
    parent->userdata = save->common.next;
    free(save);
  }

  for_each_child(child, parent)
    lstopo_destroy_userdata(child);
  for_each_memory_child(child, parent)
    lstopo_destroy_userdata(child);
  for_each_io_child(child, parent)
    lstopo_destroy_userdata(child);
  for_each_misc_child(child, parent)
    lstopo_destroy_userdata(child);
}

void usage(const char *name, FILE *where)
{
  fprintf (where, "Usage: %s [ options ] ... [ filename.format ]\n\n", name);
  fprintf (where, "See lstopo(1) for more details.\n");

  fprintf (where, "\nDefault output is "
#if (defined LSTOPO_HAVE_GRAPHICS) && (defined HWLOC_WIN_SYS)
		  "graphical window"
#elif (defined LSTOPO_HAVE_GRAPHICS) && (defined LSTOPO_HAVE_X11)
		  "graphical window (X11) if DISPLAY is set, console otherwise"
#else
		  "console"
#endif
		  ".\n");

  fprintf (where, "Supported output file formats: console, ascii, tikz, fig"
#ifdef LSTOPO_HAVE_GRAPHICS
#ifdef CAIRO_HAS_PDF_SURFACE
		  ", pdf"
#endif /* CAIRO_HAS_PDF_SURFACE */
#ifdef CAIRO_HAS_PS_SURFACE
		  ", ps"
#endif /* CAIRO_HAS_PS_SURFACE */
#ifdef CAIRO_HAS_PNG_FUNCTIONS
		  ", png"
#endif /* CAIRO_HAS_PNG_FUNCTIONS */
#ifdef CAIRO_HAS_SVG_SURFACE
		  ", svg(cairo,native)"
#endif /* CAIRO_HAS_SVG_SURFACE */
#endif /* LSTOPO_HAVE_GRAPHICS */
#if !(defined LSTOPO_HAVE_GRAPHICS) || !(defined CAIRO_HAS_SVG_SURFACE)
		  ", svg(native)"
#endif
		  ", xml, synthetic"
		  "\n");
  fprintf (where, "\nFormatting options:\n");
  fprintf (where, "  -l --logical          Display hwloc logical object indexes\n");
  fprintf (where, "  -p --physical         Display OS/physical object indexes\n");
  fprintf (where, "Output options:\n");
  fprintf (where, "  --output-format <format>\n");
  fprintf (where, "  --of <format>         Force the output to use the given format\n");
  fprintf (where, "  -f --force            Overwrite the output file if it exists\n");
  fprintf (where, "Textual output options:\n");
  fprintf (where, "  --only <type>         Only show objects of the given type in the textual output\n");
  fprintf (where, "  -v --verbose          Include additional details\n");
  fprintf (where, "  -q --quiet -s         Reduce the amount of details to show\n");
  fprintf (where, "  --distances           Only show distance matrices\n");
  fprintf (where, "  --distances-transform <links|merge-switch-ports|transitive-closure>\n");
  fprintf (where, "                        Transform distances before displaying them\n");
  fprintf (where, "  --memattrs            Only show memory attributes\n");
  fprintf (where, "  --cpukinds            Only show CPU kinds\n");
#ifdef HWLOC_WIN_SYS
  fprintf (where, "  --windows-processor-groups    Only show Windows processor groups\n");
#endif
  fprintf (where, "  -c --cpuset           Show the cpuset of each object\n");
  fprintf (where, "  -C --cpuset-only      Only show the cpuset of each object\n");
  fprintf (where, "  --cpuset-output-format <hwloc|list|taskset>\n"
                  "  --cof <hwloc|list|taskset>\n"
                  "                        Change the format of cpuset outputs\n");
  fprintf (where, "Object filtering options:\n");
  fprintf (where, "  --filter <type>:<knd> Filter objects of the given type, or all.\n");
  fprintf (where, "     <knd> may be `all' (keep all), `none' (remove all), `structure' or `important'\n");
  fprintf (where, "  --ignore <type>       Ignore objects of the given type\n");
  fprintf (where, "  --no-smt              Ignore PUs\n");
  fprintf (where, "  --no-caches           Do not show caches\n");
  fprintf (where, "  --no-useless-caches   Do not show caches which do not have a hierarchical\n"
                  "                        impact\n");
  fprintf (where, "  --no-icaches          Do not show instruction caches\n");
  fprintf (where, "  --merge               Do not show levels that do not have a hierarchical\n"
                  "                        impact\n");
  fprintf (where, "  --no-collapse         Do not collapse identical PCI devices\n");
  fprintf (where, "  --restrict [nodeset=]<bitmap>\n");
  fprintf (where, "                        Restrict the topology to some processors or NUMA nodes.\n");
  fprintf (where, "  --restrict binding    Restrict the topology to the current process binding\n");
  fprintf (where, "  --restrict-flags <n>  Set the flags to be used during restrict\n");
  fprintf (where, "  --no-io               Do not show any I/O device or bridge\n");
  fprintf (where, "  --no-bridges          Do not any I/O bridge except hostbridges\n");
  fprintf (where, "  --whole-io            Show all I/O devices and bridges\n");
  fprintf (where, "Input options:\n");
  hwloc_utils_input_format_usage(where, 6);
  fprintf (where, "  --thissystem          Assume that the input topology provides the topology\n"
		  "                        for the system on which we are running\n");
  fprintf (where, "  --pid <pid>           Detect topology as seen by process <pid>\n");
  fprintf (where, "  --disallowed          Include objects disallowed by administrative limitations\n");
  fprintf (where, "  --allow <all|local|...>   Change the set of objects marked as allowed\n");
  fprintf (where, "  --flags <n>           Set the topology flags\n");
  fprintf (where, "Graphical output options:\n");
  fprintf (where, "  --children-order <memory:above|io:right:vert|...|plain>\n"
		  "                        Change the layout of Memory, I/O or Misc children\n");
  fprintf (where, "  --no-factorize        Do not factorize identical objects\n");
  fprintf (where, "  --no-factorize=<type> Do not factorize identical objects of type <type>\n");
  fprintf (where, "  --factorize           Factorize identical objects (default)\n");
  fprintf (where, "  --factorize=[<type>,]<N>[,<L>[,<F>]]\n");
  fprintf (where, "                        Set the minimum number <N> of objects to factorize,\n");
  fprintf (where, "                        the numbers of first <F> and last <L> to keep,\n");
  fprintf (where, "                        for all or only the given object type <type>\n");
  fprintf (where, "  --no-cpukinds         Do not show CPU kinds\n");
  fprintf (where, "  --fontsize 10         Set size of text font\n");
  fprintf (where, "  --gridsize 7          Set size of margin between elements\n");
  fprintf (where, "  --linespacing 4       Set spacing between lines of text\n");
  fprintf (where, "  --thickness 1         Set thickness of lines and boxes\n");
  fprintf (where, "  --horiz[=<type,...>]  Horizontal graphical layout instead of nearly 4/3 ratio\n");
  fprintf (where, "  --vert[=<type,...>]   Vertical graphical layout instead of nearly 4/3 ratio\n");
  fprintf (where, "  --rect[=<type,...>]   Rectangular graphical layout with nearly 4/3 ratio\n");
  fprintf (where, "  --text[=<type,...>]   Display text for the given object types\n");
  fprintf (where, "  --no-text[=<type,..>] Do not display text for the given object types\n");
  fprintf (where, "  --index=[<type,...>]  Display indexes for the given object types\n");
  fprintf (where, "  --no-index=[<type,.>] Do not display indexes for the given object types\n");
  fprintf (where, "  --attrs=[<type,...>]  Display attributes for the given object types\n");
  fprintf (where, "  --no-attrs=[<type,.>] Do not display attributes for the given object types\n");
  fprintf (where, "  --no-legend           Remove all text legend lines at the bottom\n");
  fprintf (where, "  --no-default-legend   Remove default text legend lines at the bottom\n");
  fprintf (where, "  --append-legend <s>   Append a new line of text at the bottom of the legend\n");
  fprintf (where, "  --grey --palette grey Use greyscale instead of colors\n");
  fprintf (where, "  --palette white       Use white instead of colors for background\n");
  fprintf (where, "  --palette <type>=<#xxyyzz>\n"
                  "                        Replace the color for object of the given type\n");
  fprintf (where, "  --binding-color <none|#xxyyzz>\n"
                  "                        Disable or change binding PU and NUMA nodes color\n");
  fprintf (where, "  --disallowed-color <none|#xxyyzz>\n"
                  "                        Disable or change disallowed PU and NUMA nodes color\n");
  fprintf (where, "  --top-color <none|#xxyyzz>\n"
                  "                        Disable or change task background color for --top\n");
  fprintf (where, "Miscellaneous options:\n");
  fprintf (where, "  --logical-index-prefix <s> --os-index-prefix <s>\n");
  fprintf (where, "                        Use <s> as a prefix for logical or physical/OS indexes\n");
  fprintf (where, "  --export-xml-flags <n>\n"
		  "                        Set flags during the XML topology export\n");
  fprintf (where, "  --export-synthetic-flags <n>\n"
		  "                        Set flags during the synthetic topology export\n");
  /* --shmem-output-addr is undocumented on purpose */
  fprintf (where, "  --ps --top            Display processes within the hierarchy\n");
  fprintf (where, "  --misc-from <file>    Create Misc objects as defined in <file>");
  fprintf (where, "  --version             Report version and exit\n");
  fprintf (where, "  -h --help             Show this usage\n");
}

void lstopo_show_interactive_help(void)
{
#ifdef HAVE_ISATTY
  if (!isatty(STDOUT_FILENO))
    /* don't send the interactive help when not a terminal */
    return;
#endif

  printf("\n");
  printf("Keyboard shortcuts:\n");
  printf(" Zooming, scrolling and closing:\n");
  printf("  Zoom-in or out ...................... + -\n");
  printf("  Reset scale to default .............. 1\n");
  printf("  Try to fit scale to window .......... F\n");
  printf("  Resize window to the drawing ........ r\n");
  printf("  Toggle auto-resizing of the window .. R\n");
  printf("  Scroll vertically ................... Up Down PageUp PageDown\n");
  printf("  Scroll horizontally ................. Left Right Ctrl+PageUp/Down\n");
  printf("  Scroll to the top-left corner ....... Home\n");
  printf("  Scroll to the bottom-right corner ... End\n");
  printf("  Refresh the topology ................ F5\n");
  printf("  Show this help ...................... h H ?\n");
  printf("  Exit ................................ q Q Esc\n");
  printf(" Configuration tweaks:\n");
  printf("  Toggle factorizing or collapsing .... f\n");
  printf("  Switch display mode for indexes ..... i\n");
  printf("  Toggle displaying of object text .... t\n");
  printf("  Toggle displaying of obj attributes . a\n");
  printf("  Toggle displaying of CPU kinds ...... k\n");
  printf("  Toggle color for disallowed objects . d\n");
  printf("  Toggle color for binding objects .... b\n");
  printf("  Toggle displaying of legend lines ... l\n");
  printf("  Export to file with current config .. E\n");
  printf("\n\n");
  fflush(stdout);
}

static void lstopo__show_interactive_cli_options(const struct lstopo_output *loutput)
{
  if (loutput->index_type == LSTOPO_INDEX_TYPE_PHYSICAL)
    printf(" -p");
  else if (loutput->index_type == LSTOPO_INDEX_TYPE_LOGICAL)
    printf(" -l");
  else if (loutput->index_type == LSTOPO_INDEX_TYPE_NONE)
    printf(" --no-index");

  if (!loutput->show_attrs_enabled)
    printf(" --no-attrs");
  if (!loutput->show_text_enabled)
    printf(" --no-text");

  if(!loutput->factorize_enabled)
    printf(" --no-factorize");
  if (!loutput->pci_collapse_enabled)
    printf(" --no-collapse");
  if (!loutput->show_cpukinds)
    printf(" --no-cpukinds");
  if (!loutput->show_binding)
    printf(" --binding-color none");
  if (!loutput->show_disallowed)
    printf(" --disallowed-color none");
  if (loutput->show_legend == LSTOPO_SHOW_LEGEND_NONE)
    printf(" --no-legend");
  else if (loutput->show_legend == LSTOPO_SHOW_LEGEND_NO_DEFAULT)
    printf(" --no-default-legend");
}

void lstopo_show_interactive_cli_options(const struct lstopo_output *loutput)
{
#if (defined LSTOPO_HAVE_GRAPHICS) && (defined CAIRO_HAS_PDF_SURFACE)
  const char *format = "PDF";
  const char *extension = "pdf";
#else
  const char *format = "SVG";
  const char *extension = "svg";
#endif
  printf("\nCommand-line options for the current configuration tweaks:\n");
  lstopo__show_interactive_cli_options(loutput);
  printf("\n\nTo export to %s:\n", format);
  printf("  lstopo  <your options>");
  lstopo__show_interactive_cli_options(loutput);
  printf(" topology.%s\n\n", extension);
}

enum output_format {
  LSTOPO_OUTPUT_DEFAULT,
  LSTOPO_OUTPUT_WINDOW,
  LSTOPO_OUTPUT_CONSOLE,
  LSTOPO_OUTPUT_SYNTHETIC,
  LSTOPO_OUTPUT_ASCII,
  LSTOPO_OUTPUT_TIKZ,
  LSTOPO_OUTPUT_FIG,
  LSTOPO_OUTPUT_PNG,
  LSTOPO_OUTPUT_PDF,
  LSTOPO_OUTPUT_PS,
  LSTOPO_OUTPUT_SVG,
  LSTOPO_OUTPUT_CAIROSVG,
  LSTOPO_OUTPUT_NATIVESVG,
  LSTOPO_OUTPUT_XML,
  LSTOPO_OUTPUT_SHMEM,
  LSTOPO_OUTPUT_ERROR
};

static enum output_format
parse_output_format(const char *name, char *callname __hwloc_attribute_unused)
{
  if (!hwloc_strncasecmp(name, "default", 3))
    return LSTOPO_OUTPUT_DEFAULT;
  else if (!hwloc_strncasecmp(name, "window", 3))
    return LSTOPO_OUTPUT_WINDOW;
  else if (!hwloc_strncasecmp(name, "console", 3))
    return LSTOPO_OUTPUT_CONSOLE;
  else if (!strcasecmp(name, "synthetic"))
    return LSTOPO_OUTPUT_SYNTHETIC;
  else if (!strcasecmp(name, "ascii")
	   || !strcasecmp(name, "txt") /* backward compat with 1.10 */)
    return LSTOPO_OUTPUT_ASCII;
  else if (!strcasecmp(name, "tikz") || !strcasecmp(name, "tex"))
    return LSTOPO_OUTPUT_TIKZ;
  else if (!strcasecmp(name, "fig"))
    return LSTOPO_OUTPUT_FIG;
  else if (!strcasecmp(name, "png"))
    return LSTOPO_OUTPUT_PNG;
  else if (!strcasecmp(name, "pdf"))
    return LSTOPO_OUTPUT_PDF;
  else if (!strcasecmp(name, "ps"))
    return LSTOPO_OUTPUT_PS;
  else if (!strcasecmp(name, "svg"))
    return LSTOPO_OUTPUT_SVG;
  else if (!strcasecmp(name, "cairosvg") || !strcasecmp(name, "svg(cairo)"))
    return LSTOPO_OUTPUT_CAIROSVG;
  else if (!strcasecmp(name, "nativesvg") || !strcasecmp(name, "svg(native)"))
    return LSTOPO_OUTPUT_NATIVESVG;
  else if (!strcasecmp(name, "xml"))
    return LSTOPO_OUTPUT_XML;
  else if (!strcasecmp(name, "shmem"))
    return LSTOPO_OUTPUT_SHMEM;
  else
    return LSTOPO_OUTPUT_ERROR;
}

static const char *
output_format_name(enum output_format format)
{
  switch (format) {
  case LSTOPO_OUTPUT_DEFAULT: return "default";
  case LSTOPO_OUTPUT_WINDOW: return "window";
  case LSTOPO_OUTPUT_CONSOLE: return "console";
  case LSTOPO_OUTPUT_SYNTHETIC: return "synthetic";
  case LSTOPO_OUTPUT_ASCII: return "ascii";
  case LSTOPO_OUTPUT_TIKZ: return "tikz";
  case LSTOPO_OUTPUT_FIG: return "fig";
  case LSTOPO_OUTPUT_PNG: return "png";
  case LSTOPO_OUTPUT_PDF: return "pdf";
  case LSTOPO_OUTPUT_PS: return "ps";
  case LSTOPO_OUTPUT_SVG: return "svg";
  case LSTOPO_OUTPUT_CAIROSVG: return "cairosvg";
  case LSTOPO_OUTPUT_NATIVESVG: return "nativesvg";
  case LSTOPO_OUTPUT_XML: return "xml";
  case LSTOPO_OUTPUT_SHMEM: return "shmem";
  case LSTOPO_OUTPUT_ERROR: return "error";
  }
  abort();
}

/****************************************************
 * Store filters during parsing and apply them later
 */

struct lstopo_type_filter { enum hwloc_type_filter_e filter; int changed; };

/* if these assert fails, some types were added,
 * assumptions in macros below must be rechecked
 */
#define init_type_filters() do { \
  unsigned _i; \
  HWLOC_BUILD_ASSERT(HWLOC_OBJ_TYPE_MIN == 0); \
  HWLOC_BUILD_ASSERT(HWLOC_OBJ_TYPE_MAX == 20); \
  for(_i=HWLOC_OBJ_TYPE_MIN; _i<HWLOC_OBJ_TYPE_MAX; _i++) \
    type_filter[_i].changed = 0; \
} while (0)

#define set_type_filter(_type, _filter) do { \
  type_filter[_type].filter = _filter; \
  type_filter[_type].changed = 1; \
} while (0)

#define set_all_types_filter(_filter) do { \
  unsigned _i; \
  for(_i=HWLOC_OBJ_TYPE_MIN; _i<HWLOC_OBJ_TYPE_MAX; _i++) \
    set_type_filter(_i, _filter);                         \
} while (0)

/* must operate on same types as hwloc_topology_set_io_types_filter() */
#define set_io_types_filter(_filter) do { \
  set_type_filter(HWLOC_OBJ_BRIDGE, _filter); \
  set_type_filter(HWLOC_OBJ_PCI_DEVICE, _filter); \
  set_type_filter(HWLOC_OBJ_OS_DEVICE, _filter); \
} while (0)

/* must operate on same types as hwloc_topology_set_cache_types_filter() */
#define set_cache_types_filter(_filter) do { \
  unsigned _i; \
  for(_i=HWLOC_OBJ_L1CACHE; _i<=HWLOC_OBJ_L3ICACHE; _i++) \
    set_type_filter(_i, _filter);                         \
} while (0)

/* must operate on same types as hwloc_topology_set_icache_types_filter() */
#define set_icache_types_filter(_filter) do { \
  unsigned _i; \
  for(_i=HWLOC_OBJ_L1ICACHE; _i<=HWLOC_OBJ_L3ICACHE; _i++) \
    set_type_filter(_i, _filter);                          \
} while (0)

#define apply_type_filters(_topo) do { \
  unsigned _i; \
  for(_i=HWLOC_OBJ_TYPE_MIN; _i<HWLOC_OBJ_TYPE_MAX; _i++) \
    if (type_filter[_i].changed) \
      hwloc_topology_set_type_filter(_topo, _i, type_filter[_i].filter); \
} while (0)


#define LSTOPO_VERBOSE_MODE_DEFAULT 1

int
main (int argc, char *argv[])
{
  int err;
  hwloc_topology_t topology;
  const char *filename = NULL;
  unsigned long flags = HWLOC_TOPOLOGY_FLAG_IMPORT_SUPPORT;
  unsigned long restrict_flags = 0;
  unsigned long allow_flags = 0;
  hwloc_bitmap_t allow_cpuset = NULL, allow_nodeset = NULL;
  char * callname;
  char * input = NULL;
  const char *show_only_string = NULL;
  struct hwloc_utils_input_format_s input_format = HWLOC_UTILS_INPUT_FORMAT_DEFAULT;
  enum output_format output_format = LSTOPO_OUTPUT_DEFAULT;
  struct lstopo_type_filter type_filter[HWLOC_OBJ_TYPE_MAX];
  char *restrictstring = NULL;
  struct lstopo_output loutput;
  output_method *output_func;
  hwloc_membind_policy_t policy;
#ifdef HAVE_CLOCK_GETTIME
  struct timespec ts1, ts2;
  unsigned long ms;
  int measure_load_time = !!getenv("HWLOC_DEBUG_LOAD_TIME");
#endif
  char *env;
  int top = 0;
  FILE * miscfrom = NULL;
  int opt;
  unsigned i;

  callname = strrchr(argv[0], '/');
  if (!callname)
    callname = argv[0];
  else
    callname++;
  /* skip argv[0], handle options */
  argc--;
  argv++;

  hwloc_utils_check_api_version(callname);

  loutput.refreshing = 0;

  loutput.overwrite = 0;

  loutput.index_type = LSTOPO_INDEX_TYPE_DEFAULT;
  loutput.verbose_mode = LSTOPO_VERBOSE_MODE_DEFAULT;
  loutput.ignore_pus = 0;
  loutput.ignore_numanodes = 0;
  loutput.pci_collapse_enabled = 1;
  loutput.pid_number = -1;
  loutput.pid = 0;
  loutput.need_pci_domain = 0;

  init_type_filters();

  loutput.factorize_enabled = 1;
  for(i=HWLOC_OBJ_TYPE_MIN; i<HWLOC_OBJ_TYPE_MAX; i++)
    loutput.factorize_min[i] = FACTORIZE_MIN_DEFAULT;
  lstopo_update_factorize_alltypes_bounds(&loutput);

  loutput.export_synthetic_flags = 0;
  loutput.export_xml_flags = 0;
  loutput.shmem_output_addr = 0;

  loutput.show_legend = LSTOPO_SHOW_LEGEND_ALL;
  loutput.legend_append = NULL;
  loutput.legend_append_nr = 0;
  snprintf(loutput.title, sizeof(loutput.title), "lstopo");

  loutput.show_distances_only = 0;
  loutput.show_memattrs_only = 0;
  loutput.show_cpukinds_only = 0;
  loutput.show_windows_processor_groups_only = 0;
  loutput.show_cpuset = 0;
  loutput.cpuset_output_format = HWLOC_UTILS_CPUSET_FORMAT_HWLOC;
  loutput.transform_distances = -1;

  loutput.nr_cpukind_styles = 0;

  loutput.backend_data = NULL;
  loutput.backend_flags = 0;
  loutput.methods = NULL;

  loutput.children_order = LSTOPO_ORDER_MEMORY_ABOVE | LSTOPO_ORDER_IO_RIGHT | LSTOPO_ORDER_MISC_RIGHT;
  loutput.fontsize = 10;
  loutput.gridsize = 7;
  loutput.linespacing = 4;
  loutput.thickness = 1;

  loutput.text_xscale = 1.0f;
  env = getenv("LSTOPO_TEXT_XSCALE");
  if (env)
    loutput.text_xscale = (float) atof(env);

  for(i=HWLOC_OBJ_TYPE_MIN; i<HWLOC_OBJ_TYPE_MAX; i++)
    loutput.force_orient[i] = LSTOPO_ORIENT_NONE;
  loutput.force_orient[HWLOC_OBJ_PU] = LSTOPO_ORIENT_HORIZ;
  for(i=HWLOC_OBJ_L1CACHE; i<=HWLOC_OBJ_L3ICACHE; i++)
    loutput.force_orient[i] = LSTOPO_ORIENT_HORIZ;
  loutput.force_orient[HWLOC_OBJ_NUMANODE] = LSTOPO_ORIENT_HORIZ;
  loutput.force_orient[HWLOC_OBJ_MEMCACHE] = LSTOPO_ORIENT_HORIZ;
  loutput.above_force_orient = LSTOPO_ORIENT_NONE;
  loutput.right_force_orient = LSTOPO_ORIENT_NONE;
  loutput.below_force_orient = LSTOPO_ORIENT_NONE;
  for(i=HWLOC_OBJ_TYPE_MIN; i<HWLOC_OBJ_TYPE_MAX; i++) {
    loutput.show_indexes[i] = 1;
    loutput.show_attrs[i] = 1;
    loutput.show_text[i] = 1;
  }
  loutput.show_attrs_enabled = 1;
  loutput.show_text_enabled = 1;
  loutput.os_index_prefix = (char *) " P#";
  loutput.logical_index_prefix = (char *) " L#";

  loutput.show_binding = 1;
  loutput.show_disallowed = 1;
  loutput.show_cpukinds = 1;

  loutput.show_process_color = 1;
  lstopo_palette_init(&loutput);

  /* show all error messages */
  if (!getenv("HWLOC_HIDE_ERRORS"))
    putenv((char *) "HWLOC_HIDE_ERRORS=0");
  /* enable verbose backends */
  if (!getenv("HWLOC_XML_VERBOSE"))
    putenv((char *) "HWLOC_XML_VERBOSE=1");
  if (!getenv("HWLOC_SYNTHETIC_VERBOSE"))
    putenv((char *) "HWLOC_SYNTHETIC_VERBOSE=1");

  /* Use localized time prints, and utf-8 characters in the ascii output */
#ifdef HAVE_SETLOCALE
  setlocale(LC_ALL, "");
#endif

  loutput.cpubind_set = hwloc_bitmap_alloc();
  loutput.membind_set = hwloc_bitmap_alloc();
  if (!loutput.cpubind_set || !loutput.membind_set)
    goto out;

  while (argc >= 1)
    {
      opt = 0;
      if (!strcmp (argv[0], "-v") || !strcmp (argv[0], "--verbose")) {
	loutput.verbose_mode++;
      } else if (!strcmp (argv[0], "-q") || !strcmp (argv[0], "--quiet")
                 || !strcmp (argv[0], "-s") || !strcmp (argv[0], "--silent")) {
	loutput.verbose_mode--;
      } else if (!strcmp (argv[0], "--distances")) {
	loutput.show_distances_only = 1;
      } else if (!strcmp (argv[0], "--distances-transform")) {
	if (argc < 2)
	  goto out_usagefailure;
        if (!strcmp (argv[1], "links"))
          loutput.transform_distances = HWLOC_DISTANCES_TRANSFORM_LINKS;
        else if (!strcmp (argv[1], "merge-switch-ports"))
          loutput.transform_distances = HWLOC_DISTANCES_TRANSFORM_MERGE_SWITCH_PORTS;
        else if (!strcmp (argv[1], "transitive-closure"))
          loutput.transform_distances = HWLOC_DISTANCES_TRANSFORM_TRANSITIVE_CLOSURE;
        else {
          fprintf(stderr, "Unrecognized argument `%s' passed to --distances-transform\n", argv[1]);
          goto out_usagefailure;
        }
        opt = 1;
      } else if (!strcmp (argv[0], "--memattrs")) {
        loutput.show_memattrs_only = 1;
      } else if (!strcmp (argv[0], "--cpukinds")) {
        loutput.show_cpukinds_only = 1;
#ifdef HWLOC_WIN_SYS
      } else if (!strcmp (argv[0], "--windows-processor-groups")) {
        loutput.show_windows_processor_groups_only = 1;
#endif
      } else if (!strcmp (argv[0], "-h") || !strcmp (argv[0], "--help")) {
	usage(callname, stdout);
        exit(EXIT_SUCCESS);
      } else if (!strcmp (argv[0], "-f") || !strcmp (argv[0], "--force"))
	loutput.overwrite = 1;
      else if (!strcmp (argv[0], "-l") || !strcmp (argv[0], "--logical"))
	loutput.index_type = LSTOPO_INDEX_TYPE_LOGICAL;
      else if (!strcmp (argv[0], "-p") || !strcmp (argv[0], "--physical"))
	loutput.index_type = LSTOPO_INDEX_TYPE_PHYSICAL;
      else if (!strcmp (argv[0], "-c") || !strcmp (argv[0], "--cpuset"))
	loutput.show_cpuset = 1;
      else if (!strcmp (argv[0], "-C") || !strcmp (argv[0], "--cpuset-only"))
	loutput.show_cpuset = 2;
      else if (!strcmp(argv[0], "--cpuset-output-format") || !strcmp(argv[0], "--cof")) {
        if (argc < 2) {
          usage (callname, stderr);
          exit(EXIT_FAILURE);
        }
        loutput.cpuset_output_format = hwloc_utils_parse_cpuset_format(argv[1]);
        if (HWLOC_UTILS_CPUSET_FORMAT_UNKNOWN == loutput.cpuset_output_format) {
          fprintf(stderr, "Unrecognized %s argument %s\n", argv[0], argv[1]);
          goto out_usagefailure;
        }
        if (!loutput.show_cpuset)
	  loutput.show_cpuset = 1;
        opt = 1;
      }
      else if (!strcmp (argv[0], "--taskset")) {
	loutput.cpuset_output_format = HWLOC_UTILS_CPUSET_FORMAT_TASKSET;
	if (!loutput.show_cpuset)
	  loutput.show_cpuset = 1;
      } else if (!strcmp (argv[0], "--only")) {
	if (argc < 2)
	  goto out_usagefailure;
        show_only_string = argv[1];
	opt = 1;
      }
      else if (!strcmp (argv[0], "--filter")) {
	hwloc_obj_type_t type = HWLOC_OBJ_TYPE_NONE;
	char *colon;
	enum hwloc_type_filter_e filter = HWLOC_TYPE_FILTER_KEEP_ALL;
	int all = 0;
	int allio = 0;
	int allcaches = 0;
	int allicaches = 0;
	if (argc < 2)
	  goto out_usagefailure;
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
	    goto out_usagefailure;
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
	  goto out_usagefailure;
	}
	if (type == HWLOC_OBJ_PU) {
	  if (filter == HWLOC_TYPE_FILTER_KEEP_NONE)
	    loutput.ignore_pus = 1;
	}
	else if (type == HWLOC_OBJ_NUMANODE) {
	  if (filter == HWLOC_TYPE_FILTER_KEEP_NONE)
	    loutput.ignore_numanodes = 1;
	}
	else if (all)
	  set_all_types_filter(filter);
	else if (allio)
	  set_io_types_filter(filter);
	else if (allcaches) {
	  set_cache_types_filter(filter);
	  set_type_filter(HWLOC_OBJ_MEMCACHE, filter);
	} else if (allicaches)
	  set_icache_types_filter(filter);
	else
	  set_type_filter(type, filter);
	opt = 1;
      }
      else if (!strcmp (argv[0], "--ignore")) {
	hwloc_obj_type_t type;
	if (argc < 2)
	  goto out_usagefailure;
	if (!strcasecmp(argv[1], "cache")) {
	  fprintf(stderr, "--ignore Cache not supported anymore, use --no-caches instead.\n");
	  goto out_usagefailure;
	}
	if (hwloc_type_sscanf(argv[1], &type, NULL, 0) < 0)
	  fprintf(stderr, "Unsupported type `%s' passed to --ignore, ignoring.\n", argv[1]);
	else if (type == HWLOC_OBJ_PU)
	  loutput.ignore_pus = 1;
	else if (type == HWLOC_OBJ_NUMANODE)
	  loutput.ignore_numanodes = 1;
	else
          set_type_filter(type, HWLOC_TYPE_FILTER_KEEP_NONE);
	opt = 1;
      }
      else if (!strcmp (argv[0], "--no-smt")) {
	loutput.ignore_pus = 1;
      }
      else if (!strcmp (argv[0], "--no-caches")) {
        set_cache_types_filter(HWLOC_TYPE_FILTER_KEEP_NONE);
        set_type_filter(HWLOC_OBJ_MEMCACHE, HWLOC_TYPE_FILTER_KEEP_NONE);
      }
      else if (!strcmp (argv[0], "--no-useless-caches")) {
        set_cache_types_filter(HWLOC_TYPE_FILTER_KEEP_STRUCTURE);
        set_type_filter(HWLOC_OBJ_MEMCACHE, HWLOC_TYPE_FILTER_KEEP_STRUCTURE);
      }
      else if (!strcmp (argv[0], "--no-icaches")) {
	set_icache_types_filter(HWLOC_TYPE_FILTER_KEEP_NONE);
      }
      else if (!strcmp (argv[0], "--disallowed") || !strcmp (argv[0], "--whole-system"))
	flags |= HWLOC_TOPOLOGY_FLAG_INCLUDE_DISALLOWED;
      else if (!strcmp (argv[0], "--allow")) {
	if (argc < 2)
	  goto out_usagefailure;
	if (!strcmp(argv[1], "all")) {
	  allow_flags = HWLOC_ALLOW_FLAG_ALL;
	} else if (!strcmp(argv[1], "local")) {
	  allow_flags = HWLOC_ALLOW_FLAG_LOCAL_RESTRICTIONS;
	  flags |= HWLOC_TOPOLOGY_FLAG_IS_THISSYSTEM;
	} else {
	  hwloc_bitmap_t set = hwloc_bitmap_alloc();
	  const char *begin = argv[1];
	  if (!strncmp(begin, "nodeset=", 8))
	    begin += 8;
	  hwloc_bitmap_sscanf(set, begin);
	  if (begin == argv[1])
	    allow_cpuset = set;
	  else
	    allow_nodeset = set;
	  allow_flags = HWLOC_ALLOW_FLAG_CUSTOM;
	}
	opt = 1;
	flags |= HWLOC_TOPOLOGY_FLAG_INCLUDE_DISALLOWED;

      } else if (!strcmp (argv[0], "--no-io")) {
	set_io_types_filter(HWLOC_TYPE_FILTER_KEEP_NONE);
      } else if (!strcmp (argv[0], "--no-bridges")) {
	set_type_filter(HWLOC_OBJ_BRIDGE, HWLOC_TYPE_FILTER_KEEP_NONE);
      } else if (!strcmp (argv[0], "--whole-io")) {
	set_io_types_filter(HWLOC_TYPE_FILTER_KEEP_ALL);
      } else if (!strcmp (argv[0], "--merge")) {
	set_all_types_filter(HWLOC_TYPE_FILTER_KEEP_STRUCTURE);
      }
      else if (!strcmp (argv[0], "--no-collapse"))
	loutput.pci_collapse_enabled = 0;

      else if (!strcmp (argv[0], "--no-factorize")) {
	for(i=HWLOC_OBJ_TYPE_MIN; i<HWLOC_OBJ_TYPE_MAX; i++)
	  loutput.factorize_min[i] = FACTORIZE_MIN_DISABLED;
      }
      else if (!strncmp (argv[0], "--no-factorize=", 15)) {
	hwloc_obj_type_t type;
	const char *tmp = argv[0]+15;
	if (hwloc_type_sscanf(tmp, &type, NULL, 0) < 0) {
	  fprintf(stderr, "Unsupported parameter `%s' passed to %s, ignoring.\n", tmp, argv[0]);
	  goto out_usagefailure;
	}
	loutput.factorize_min[type] = FACTORIZE_MIN_DISABLED;
      }
      else if (!strcmp (argv[0], "--factorize")) {
	for(i=HWLOC_OBJ_TYPE_MIN; i<HWLOC_OBJ_TYPE_MAX; i++)
	  loutput.factorize_min[i] = FACTORIZE_MIN_DEFAULT;
	lstopo_update_factorize_alltypes_bounds(&loutput);
      }
      else if (!strncmp (argv[0], "--factorize=", 12)) {
	hwloc_obj_type_t type, type_min, type_max;
	unsigned min, first, last;
	const char *tmp = argv[0]+12;
	const char *sep1, *sep2, *sep3;

	if (*tmp < '0' || *tmp > '9') {
	  if (hwloc_type_sscanf(tmp, &type, NULL, 0) < 0) {
	    fprintf(stderr, "Unsupported type `%s' passed to %s, ignoring.\n", tmp, argv[0]);
	    goto out_usagefailure;
	  }
	  type_min = type;
	  type_max = type+1;
	  sep1 = strchr(tmp, ',');
	} else {
	  type_min = HWLOC_OBJ_TYPE_MIN;
	  type_max = HWLOC_OBJ_TYPE_MAX;
	  sep1 = tmp-1;
	}

	if (!sep1) {
	  min = FACTORIZE_MIN_DEFAULT;
	  lstopo_update_factorize_bounds(min, &first, &last);
	} else {
	  min = atoi(sep1+1);
	  lstopo_update_factorize_bounds(min, &first, &last);
	  sep2 = strchr(sep1+1, ',');
	  if (sep2) {
	    first = atoi(sep2+1);
	    sep3 = strchr(sep2+1, ',');
	    if (sep3)
	      last = atoi(sep3+1);
	  }
	}

	for(i=type_min; i<(unsigned)type_max; i++) {
	  loutput.factorize_min[i] = min;
	  loutput.factorize_first[i] = first;
	  loutput.factorize_last[i] = last;
	}
      }

      else if (!strcmp (argv[0], "--thissystem"))
	flags |= HWLOC_TOPOLOGY_FLAG_IS_THISSYSTEM;
      else if (!strcmp (argv[0], "--flags")) {
	if (argc < 2)
	  goto out_usagefailure;
	flags = hwloc_utils_parse_topology_flags(argv[1]);
        if(flags == (unsigned long)-1)
          goto out;
	opt = 1;
      }
      else if (!strcmp (argv[0], "--restrict")) {
	if (argc < 2)
	  goto out_usagefailure;
        if(strncmp(argv[1], "nodeset=", 8)) {
          restrictstring = strdup(argv[1]);
        } else {
          restrictstring = strdup(argv[1]+8);
          restrict_flags |= HWLOC_RESTRICT_FLAG_BYNODESET;
        }
	opt = 1;
      }
      else if (!strcmp (argv[0], "--restrict-flags")) {
	if (argc < 2)
	  goto out_usagefailure;
	restrict_flags = hwloc_utils_parse_restrict_flags(argv[1]);
        if(restrict_flags == (unsigned long)-1)
          goto out;
	opt = 1;
      }
      else if (!strcmp (argv[0], "--export-xml-flags")) {
	if (argc < 2)
	  goto out_usagefailure;
	loutput.export_xml_flags = hwloc_utils_parse_export_xml_flags(argv[1]);
        if(loutput.export_xml_flags == (unsigned long)-1)
          goto out;
	opt = 1;
      }
      else if (!strcmp (argv[0], "--export-synthetic-flags")) {
	if (argc < 2)
	  goto out_usagefailure;
	loutput.export_synthetic_flags = hwloc_utils_parse_export_synthetic_flags(argv[1]);
        if(loutput.export_synthetic_flags == (unsigned long)-1)
          goto out;
	opt = 1;
      }
      else if (!strcmp (argv[0], "--horiz"))
	for(i=HWLOC_OBJ_TYPE_MIN; i<HWLOC_OBJ_TYPE_MAX; i++)
	  loutput.force_orient[i] = LSTOPO_ORIENT_HORIZ;
      else if (!strcmp (argv[0], "--vert"))
	for(i=HWLOC_OBJ_TYPE_MIN; i<HWLOC_OBJ_TYPE_MAX; i++)
	  loutput.force_orient[i] = LSTOPO_ORIENT_VERT;
      else if (!strcmp (argv[0], "--rect"))
	for(i=HWLOC_OBJ_TYPE_MIN; i<HWLOC_OBJ_TYPE_MAX; i++)
	  loutput.force_orient[i] = LSTOPO_ORIENT_RECT;
      else if (!strncmp (argv[0], "--horiz=", 8)
	       || !strncmp (argv[0], "--vert=", 7)
	       || !strncmp (argv[0], "--rect=", 7)) {
	enum lstopo_orient_e orient = (argv[0][2] == 'h') ? LSTOPO_ORIENT_HORIZ : (argv[0][2] == 'v') ? LSTOPO_ORIENT_VERT : LSTOPO_ORIENT_RECT;
	char *tmp = argv[0] + ((argv[0][2] == 'h') ? 8 : 7);
	while (tmp) {
	  char *end = strchr(tmp, ',');
	  hwloc_obj_type_t type;
	  if (end)
	    *end = '\0';
	  if (hwloc_type_sscanf(tmp, &type, NULL, 0) < 0)
	    fprintf(stderr, "Unsupported type `%s' passed to %s, ignoring.\n", tmp, argv[0]);
	  else
	    loutput.force_orient[type] = orient;
	  if (!end)
	    break;
	  tmp = end+1;
        }
      }

      else if (!strcmp (argv[0], "--grey") || !strcmp (argv[0], "--greyscale"))
        lstopo_palette_select(&loutput, argv[0]+2);

      else if (!strcmp (argv[0], "--palette")) {
        char *equal;
	if (argc < 2)
	  goto out_usagefailure;
        equal = strchr(argv[1], '=');
        if (equal) {
          if (equal[1] != '#')
            fprintf(stderr, "Unsupported palette color modification `%s' passed to %s, ignoring.\n", argv[1], argv[0]);
          else {
            *equal = '\0';
            lstopo_palette_set_color_by_name(&loutput, argv[1], strtoul(equal+2, NULL, 16));
          }
        } else {
          lstopo_palette_select(&loutput, argv[1]);
        }
	opt = 1;
      }
      else if (!strcmp (argv[0], "--binding-color")) {
	if (argc < 2)
	  goto out_usagefailure;
	if (!strcmp(argv[1], "none"))
	  loutput.show_binding = 0;
        else if (*argv[1] == '#')
          lstopo_palette_set_color(&loutput.palette->binding, strtoul(argv[1]+1, NULL, 16));
	else
	  fprintf(stderr, "Unsupported color `%s' passed to %s, ignoring.\n", argv[1], argv[0]);
	opt = 1;
      }
      else if (!strcmp (argv[0], "--disallowed-color")) {
	if (argc < 2)
	  goto out_usagefailure;
	if (!strcmp(argv[1], "none"))
	  loutput.show_disallowed = 0;
        else if (*argv[1] == '#')
          lstopo_palette_set_color(&loutput.palette->disallowed, strtoul(argv[1]+1, NULL, 16));
        else
	  fprintf(stderr, "Unsupported color `%s' passed to %s, ignoring.\n", argv[1], argv[0]);
	opt = 1;
      }
      else if (!strcmp (argv[0], "--top-color")) {
	if (argc < 2)
	  goto out_usagefailure;
        if (!strcmp(argv[1], "none"))
          loutput.show_process_color = 0;
        else if (*argv[1] == '#')
          lstopo_palette_set_color(&loutput.palette->process, strtoul(argv[1]+1, NULL, 16));
        else
	  fprintf(stderr, "Unsupported color `%s' passed to %s, ignoring.\n", argv[1], argv[0]);
        opt = 1;
      }
      else if (!strcmp(argv[0], "--os-index-prefix")) {
	if (argc < 2)
	  goto out_usagefailure;
        loutput.os_index_prefix = argv[1];
        opt = 1;
      }
      else if (!strcmp(argv[0], "--logical-index-prefix")) {
	if (argc < 2)
	  goto out_usagefailure;
        loutput.logical_index_prefix = argv[1];
        opt = 1;
      }
      else if (!strncmp (argv[0], "--no-text", 9)
	       || !strncmp (argv[0], "--text", 6)
	       || !strncmp (argv[0], "--no-index", 10)
	       || !strncmp (argv[0], "--index", 7)
	       || !strncmp (argv[0], "--no-attrs", 10)
	       || !strncmp (argv[0], "--attrs", 7)) {
	int enable = argv[0][2] != 'n';
	const char *kind = enable ? argv[0]+2 : argv[0]+5;
	const char *end;
	int *array;
	if (*kind == 't') {
	  array = loutput.show_text;
	  end = kind+4;
	} else if (*kind == 'a') {
	  array = loutput.show_attrs;
	  end = kind+5;
	} else if (*kind == 'i') {
	  array = loutput.show_indexes;
	  end = kind+5;
	} else {
	  abort();
	}
	if (!*end) {
	  for(i=HWLOC_OBJ_TYPE_MIN; i<HWLOC_OBJ_TYPE_MAX; i++)
	    array[i] = enable;

	} else if (*end == '=') {
	  const char *tmp = end+1;
	  while (tmp) {
	    char *sep = strchr(tmp, ',');
	    hwloc_obj_type_t type;
	    if (sep)
	      *sep = '\0';
	    if (hwloc_type_sscanf(tmp, &type, NULL, 0) < 0)
	      if (!hwloc_strncasecmp(tmp, "cache", 5)) {
		for(i=HWLOC_OBJ_TYPE_MIN; i<HWLOC_OBJ_TYPE_MAX; i++)
		  if (hwloc_obj_type_is_cache(i))
		    array[i] = enable;
	      } else if (!hwloc_strncasecmp(tmp, "io", 2)) {
		for(i=HWLOC_OBJ_TYPE_MIN; i<HWLOC_OBJ_TYPE_MAX; i++)
		  if (hwloc_obj_type_is_io(i))
		    array[i] = enable;
	      } else
		fprintf(stderr, "Unsupported type `%s' passed to %s, ignoring.\n", tmp, argv[0]);
	    else
	      array[type] = enable;
	    if (!sep)
	      break;
	    tmp = sep+1;
	  }

	} else {
	  fprintf(stderr, "Unexpected character %c in option %s\n", *end, argv[0]);
	  goto out_usagefailure;
	}
      }

      else if (!strcmp (argv[0], "--children-order")) {
	if (argc < 2)
	  goto out_usagefailure;
        lstopo_parse_children_order(argv[1], &loutput.children_order,
                                    &loutput.above_force_orient, &loutput.right_force_orient, &loutput.below_force_orient);
	opt = 1;
      }
      else if (!strcmp (argv[0], "--no-cpukinds")) {
        loutput.show_cpukinds = 0;
      }
      else if (!strcmp (argv[0], "--fontsize")) {
	if (argc < 2)
	  goto out_usagefailure;
	loutput.fontsize = atoi(argv[1]);
	opt = 1;
      }
      else if (!strcmp (argv[0], "--gridsize")) {
	if (argc < 2)
	  goto out_usagefailure;
	loutput.gridsize = atoi(argv[1]);
	opt = 1;
      }
      else if (!strcmp (argv[0], "--linespacing")) {
	if (argc < 2)
	  goto out_usagefailure;
	loutput.linespacing = atoi(argv[1]);
	opt = 1;
      }
      else if (!strcmp (argv[0], "--thickness")) {
	if (argc < 2)
	  goto out_usagefailure;
	loutput.thickness = atoi(argv[1]);
	opt = 1;
      }
      else if (!strcmp (argv[0], "--no-legend")) {
	loutput.show_legend = LSTOPO_SHOW_LEGEND_NONE;
      }
      else if (!strcmp (argv[0], "--no-default-legend")) {
	loutput.show_legend = LSTOPO_SHOW_LEGEND_NO_DEFAULT;
      }
      else if (!strcmp (argv[0], "--append-legend")) {
	char **tmp;
	if (argc < 2)
	  goto out_usagefailure;
	tmp = realloc(loutput.legend_append, (loutput.legend_append_nr+1) * sizeof(*loutput.legend_append));
	if (!tmp) {
	  fprintf(stderr, "Failed to realloc legend append array, legend ignored.\n");
	} else {
	  loutput.legend_append = tmp;
	  loutput.legend_append[loutput.legend_append_nr] = strdup(argv[1]);
	  loutput.legend_append_nr++;
	}
	opt = 1;
      }

      else if (!strcmp (argv[0], "--shmem-output-addr")) {
	if (argc < 2)
	  goto out_usagefailure;
	loutput.shmem_output_addr = strtoull(argv[1], NULL, 0);
	opt = 1;
      }

      else if (hwloc_utils_lookup_input_option(argv, argc, &opt,
					       &input, &input_format,
					       callname)) {
	/* nothing to do anymore */

      } else if (!strcmp (argv[0], "--pid")) {
	if (argc < 2)
	  goto out_usagefailure;
	loutput.pid_number = atoi(argv[1]); opt = 1;
      } else if (!strcmp (argv[0], "--ps") || !strcmp (argv[0], "--top"))
        top = 1;
      else if (!strcmp (argv[0], "--misc-from")) {
	if (argc < 2)
	  goto out_usagefailure;
        if (!strcmp(argv[1], "-"))
          miscfrom = stdin;
        else
          miscfrom = fopen(argv[1], "r");
        if (!miscfrom) {
          fprintf(stderr, "Failed open --misc-from %s file for reading (%s)\n", argv[1], strerror(errno));
          exit(EXIT_FAILURE);
        }
        opt = 1;
      } else if (!strcmp (argv[0], "--version")) {
          printf("%s %s\n", callname, HWLOC_VERSION);
          exit(EXIT_SUCCESS);
      } else if (!strcmp (argv[0], "--output-format") || !strcmp (argv[0], "--of")) {
	if (argc < 2)
	  goto out_usagefailure;
        output_format = parse_output_format(argv[1], callname);
        opt = 1;
      } else {
	if (filename) {
	  fprintf (stderr, "Unrecognized option `%s', cannot be used as output filename (`%s' already given).\n",
                   argv[0], filename);
	  goto out_usagefailure;
	} else
	  filename = argv[0];
      }
      argc -= opt+1;
      argv += opt+1;
    }

  if (!loutput.fontsize) {
    for(i=HWLOC_OBJ_TYPE_MIN; i<HWLOC_OBJ_TYPE_MAX; i++)
      loutput.show_text[i] = 0;
    loutput.show_legend = LSTOPO_SHOW_LEGEND_NONE;
  }

  /***********************
   * Configure the output
   */

  /* if output is -.format but --of was given, ignore .format */
  if (output_format != LSTOPO_OUTPUT_DEFAULT
      && filename && filename[0] == '-' && filename[1] == '.') {
    fprintf(stderr, "Ignoring extension in stdout output `%s' since --of was also given format `%s'.\n",
            filename, output_format_name(output_format));
    filename = "-"; /* to simplify things later */
  }

  /* if the output format wasn't enforced, look at the filename */
  if (filename && output_format == LSTOPO_OUTPUT_DEFAULT) {
    if (!strcmp(filename, "-")
	|| !strcmp(filename, "/dev/stdout")) {
      output_format = LSTOPO_OUTPUT_CONSOLE;
      filename = "-"; /* to simplify things later */
    } else {
      char *dot = strrchr(filename, '.');
      if (dot) {
        output_format = parse_output_format(dot+1, callname);
        if (dot == filename+1 && filename[0] == '-' && output_format != LSTOPO_OUTPUT_ERROR)
          filename = "-"; /* to simplify things later */
      } else {
	fprintf(stderr, "Cannot infer output type for file `%s' without any extension, using default output.\n", filename);
	filename = NULL;
      }
    }
  }
  if (output_format == LSTOPO_OUTPUT_ERROR)
    goto out_usagefailure;

  /* if  the output format wasn't enforced, think a bit about what the user probably want */
  if (output_format == LSTOPO_OUTPUT_DEFAULT) {
    if (loutput.show_cpuset
        || show_only_string
	|| loutput.show_distances_only
        || loutput.show_memattrs_only
        || loutput.show_cpukinds_only
        || loutput.show_windows_processor_groups_only
        || loutput.verbose_mode != LSTOPO_VERBOSE_MODE_DEFAULT)
      output_format = LSTOPO_OUTPUT_CONSOLE;
  }

  switch (output_format) {
  case LSTOPO_OUTPUT_DEFAULT: {
#ifdef LSTOPO_HAVE_GRAPHICS
#if (defined LSTOPO_HAVE_X11)
    int want_console = 0;
#if (defined HAVE_ISATTY) && (defined HAVE_TCGETPGRP)
    /* If stdout isn't a tty, we're likely redirecting stdout, use console mode.
     * However, if launched from the window manager, we still want graphical mode.
     * tcgetpgrp(STDIN) should fail with ENOTTY in this case.
     * We don't specifically check for errno==ENOTTY: if tcgetpgrp() ever fails
     * for another reason, don't assume we're redirected, keep the graphical mode too.
     */
    if (!isatty(STDOUT_FILENO) && tcgetpgrp(STDIN_FILENO) != -1)
      want_console = 1;
#endif
    if (getenv("DISPLAY") && !want_console) {
      output_func = output_x11;
      output_format = LSTOPO_OUTPUT_WINDOW;
    } else
#endif /* LSTOPO_HAVE_X11 */
#ifdef HWLOC_WIN_SYS
    {
      output_func = output_windows;
      output_format = LSTOPO_OUTPUT_WINDOW;
    }
#endif
#endif /* !LSTOPO_HAVE_GRAPHICS */
#if !defined HWLOC_WIN_SYS || !defined LSTOPO_HAVE_GRAPHICS
    {
      output_func = output_console;
      output_format = LSTOPO_OUTPUT_CONSOLE;
    }
#endif
#ifdef ANDROID
    setJNIEnv();
    output_func = output_android;
    output_format = LSTOPO_OUTPUT_WINDOW;
#endif
    }
    break;

  case LSTOPO_OUTPUT_WINDOW:
#if (defined LSTOPO_HAVE_GRAPHICS) && (defined LSTOPO_HAVE_X11)
    if (getenv("DISPLAY")) {
      output_func = output_x11;
    } else {
      fprintf(stderr, "X11 graphical window output requires a DISPLAY environment variable.\n");
      goto out;
    }
#elif (defined LSTOPO_HAVE_GRAPHICS) && (defined HWLOC_WIN_SYS)
    output_func = output_windows;
#else
    fprintf(stderr, "Graphical window output not supported.\n");
    goto out;
#endif
    break;

  case LSTOPO_OUTPUT_CONSOLE:
    output_func = output_console;
    break;
  case LSTOPO_OUTPUT_SYNTHETIC:
    output_func = output_synthetic;
    break;
  case LSTOPO_OUTPUT_ASCII:
    output_func = output_ascii;
    break;
  case LSTOPO_OUTPUT_TIKZ:
    output_func = output_tikz;
    break;
  case LSTOPO_OUTPUT_FIG:
    output_func = output_fig;
    break;
#ifdef LSTOPO_HAVE_GRAPHICS
# ifdef CAIRO_HAS_PNG_FUNCTIONS
  case LSTOPO_OUTPUT_PNG:
    output_func = output_png;
    break;
# endif /* CAIRO_HAS_PNG_FUNCTIONS */
# ifdef CAIRO_HAS_PDF_SURFACE
  case LSTOPO_OUTPUT_PDF:
    output_func = output_pdf;
    break;
# endif /* CAIRO_HAS_PDF_SURFACE */
# ifdef CAIRO_HAS_PS_SURFACE
  case LSTOPO_OUTPUT_PS:
    output_func = output_ps;
    break;
# endif /* CAIRO_HAS_PS_SURFACE */
# ifdef CAIRO_HAS_SVG_SURFACE
  case LSTOPO_OUTPUT_SVG:
  case LSTOPO_OUTPUT_CAIROSVG:
    output_func = output_cairosvg;
    output_format = LSTOPO_OUTPUT_CAIROSVG;
    break;
# endif /* CAIRO_HAS_SVG_SURFACE */
#endif /* LSTOPO_HAVE_GRAPHICS */
#if !(defined LSTOPO_HAVE_GRAPHICS) || !(defined CAIRO_HAS_SVG_SURFACE)
  case LSTOPO_OUTPUT_SVG:
#endif
  case LSTOPO_OUTPUT_NATIVESVG:
    output_func = output_nativesvg;
    output_format = LSTOPO_OUTPUT_NATIVESVG;
    break;
  case LSTOPO_OUTPUT_XML:
    output_func = output_xml;
    break;
#ifndef HWLOC_WIN_SYS
  case LSTOPO_OUTPUT_SHMEM:
    output_func = output_shmem;
    break;
#endif
  default:
    fprintf(stderr, "file format not supported\n");
    goto out_usagefailure;
  }

 refresh:
  loutput.needs_topology_refresh = 0;

  /*************************
   * Configure the topology
   */

  err = hwloc_topology_init (&topology);
  if (err)
    goto out;
  hwloc_topology_set_all_types_filter(topology, HWLOC_TYPE_FILTER_KEEP_ALL);
  hwloc_topology_set_io_types_filter(topology, HWLOC_TYPE_FILTER_KEEP_IMPORTANT);

  err = hwloc_topology_set_flags(topology, flags);
  if (err < 0) {
    fprintf(stderr, "Failed to set flags 0x%lx (%s).\n", flags, strerror(errno));
    goto out_with_topology;
  }

  if (input) {
    err = hwloc_utils_enable_input_format(topology, flags, input, &input_format, loutput.verbose_mode > 1, callname);
    if (err)
      goto out_with_topology;

    if (input_format.format != HWLOC_UTILS_INPUT_DEFAULT) {
      /* add the input path to the window title */
      snprintf(loutput.title, sizeof(loutput.title), "lstopo - %s", input);

#ifndef HWLOC_WIN_SYS
      /* try to only add the last part of the input path to the window title.
       * disabled on windows because it requires to deal with / or \ in both cygwin and native paths.
       * looks like _fullpath() is good way to replace realpath() on !cygwin.
       */
      /* sanitize the path to avoid / ./ or ../ at the end */
      char *fullpath = realpath(input, NULL);
      if (fullpath) {
	char *pos = strrchr(fullpath, '/');
	/* now only keep the last part */
	if (pos)
	  pos++;
	else
	  pos = fullpath;
	snprintf(loutput.title, sizeof(loutput.title), "lstopo - %s", pos);
	free(fullpath);
      }
#endif
    }
  }

  if (loutput.pid_number > 0) {
    if (hwloc_pid_from_number(&loutput.pid, loutput.pid_number, 0, 1 /* verbose */) < 0
	|| hwloc_topology_set_pid(topology, loutput.pid)) {
      perror("Setting target pid");
      goto out_with_topology;
    }
  }

  if (input_format.format == HWLOC_UTILS_INPUT_XML
      && output_format == LSTOPO_OUTPUT_XML) {
    /* must be after parsing output format and before loading the topology */
    putenv((char *) "HWLOC_XML_USERDATA_NOT_DECODED=1");
    hwloc_topology_set_userdata_import_callback(topology, hwloc_utils_userdata_import_cb);
    hwloc_topology_set_userdata_export_callback(topology, hwloc_utils_userdata_export_cb);
  }

  apply_type_filters(topology);

  /*********************
   * Build the topology
   */

#ifdef HAVE_CLOCK_GETTIME
  if (measure_load_time)
    clock_gettime(CLOCK_MONOTONIC, &ts1);
#endif

  if (input_format.format == HWLOC_UTILS_INPUT_SHMEM) {
#ifdef HWLOC_WIN_SYS
    fprintf(stderr, "shmem topology not supported\n"); /* this line must match the grep line in test-lstopo-shmem */
    goto out_with_topology;
#else /* !HWLOC_WIN_SYS */
    /* load from shmem, and duplicate onto topology, so that we may modify it */
    hwloc_topology_destroy(topology);
    err = lstopo_shmem_adopt(input, &topology);
    if (err < 0)
      goto out;
    hwloc_utils_userdata_clear_recursive(hwloc_get_root_obj(topology));
#endif /* !HWLOC_WIN_SYS */

  } else {
    /* normal load */
    err = hwloc_topology_load (topology);
    if (err) {
      fprintf(stderr, "hwloc_topology_load() failed (%s).\n", strerror(errno));
      goto out_with_topology;
    }
  }

#ifdef HAVE_CLOCK_GETTIME
  if (measure_load_time) {
    clock_gettime(CLOCK_MONOTONIC, &ts2);
    ms = (ts2.tv_nsec-ts1.tv_nsec)/1000000+(ts2.tv_sec-ts1.tv_sec)*1000UL;
    printf("hwloc_topology_load() took %lu ms\n", ms);
  }
#endif

  /********************************
   * Clean input format's internal variables
   */

  if (input) {
    hwloc_utils_disable_input_format(&input_format);
  }

  /********************************
   * Tweak the topology and output
   */

  if (allow_flags) {
    if (allow_flags == HWLOC_ALLOW_FLAG_CUSTOM) {
      err = hwloc_topology_allow(topology, allow_cpuset, allow_nodeset, HWLOC_ALLOW_FLAG_CUSTOM);
    } else {
      err = hwloc_topology_allow(topology, NULL, NULL, allow_flags);
    }
    if (err < 0) {
      fprintf(stderr, "hwloc_topology_allow() failed (%s)\n", strerror(errno));
      goto out_with_topology;
    }
  }

  hwloc_bitmap_fill(loutput.cpubind_set);
  if (loutput.pid_number != -1 && loutput.pid_number != 0)
    hwloc_get_proc_cpubind(topology, loutput.pid, loutput.cpubind_set, 0);
  else
    /* get our binding even if --pid not given, it may be used by --restrict */
    hwloc_get_cpubind(topology, loutput.cpubind_set, 0);

  hwloc_bitmap_fill(loutput.membind_set);
  if (loutput.pid_number != -1 && loutput.pid_number != 0)
    hwloc_get_proc_membind(topology, loutput.pid, loutput.membind_set, &policy, HWLOC_MEMBIND_BYNODESET);
  else
    /* get our binding even if --pid not given, it may be used by --restrict */
    hwloc_get_membind(topology, loutput.membind_set, &policy, HWLOC_MEMBIND_BYNODESET);

  loutput.need_pci_domain = lstopo_check_pci_domains(topology);

  if (top)
    add_process_objects(topology);
  if (miscfrom)
    add_misc_objects_from(topology, miscfrom);

  if (restrictstring) {
    hwloc_bitmap_t restrictset = hwloc_bitmap_alloc();
    if (!strcmp (restrictstring, "binding")) {
      hwloc_bitmap_copy(restrictset, loutput.cpubind_set);
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

  loutput.topology = topology;
  loutput.depth = hwloc_topology_get_depth(topology);
  loutput.file = NULL;

  if (filename && output_format == LSTOPO_OUTPUT_WINDOW) {
    fprintf(stderr, "Output filename `%s' ignored when using graphical window output.\n",
            filename);
  }

#ifdef HAVE_ISATTY
  /* if exporting to a file (which is not stdout, rewritten as "-" above),
   * and stdout is a terminal,
   * show a message about the export filename and format.
   */
  if (filename && strcmp(filename, "-") && output_format != LSTOPO_OUTPUT_WINDOW && isatty(STDOUT_FILENO))
    printf("Exporting format `%s' to file `%s'\n", output_format_name(output_format), filename);
#endif

  if (output_format != LSTOPO_OUTPUT_XML) {
    /* there might be some xml-imported userdata in objects, add lstopo-specific userdata in front of them */
    lstopo_populate_userdata(hwloc_get_root_obj(topology));
    lstopo_add_cpukind_style(&loutput, topology);
    /* cpukinds must be before factorizing */
    lstopo_add_factorized_attributes(&loutput, topology, hwloc_get_root_obj(topology));
    lstopo_add_collapse_attributes(topology);
  }

  loutput.show_only.depth = HWLOC_TYPE_DEPTH_UNKNOWN; /* disable this feature by default */
  if (show_only_string) {
    err = hwloc_calc_parse_level(NULL, topology, show_only_string, strlen(show_only_string), &loutput.show_only);
    if (err < 0 && loutput.show_only.depth == HWLOC_TYPE_DEPTH_UNKNOWN) {
      fprintf(stderr, "level %s passed to --only is unavailable.\n", show_only_string);
      goto out_with_topology;
    }
    /* multiple depth is ok */
  }

  /******************
   * Output for real
   */
  err = output_func(&loutput, filename);

  if (output_format != LSTOPO_OUTPUT_XML) {
    /* remove lstopo-specific userdata in front of the list of userdata */
    lstopo_destroy_userdata(hwloc_get_root_obj(topology));
  }
  /* remove the remaining lists of xml-imported userdata */
  hwloc_utils_userdata_free_recursive(hwloc_get_root_obj(topology));

  hwloc_topology_destroy (topology);

  if (loutput.needs_topology_refresh) {
    loutput.refreshing = 1;
    goto refresh;
  }

  for(i=0; i<loutput.legend_append_nr; i++)
    free(loutput.legend_append[i]);
  free(loutput.legend_append);

  hwloc_bitmap_free(loutput.cpubind_set);
  hwloc_bitmap_free(loutput.membind_set);

  return err ? EXIT_FAILURE : EXIT_SUCCESS;

 out_usagefailure:
  usage (callname, stderr);
  goto out;

 out_with_topology:
  lstopo_destroy_userdata(hwloc_get_root_obj(topology));
  hwloc_topology_destroy(topology);
 out:
  if (input) hwloc_utils_disable_input_format(&input_format);
  hwloc_bitmap_free(allow_cpuset);
  hwloc_bitmap_free(allow_nodeset);
  hwloc_bitmap_free(loutput.cpubind_set);
  hwloc_bitmap_free(loutput.membind_set);
  if (miscfrom && miscfrom != stdin)
    fclose(miscfrom);
  return EXIT_FAILURE;
}
