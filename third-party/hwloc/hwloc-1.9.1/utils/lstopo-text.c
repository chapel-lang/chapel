/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2014 Inria.  All rights reserved.
 * Copyright © 2009-2012 Université Bordeaux 1
 * Copyright © 2009-2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <private/autogen/config.h>
#include <hwloc.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef HAVE_SETLOCALE
#include <locale.h>
#endif /* HAVE_SETLOCALE */

#ifdef HAVE_NL_LANGINFO
#include <langinfo.h>
#endif /* HAVE_NL_LANGINFO */

#ifdef HAVE_PUTWC
#include <wchar.h>
#endif /* HAVE_PUTWC */

#ifdef HWLOC_HAVE_LIBTERMCAP
#ifdef HWLOC_USE_NCURSES
#  include <ncurses.h>
#else
#  include <curses.h>
#endif
#include <term.h>
#endif /* HWLOC_HAVE_LIBTERMCAP */

#include "lstopo.h"
#include "misc.h"

#define indent(output, i) \
  fprintf (output, "%*s", (int) i, "");

/*
 * Console fashion text output
 */

static void
output_console_obj (hwloc_topology_t topology, hwloc_obj_t l, FILE *output, int logical, int verbose_mode)
{
  char type[32], *attr, phys[32] = "";
  unsigned idx = logical ? l->logical_index : l->os_index;
  const char *indexprefix = logical ? " L#" :  " P#";
  if (lstopo_show_cpuset < 2) {
    int len;
    if (l->type == HWLOC_OBJ_MISC && l->name)
      fprintf(output, "%s", l->name);
    else {
      hwloc_obj_type_snprintf (type, sizeof(type), l, verbose_mode-1);
      fprintf(output, "%s", type);
    }
    if (l->depth != 0 && idx != (unsigned)-1
        && l->type != HWLOC_OBJ_PCI_DEVICE
        && (l->type != HWLOC_OBJ_BRIDGE || l->attr->bridge.upstream_type == HWLOC_OBJ_BRIDGE_HOST))
      fprintf(output, "%s%u", indexprefix, idx);
    if (logical && l->os_index != (unsigned) -1 &&
	(verbose_mode >= 2 || l->type == HWLOC_OBJ_PU || l->type == HWLOC_OBJ_NODE))
      snprintf(phys, sizeof(phys), "P#%u", l->os_index);
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
    if ((l->type == HWLOC_OBJ_OS_DEVICE || verbose_mode >= 2) && l->name && l->type != HWLOC_OBJ_MISC)
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
      printf(" (offline)");
    else if (lstopo_pu_forbidden(l))
      printf(" (forbidden)");
    else if (lstopo_pu_running(topology, l))
      printf(" (running)");
  }
}

/* Recursively output topology in a console fashion */
static void
output_topology (hwloc_topology_t topology, hwloc_obj_t l, hwloc_obj_t parent, FILE *output, int i, int logical, int verbose_mode)
{
  unsigned x;
  int group_identical = (verbose_mode <= 1) && !lstopo_show_cpuset;
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
  output_console_obj(topology, l, output, logical, verbose_mode);
  if (l->arity || (!i && !l->arity))
    {
      for (x=0; x<l->arity; x++)
	if (l->children[x]->type != HWLOC_OBJ_PU || !lstopo_ignore_pus)
	  output_topology (topology, l->children[x], l, output, i, logical, verbose_mode);
  }
}

/* Recursive so that multiple depth types are properly shown */
static void
output_only (hwloc_topology_t topology, hwloc_obj_t l, FILE *output, int logical, int verbose_mode)
{
  unsigned x;
  if (lstopo_show_only == l->type) {
    output_console_obj (topology, l, output, logical, verbose_mode);
    fprintf (output, "\n");
  }
  for (x=0; x<l->arity; x++)
    output_only (topology, l->children[x], output, logical, verbose_mode);
}

