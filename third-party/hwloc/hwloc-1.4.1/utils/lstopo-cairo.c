/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2010 inria.  All rights reserved.
 * Copyright © 2009-2010 Université Bordeaux 1
 * Copyright © 2009-2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <private/autogen/config.h>

#ifdef HWLOC_HAVE_CAIRO
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

#ifndef HWLOC_HAVE_X11
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
/* Cairo methods */
static void
topo_cairo_box(void *output, int r, int g, int b, unsigned depth __hwloc_attribute_unused, unsigned x, unsigned width, unsigned y, unsigned height)
{
  cairo_t *c = output;
  cairo_rectangle(c, x, y, width, height);
  cairo_set_source_rgb(c, (float)r / 255, (float) g / 255, (float) b / 255);
  cairo_fill(c);

  cairo_rectangle(c, x, y, width, height);
  cairo_set_source_rgb(c, 0, 0, 0);
  cairo_set_line_width(c, 1);
  cairo_stroke(c);
}

static void
topo_cairo_line(void *output, int r, int g, int b, unsigned depth __hwloc_attribute_unused, unsigned x1, unsigned y1, unsigned x2, unsigned y2)
{
  cairo_t *c = output;
  cairo_move_to(c, x1, y1);
  cairo_set_source_rgb(c, (float) r / 255, (float) g / 255, (float) b / 255);
  cairo_set_line_width(c, 1);
  cairo_line_to(c, x2, y2);
  cairo_stroke(c);
}

static void
topo_cairo_text(void *output, int r, int g, int b, int size, unsigned depth __hwloc_attribute_unused, unsigned x, unsigned y, const char *text)
{
  cairo_t *c = output;
  cairo_move_to(c, x, y + size);
  cairo_set_font_size(c, size);
  cairo_set_source_rgb(c, (float)r / 255, (float) g / 255, (float) b / 255);
  cairo_show_text(c, text);
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
topo_cairo_paint(struct draw_methods *methods, int logical, int legend, hwloc_topology_t topology, cairo_surface_t *cs)
{
  cairo_t *c;
  c = cairo_create(cs);
  output_draw(methods, logical, legend, topology, c);
  cairo_show_page(c);
  cairo_destroy(c);
}

static void null_declare_color (void *output __hwloc_attribute_unused, int r __hwloc_attribute_unused, int g __hwloc_attribute_unused, int b __hwloc_attribute_unused) {}
#endif /* (CAIRO_HAS_XLIB_SURFACE + CAIRO_HAS_PNG_FUNCTIONS + CAIRO_HAS_PDF_SURFACE + CAIRO_HAS_PS_SURFACE + CAIRO_HAS_SVG_SURFACE) */


#if CAIRO_HAS_XLIB_SURFACE
/* X11 back-end */

struct display {
  Display *dpy;
  int scr;
  cairo_surface_t *cs;
  Window top, win;
  Cursor hand;
  unsigned int orig_fontsize, orig_gridsize;
  int screen_width, screen_height;		/** visible part size */
  int last_screen_width, last_screen_height;	/** last visible part size */
  int width, height;				/** total normal display size */
  int x, y;					/** top left corner of the visible part */
};

static void
x11_create(struct display *disp, int width, int height)
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
  disp->cs = cairo_xlib_surface_create(disp->dpy, disp->win, DefaultVisual(disp->dpy, disp->scr), width, height);

}

static void
x11_destroy(struct display *disp)
{
  cairo_surface_destroy(disp->cs);
  XDestroyWindow(disp->dpy, disp->win);
}

