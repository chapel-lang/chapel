/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2014 Inria.  All rights reserved.
 * Copyright © 2009-2010, 2012 Université Bordeaux 1
 * Copyright © 2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#ifndef UTILS_LSTOPO_H
#define UTILS_LSTOPO_H

#include <hwloc.h>

extern int lstopo_ignore_pus;
extern hwloc_obj_type_t lstopo_show_only;
extern int lstopo_show_cpuset;
extern int lstopo_show_taskset;
extern int lstopo_pid_number;
extern hwloc_pid_t lstopo_pid;
extern char ** lstopo_append_legends;
extern unsigned lstopo_append_legends_nr;

typedef void output_method (struct hwloc_topology *topology, const char *output, int overwrite, int logical, int legend, int verbose_mode);

FILE *open_output(const char *filename, int overwrite) __hwloc_attribute_malloc;

extern output_method output_console, output_synthetic, output_text, output_x11, output_fig, output_png, output_pdf, output_ps, output_svg, output_windows, output_xml;

struct draw_methods {
  void* (*start) (void *output, int width, int height);
  void (*declare_color) (void *output, int r, int g, int b);
  void (*box) (void *output, int r, int g, int b, unsigned depth, unsigned x, unsigned width, unsigned y, unsigned height);
  void (*line) (void *output, int r, int g, int b, unsigned depth, unsigned x1, unsigned y1, unsigned x2, unsigned y2);
  void (*text) (void *output, int r, int g, int b, int size, unsigned depth, unsigned x, unsigned y, const char *text);
};

extern unsigned int gridsize, fontsize;

enum lstopo_orient_e {
  LSTOPO_ORIENT_NONE = 0,
  LSTOPO_ORIENT_HORIZ,
  LSTOPO_ORIENT_VERT
};
/* orientation of children within an object of the given type */
extern enum lstopo_orient_e force_orient[];

extern void *output_draw_start(struct draw_methods *draw_methods, int logical, int legend, struct hwloc_topology *topology, void *output);
extern void output_draw(struct draw_methods *draw_methods, int logical, int legend, struct hwloc_topology *topology, void *output);

int rgb_to_color(int r, int g, int b) __hwloc_attribute_const;
int declare_color(int r, int g, int b);

static __hwloc_inline int lstopo_pu_offline(hwloc_obj_t l)
{
  return !hwloc_bitmap_isset(l->online_cpuset, l->os_index);
}

static __hwloc_inline int lstopo_pu_forbidden(hwloc_obj_t l)
{
  return !hwloc_bitmap_isset(l->allowed_cpuset, l->os_index);
}

static __hwloc_inline int lstopo_pu_running(hwloc_topology_t topology, hwloc_obj_t l)
{
  hwloc_bitmap_t bind = hwloc_bitmap_alloc();
  int res;
  if (lstopo_pid_number != -1 && lstopo_pid_number != 0)
    hwloc_get_proc_cpubind(topology, lstopo_pid, bind, 0);
  else if (lstopo_pid_number == 0)
    hwloc_get_cpubind(topology, bind, 0);
  res = bind && hwloc_bitmap_isset(bind, l->os_index);
  hwloc_bitmap_free(bind);
  return res;
}


#endif /* UTILS_LSTOPO_H */
