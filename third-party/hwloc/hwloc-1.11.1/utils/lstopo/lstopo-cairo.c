/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2015 Inria.  All rights reserved.
 * Copyright © 2009-2010, 2014 Université Bordeaux
 * Copyright © 2009-2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <private/autogen/config.h>

#include <cairo.h>

#if CAIRO_HAS_PDF_SURFACE
#include <cairo-pdf.h>
#endif /* CAIRO_HAS_PDF_SURFACE */

#if CAIRO_HAS_PS_SURFACE
#include <cairo-ps.h>
#endif /* CAIRO_HAS_PS_SURFACE */

#if CAIRO_HAS_SVG_SURFACE
#include <cairo-svg.h>
#endif /* CAIRO_HAS_SVG_SURFACE */

#ifndef HWLOC_HAVE_X11_KEYSYM
/* In case X11 headers aren't availble, forcefully disable Cairo/Xlib.  */
# undef CAIRO_HAS_XLIB_SURFACE
# define CAIRO_HAS_XLIB_SURFACE 0
#endif

#if CAIRO_HAS_XLIB_SURFACE
#include <cairo-xlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>
/* Avoid Xwindow's definition conflict with Windows' use for fields names.  */
#undef Status
#endif /* CAIRO_HAS_XLIB_SURFACE */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "lstopo.h"

#if (CAIRO_HAS_XLIB_SURFACE + CAIRO_HAS_PNG_FUNCTIONS + CAIRO_HAS_PDF_SURFACE + CAIRO_HAS_PS_SURFACE + CAIRO_HAS_SVG_SURFACE)
struct lstopo_cairo_output {
  struct lstopo_output loutput; /* must be at the beginning */
  cairo_surface_t *surface;
  cairo_t *context;
  unsigned max_x;
  unsigned max_y;
  int drawing;
};

/* Cairo methods */
static void
topo_cairo_box(void *_output, int r, int g, int b, unsigned depth __hwloc_attribute_unused, unsigned x, unsigned width, unsigned y, unsigned height)
{
  struct lstopo_cairo_output *coutput = _output;
  cairo_t *c = coutput->context;

  if (!coutput->drawing) {
    if (x > coutput->max_x)
      coutput->max_x = x;
    if (x + width > coutput->max_x)
      coutput->max_x = x + width;
    if (y > coutput->max_y)
      coutput->max_y = y;
    if (y + height > coutput->max_y)
      coutput->max_y = y + height;
    return;
  }

  cairo_rectangle(c, x, y, width, height);
  cairo_set_source_rgb(c, (float)r / 255, (float) g / 255, (float) b / 255);
  cairo_fill(c);

  cairo_rectangle(c, x, y, width, height);
  cairo_set_source_rgb(c, 0, 0, 0);
  cairo_set_line_width(c, 1);
  cairo_stroke(c);
}

static void
topo_cairo_line(void *_output, int r, int g, int b, unsigned depth __hwloc_attribute_unused, unsigned x1, unsigned y1, unsigned x2, unsigned y2)
{
  struct lstopo_cairo_output *coutput = _output;
  cairo_t *c = coutput->context;

  if (!coutput->drawing) {
    if (x1 > coutput->max_x)
      coutput->max_x = x1;
    if (x2 > coutput->max_x)
      coutput->max_x = x2;
    if (y1 > coutput->max_y)
      coutput->max_y = y1;
    if (y2 > coutput->max_y)
      coutput->max_y = y2;
    return;
  }

  cairo_move_to(c, x1, y1);
  cairo_set_source_rgb(c, (float) r / 255, (float) g / 255, (float) b / 255);
  cairo_set_line_width(c, 1);
  cairo_line_to(c, x2, y2);
  cairo_stroke(c);
}

static void
topo_cairo_text(void *_output, int r, int g, int b, int fontsize, unsigned depth __hwloc_attribute_unused, unsigned x, unsigned y, const char *text)
{
  struct lstopo_cairo_output *coutput = _output;
  cairo_t *c = coutput->context;

  if (!coutput->drawing)
    return;

  cairo_move_to(c, x, y + fontsize);
  cairo_set_source_rgb(c, (float)r / 255, (float) g / 255, (float) b / 255);
  cairo_show_text(c, text);
}

static void
topo_cairo_textsize(void *_output, const char *text, unsigned textlength __hwloc_attribute_unused, unsigned fontsize __hwloc_attribute_unused, unsigned *width)
{
  struct lstopo_cairo_output *coutput = _output;
  cairo_t *c = coutput->context;
  cairo_text_extents_t extents;
  cairo_text_extents(c, text, &extents);
  *width = extents.width;
}


