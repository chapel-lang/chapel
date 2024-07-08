/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2021 Inria.  All rights reserved.
 * Copyright © 2009-2012, 2020 Université Bordeaux
 * Copyright © 2009-2018 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

/*
 * Pretty text output
 */

#include "private/autogen/config.h"
#include "hwloc.h"

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

#define TERM_COLOR_START 16

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
  const struct lstopo_color *fcolor, *bcolor;
#endif /* HWLOC_HAVE_LIBTERMCAP */
};

struct lstopo_ascii_output {
  struct lstopo_output *loutput;
  struct cell **cells;
  int utf8;
  /* loutput->width and ->height converted to array of chars to simplify internal management below*/
  int width;
  int height;
};

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
set_color(const struct lstopo_color *fcolor, const struct lstopo_color *bcolor)
{
  char *toput;
  int color, textcolor;

  if (initc || initp) {
    /* Can set rgb color, easy */
    textcolor = fcolor->private.ascii.color;
    color = bcolor->private.ascii.color;
  } else {
    /* Magic trigger: it seems to separate colors quite well */
    int brr = bcolor->r >= 0xe0;
    int bgg = bcolor->g >= 0xe0;
    int bbb = bcolor->b >= 0xe0;

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

#ifdef HWLOC_HAVE_LIBTERMCAP
static int ascii_color_index = TERM_COLOR_START, ascii_color_index_step = 1;
#endif
static struct lstopo_color *default_color = NULL;

/* When we can, allocate rgb colors */
static int
ascii_declare_color(struct lstopo_output *loutput __hwloc_attribute_unused, struct lstopo_color *lcolor)
{
  int r = lcolor->r, g = lcolor->g, b = lcolor->b;
#ifdef HWLOC_HAVE_LIBTERMCAP
  int rr, gg, bb;
  char *toput;

  lcolor->private.ascii.color = ascii_color_index;
  ascii_color_index += ascii_color_index_step;

  /* Yes, values seem to range from 0 to 1000 inclusive */
  rr = (r * 1001) / 256;
  gg = (g * 1001) / 256;
  bb = (b * 1001) / 256;

  if (initc) {
    if ((toput = tparm(initc, lcolor->private.ascii.color, rr, gg, bb, 0, 0, 0, 0, 0)))
      tputs(toput, 1, myputchar);
  } else if (initp) {
    if ((toput = tparm(initp, lcolor->private.ascii.color, 0, 0, 0, rr, gg, bb, 0, 0)))
      tputs(toput, 1, myputchar);
  }
#endif /* HWLOC_HAVE_LIBTERMCAP */

  if (!default_color && !r && !g && !b)
    default_color = lcolor;

  return 0;
}

/* output text, erasing any previous content */
static void
put(struct lstopo_ascii_output *disp, int x, int y, character c, const struct lstopo_color *fcolor __hwloc_attribute_unused, const struct lstopo_color *bcolor __hwloc_attribute_unused)
{
  if (x >= disp->width || y >= disp->height) {
    /* fprintf(stderr, "%"PRIchar" overflowed to (%d,%d)\n", c, x, y); */
    return;
  }
  disp->cells[y][x].c = c;
#ifdef HWLOC_HAVE_LIBTERMCAP
  if (fcolor)
    disp->cells[y][x].fcolor = fcolor;
  if (bcolor)
    disp->cells[y][x].bcolor = bcolor;
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
to_directions(struct lstopo_ascii_output *disp, character c)
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
from_directions(struct lstopo_ascii_output *disp, int direction)
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
merge(struct lstopo_ascii_output *disp, int x, int y, int or, int andnot, const struct lstopo_color *color)
{
  character current;
  int directions;
  if (x >= disp->width || y >= disp->height) {
    /* fprintf(stderr, "|%x &~%x overflowed to (%d,%d)\n", or, andnot, x, y); */
    return;
  }
  current = disp->cells[y][x].c;
  directions = (to_directions(disp, current) & ~andnot) | or;
  put(disp, x, y, from_directions(disp, directions), NULL, color);
}

/* 10 is the only allowed gridsize/fontsize/linespacing (enforced in output_ascii()).
 * vertically, it's a line of text.
 * horizontally, it's 2 chars (so that we get a space between text and boxes
 */
#define SCALE 10
/* convert generic coordinates to ASCII coordinates */
#define XSCALE(x) ((x)*2/SCALE)
#define YSCALE(y) ((y)/SCALE)
/* reverse conversion from text to generic x coordinate */
#define XSCALE_REVERSE(x) ((x)*SCALE/2)

/* Now we can implement the standard drawing helpers */
static void
ascii_box(struct lstopo_output *loutput, const struct lstopo_color *lcolor, unsigned depth __hwloc_attribute_unused, unsigned x1, unsigned width, unsigned y1, unsigned height, hwloc_obj_t obj __hwloc_attribute_unused, unsigned box_id __hwloc_attribute_unused)
{
  struct lstopo_ascii_output *disp = loutput->backend_data;
  unsigned i, j;
  unsigned x2, y2;

  x1 = XSCALE(x1);
  width = XSCALE(width);
  y1 = YSCALE(y1);
  height = YSCALE(height);
  x2 = x1 + width - 1;
  y2 = y1 + height - 1;

  /* Corners */
  merge(disp, x1, y1, down|right, 0, lcolor);
  merge(disp, x2, y1, down|left, 0, lcolor);
  merge(disp, x1, y2, up|right, 0, lcolor);
  merge(disp, x2, y2, up|left, 0, lcolor);

  for (i = 1; i < width - 1; i++) {
    /* upper line */
    merge(disp, x1 + i, y1, left|right, down, lcolor);
    /* lower line */
    merge(disp, x1 + i, y2, left|right, up, lcolor);
  }
  for (j = 1; j < height - 1; j++) {
    /* left line */
    merge(disp, x1, y1 + j, up|down, right, lcolor);
    /* right line */
    merge(disp, x2, y1 + j, up|down, left, lcolor);
  }
  for (j = y1 + 1; j < y2; j++) {
    for (i = x1 + 1; i < x2; i++) {
      put(disp, i, j, ' ', NULL, lcolor);
    }
  }
}

static void
ascii_line(struct lstopo_output *loutput, unsigned depth __hwloc_attribute_unused, unsigned x1, unsigned y1, unsigned x2, unsigned y2, hwloc_obj_t obj __hwloc_attribute_unused, unsigned line_id __hwloc_attribute_unused)
{
  struct lstopo_ascii_output *disp = loutput->backend_data;
  unsigned i, j, z;

  x1 = XSCALE(x1);
  y1 = YSCALE(y1);
  x2 = XSCALE(x2);
  y2 = YSCALE(y2);

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
      merge(disp, x1, y1, down, 0, NULL);
      /* bottom */
      merge(disp, x1, y2, up, 0, NULL);
    }
    for (j = y1 + 1; j < y2; j++)
      merge(disp, x1, j, down|up, 0, NULL);
  } else if (y1 == y2) {
    /* Horizontal */
    /* left */
    merge(disp, x1, y1, right, 0, NULL);
    /* right */
    merge(disp, x2, y1, left, 0, NULL);
    for (i = x1 + 1; i < x2; i++)
      merge(disp, i, y1, left|right, 0, NULL);
  } else {
    /* Unsupported, sorry */
  }
}

static void
ascii_text(struct lstopo_output *loutput, const struct lstopo_color *lcolor, int size __hwloc_attribute_unused, unsigned depth __hwloc_attribute_unused, unsigned x, unsigned y, const char *text, hwloc_obj_t obj __hwloc_attribute_unused, unsigned text_id __hwloc_attribute_unused)
{
  struct lstopo_ascii_output *disp = loutput->backend_data;

  x = XSCALE(x);
  y = YSCALE(y);

#if defined(HAVE_PUTWC) && !defined(__MINGW32__) && !defined(_MSC_VER)
  {
    size_t len = strlen(text) + 1;
    wchar_t *wbuf = malloc(len * sizeof(wchar_t)), *wtext;
    swprintf(wbuf, len, L"%s", text);
    for (wtext = wbuf ; *wtext; wtext++)
      put(disp, x++, y, *wtext, lcolor, NULL);
    free(wbuf);
  }
#else
  for ( ; *text; text++)
    put(disp, x++, y, *text, lcolor, NULL);
#endif
}

static void
ascii_textsize(struct lstopo_output *loutput __hwloc_attribute_unused, const char *text __hwloc_attribute_unused, unsigned textlength, unsigned fontsize __hwloc_attribute_unused, unsigned *width)
{
  *width = XSCALE_REVERSE(textlength);
}

static struct draw_methods ascii_draw_methods = {
  ascii_declare_color,
  NULL,
  ascii_box,
  ascii_line,
  ascii_text,
  ascii_textsize,
};

int
output_ascii(struct lstopo_output *loutput, const char *filename)
{
  FILE *output;
  struct lstopo_ascii_output disp;
  int i, j;
#ifdef HWLOC_HAVE_LIBTERMCAP
  const struct lstopo_color *lfcolor = NULL; /* Last foreground color */
  const struct lstopo_color *lbcolor = NULL; /* Last background color */
  int term = 0;
  char *tmp;
#endif
  int width, height;

  output = open_output(filename, loutput->overwrite);
  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    return -1;
  }

  loutput->gridsize = 10;
  loutput->fontsize = 10;
  loutput->linespacing = 10;

  /* cannot write between lines of the terminal */
  loutput->backend_flags |= LSTOPO_BACKEND_FLAG_NO_HALF_LINES;

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
      if (max_pairs <= TERM_COLOR_START || !initp || !set_color_pair) {
	/* Can't use max_pairs to define our own colors */
	initp = NULL;
	if (max_colors > TERM_COLOR_START) {
	  /* Better start overwriting last colors */
	  ascii_color_index = max_colors-1;
	  ascii_color_index_step = -1;
	  if (can_change)
            initc = initialize_color;
        }
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

  disp.loutput = loutput;
  loutput->backend_data = &disp;
  loutput->methods = &ascii_draw_methods;

  /* recurse once for preparing sizes and positions */
  loutput->drawing = LSTOPO_DRAWING_PREPARE;
  output_draw(loutput);
  width = disp.width = XSCALE(loutput->width + 1);
  height = disp.height = YSCALE(loutput->height + 1);
  loutput->drawing = LSTOPO_DRAWING_DRAW;

  /* prepare colors */
  declare_colors(loutput);
  lstopo_prepare_custom_styles(loutput);

  /* terminals usually have narrow characters, so let's make them wider */
  disp.cells = malloc(height * sizeof(*disp.cells));
  for (j = 0; j < height; j++) {
    disp.cells[j] = calloc(width, sizeof(**disp.cells));
    for (i = 0; i < width; i++) {
      disp.cells[j][i].c = ' ';
#ifdef HWLOC_HAVE_LIBTERMCAP
      disp.cells[j][i].fcolor = default_color;
      disp.cells[j][i].bcolor = default_color;
#endif
    }
  }
#ifdef HAVE_NL_LANGINFO
  disp.utf8 = !strcmp(nl_langinfo(CODESET), "UTF-8");
#endif /* HAVE_NL_LANGINFO */

  /* ready */
  output_draw(loutput);

  for (j = 0; j < disp.height; j++) {
    for (i = 0; i < disp.width; i++) {
#ifdef HWLOC_HAVE_LIBTERMCAP
      if (term) {
	/* TTY output, use colors */
	const struct lstopo_color *fcolor = disp.cells[j][i].fcolor;
	const struct lstopo_color *bcolor = disp.cells[j][i].bcolor;

	/* Avoid too much work for the TTY */
	if (fcolor != lfcolor || bcolor != lbcolor) {
	  set_color(fcolor, bcolor);
	  lfcolor = fcolor;
	  lbcolor = bcolor;
	}
      }
#endif /* HWLOC_HAVE_LIBTERMCAP */
      putcharacter(disp.cells[j][i].c, output);
    }
#ifdef HWLOC_HAVE_LIBTERMCAP
    /* Keep the rest of the line as default */
    if (term && orig_pair) {
      lfcolor = NULL;
      lbcolor = NULL;
      tputs(orig_pair, 1, myputchar);
    }
#endif /* HWLOC_HAVE_LIBTERMCAP */
    putcharacter('\n', output);
  }

  for (j = 0; j < disp.height; j++)
    free(disp.cells[j]);
  free(disp.cells);

  if (output != stdout)
    fclose(output);

  destroy_colors(loutput);
  return 0;
}