static void *
x11_start(void *output __hwloc_attribute_unused, int width, int height)
{
  Display *dpy;
  Window root, top;
  int scr;
  Screen *screen;
  int screen_width = width, screen_height = height;
  struct display *disp;

  if (!(dpy = XOpenDisplay(NULL))) {
    fprintf(stderr, "couldn't connect to X\n");
    exit(EXIT_FAILURE);
  }

  disp = malloc(sizeof(*disp));
  disp->dpy = dpy;
  disp->scr = scr = DefaultScreen(dpy);

  screen = ScreenOfDisplay(dpy, scr);
  if (screen_width >= screen->width)
    screen_width = screen->width;
  if (screen_height >= screen->height)
    screen_height = screen->height;
  disp->screen_width = screen_width;
  disp->screen_height = screen_height;
  disp->width = width;
  disp->height = height;
  disp->orig_fontsize = fontsize;
  disp->orig_gridsize = gridsize;
  disp->x = 0;
  disp->y = 0;

  root = RootWindow(dpy, scr);
  disp->top = top = XCreateSimpleWindow(dpy, root, 0, 0, screen_width, screen_height, 0, WhitePixel(dpy, scr), WhitePixel(dpy, scr));
  XSelectInput(dpy,top, StructureNotifyMask);
  XMapWindow(dpy, top);

  x11_create(disp, width, height);

  return disp;
}

static struct draw_methods x11_draw_methods = {
  x11_start,
  null_declare_color,
  topo_cairo_box,
  topo_cairo_line,
  topo_cairo_text,
};

/** Clip coordinates of the visible part. */
static void
move_x11(struct display *disp, int logical, int legend, hwloc_topology_t topology)
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
    topo_cairo_paint(&x11_draw_methods, logical, legend, topology, disp->cs);
  }
}

void
output_x11(hwloc_topology_t topology, const char *filename __hwloc_attribute_unused, int logical, int legend, int verbose_mode __hwloc_attribute_unused)
{
  struct display *disp = output_draw_start(&x11_draw_methods, logical, legend, topology, NULL);
  int finish = 0;
  int state = 0;
  int x = 0, y = 0; /* shut warning down */
  int lastx = disp->x, lasty = disp->y;

  topo_cairo_paint(&x11_draw_methods, logical, legend, topology, disp->cs);

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
	  topo_cairo_paint(&x11_draw_methods, logical, legend, topology, disp->cs);
	break;
      case MotionNotify:
	if (state) {
	  disp->x -= e.xmotion.x_root - x;
	  disp->y -= e.xmotion.y_root - y;
	  x = e.xmotion.x_root;
	  y = e.xmotion.y_root;
          move_x11(disp, logical, legend, topology);
	}
	break;
      case ConfigureNotify:
	disp->screen_width = e.xconfigure.width;
	disp->screen_height = e.xconfigure.height;
        move_x11(disp, logical, legend, topology);
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
            move_x11(disp, logical, legend, topology);
            break;
          case XK_Right:
            disp->x += disp->screen_width/10;
            move_x11(disp, logical, legend, topology);
            break;
          case XK_Up:
            disp->y -= disp->screen_height/10;
            move_x11(disp, logical, legend, topology);
            break;
          case XK_Down:
            disp->y += disp->screen_height/10;
            move_x11(disp, logical, legend, topology);
            break;
          case XK_Page_Up:
            if (e.xkey.state & ControlMask) {
              disp->x -= disp->screen_width;
              move_x11(disp, logical, legend, topology);
            } else {
              disp->y -= disp->screen_height;
              move_x11(disp, logical, legend, topology);
            }
            break;
          case XK_Page_Down:
            if (e.xkey.state & ControlMask) {
              disp->x += disp->screen_width;
              move_x11(disp, logical, legend, topology);
            } else {
              disp->y += disp->screen_height;
              move_x11(disp, logical, legend, topology);
            }
            break;
          case XK_Home:
            disp->x = 0;
            disp->y = 0;
            move_x11(disp, logical, legend, topology);
            break;
          case XK_End:
            disp->x = INT_MAX;
            disp->y = INT_MAX;
            move_x11(disp, logical, legend, topology);
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
  free(disp);
}
#endif /* CAIRO_HAS_XLIB_SURFACE */


#if CAIRO_HAS_PNG_FUNCTIONS
/* PNG back-end */
static void *
png_start(void *output __hwloc_attribute_unused, int width, int height)
{
  return cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
}