#if (CAIRO_HAS_PNG_FUNCTIONS + CAIRO_HAS_PDF_SURFACE + CAIRO_HAS_PS_SURFACE + CAIRO_HAS_SVG_SURFACE)
static cairo_status_t
topo_cairo_write(void *closure, const unsigned char *data, unsigned int length)
{
  if (fwrite(data, length, 1, closure) < 1)
    return CAIRO_STATUS_WRITE_ERROR;
  return CAIRO_STATUS_SUCCESS;
}
#endif /* (CAIRO_HAS_PNG_FUNCTIONS + CAIRO_HAS_PDF_SURFACE + CAIRO_HAS_PS_SURFACE + CAIRO_HAS_SVG_SURFACE) */

static void
topo_cairo_paint(struct lstopo_cairo_output *coutput)
{
  cairo_surface_t *cs = coutput->surface;
  cairo_t *c = cairo_create(cs);
  coutput->context = c;
  cairo_set_font_size(c, fontsize);
  output_draw(&coutput->loutput);
  cairo_show_page(c);
  cairo_destroy(c);
  coutput->context = NULL;
}

static void null_declare_color (void *output __hwloc_attribute_unused, int r __hwloc_attribute_unused, int g __hwloc_attribute_unused, int b __hwloc_attribute_unused) {}
#endif /* (CAIRO_HAS_XLIB_SURFACE + CAIRO_HAS_PNG_FUNCTIONS + CAIRO_HAS_PDF_SURFACE + CAIRO_HAS_PS_SURFACE + CAIRO_HAS_SVG_SURFACE) */


#if CAIRO_HAS_XLIB_SURFACE
/* X11 back-end */
static struct draw_methods x11_draw_methods;

struct lstopo_x11_output {
  struct lstopo_cairo_output coutput; /* must be at the beginning */
  Display *dpy;
  int scr;
  Window top, win;
  Cursor hand;
  unsigned int orig_fontsize, orig_gridsize;
  int screen_width, screen_height;		/** visible part size */
  int last_screen_width, last_screen_height;	/** last visible part size */
  int width, height;				/** total normal display size */
  int x, y;					/** top left corner of the visible part */
};

static void
x11_create(struct lstopo_x11_output *disp, int width, int height)
{
  disp->win = XCreateSimpleWindow(disp->dpy, disp->top, 0, 0, width, height, 0, WhitePixel(disp->dpy, disp->scr), WhitePixel(disp->dpy, disp->scr));
  disp->hand = XCreateFontCursor(disp->dpy, XC_fleur);
  XDefineCursor(disp->dpy, disp->win, disp->hand);
  XSelectInput(disp->dpy, disp->win,
      KeyPressMask |
      ButtonPressMask | ButtonReleaseMask |
      PointerMotionMask |
      ExposureMask);
  XMapWindow(disp->dpy, disp->win);
  disp->coutput.surface = cairo_xlib_surface_create(disp->dpy, disp->win, DefaultVisual(disp->dpy, disp->scr), width, height);
}

static void
x11_destroy(struct lstopo_x11_output *disp)
{
  cairo_surface_destroy(disp->coutput.surface);
  XDestroyWindow(disp->dpy, disp->win);
}

static void
x11_init(void *_disp)
{
  struct lstopo_x11_output *disp = _disp;
  struct lstopo_cairo_output *coutput = &disp->coutput;
  Display *dpy;
  Window root, top;
  int scr;
  Screen *screen;
  int screen_width, screen_height;

  /* create the toplevel window */
  if (!(dpy = XOpenDisplay(NULL))) {
    fprintf(stderr, "couldn't connect to X\n");
    exit(EXIT_FAILURE);
  }

  disp->dpy = dpy;
  disp->scr = scr = DefaultScreen(dpy);
  screen = ScreenOfDisplay(dpy, scr);

  /* compute the maximal needed size using the root window */
  root = RootWindow(dpy, scr);
  disp->top = root;
  coutput->drawing = 0;
  x11_create(disp, 1, 1);
  topo_cairo_paint(coutput);
  x11_destroy(disp);
  coutput->drawing = 1;

  /* now create the actual window with the computed max size */
  screen_width = coutput->max_x;
  screen_height = coutput->max_y;

  disp->top = top = XCreateSimpleWindow(dpy, root, 0, 0, screen_width, screen_height, 0, WhitePixel(dpy, scr), WhitePixel(dpy, scr));
  XStoreName(dpy, top, "lstopo");
  XSetIconName(dpy, top, "lstopo");
  XSelectInput(dpy,top, StructureNotifyMask);

  if (screen_width >= screen->width)
    screen_width = screen->width;
  if (screen_height >= screen->height)
    screen_height = screen->height;
  disp->screen_width = screen_width;
  disp->screen_height = screen_height;
  disp->width = coutput->max_x;
  disp->height = coutput->max_y;
  disp->orig_fontsize = fontsize;
  disp->orig_gridsize = gridsize;
  disp->x = 0;
  disp->y = 0;

  x11_create(disp, coutput->max_x, coutput->max_y);

  XMapWindow(dpy, top);
}

