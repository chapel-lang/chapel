/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2021 Inria.  All rights reserved.
 * Copyright © 2009-2010 Université Bordeaux
 * Copyright © 2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "private/autogen/config.h"
#include "hwloc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "lstopo.h"

/* .fig back-end.  */

#define FIG_FACTOR 20

static int fig_color_index = 32;

static int
fig_declare_color(struct lstopo_output *loutput, struct lstopo_color *lcolor)
{
  FILE *file = loutput->file;
  int r = lcolor->r, g = lcolor->g, b = lcolor->b;

  if (r == 0xff && g == 0xff && b == 0xff) {
    lcolor->private.fig.color = 7;
    return 0;
  } else if (!r && !g && !b) {
    lcolor->private.fig.color = 0;
    return 0;
  } else {
    lcolor->private.fig.color = fig_color_index++;
  }

  fprintf(file, "0 %d #%02x%02x%02x\n", lcolor->private.fig.color, (unsigned) r, (unsigned) g, (unsigned) b);
  return 0;
}

static void
fig_box(struct lstopo_output *loutput, const struct lstopo_color *lcolor, unsigned depth, unsigned x, unsigned width, unsigned y, unsigned height, hwloc_obj_t obj __hwloc_attribute_unused, unsigned box_id __hwloc_attribute_unused)
{
  FILE *file = loutput->file;
  unsigned cpukind_style = lstopo_obj_cpukind_style(loutput, obj);
  unsigned linestyle = 0; /* solid */
  unsigned dashspace = 0; /* no dash */
  unsigned thickness = loutput->thickness;

  if (!width || !height)
    return;

  if (cpukind_style) {
    linestyle = 1; /* dash */
    dashspace = 1U << cpukind_style;
    thickness *= (1 + cpukind_style);
  }

  x *= FIG_FACTOR;
  y *= FIG_FACTOR;
  width *= FIG_FACTOR;
  height *= FIG_FACTOR;
  fprintf(file, "2 2 %u %u 0 %d %u -1 20 %u.0 0 0 -1 0 0 5\n\t",
          linestyle, thickness, lcolor->private.fig.color, depth, dashspace);
  fprintf(file, " %u %u", x, y);
  fprintf(file, " %u %u", x + width, y);
  fprintf(file, " %u %u", x + width, y + height);
  fprintf(file, " %u %u", x, y + height);
  fprintf(file, " %u %u", x, y);
  fprintf(file, "\n");
}

static void
fig_line(struct lstopo_output *loutput, unsigned depth, unsigned x1, unsigned y1, unsigned x2, unsigned y2, hwloc_obj_t obj __hwloc_attribute_unused, unsigned line_id __hwloc_attribute_unused)
{
  FILE *file = loutput->file;

  x1 *= FIG_FACTOR;
  y1 *= FIG_FACTOR;
  x2 *= FIG_FACTOR;
  y2 *= FIG_FACTOR;
  fprintf(file, "2 1 0 %u 0 0 %u -1 -1 0.0 0 0 -1 0 0 2\n\t", loutput->thickness, depth);
  fprintf(file, " %u %u", x1, y1);
  fprintf(file, " %u %u", x2, y2);
  fprintf(file, "\n");
}

#define FIG_FONTSIZE_SCALE(size) (((size) * 11) / 10)
/* assume character width is half their height on average */
#define FIG_TEXT_WIDTH(length, fontsize) (((length) * (fontsize))/2)

static void
fig_text(struct lstopo_output *loutput, const struct lstopo_color *lcolor, int size, unsigned depth, unsigned x, unsigned y, const char *text, hwloc_obj_t obj __hwloc_attribute_unused, unsigned text_id __hwloc_attribute_unused)
{
  FILE *file = loutput->file;
  unsigned cpukind_style = lstopo_obj_cpukind_style(loutput, obj);
  unsigned fontflags = 0; /* normal */
  int len = (int)strlen(text);
  int color;

  if (cpukind_style % 2)
    fontflags = 2; /* bold for odd styles */

  color = lcolor->private.fig.color;
  x *= FIG_FACTOR;
  y *= FIG_FACTOR;
  /* move the origin of the text away from the box corner */
  x += (size * FIG_FACTOR * 2) / 10;
  y += (size * FIG_FACTOR * 4) / 10;

  size = FIG_FONTSIZE_SCALE(size);
  fprintf(file, "4 0 %d %u -1 %u %d 0.0 4 %d %d %u %u %s\\001\n",
          color, depth, fontflags, size, size * FIG_FACTOR, FIG_TEXT_WIDTH(len, size) * FIG_FACTOR, x, y + size * 10, text);
}

static void
fig_textsize(struct lstopo_output *loutput __hwloc_attribute_unused, const char *text __hwloc_attribute_unused, unsigned textlength, unsigned fontsize, unsigned *width)
{
  fontsize = FIG_FONTSIZE_SCALE(fontsize);
  *width = FIG_TEXT_WIDTH(textlength, fontsize);
}

static struct draw_methods fig_draw_methods = {
  fig_declare_color,
  NULL,
  fig_box,
  fig_line,
  fig_text,
  fig_textsize,
};

int
output_fig (struct lstopo_output *loutput, const char *filename)
{
  FILE *output = open_output(filename, loutput->overwrite);
  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    return -1;
  }

  loutput->file = output;
  loutput->methods = &fig_draw_methods;
  loutput->backend_flags |= LSTOPO_BACKEND_FLAG_APPROXIMATIVE_TEXTWIDTH;

  /* recurse once for preparing sizes and positions */
  loutput->drawing = LSTOPO_DRAWING_PREPARE;
  output_draw(loutput);
  loutput->drawing = LSTOPO_DRAWING_DRAW;

  fprintf(output, "#FIG 3.2  Produced by hwloc's lstopo\n");
  fprintf(output, "Landscape\n");
  fprintf(output, "Center\n");
  fprintf(output, "Inches\n");
  fprintf(output, "letter\n");
  fprintf(output, "100.00\n");	/* magnification */
  fprintf(output, "Single\n");	/* single page */
  fprintf(output, "-2\n");	/* no transparent color */
  fprintf(output, "1200 2\n");	/* 1200 ppi resolution, upper left origin */

  /* ready */
  declare_colors(loutput);
  lstopo_prepare_custom_styles(loutput);

  output_draw(loutput);

  if (output != stdout)
    fclose(output);

  destroy_colors(loutput);
  return 0;
}