static struct draw_methods png_draw_methods = {
  png_start,
  null_declare_color,
  topo_cairo_box,
  topo_cairo_line,
  topo_cairo_text,
};

void
output_png(hwloc_topology_t topology, const char *filename, int logical, int legend, int verbose_mode __hwloc_attribute_unused)
{
  FILE *output = open_file(filename, "w");
  cairo_surface_t *cs;

  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    return;
  }

  cs = output_draw_start(&png_draw_methods, logical, legend, topology, output);

  topo_cairo_paint(&png_draw_methods, logical, legend, topology, cs);
  cairo_surface_write_to_png_stream(cs, topo_cairo_write, output);
  cairo_surface_destroy(cs);

  if (output != stdout)
    fclose(output);
}
#endif /* CAIRO_HAS_PNG_FUNCTIONS */


#if CAIRO_HAS_PDF_SURFACE
/* PDF back-end */
static void *
pdf_start(void *output, int width, int height)
{
  return cairo_pdf_surface_create_for_stream(topo_cairo_write, output, width, height);
}

static struct draw_methods pdf_draw_methods = {
  pdf_start,
  null_declare_color,
  topo_cairo_box,
  topo_cairo_line,
  topo_cairo_text,
};

void
output_pdf(hwloc_topology_t topology, const char *filename, int logical, int legend, int verbose_mode __hwloc_attribute_unused)
{
  FILE *output = open_file(filename, "w");
  cairo_surface_t *cs;

  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    return;
  }

  cs = output_draw_start(&pdf_draw_methods, logical, legend, topology, output);

  topo_cairo_paint(&pdf_draw_methods, logical, legend, topology, cs);
  cairo_surface_flush(cs);
  cairo_surface_destroy(cs);

  if (output != stdout)
    fclose(output);
}
#endif /* CAIRO_HAS_PDF_SURFACE */


#if CAIRO_HAS_PS_SURFACE
/* PS back-end */
static void *
ps_start(void *output, int width, int height)
{
  return cairo_ps_surface_create_for_stream(topo_cairo_write, output, width, height);
}

static struct draw_methods ps_draw_methods = {
  ps_start,
  null_declare_color,
  topo_cairo_box,
  topo_cairo_line,
  topo_cairo_text,
};

void
output_ps(hwloc_topology_t topology, const char *filename, int logical, int legend, int verbose_mode __hwloc_attribute_unused)
{
  FILE *output = open_file(filename, "w");
  cairo_surface_t *cs;

  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    return;
  }

  cs = output_draw_start(&ps_draw_methods, logical, legend, topology, output);

  topo_cairo_paint(&ps_draw_methods, logical, legend, topology, cs);
  cairo_surface_flush(cs);
  cairo_surface_destroy(cs);

  if (output != stdout)
    fclose(output);
}
#endif /* CAIRO_HAS_PS_SURFACE */


#if CAIRO_HAS_SVG_SURFACE
/* SVG back-end */
static void *
svg_start(void *output, int width, int height)
{
  return cairo_svg_surface_create_for_stream(topo_cairo_write, output, width, height);
}

static struct draw_methods svg_draw_methods = {
  svg_start,
  null_declare_color,
  topo_cairo_box,
  topo_cairo_line,
  topo_cairo_text,
};

void
output_svg(hwloc_topology_t topology, const char *filename, int logical, int legend, int verbose_mode __hwloc_attribute_unused)
{
  FILE *output;
  cairo_surface_t *cs;

  output = open_file(filename, "w");
  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    return;
  }

  cs = output_draw_start(&svg_draw_methods, logical, legend, topology, output);

  topo_cairo_paint(&svg_draw_methods, logical, legend, topology, cs);
  cairo_surface_flush(cs);
  cairo_surface_destroy(cs);

  if (output != stdout)
    fclose(output);
}
#endif /* CAIRO_HAS_SVG_SURFACE */

#endif /* CAIRO */