static struct draw_methods x11_draw_methods = {
  x11_init,
  null_declare_color,
  topo_cairo_box,
  topo_cairo_line,
  topo_cairo_text,
  topo_cairo_textsize,
};

/** Clip coordinates of the visible part. */
static void
move_x11(struct lstopo_x11_output *disp)
{
  if (disp->width <= disp->screen_width) {
    disp->x = 0;
  } else {
    if (disp->x < 0)
      disp->x = 0;
    if (disp->x >= disp->width - disp->screen_width)
      disp->x = disp->width - disp->screen_width;
  }

  if (disp->height <= disp->screen_height) {
    disp->y = 0;
  } else {
    if (disp->y < 0)
      disp->y = 0;
    if (disp->y >= disp->height - disp->screen_height)
      disp->y = disp->height - disp->screen_height;
  }

 if (disp->screen_width > disp->width && disp->screen_height > disp->height
   && (disp->screen_width != disp->last_screen_width
   || disp->screen_height != disp->last_screen_height)) {
    disp->last_screen_width = disp->screen_width;
    disp->last_screen_height = disp->screen_height;
    fontsize = disp->orig_fontsize;
    gridsize = disp->orig_gridsize;
    if (disp->screen_width > disp->width) {
      fontsize = disp->orig_fontsize * disp->screen_width / disp->width;
      gridsize = disp->orig_gridsize * disp->screen_width / disp->width;
    }
    if (disp->screen_height > disp->height) {
      unsigned int new_fontsize = disp->orig_fontsize * disp->screen_height / disp->height;
      unsigned int new_gridsize = disp->orig_gridsize * disp->screen_height / disp->height;
      if (new_fontsize < fontsize)
	fontsize = new_fontsize;
      if (new_gridsize < gridsize)
	gridsize = new_gridsize;
    }

    x11_destroy(disp);
    x11_create(disp, disp->screen_width, disp->screen_height);
    topo_cairo_paint(&disp->coutput);
  }
}

