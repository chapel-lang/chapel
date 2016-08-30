/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2016 Inria.  All rights reserved.
 * Copyright © 2009-2010, 2012, 2015 Université Bordeaux
 * Copyright © 2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#ifndef UTILS_LSTOPO_H
#define UTILS_LSTOPO_H

#include <private/autogen/config.h>
#include <hwloc.h>
#include <misc.h>

extern int lstopo_ignore_pus;
extern int lstopo_collapse;
extern hwloc_obj_type_t lstopo_show_only;
extern int lstopo_show_cpuset;
extern int lstopo_show_taskset;
extern int lstopo_pid_number;
extern hwloc_pid_t lstopo_pid;
extern char ** lstopo_append_legends;
extern unsigned lstopo_append_legends_nr;
extern unsigned long lstopo_export_synthetic_flags;

FILE *open_output(const char *filename, int overwrite) __hwloc_attribute_malloc;

struct draw_methods;

/* if embedded in backend-specific output structure, must be at the beginning */
struct lstopo_output {
  FILE *file;
  hwloc_topology_t topology;
  int logical;
  int legend;
  int overwrite;
  int verbose_mode;
  struct draw_methods *methods;
  unsigned min_pu_textwidth;
};

struct style {
  struct stylecolor { int r, g, b; }
	bg,	/* main box background color */
	t,	/* main text color */
	bg2,	/* other box background color */
	t2;	/* other text color */
};

struct lstopo_obj_userdata {
  /* original common userdata (we replace the first one with this extended structure) */
  struct hwloc_utils_userdata common;

  /* draw info */
  unsigned width;
  unsigned height;
  unsigned fontsize;
  unsigned gridsize;

  /* custom style */
  struct style style;
  unsigned style_set; /* 0x1 if bg set, 0x2 for t, 0x4 for bg2, 0x8 for t2 */
};

typedef void output_method (struct lstopo_output *output, const char *filename);

extern output_method output_console, output_synthetic, output_ascii, output_x11, output_fig, output_png, output_pdf, output_ps, output_svg, output_windows, output_xml;

struct draw_methods {
  void (*init) (void *output);
  void (*declare_color) (void *output, int r, int g, int b);
  void (*box) (void *output, int r, int g, int b, unsigned depth, unsigned x, unsigned width, unsigned y, unsigned height);
  void (*line) (void *output, int r, int g, int b, unsigned depth, unsigned x1, unsigned y1, unsigned x2, unsigned y2);
  void (*text) (void *output, int r, int g, int b, int size, unsigned depth, unsigned x, unsigned y, const char *text);
  void (*textsize) (void *output, const char *text, unsigned textlength, unsigned fontsize, unsigned *width);
};

extern unsigned int gridsize, fontsize;

enum lstopo_orient_e {
  LSTOPO_ORIENT_NONE = 0,
  LSTOPO_ORIENT_HORIZ,
  LSTOPO_ORIENT_VERT,
  LSTOPO_ORIENT_RECT
};
/* orientation of children within an object of the given type */
extern enum lstopo_orient_e force_orient[];

extern void output_draw_start(struct lstopo_output *output);
extern void output_draw(struct lstopo_output *output);

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