void output_console(hwloc_topology_t topology, const char *filename, int overwrite, int logical, int legend __hwloc_attribute_unused, int verbose_mode)
{
  unsigned topodepth;
  FILE *output;

  output = open_output(filename, overwrite);
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
      printf("latency matrix between %ss (depth %u) by %s indexes:\n",
	     hwloc_obj_type_string(hwloc_get_depth_type(topology, depth)),
	     depth,
	     logical ? "logical" : "physical");
      hwloc_utils_print_distance_matrix(topology, hwloc_get_root_obj(topology), distances->nbobjs, depth, distances->latency, logical);
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

void output_synthetic(hwloc_topology_t topology, const char *filename, int overwrite, int logical __hwloc_attribute_unused, int legend __hwloc_attribute_unused, int verbose_mode __hwloc_attribute_unused)
{
  FILE *output;
  hwloc_obj_t obj = hwloc_get_root_obj(topology);
  int arity;

  if (!obj->symmetric_subtree) {
    fprintf(stderr, "Cannot output assymetric topology in synthetic format.\n");
    fprintf(stderr, "Adding --no-io may help making the topology symmetric.\n");
    return;
  }

  output = open_output(filename, overwrite);
  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    return;
  }

  arity = obj->arity;
  while (arity) {
    char types[64];
    obj = obj->first_child;
    hwloc_obj_type_snprintf(types, sizeof(types), obj, 1);
    fprintf(output, "%s:%u ", types, arity);
    arity = obj->arity;
  }
  fprintf(output, "\n");

  if (output != stdout)
    fclose(output);
}

/*
 * Pretty text output
 */

/* Uses unicode bars if available */
#ifdef HAVE_PUTWC
typedef wchar_t character;
#define PRIchar "lc"
#define putcharacter(c,f) putwc(c,f)
#else /* HAVE_PUTWC */
typedef unsigned char character;
#define PRIchar "c"
#define putcharacter(c,f) putc(c,f)
#endif /* HAVE_PUTWC */

#ifdef HWLOC_HAVE_LIBTERMCAP
static int myputchar(int c) {
  return putcharacter(c, stdout);
}
#endif /* HWLOC_HAVE_LIBTERMCAP */

/* Off-screen rendering buffer */
struct cell {
  character c;
#ifdef HWLOC_HAVE_LIBTERMCAP
  int fr, fg, fb;
  int br, bg, bb;
#endif /* HWLOC_HAVE_LIBTERMCAP */
};

struct display {
  struct cell **cells;
  int width;
  int height;
  int utf8;
};

/* Allocate the off-screen buffer */
static void *
text_start(void *output __hwloc_attribute_unused, int width, int height)
{
  int j, i;
  struct display *disp = malloc(sizeof(*disp));
  /* terminals usually have narrow characters, so let's make them wider */
  width /= (gridsize/2);
  height /= gridsize;
  disp->cells = malloc(height * sizeof(*disp->cells));
  disp->width = width;
  disp->height = height;
  for (j = 0; j < height; j++) {
    disp->cells[j] = calloc(width, sizeof(**disp->cells));
    for (i = 0; i < width; i++)
      disp->cells[j][i].c = ' ';
  }
#ifdef HAVE_NL_LANGINFO
  disp->utf8 = !strcmp(nl_langinfo(CODESET), "UTF-8");
#endif /* HAVE_NL_LANGINFO */
  return disp;
}

#ifdef HWLOC_HAVE_LIBTERMCAP
/* Standard terminfo strings */
static char *initc = NULL, *initp = NULL;

/* Set text color to bright white or black according to the background */
static int set_textcolor(int rr, int gg, int bb)
{
  if (!initc && !initp && rr + gg + bb < 2) {
    if (enter_bold_mode)
      tputs(enter_bold_mode, 1, myputchar);
    return 7;
  } else {
    if (exit_attribute_mode)
      tputs(exit_attribute_mode, 1, myputchar);
    return 0;
  }
}

static void
set_color(int fr, int fg, int fb, int br, int bg, int bb)
{
  char *toput;
  int color, textcolor;

  if (initc || initp) {
    /* Can set rgb color, easy */
    textcolor = rgb_to_color(fr, fg, fb) + 16;
    color = rgb_to_color(br, bg, bb) + 16;
  } else {
    /* Magic trigger: it seems to separate colors quite well */
    int brr = br >= 0xe0;
    int bgg = bg >= 0xe0;
    int bbb = bb >= 0xe0;

    if (set_a_background)
      /* ANSI colors */
      color = (brr << 0) | (bgg << 1) | (bbb << 2);
    else
      /* Legacy colors */
      color = (brr << 2) | (bgg << 1) | (bbb << 0);
    textcolor = set_textcolor(brr, bgg, bbb);
  }

  /* And now output magic string to TTY */
  if (set_a_foreground) {
    /* foreground */
    if ((toput = tparm(set_a_foreground, textcolor, 0, 0, 0, 0, 0, 0, 0, 0)))
      tputs(toput, 1, myputchar);
    /* background */
    if ((toput = tparm(set_a_background, color, 0, 0, 0, 0, 0, 0, 0, 0)))
      tputs(toput, 1, myputchar);
  } else if (set_foreground) {
    /* foreground */
    if ((toput = tparm(set_foreground, textcolor, 0, 0, 0, 0, 0, 0, 0, 0)))
      tputs(toput, 1, myputchar);
    /* background */
    if ((toput = tparm(set_background, color, 0, 0, 0, 0, 0, 0, 0, 0)))
      tputs(toput, 1, myputchar);
  } else if (set_color_pair) {
    /* pair */
    if ((toput = tparm(set_color_pair, color, 0, 0, 0, 0, 0, 0, 0, 0)))
      tputs(toput, 1, myputchar);
  }
}
#endif /* HWLOC_HAVE_LIBTERMCAP */