void
output_x11(struct lstopo_output *loutput, const char *filename)
{
  struct lstopo_x11_output _disp, *disp = &_disp;
  struct lstopo_cairo_output *coutput;
  int finish = 0;
  int state = 0;
  int x = 0, y = 0; /* shut warning down */
  int lastx, lasty;

  coutput = &disp->coutput;
  memset(coutput, 0, sizeof(*coutput));
  memcpy(&coutput->loutput, loutput, sizeof(*loutput));
  coutput->loutput.methods = &x11_draw_methods;

  output_draw_start(&coutput->loutput);
  lastx = disp->x;
  lasty = disp->y;

  topo_cairo_paint(coutput);

  while (!finish) {
    XEvent e;
    if (!XEventsQueued(disp->dpy, QueuedAfterFlush)) {
      /* No pending event, flush moving windows before waiting for next event */
      if (disp->x != lastx || disp->y != lasty) {
	XMoveWindow(disp->dpy, disp->win, -disp->x, -disp->y);
	lastx = disp->x;
	lasty = disp->y;
      }
    }
    XNextEvent(disp->dpy, &e);
    switch (e.type) {
      case Expose:
	if (e.xexpose.count < 1)
	  topo_cairo_paint(coutput);
	break;
      case MotionNotify:
	if (state) {
	  disp->x -= e.xmotion.x_root - x;
	  disp->y -= e.xmotion.y_root - y;
	  x = e.xmotion.x_root;
	  y = e.xmotion.y_root;
          move_x11(disp);
	}
	break;
      case ConfigureNotify:
	disp->screen_width = e.xconfigure.width;
	disp->screen_height = e.xconfigure.height;
        move_x11(disp);
	if (disp->x != lastx || disp->y != lasty)
	  XMoveWindow(disp->dpy, disp->win, -disp->x, -disp->y);
	break;
      case ButtonPress:
	  if (e.xbutton.button == Button1) {
	  state = 1;
	  x = e.xbutton.x_root;
	  y = e.xbutton.y_root;
	}
	break;
      case ButtonRelease:
	if (e.xbutton.button == Button1)
	  state = 0;
	break;
      case MappingNotify:
	XRefreshKeyboardMapping(&e.xmapping);
	break;
      case KeyPress: {
	KeySym keysym;
	XLookupString(&e.xkey, NULL, 0, &keysym, NULL);
        switch (keysym) {
          case XK_q:
          case XK_Q:
          case XK_Escape:
            finish = 1;
            break;
          case XK_Left:
            disp->x -= disp->screen_width/10;
            move_x11(disp);
            break;
          case XK_Right:
            disp->x += disp->screen_width/10;
            move_x11(disp);
            break;
          case XK_Up:
            disp->y -= disp->screen_height/10;
            move_x11(disp);
            break;
          case XK_Down:
            disp->y += disp->screen_height/10;
            move_x11(disp);
            break;
          case XK_Page_Up:
            if (e.xkey.state & ControlMask) {
              disp->x -= disp->screen_width;
              move_x11(disp);
            } else {
              disp->y -= disp->screen_height;
              move_x11(disp);
            }
            break;
          case XK_Page_Down:
            if (e.xkey.state & ControlMask) {
              disp->x += disp->screen_width;
              move_x11(disp);
            } else {
              disp->y += disp->screen_height;
              move_x11(disp);
            }
            break;
          case XK_Home:
            disp->x = 0;
            disp->y = 0;
            move_x11(disp);
            break;
          case XK_End:
            disp->x = INT_MAX;
            disp->y = INT_MAX;
            move_x11(disp);
            break;
        }
	break;
      }
    }
  }
  x11_destroy(disp);
  XDestroyWindow(disp->dpy, disp->top);
  XFreeCursor(disp->dpy, disp->hand);
  XCloseDisplay(disp->dpy);
}
#endif /* CAIRO_HAS_XLIB_SURFACE */


#if CAIRO_HAS_PNG_FUNCTIONS
/* PNG back-end */
static struct draw_methods png_draw_methods;

static void
png_init(void *_coutput)
{
  struct lstopo_cairo_output *coutput = _coutput;
  cairo_surface_t *fakecs, *cs;

  /* create a fake surface */
  fakecs = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1, 1);
  coutput->surface = fakecs;

  /* compute the maximal size using the fake surface */
  coutput->drawing = 0;
  topo_cairo_paint(coutput);
  coutput->drawing = 1;
  cairo_surface_destroy(fakecs);

  /* create the actual surface with the right size */
  cs = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, coutput->max_x, coutput->max_y);
  coutput->surface = cs;
}

static struct draw_methods png_draw_methods = {
  png_init,
  null_declare_color,
  topo_cairo_box,
  topo_cairo_line,
  topo_cairo_text,
  topo_cairo_textsize,
};

void
output_png(struct lstopo_output *loutput, const char *filename)
{
  struct lstopo_cairo_output coutput;
  FILE *output;

  output = open_output(filename, loutput->overwrite);
  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    return;
  }

  memset(&coutput, 0, sizeof(coutput));
  memcpy(&coutput.loutput, loutput, sizeof(*loutput));
  coutput.loutput.file = output;
  coutput.loutput.methods = &png_draw_methods;

  output_draw_start(&coutput.loutput);

  topo_cairo_paint(&coutput);
  cairo_surface_write_to_png_stream(coutput.surface, topo_cairo_write, output);
  cairo_surface_destroy(coutput.surface);

  if (output != stdout)
    fclose(output);
}
#endif /* CAIRO_HAS_PNG_FUNCTIONS */


#if CAIRO_HAS_PDF_SURFACE
/* PDF back-end */
static struct draw_methods pdf_draw_methods;

static void
pdf_init(void *_coutput)
{
  struct lstopo_cairo_output *coutput = _coutput;
  cairo_surface_t *fakecs, *cs;

  /* create a fake surface */
  fakecs = cairo_pdf_surface_create_for_stream(NULL, NULL, 1, 1);
  coutput->surface = fakecs;

  /* compute the maximal size using the fake surface */
  coutput->drawing = 0;
  topo_cairo_paint(coutput);
  coutput->drawing = 1;
  cairo_surface_destroy(fakecs);

  /* create the actual surface with the right size */
  cs = cairo_pdf_surface_create_for_stream(topo_cairo_write, coutput->loutput.file, coutput->max_x, coutput->max_y);
  coutput->surface = cs;
}

