/*
 * Copyright © 2020 Hewlett Packard Enterprise.  All rights reserved.
 * Copyright © 2020-2021 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "private/autogen/config.h"
#include "hwloc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "lstopo.h"

#define TIKZ_TEXT_WIDTH(length, fontsize) (((length) * (fontsize) * 10) / 22)
#define TIKZ_FONTSIZE_SCALE(size) (((size) * 13) / 9)

#define TIKZ_FONTFAMILY_ENV "LSTOPO_TIKZ_FONTFAMILY"

static const char *
tikz_get_font_family(void)
{
  /* Authorized values (case insensitive): sf, rm, tt */
  char *font_request = getenv(TIKZ_FONTFAMILY_ENV);
  if (!font_request || !(*font_request))
    return ""; /* use latex default font family */
  else if (!strcasecmp(font_request, "sf"))
    return "\\sffamily";
  else if (!strcasecmp(font_request, "rm"))
    return "\\rmfamily";
  else if (!strcasecmp(font_request, "tt"))
    return "\\ttfamily";
  else
    return NULL;
}

static int
tikz_declare_color(struct lstopo_output *loutput, struct lstopo_color *lcolor)
{
  int r = lcolor->r, g = lcolor->g, b = lcolor->b;
  FILE *file = loutput->backend_data;

  /* TODO: add random seed in name to avoid collisions
   * name format proposed: hwloc-color-<seed>-<r>-<g>-<b>
   */
  fprintf(file, "\\definecolor{hwloc-color-%d-%d-%d}{RGB}{%d,%d,%d}\n",
          r, g, b, r, g, b);
  return 0;
}

static void
tikz_box(struct lstopo_output *loutput, const struct lstopo_color *lcolor, unsigned depth __hwloc_attribute_unused, unsigned x, unsigned width, unsigned y, unsigned height, hwloc_obj_t obj, unsigned box_id __hwloc_attribute_unused)
{
  FILE *file = loutput->file;
  int r = lcolor->r, g = lcolor->g, b = lcolor->b;
  unsigned cpukind_style = lstopo_obj_cpukind_style(loutput, obj);
  char linestyle[64] = "solid";
  unsigned thickness = loutput->thickness;
  float dashspace = 1.15f; /* default dash size: 1.15pt */

  if (cpukind_style) {
    char dashsize[20], *comma = NULL;
    thickness *= cpukind_style;
    dashspace *= 1U << cpukind_style;
    snprintf(dashsize, 20, "%.4f", dashspace);
    comma = strchr(dashsize, ',');
    if (comma)
      *comma = '.'; /* Use decimal dot despite the locale's opinion. */
    snprintf(linestyle, sizeof(linestyle), "dash pattern=on %spt off %spt",
             dashsize, dashsize);
  }

  fprintf(file, "\t\\filldraw [fill=hwloc-color-%d-%d-%d,draw=black,line width=%upt,%s] (%u,%u) rectangle ++(%u,%u);\n",
          r, g, b, thickness, linestyle, x, y, width, height);
}


static void
tikz_line(struct lstopo_output *loutput, unsigned depth __hwloc_attribute_unused, unsigned x1, unsigned y1, unsigned x2, unsigned y2, hwloc_obj_t obj __hwloc_attribute_unused, unsigned line_id __hwloc_attribute_unused)
{
  FILE *file = loutput->file;

  fprintf(file, "\t\\draw [draw=black,line width=%upt] (%u,%u) -- (%u,%u);\n",
          loutput->thickness, x1, y1, x2, y2);
}

static void
tikz_textsize(struct lstopo_output *loutput __hwloc_attribute_unused, const char *text __hwloc_attribute_unused, unsigned textlength, unsigned fontsize, unsigned *width)
{
  fontsize = TIKZ_FONTSIZE_SCALE(fontsize);
  *width = TIKZ_TEXT_WIDTH(textlength, fontsize);
}