/* We we can, allocate rgb colors */
static void
text_declare_color(void *output __hwloc_attribute_unused, int r __hwloc_attribute_unused, int g __hwloc_attribute_unused, int b __hwloc_attribute_unused)
{
#ifdef HWLOC_HAVE_LIBTERMCAP
  int color = declare_color(r, g, b);
  /* Yes, values seem to range from 0 to 1000 inclusive */
  int rr = (r * 1001) / 256;
  int gg = (g * 1001) / 256;
  int bb = (b * 1001) / 256;
  char *toput;

  if (initc) {
    if ((toput = tparm(initc, color + 16, rr, gg, bb, 0, 0, 0, 0, 0)))
      tputs(toput, 1, myputchar);
  } else if (initp) {
    if ((toput = tparm(initp, color + 16, 0, 0, 0, rr, gg, bb, 0, 0)))
      tputs(toput, 1, myputchar);
  }
#endif /* HWLOC_HAVE_LIBTERMCAP */
}

/* output text, erasing any previous content */
static void
put(struct display *disp, int x, int y, character c, int fr __hwloc_attribute_unused, int fg __hwloc_attribute_unused, int fb __hwloc_attribute_unused, int br __hwloc_attribute_unused, int bg __hwloc_attribute_unused, int bb __hwloc_attribute_unused)
{
  if (x >= disp->width || y >= disp->height) {
    /* fprintf(stderr, "%"PRIchar" overflowed to (%d,%d)\n", c, x, y); */
    return;
  }
  disp->cells[y][x].c = c;
#ifdef HWLOC_HAVE_LIBTERMCAP
  if (fr != -1) {
    disp->cells[y][x].fr = fr;
    disp->cells[y][x].fg = fg;
    disp->cells[y][x].fb = fb;
  }
  if (br != -1) {
    disp->cells[y][x].br = br;
    disp->cells[y][x].bg = bg;
    disp->cells[y][x].bb = bb;
  }
#endif /* HWLOC_HAVE_LIBTERMCAP */
}

/* Where bars of a character go to */
enum {
  up = (1<<0),
  down = (1<<1),
  left = (1<<2),
  right = (1<<3)
};

/* Convert a bar character into its directions */
static int
to_directions(struct display *disp, character c)
{
#ifdef HAVE_PUTWC
  if (disp->utf8) {
    switch (c) {
      case L'\x250c': return down|right;
      case L'\x2510': return down|left;
      case L'\x2514': return up|right;
      case L'\x2518': return up|left;
      case L'\x2500': return left|right;
      case L'\x2502': return down|up;
      case L'\x2577': return down;
      case L'\x2575': return up;
      case L'\x2576': return right;
      case L'\x2574': return left;
      case L'\x251c': return down|up|right;
      case L'\x2524': return down|up|left;
      case L'\x252c': return down|left|right;
      case L'\x2534': return up|left|right;
      case L'\x253c': return down|up|left|right;
      default: return 0;
    }
  } else
#endif /* HAVE_PUTWC */
  {
    switch (c) {
      case L'-': return left|right;
      case L'|': return down|up;
      case L'/':
      case L'\\':
      case L'+': return down|up|left|right;
      default: return 0;
    }
  }
}