static struct draw_methods pdf_draw_methods = {
  pdf_init,
  null_declare_color,
  topo_cairo_box,
  topo_cairo_line,
  topo_cairo_text,
  topo_cairo_textsize,
};

void
output_pdf(struct lstopo_output *loutput, const char *filename)
{
  struct lstopo_cairo_output coutput;
  FILE *output;

  output = open_output(filename, loutput->overwrite);
  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    return;
  }

  memset(&coutput, 0, sizeof(coutput));
  memcpy(&coutput.loutput, loutput, sizeof(*loutput));
  coutput.loutput.file = output;
  coutput.loutput.methods = &pdf_draw_methods;

  output_draw_start(&coutput.loutput);

  topo_cairo_paint(&coutput);
  cairo_surface_flush(coutput.surface);
  cairo_surface_destroy(coutput.surface);

  if (output != stdout)
    fclose(output);
}
#endif /* CAIRO_HAS_PDF_SURFACE */


#if CAIRO_HAS_PS_SURFACE
/* PS back-end */
static struct draw_methods ps_draw_methods;

static void
ps_init(void *_coutput)
{
  struct lstopo_cairo_output *coutput = _coutput;
  cairo_surface_t *fakecs, *cs;

  /* create a fake surface */
  fakecs = cairo_ps_surface_create_for_stream(NULL, NULL, 1, 1);
  coutput->surface = fakecs;

  /* compute the maximal size using the fake surface */
  coutput->drawing = 0;
  topo_cairo_paint(coutput);
  coutput->drawing = 1;
  cairo_surface_destroy(fakecs);

  /* create the actual surface with the right size */
  cs = cairo_ps_surface_create_for_stream(topo_cairo_write, coutput->loutput.file, coutput->max_x, coutput->max_y);
  coutput->surface = cs;
}

static struct draw_methods ps_draw_methods = {
  ps_init,
  null_declare_color,
  topo_cairo_box,
  topo_cairo_line,
  topo_cairo_text,
  topo_cairo_textsize,
};

void
output_ps(struct lstopo_output *loutput, const char *filename)
{
  struct lstopo_cairo_output coutput;
  FILE *output;

  output = open_output(filename, loutput->overwrite);
  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    return;
  }

  memset(&coutput, 0, sizeof(coutput));
  memcpy(&coutput.loutput, loutput, sizeof(*loutput));
  coutput.loutput.file = output;
  coutput.loutput.methods = &ps_draw_methods;

  output_draw_start(&coutput.loutput);

  topo_cairo_paint(&coutput);
  cairo_surface_flush(coutput.surface);
  cairo_surface_destroy(coutput.surface);

  if (output != stdout)
    fclose(output);
}
#endif /* CAIRO_HAS_PS_SURFACE */


#if CAIRO_HAS_SVG_SURFACE
/* SVG back-end */
static struct draw_methods svg_draw_methods;

static void
svg_init(void *_coutput)
{
  struct lstopo_cairo_output *coutput = _coutput;
  cairo_surface_t *fakecs, *cs;

  /* create a fake surface */
  fakecs = cairo_svg_surface_create_for_stream(NULL, NULL, 1, 1);
  coutput->surface = fakecs;

  /* compute the maximal size using the fake surface */
  coutput->drawing = 0;
  topo_cairo_paint(coutput);
  coutput->drawing = 1;
  cairo_surface_destroy(fakecs);

  /* create the actual surface with the right size */
  cs = cairo_svg_surface_create_for_stream(topo_cairo_write, coutput->loutput.file, coutput->max_x, coutput->max_y);
  coutput->surface = cs;
}

static struct draw_methods svg_draw_methods = {
  svg_init,
  null_declare_color,
  topo_cairo_box,
  topo_cairo_line,
  topo_cairo_text,
  topo_cairo_textsize,
};

void
output_svg(struct lstopo_output *loutput, const char *filename)
{
  struct lstopo_cairo_output coutput;
  FILE *output;

  output = open_output(filename, loutput->overwrite);
  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    return;
  }

  memset(&coutput, 0, sizeof(coutput));
  memcpy(&coutput.loutput, loutput, sizeof(*loutput));
  coutput.loutput.file = output;
  coutput.loutput.methods = &svg_draw_methods;

  output_draw_start(&coutput.loutput);

  topo_cairo_paint(&coutput);
  cairo_surface_flush(coutput.surface);
  cairo_surface_destroy(coutput.surface);

  if (output != stdout)
    fclose(output);
}
#endif /* CAIRO_HAS_SVG_SURFACE */
