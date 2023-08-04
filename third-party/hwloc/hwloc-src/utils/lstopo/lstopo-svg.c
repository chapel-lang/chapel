/*
 * Copyright © 2019-2021 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "private/autogen/config.h"
#include "hwloc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "lstopo.h"

#define SVG_TEXT_WIDTH(length, fontsize) (((length) * (fontsize))/2)
#define SVG_FONTSIZE_SCALE(size) (((size) * 11) / 9)

static void
native_svg_box(struct lstopo_output *loutput, const struct lstopo_color *lcolor, unsigned depth __hwloc_attribute_unused, unsigned x, unsigned width, unsigned y, unsigned height, hwloc_obj_t obj, unsigned box_id)
{
  FILE *file = loutput->file;
  unsigned cpukind_style = lstopo_obj_cpukind_style(loutput, obj);
  unsigned thickness = loutput->thickness;
  int r = lcolor->r, g = lcolor->g, b = lcolor->b;
  char id[128] = "";
  char class[128] = "";
  char complement[12] = "";
  char dash[32] = "";

  if (box_id)
    snprintf(complement, sizeof complement, "_%u", box_id);

  if (obj) {
    char type[64];
    hwloc_obj_type_snprintf(type, sizeof(type), obj, 0);
    snprintf(id, sizeof id, " id='%s_%u_rect%s'", type, obj->logical_index, complement);
    snprintf(class, sizeof class, " class='%s'", type);
  } else {
    snprintf(id, sizeof id, " id='anon_rect%s'", complement);
  }

  if (cpukind_style) {
    snprintf(dash, sizeof(dash), " stroke-dasharray=\"%u\"", 1U << cpukind_style);
    thickness *= cpukind_style;
  }

  fprintf(file,"\t<rect%s%s x='%u' y='%u' width='%u' height='%u' fill='rgb(%d,%d,%d)' stroke='rgb(0,0,0)' stroke-width='%u'%s/>\n",
	  id, class, x, y, width, height, r, g, b, thickness, dash);
}


static void
native_svg_line(struct lstopo_output *loutput, unsigned depth __hwloc_attribute_unused, unsigned x1, unsigned y1, unsigned x2, unsigned y2, hwloc_obj_t obj, unsigned line_id)
{
  FILE *file = loutput->file;
  char id[128] = "";
  char class[128] = "";
  char complement[12] = "";
  if (line_id)
    snprintf(complement, sizeof complement, "_%u", line_id);

  if (obj) {
    char type[64];
    hwloc_obj_type_snprintf(type, sizeof(type), obj, 0);
    snprintf(id, sizeof id, " id='%s_%u_line%s'", type, obj->logical_index, complement);
    snprintf(class, sizeof class, " class='%s'", type);
  } else {
    snprintf(id, sizeof id, " id='anon_line%s'", complement);
  }

  fprintf(file,"\t<line%s%s x1='%u' y1='%u' x2='%u' y2='%u' stroke='rgb(0,0,0)' stroke-width='%u'/>\n",
	  id, class, x1, y1, x2, y2, loutput->thickness);
}

static void
native_svg_textsize(struct lstopo_output *loutput __hwloc_attribute_unused, const char *text __hwloc_attribute_unused, unsigned textlength, unsigned fontsize, unsigned *width)
{
  fontsize = SVG_FONTSIZE_SCALE(fontsize);
  *width = SVG_TEXT_WIDTH(textlength, fontsize);
}


static void
native_svg_text(struct lstopo_output *loutput, const struct lstopo_color *lcolor, int size, unsigned depth __hwloc_attribute_unused, unsigned x, unsigned y, const char *text, hwloc_obj_t obj, unsigned text_id)
{
  FILE *file = loutput->file;
  unsigned cpukind_style = lstopo_obj_cpukind_style(loutput, obj);
  int r = lcolor->r, g = lcolor->g, b = lcolor->b;
  char id[128] = "";
  char class[128] = "";
  char complement[12] = "";
  const char *fontweight = "";

  if (text_id)
    snprintf(complement, sizeof complement, "_%u", text_id);

  if (obj) {
    char type[64];
    hwloc_obj_type_snprintf(type, sizeof(type), obj, 0);
    snprintf(id, sizeof id, " id='%s_%u_text%s'", type, obj->logical_index, complement);
    snprintf(class, sizeof class, " class='%s'", type);
  } else {
    snprintf(id, sizeof id, " id='anon_text%s'", complement);
  }

  if (cpukind_style % 2)
    fontweight = " font-weight='bold'";

  fprintf(file,"\t<text%s%s font-family='Monospace'%s x='%u' y='%u' fill='rgb(%d,%d,%d)' font-size='%dpx'>%s</text>\n",
	  id, class,
          fontweight,
          x, y+size, r, g, b, size, text);
}

static struct draw_methods native_svg_draw_methods = {
  NULL,
  NULL,
  native_svg_box,
  native_svg_line,
  native_svg_text,
  native_svg_textsize,
};

int output_nativesvg(struct lstopo_output * loutput, const char *filename)
{
  FILE *output = open_output(filename, loutput->overwrite);
  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    return -1;
  }

  loutput->file = output;
  loutput->methods = &native_svg_draw_methods;
  loutput->backend_flags |= LSTOPO_BACKEND_FLAG_APPROXIMATIVE_TEXTWIDTH;

  /* recurse once for preparing sizes and positions */
  loutput->drawing = LSTOPO_DRAWING_PREPARE;
  output_draw(loutput);
  loutput->drawing = LSTOPO_DRAWING_DRAW;

  fprintf(output, "<?xml version='1.0' encoding='UTF-8'?>\n");
  fprintf(output, "<svg xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' width='%upx' height='%upx' viewBox='0 0 %upx %upx' version='1.1'>\n",
	  loutput->width, loutput->height, loutput->width, loutput->height);

  /* ready */
  declare_colors(loutput);
  lstopo_prepare_custom_styles(loutput);

  output_draw(loutput);
  fprintf(output,"</svg>\n");

  if (output != stdout)
    fclose(output);

  destroy_colors(loutput);
  return 0;
}