/* Produce a bar character given the wanted directions */
static character
from_directions(struct display *disp, int direction)
{
#ifdef HAVE_PUTWC
  if (disp->utf8) {
    switch (direction) {
    case down|right:		return L'\x250c';
    case down|left:		return L'\x2510';
    case up|right:		return L'\x2514';
    case up|left:		return L'\x2518';
    case left|right:		return L'\x2500';
    case down|up:		return L'\x2502';
    case down:			return L'\x2577';
    case up:			return L'\x2575';
    case right:			return L'\x2576';
    case left:			return L'\x2574';
    case down|up|right:		return L'\x251c';
    case down|up|left:		return L'\x2524';
    case down|left|right:	return L'\x252c';
    case up|left|right:		return L'\x2534';
    case down|up|left|right:	return L'\x253c';
    default:			return L' ';
    };
  } else
#endif /* HAVE_PUTWC */
  {
    switch (direction) {
    case down|right:		return '/';
    case down|left:		return '\\';
    case up|right:		return '\\';
    case up|left:		return '/';
    case left|right:		return '-';
    case down|up:		return '|';
    case down:			return '|';
    case up:			return '|';
    case right:			return '-';
    case left:			return '-';
    case down|up|right:		return '+';
    case down|up|left:		return '+';
    case down|left|right:	return '+';
    case up|left|right:		return '+';
    case down|up|left|right:	return '+';
    default:			return ' ';
    };
  }
}

/* output bars, merging with existing bars: `andnot' are removed, `or' are added */
static void
merge(struct display *disp, int x, int y, int or, int andnot, int r, int g, int b)
{
  character current;
  int directions;
  if (x >= disp->width || y >= disp->height) {
    /* fprintf(stderr, "|%x &~%x overflowed to (%d,%d)\n", or, andnot, x, y); */
    return;
  }
  current = disp->cells[y][x].c;
  directions = (to_directions(disp, current) & ~andnot) | or;
  put(disp, x, y, from_directions(disp, directions), -1, -1, -1, r, g, b);
}

/* Now we can implement the standard drawing helpers */
static void
text_box(void *output, int r, int g, int b, unsigned depth __hwloc_attribute_unused, unsigned x1, unsigned width, unsigned y1, unsigned height)
{
  struct display *disp = output;
  unsigned i, j;
  unsigned x2, y2;
  x1 /= (gridsize/2);
  width /= (gridsize/2);
  y1 /= gridsize;
  height /= gridsize;
  x2 = x1 + width - 1;
  y2 = y1 + height - 1;

  /* Corners */
  merge(disp, x1, y1, down|right, 0, r, g, b);
  merge(disp, x2, y1, down|left, 0, r, g, b);
  merge(disp, x1, y2, up|right, 0, r, g, b);
  merge(disp, x2, y2, up|left, 0, r, g, b);

  for (i = 1; i < width - 1; i++) {
    /* upper line */
    merge(disp, x1 + i, y1, left|right, down, r, g, b);
    /* lower line */
    merge(disp, x1 + i, y2, left|right, up, r, g, b);
  }
  for (j = 1; j < height - 1; j++) {
    /* left line */
    merge(disp, x1, y1 + j, up|down, right, r, g, b);
    /* right line */
    merge(disp, x2, y1 + j, up|down, left, r, g, b);
  }
  for (j = y1 + 1; j < y2; j++) {
    for (i = x1 + 1; i < x2; i++) {
      put(disp, i, j, ' ', -1, -1, -1, r, g, b);
    }
  }
}

static void
text_line(void *output, int r __hwloc_attribute_unused, int g __hwloc_attribute_unused, int b __hwloc_attribute_unused, unsigned depth __hwloc_attribute_unused, unsigned x1, unsigned y1, unsigned x2, unsigned y2)
{
  struct display *disp = output;
  unsigned i, j, z;
  x1 /= (gridsize/2);
  y1 /= gridsize;
  x2 /= (gridsize/2);
  y2 /= gridsize;

  /* Canonicalize coordinates */
  if (x1 > x2) {
    z = x1;
    x1 = x2;
    x2 = z;
  }
  if (y1 > y2) {
    z = y1;
    y1 = y2;
    y2 = z;
  }

  /* vertical/horizontal should be enough, but should mix with existing
   * characters for better output ! */

  if (x1 == x2) {
    /* Vertical */
    if (y1 == y2) {
      /* Hu ?! That's a point, let's do nothing... */
    } else {
      /* top */
      merge(disp, x1, y1, down, 0, -1, -1, -1);
      /* bottom */
      merge(disp, x1, y2, up, 0, -1, -1, -1);
    }
    for (j = y1 + 1; j < y2; j++)
      merge(disp, x1, j, down|up, 0, -1, -1, -1);
  } else if (y1 == y2) {
    /* Horizontal */
    /* left */
    merge(disp, x1, y1, right, 0, -1, -1, -1);
    /* right */
    merge(disp, x2, y1, left, 0, -1, -1, -1);
    for (i = x1 + 1; i < x2; i++)
      merge(disp, i, y1, left|right, 0, -1, -1, -1);
  } else {
    /* Unsupported, sorry */
  }
}

