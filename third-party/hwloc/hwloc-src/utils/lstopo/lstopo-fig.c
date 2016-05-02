/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2016 Inria.  All rights reserved.
 * Copyright © 2009-2010 Université Bordeaux
 * Copyright © 2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <private/autogen/config.h>
#include <hwloc.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "lstopo.h"

/* .fig back-end.  */

#define FIG_FACTOR 20

static void
fig_init(void *loutput_)
{
  struct lstopo_output *loutput = loutput_;
  FILE *file = loutput->file;
  fprintf(file, "#FIG 3.2  Produced by hwloc's lstopo\n");
  fprintf(file, "Landscape\n");
  fprintf(file, "Center\n");
  fprintf(file, "Inches\n");
  fprintf(file, "letter\n");
  fprintf(file, "100.00\n");	/* magnification */
  fprintf(file, "Single\n");	/* single page */
  fprintf(file, "-2\n");	/* no transparent color */
  fprintf(file, "1200 2\n");	/* 1200 ppi resolution, upper left origin */
}

static int __hwloc_attribute_const
rgb_to_fig(int r, int g, int b)
{
  if (r == 0xff && g == 0xff && b == 0xff)
    return 7;

  if (!r && !g && !b)
    return 0;

  return 32 + rgb_to_color(r, g, b);
}

static void
fig_declare_color(void *loutput_, int r, int g, int b)
{
  struct lstopo_output *loutput = loutput_;
  FILE *file = loutput->file;
  int color;

  if (r == 0xff && g == 0xff && b == 0xff)
    return;

  if (!r && !g && !b)
    return;

  color = declare_color(r, g, b);

  fprintf(file, "0 %d #%02x%02x%02x\n", 32 + color, r, g, b);
}

static void
fig_box(void *loutput_, int r, int g, int b, unsigned depth, unsigned x, unsigned width, unsigned y, unsigned height)
{
  struct lstopo_output *loutput = loutput_;
  FILE *file = loutput->file;
  x *= FIG_FACTOR;
  y *= FIG_FACTOR;
  width *= FIG_FACTOR;
  height *= FIG_FACTOR;
  fprintf(file, "2 2 0 1 0 %d %u -1 20 0.0 0 0 -1 0 0 5\n\t", rgb_to_fig(r, g, b), depth);
  fprintf(file, " %u %u", x, y);
  fprintf(file, " %u %u", x + width, y);
  fprintf(file, " %u %u", x + width, y + height);
  fprintf(file, " %u %u", x, y + height);
  fprintf(file, " %u %u", x, y);
  fprintf(file, "\n");
}

static void
fig_line(void *loutput_, int r, int g, int b, unsigned depth, unsigned x1, unsigned y1, unsigned x2, unsigned y2)
{
  struct lstopo_output *loutput = loutput_;
  FILE *file = loutput->file;
  x1 *= FIG_FACTOR;
  y1 *= FIG_FACTOR;
  x2 *= FIG_FACTOR;
  y2 *= FIG_FACTOR;
  fprintf(file, "2 1 0 1 0 %d %u -1 -1 0.0 0 0 -1 0 0 2\n\t", rgb_to_fig(r, g, b), depth);
  fprintf(file, " %u %u", x1, y1);
  fprintf(file, " %u %u", x2, y2);
  fprintf(file, "\n");
}

static void
fig_text(void *loutput_, int r, int g, int b, int size, unsigned depth, unsigned x, unsigned y, const char *text)
{
  struct lstopo_output *loutput = loutput_;
  FILE *file = loutput->file;
  int len = (int)strlen(text);
  int color = rgb_to_fig(r, g, b);
  x *= FIG_FACTOR;
  y *= FIG_FACTOR;
  size = (size * 16) / 10;
  fprintf(file, "4 0 %d %u -1 0 %d 0.0 4 %d %u %u %u %s\\001\n", color, depth, size, size * 10, len * size * 10, x, y + size * 10, text);
}

static struct draw_methods fig_draw_methods = {
  fig_init,
  fig_declare_color,
  fig_box,
  fig_line,
  fig_text,
  NULL, /* textsize not supported, fallback to default estimation in get_textsize() */
};

void
output_fig (struct lstopo_output *loutput, const char *filename)
{
  FILE *output = open_output(filename, loutput->overwrite);
  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    return;
  }

  loutput->file = output;
  loutput->methods = &fig_draw_methods;

  output_draw_start(loutput);
  output_draw(loutput);

  if (output != stdout)
    fclose(output);
}