static void
tikz_text(struct lstopo_output *loutput, const struct lstopo_color *lcolor, int size __hwloc_attribute_unused, unsigned depth __hwloc_attribute_unused, unsigned x, unsigned y, const char *text, hwloc_obj_t obj, unsigned text_id __hwloc_attribute_unused)
{
  FILE *file = loutput->file;
  int r = lcolor->r, g = lcolor->g, b = lcolor->b;
  unsigned cpukind_style = lstopo_obj_cpukind_style(loutput, obj);
  const char *bf_style = "";

  const char *tikzdelim = "{}%&#";

  if (cpukind_style % 2)
      bf_style = "-bold";

  fprintf(file, "\t\\node [hwloc-label%s,text=hwloc-color-%d-%d-%d] at (%u,%u) {",
          bf_style, r, g, b, x, y);
  while (*text) {
    size_t chunksize = strcspn(text, tikzdelim), n_delim;
    fprintf(file, "%.*s", (int) chunksize, text);
    text += chunksize;
    for (n_delim = strspn(text, tikzdelim); *text && n_delim; ++text, --n_delim)
      fprintf(file, "\\%c", *text);
  }
  fprintf(file, "};\n");
}

static struct draw_methods tikz_draw_methods = {
  tikz_declare_color,
  NULL,
  tikz_box,
  tikz_line,
  tikz_text,
  tikz_textsize,
};

int output_tikz(struct lstopo_output * loutput, const char *filename)
{
  const char *font_family;
  FILE *output = open_output(filename, loutput->overwrite);
  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    return -1;
  }

  font_family = tikz_get_font_family();
  if (!font_family) {
    fprintf(stderr, "Invalid value for %s. The only accepted values are \"rm\", \"sf\" and \"tt\".\n",
            TIKZ_FONTFAMILY_ENV);
    if (output != stdout)
      fclose(output);
    return -1;
  }

  loutput->file = output;
  loutput->methods = &tikz_draw_methods;
  loutput->backend_data = output;
  loutput->backend_flags |= LSTOPO_BACKEND_FLAG_APPROXIMATIVE_TEXTWIDTH;

  /* recurse once for preparing sizes and positions */
  loutput->drawing = LSTOPO_DRAWING_PREPARE;
  output_draw(loutput);
  loutput->drawing = LSTOPO_DRAWING_DRAW;

  /* ready */

  /* Write LaTeX header */
  fprintf(output, "\\documentclass{standalone}\n");

  /* Write required LaTeX preambule */
  fprintf(output, "\n%%%%%%%%%% If inserting in another document, the following lines below must be copied before \\begin{document} %%%%%%%%%%\n\n");
  fprintf(output, "\\usepackage{tikz}\n\\usepackage{xcolor}\n");
  declare_colors(loutput);
  lstopo_prepare_custom_styles(loutput); /* Add custom colors to the preambule */
  fprintf(output, "\n%%%%%%%%%% End of lines needed before \\begin{document}  %%%%%%%%%%\n\n");

  fprintf(output, "\\begin{document}\n");

  /* Write actual image code */
  fprintf(output, "\n%%%%%%%%%% If inserting in another document, this is the actual source code of the picture %%%%%%%%%%\n\n");
  fprintf(output, "\\begin{tikzpicture}[x=1pt,y=1pt,yscale=-1,"
                  "hwloc-label/.style={fill=none,draw=none,text=black,align=left,anchor=north west,"
                  "outer sep=0pt,inner sep=0pt,font=\\fontsize{%u}{%u}\\selectfont%s},"
                  "hwloc-label-bold/.style={hwloc-label,font=\\fontsize{%u}{%u}\\selectfont%s\\bfseries}]\n",
                  loutput->fontsize, loutput->fontsize + loutput->linespacing, font_family,
                  loutput->fontsize, loutput->fontsize + loutput->linespacing, font_family);
  fprintf(output, "\t\\clip (0,0) rectangle (%u,%u);\n", loutput->width, loutput->height);
  output_draw(loutput);
  fprintf(output,"\\end{tikzpicture}\n");
  fprintf(output, "\n%%%%%%%%%% End of actual source code of the picture to insert in another document %%%%%%%%%%\n\n");

  /* Write LaTeX footer */
  fprintf(output, "\\end{document}\n");

  if (output != stdout)
    fclose(output);

  destroy_colors(loutput);
  return 0;
}