static void
text_text(void *output, int r, int g, int b, int size __hwloc_attribute_unused, unsigned depth __hwloc_attribute_unused, unsigned x, unsigned y, const char *text)
{
  struct display *disp = output;
  x /= (gridsize/2);
  y /= gridsize;

#if defined(HAVE_PUTWC) && !defined(__MINGW32__) && !defined(_MSC_VER)
  {
    size_t len = strlen(text) + 1;
    wchar_t *wbuf = malloc(len * sizeof(wchar_t)), *wtext;
    swprintf(wbuf, len, L"%s", text);
    for (wtext = wbuf ; *wtext; wtext++)
      put(disp, x++, y, *wtext, r, g, b, -1, -1, -1);
    free(wbuf);
  }
#else
  for ( ; *text; text++)
    put(disp, x++, y, *text, r, g, b, -1, -1, -1);
#endif
}

static struct draw_methods text_draw_methods = {
  text_start,
  text_declare_color,
  text_box,
  text_line,
  text_text,
};

void output_text(hwloc_topology_t topology, const char *filename, int overwrite, int logical, int legend, int verbose_mode __hwloc_attribute_unused)
{
  FILE *output;
  struct display *disp;
  int i, j;
  int lfr, lfg, lfb; /* Last foreground color */
  int lbr, lbg, lbb; /* Last background color */
#ifdef HWLOC_HAVE_LIBTERMCAP
  int term = 0;
  char *tmp;
#endif

  output = open_output(filename, overwrite);
  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    return;
  }

  /* Try to use utf-8 characters */
#ifdef HAVE_SETLOCALE
  setlocale(LC_ALL, "");
#endif /* HAVE_SETLOCALE */

#ifdef HWLOC_HAVE_LIBTERMCAP
  /* If we are outputing to a tty, use colors */
  if (output == stdout && isatty(STDOUT_FILENO)) {
    term = !setupterm(NULL, STDOUT_FILENO, NULL);

    if (term) {
      /* reset colors */
      if (orig_colors)
        tputs(orig_colors, 1, myputchar);

      /* Get terminfo(5) strings */
      initp = initialize_pair;
      if (max_pairs <= 16 || !initp || !set_color_pair) {
	/* Can't use max_pairs to define our own colors */
	initp = NULL;
	if (max_colors > 16)
	  if (can_change)
            initc = initialize_color;
      }
      /* Prevent a trivial compiler warning because the param of
         tgetflag is (char*), not (const char*). */
      tmp = strdup("lhs");
      if (tgetflag(tmp)) {
	/* Sorry, I'm lazy to convert colors and I don't know any terminal
	 * using LHS anyway */
	initc = initp = 0;
      }
      free(tmp);
    }
  }
#endif /* HWLOC_HAVE_LIBTERMCAP */

  disp = output_draw_start(&text_draw_methods, logical, legend, topology, output);
  output_draw(&text_draw_methods, logical, legend, topology, disp);

  lfr = lfg = lfb = -1;
  lbr = lbg = lbb = -1;
  for (j = 0; j < disp->height; j++) {
    for (i = 0; i < disp->width; i++) {
#ifdef HWLOC_HAVE_LIBTERMCAP
      if (term) {
	/* TTY output, use colors */
	int fr = disp->cells[j][i].fr;
	int fg = disp->cells[j][i].fg;
	int fb = disp->cells[j][i].fb;
	int br = disp->cells[j][i].br;
	int bg = disp->cells[j][i].bg;
	int bb = disp->cells[j][i].bb;

	/* Avoid too much work for the TTY */
	if (fr != lfr || fg != lfg || fb != lfb
	 || br != lbr || bg != lbg || bb != lbb) {
	  set_color(fr, fg, fb, br, bg, bb);
	  lfr = fr;
	  lfg = fg;
	  lfb = fb;
	  lbr = br;
	  lbg = bg;
	  lbb = bb;
	}
      }
#endif /* HWLOC_HAVE_LIBTERMCAP */
      putcharacter(disp->cells[j][i].c, output);
    }
#ifdef HWLOC_HAVE_LIBTERMCAP
    /* Keep the rest of the line as default */
    if (term && orig_pair) {
      lfr = lfg = lfb = -1;
      lbr = lbg = lbb = -1;
      tputs(orig_pair, 1, myputchar);
    }
#endif /* HWLOC_HAVE_LIBTERMCAP */
    putcharacter('\n', output);
  }

  if (output != stdout)
    fclose(output);
}
