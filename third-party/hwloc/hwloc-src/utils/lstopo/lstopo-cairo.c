/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2021 Inria.  All rights reserved.
 * Copyright © 2009-2010, 2014, 2017, 2020 Université Bordeaux
 * Copyright © 2009-2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "private/autogen/config.h"

#include <cairo.h>

#ifdef CAIRO_HAS_PDF_SURFACE
#include <cairo-pdf.h>
#endif /* CAIRO_HAS_PDF_SURFACE */

#ifdef CAIRO_HAS_PS_SURFACE
#include <cairo-ps.h>
#endif /* CAIRO_HAS_PS_SURFACE */

#ifdef CAIRO_HAS_SVG_SURFACE
#include <cairo-svg.h>
#endif /* CAIRO_HAS_SVG_SURFACE */

#ifdef LSTOPO_HAVE_X11
/* configure should enable X11 only if Cairo has XLIB SURFACE and there are X11 headers */
# ifndef HWLOC_HAVE_X11_KEYSYM
#  error LSTOPO_HAVE_X11 enabled without HWLOC_HAVE_X11_KEYSYM?!
# endif
# ifndef CAIRO_HAS_XLIB_SURFACE
#  error LSTOPO_HAVE_X11 enabled without CAIRO_HAS_XLIB_SURFACE?!
# endif
# include <cairo-xlib.h>
# include <X11/Xlib.h>
# include <X11/Xutil.h>
# include <X11/Xresource.h>
# include <X11/keysym.h>
# include <X11/cursorfont.h>
/* Avoid Xwindow's definition conflict with Windows' use for fields names.  */
# undef Status
#else /* LSTOPO_HAVE_X11 */
# if (defined HWLOC_HAVE_X11_KEYSYM) && (defined CAIRO_HAS_XLIB_SURFACE)
#  error HWLOC_HAVE_X11_KEYSYM and CAIRO_HAS_XLIB_SURFACE defined but LSTOPO_HAVE_X11 not enabled?!
# endif
#endif /* LSTOPO_HAVE_X11 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "lstopo.h"

#if (defined LSTOPO_HAVE_X11) + (defined CAIRO_HAS_PNG_FUNCTIONS) + (defined CAIRO_HAS_PDF_SURFACE) + (defined CAIRO_HAS_PS_SURFACE) + (defined CAIRO_HAS_SVG_SURFACE)
struct lstopo_cairo_output {
  struct lstopo_output *loutput;
  cairo_surface_t *surface;
  cairo_t *context;
};

/* Cairo methods */
static void
topo_cairo_box(struct lstopo_output *loutput, const struct lstopo_color *lcolor, unsigned depth __hwloc_attribute_unused, unsigned x, unsigned width, unsigned y, unsigned height, hwloc_obj_t obj, unsigned box_id __hwloc_attribute_unused)
{
  struct lstopo_cairo_output *coutput = loutput->backend_data;
  unsigned cpukind_style = lstopo_obj_cpukind_style(loutput, obj);
  cairo_t *c = coutput->context;
  int r = lcolor->r, g = lcolor->g, b = lcolor->b;

  cairo_rectangle(c, x, y, width, height);
  cairo_set_source_rgb(c, (float)r / 255, (float) g / 255, (float) b / 255);
  cairo_fill(c);

  cairo_rectangle(c, x, y, width, height);
  cairo_set_source_rgb(c, 0, 0, 0);

  if (cpukind_style) {
    double dash = (double)(1U << cpukind_style);
    cairo_set_dash(c, &dash, 1, 0);
    cairo_set_line_width(c, loutput->thickness * (1 + cpukind_style));
  }

  cairo_stroke(c);

  if (cpukind_style) {
    cairo_set_dash(c, NULL, 0, 0);
    cairo_set_line_width(c, loutput->thickness);
  }
}

static void
topo_cairo_line(struct lstopo_output *loutput, unsigned depth __hwloc_attribute_unused, unsigned x1, unsigned y1, unsigned x2, unsigned y2, hwloc_obj_t obj __hwloc_attribute_unused, unsigned line_id __hwloc_attribute_unused)
{
  struct lstopo_cairo_output *coutput = loutput->backend_data;
  cairo_t *c = coutput->context;

  cairo_move_to(c, x1, y1);
  cairo_set_source_rgb(c, 0, 0, 0);
  cairo_line_to(c, x2, y2);
  cairo_stroke(c);
}

static void
topo_cairo_text(struct lstopo_output *loutput, const struct lstopo_color *lcolor, int fontsize, unsigned depth __hwloc_attribute_unused, unsigned x, unsigned y, const char *text, hwloc_obj_t obj __hwloc_attribute_unused, unsigned text_id __hwloc_attribute_unused)
{
  struct lstopo_cairo_output *coutput = loutput->backend_data;
  unsigned cpukind_style = lstopo_obj_cpukind_style(loutput, obj);
  cairo_t *c = coutput->context;
  int r = lcolor->r, g = lcolor->g, b = lcolor->b;

  cairo_move_to(c, x, y + fontsize);
  cairo_set_source_rgb(c, (float)r / 255, (float) g / 255, (float) b / 255);

  if (cpukind_style % 2)
    cairo_select_font_face(c, "default", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

  cairo_show_text(c, text);

  if (cpukind_style % 2)
    cairo_select_font_face(c, "default", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
}

static void
topo_cairo_textsize(struct lstopo_output *loutput, const char *text, unsigned textlength __hwloc_attribute_unused, unsigned fontsize __hwloc_attribute_unused, unsigned *width)
{
  struct lstopo_cairo_output *coutput = loutput->backend_data;
  cairo_t *c = coutput->context;
  cairo_text_extents_t extents;
  cairo_text_extents(c, text, &extents);
  *width = extents.width;
}

#if (defined CAIRO_HAS_PNG_FUNCTIONS) + (defined CAIRO_HAS_PDF_SURFACE) + (defined CAIRO_HAS_PS_SURFACE) + (defined CAIRO_HAS_SVG_SURFACE)
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
  unsigned fontsize = coutput->loutput->fontsize;
  cairo_surface_t *cs = coutput->surface;
  cairo_t *c = cairo_create(cs);
  coutput->context = c;
  cairo_set_font_size(c, fontsize);
  cairo_set_line_width(c, coutput->loutput->thickness);
  output_draw(coutput->loutput);
  cairo_show_page(c);
  cairo_destroy(c);
  coutput->context = NULL;
}



#endif /* (LSTOPO_HAVE_X11 + CAIRO_HAS_PNG_FUNCTIONS + CAIRO_HAS_PDF_SURFACE + CAIRO_HAS_PS_SURFACE + CAIRO_HAS_SVG_SURFACE) */


#ifdef LSTOPO_HAVE_X11
/* X11 back-end */

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
  float scale, last_scale;
  int needs_redraw;
  int needs_resize;
  int auto_resize;
  int maximized;
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

static int
x11_is_maximized(struct lstopo_x11_output *disp)
{
  Atom type, state, maxed_h, maxed_v;
  int format;
  unsigned char *props;
  unsigned long i, nr, bytesAfter;
  int got_maxed_h = 0, got_maxed_v = 0;

  state = XInternAtom(disp->dpy, "_NET_WM_STATE", True);
  props = NULL;
  if (Success == XGetWindowProperty(disp->dpy, disp->top, state, 0, (~0L), False, AnyPropertyType, &type, &format, &nr, &bytesAfter, &props)) {
    maxed_h = XInternAtom(disp->dpy, "_NET_WM_STATE_MAXIMIZED_VERT", True);
    maxed_v = XInternAtom(disp->dpy, "_NET_WM_STATE_MAXIMIZED_HORZ", True);
    for(i=0; i<nr; i++) {
      Atom prop = ((Atom*)(props))[i];
      if (prop == maxed_h)
        got_maxed_h = 1;
      else if (prop == maxed_v)
        got_maxed_v = 1;
    }
    XFree(props);
  }
  /* if the window is maximized in a single dimension, zoom in/out will unmaximize it */
  return got_maxed_h && got_maxed_v;
}

/** Clip coordinates of the visible part. */
static void
move_x11(struct lstopo_x11_output *disp)
{
  struct lstopo_cairo_output *coutput = &disp->coutput;
  struct lstopo_output *loutput = coutput->loutput;

  if (disp->scale != disp->last_scale) {
    disp->x = disp->scale / disp->last_scale * (float)disp->x;
    disp->y = disp->scale / disp->last_scale * (float)disp->y;
  }

  if (disp->needs_redraw
      || disp->screen_width != disp->last_screen_width
      || disp->screen_height != disp->last_screen_height
      || disp->scale != disp->last_scale) {
    disp->last_screen_width = disp->screen_width;
    disp->last_screen_height = disp->screen_height;
    disp->last_scale = disp->scale;
    loutput->fontsize = disp->orig_fontsize * disp->scale;
    loutput->gridsize = disp->orig_gridsize * disp->scale;

    x11_destroy(disp);

    x11_create(disp, disp->width, disp->height);
    /* recompute new sizes and positions */
    loutput->drawing = LSTOPO_DRAWING_PREPARE;
    topo_cairo_paint(coutput);
    if (loutput->width > (unsigned) disp->width || loutput->height > (unsigned) disp->height) {
      /* need to extend the window before actual redraw */
      x11_destroy(disp);
      x11_create(disp, loutput->width, loutput->height);
    }
    loutput->drawing = LSTOPO_DRAWING_DRAW;
    topo_cairo_paint(coutput);
    disp->width = loutput->width;
    disp->height = loutput->height;
    disp->needs_redraw = 0;
  }

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

  if (disp->needs_resize >= 1 && !disp->maximized) {
    if (disp->auto_resize || disp->needs_resize >= 2) {
      disp->last_screen_width = disp->screen_width = disp->width;
      disp->last_screen_height = disp->screen_height = disp->height;
      disp->x = 0;
      disp->y = 0;
      XResizeWindow(disp->dpy, disp->top, disp->screen_width, disp->screen_height);
    }
    disp->needs_resize = 0;
  }
}

static struct draw_methods x11_draw_methods = {
  NULL,
  NULL,
  topo_cairo_box,
  topo_cairo_line,
  topo_cairo_text,
  topo_cairo_textsize,
};

int
output_x11(struct lstopo_output *loutput, const char *dummy __hwloc_attribute_unused)
{
  struct lstopo_x11_output _disp, *disp = &_disp;
  struct lstopo_cairo_output *coutput;
  Display *dpy;
  Window root, top;
  XClassHint *class_hint;
  int scr;
  Screen *screen;
  int screen_width, screen_height;
  unsigned int dpi_x, dpi_y, dpi = 0;
  int finish = 0;
  int state = 0;
  int x = 0, y = 0; /* shut warning down */
  int lastx, lasty;
  char *resources;

  coutput = &disp->coutput;
  memset(coutput, 0, sizeof(*coutput));
  coutput->loutput = loutput;
  loutput->backend_data = coutput;
  loutput->methods = &x11_draw_methods;

  /* create the toplevel window */
  if (!(dpy = XOpenDisplay(NULL))) {
    fprintf(stderr, "couldn't connect to X\n");
    return -1;
  }

  XrmInitialize();

  disp->dpy = dpy;
  disp->scr = scr = DefaultScreen(dpy);
  screen = ScreenOfDisplay(dpy, scr);

  /* Get DPI from xft, most often configured properly by users */
  resources = XResourceManagerString(dpy);
  if (resources) {
    XrmDatabase database = XrmGetStringDatabase(resources);
    char *type;
    XrmValue value;

    if (XrmGetResource(database, "Xft.dpi", "Xft.dpi", &type, &value))
      if (type && strcmp(type, "String") == 0)
        dpi = atoi(value.addr);

    XrmDestroyDatabase(database);
  }

  if (dpi == 0) {
    /* Fallback to value set by X server, but very often hardwired to 96dpi :/ */
    /* 25.4mm per inch */
    dpi_x = ((double) DisplayWidth(dpy, scr) * 25.4) / DisplayWidthMM(dpy, scr);
    dpi_y = ((double) DisplayHeight(dpy, scr) * 25.4) / DisplayHeightMM(dpy, scr);
    dpi = (dpi_x + dpi_y) / 2;
  }

  /* Original values for fontsize/gridsize were tuned for 96dpi */
  coutput->loutput->fontsize = (coutput->loutput->fontsize * dpi) / 96;
  coutput->loutput->gridsize = (coutput->loutput->gridsize * dpi) / 96;

  /* recurse once for preparing sizes and positions using the root window surface */
  root = RootWindow(dpy, scr);
  disp->top = root;
  loutput->drawing = LSTOPO_DRAWING_PREPARE;
  x11_create(disp, 1, 1);
  topo_cairo_paint(coutput);
  x11_destroy(disp);
  loutput->drawing = LSTOPO_DRAWING_DRAW;

  /* now create the actual window with the computed max size */
  screen_width = loutput->width;
  screen_height = loutput->height;

  disp->top = top = XCreateSimpleWindow(dpy, root, 0, 0, screen_width, screen_height, 0, WhitePixel(dpy, scr), WhitePixel(dpy, scr));
  XStoreName(dpy, top, loutput->title);
  XSetIconName(dpy, top, "lstopo");
  class_hint = XAllocClassHint();
  if (class_hint) {
    class_hint->res_name = class_hint->res_class = (char *) "lstopo";
    XSetClassHint(dpy, top, class_hint);
    XFree(class_hint);
  }
  XSelectInput(dpy,top, StructureNotifyMask | KeyPressMask);

  if (screen_width >= screen->width)
    screen_width = screen->width;
  if (screen_height >= screen->height)
    screen_height = screen->height;
  disp->last_screen_width = 0;
  disp->last_screen_height = 0;
  disp->screen_width = screen_width;
  disp->screen_height = screen_height;
  disp->width = loutput->width;
  disp->height = loutput->height;
  disp->orig_fontsize = loutput->fontsize;
  disp->orig_gridsize = loutput->gridsize;
  disp->x = 0;
  disp->y = 0;
  disp->scale = disp->last_scale = 1.0f;
  /* TODO: if window got truncated, scale down? */
  disp->needs_redraw = 0;
  disp->needs_resize = 0;
  disp->auto_resize = 1;
  disp->maximized = 0;

  x11_create(disp, loutput->width, loutput->height);

  XMapWindow(dpy, top);

  if (!loutput->refreshing)
    lstopo_show_interactive_help();

  /* ready */
  declare_colors(loutput);
  lstopo_prepare_custom_styles(loutput);

  lastx = disp->x;
  lasty = disp->y;

  topo_cairo_paint(coutput);

  while (!finish && !loutput->needs_topology_refresh) {
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
      case ConfigureNotify: {
	float wscale, hscale;
	disp->screen_width = e.xconfigure.width;
	disp->screen_height = e.xconfigure.height;
	if (disp->screen_width != disp->last_screen_width
	    || disp->screen_height != disp->last_screen_height) {
	  wscale = disp->screen_width / (float)disp->width;
	  hscale = disp->screen_height / (float)disp->height;
	  disp->scale *= wscale > hscale ? hscale : wscale;
	  if (disp->scale < 1.0f)
	    disp->scale = 1.0f;
	  move_x11(disp);
	}
	if (disp->x != lastx || disp->y != lasty)
	  XMoveWindow(disp->dpy, disp->win, -disp->x, -disp->y);
        disp->maximized = x11_is_maximized(disp);
	break;
      }
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
        case XK_F5:
          loutput->needs_topology_refresh = 1;
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
	case XK_F: {
	  /* fit drawing to window, dont't resize the window */
	  float wscale = disp->screen_width / (float)disp->width;
	  float hscale = disp->screen_height / (float)disp->height;
	  disp->scale *= wscale > hscale ? hscale : wscale;
	  move_x11(disp);
	  break;
	}
	case XK_plus:
	case XK_KP_Add:
	  disp->scale *= 1.2f;
	  disp->needs_resize = 1;
	  move_x11(disp);
	  break;
	case XK_minus:
	case XK_KP_Subtract:
	  disp->scale /= 1.2f;
	  disp->needs_resize = 1;
	  move_x11(disp);
	  break;
	case XK_1:
	case XK_KP_1:
	  disp->scale = 1.0f;
	  disp->needs_resize = 1;
	  move_x11(disp);
	  break;
	case XK_r:
	  disp->needs_resize = 2;
	  move_x11(disp);
	  break;
	case XK_R:
	  disp->auto_resize ^= 1;
	  printf("%s window autoresizing\n", disp->auto_resize ? "enabled" : "disabled");
	  disp->needs_resize = 1;
	  move_x11(disp);
	  break;
	case XK_h:
	case XK_H:
	case XK_question:
	  lstopo_show_interactive_help();
	  break;
	case XK_a:
	  loutput->show_attrs_enabled ^= 1;
	  printf("%s object attributes\n", loutput->show_attrs_enabled ? "enabled" : "disabled");
	  disp->needs_redraw = 1;
	  disp->needs_resize = 1;
	  move_x11(disp);
	  break;
	case XK_t:
	  loutput->show_text_enabled ^= 1;
	  printf("%s object text\n", loutput->show_text_enabled ? "enabled" : "disabled");
	  disp->needs_redraw = 1;
	  disp->needs_resize = 1;
	  move_x11(disp);
	  break;
	case XK_i:
	  if (loutput->index_type == LSTOPO_INDEX_TYPE_DEFAULT) {
	    loutput->index_type = LSTOPO_INDEX_TYPE_PHYSICAL;
	    printf("switched to physical indexes\n");
	  } else if (loutput->index_type == LSTOPO_INDEX_TYPE_PHYSICAL) {
	    loutput->index_type = LSTOPO_INDEX_TYPE_LOGICAL;
	    printf("switched to logical indexes\n");
	  } else if (loutput->index_type == LSTOPO_INDEX_TYPE_LOGICAL) {
	    loutput->index_type = LSTOPO_INDEX_TYPE_NONE;
	    printf("switched to no indexes\n");
	  } else if (loutput->index_type == LSTOPO_INDEX_TYPE_NONE) {
	    loutput->index_type = LSTOPO_INDEX_TYPE_DEFAULT;
	    printf("switched to default indexes\n");
	  } else {
	    abort();
	  }
	  disp->needs_redraw = 1;
	  disp->needs_resize = 1;
	  move_x11(disp);
	  break;
	case XK_b:
	  loutput->show_binding ^= 1;
	  printf("%s coloring of binding resources\n", loutput->show_binding ? "enabled" : "disabled");
	  disp->needs_redraw = 1;
	  move_x11(disp);
	  break;
	case XK_d:
	  loutput->show_disallowed ^= 1;
	  printf("%s coloring of disallowed resources\n", loutput->show_disallowed ? "enabled" : "disabled");
	  disp->needs_redraw = 1;
	  move_x11(disp);
	  break;
        case XK_k:
          loutput->show_cpukinds ^= 1;
          printf("%s displaying of CPU kinds\n", loutput->show_cpukinds ? "enabled" : "disabled");
          disp->needs_redraw = 1;
          move_x11(disp);
          break;
	case XK_f:
	  /* alternate between factorize+collapse, collapse only, and none */
	  if (loutput->factorize_enabled && loutput->pci_collapse_enabled) {
	    loutput->factorize_enabled = 0;
	    printf("factorizing disabled, PCI collapsing still enabled\n");
	  } else if (!loutput->factorize_enabled && loutput->pci_collapse_enabled) {
	    loutput->pci_collapse_enabled = 0;
	    printf("factorizing and PCI collapsing disabled\n");
	  } else {
	    loutput->factorize_enabled = 1;
	    loutput->pci_collapse_enabled = 1;
	    printf("factorizing and PCI collapsing enabled\n");
	  }
	  disp->needs_redraw = 1;
	  disp->needs_resize = 1;
	  move_x11(disp);
	  break;
	case XK_l:
          if (loutput->show_legend == LSTOPO_SHOW_LEGEND_ALL) {
            loutput->show_legend = LSTOPO_SHOW_LEGEND_NONE;
            printf("switched to no legend lines\n");
          } else if (loutput->show_legend == LSTOPO_SHOW_LEGEND_NONE) {
            loutput->show_legend = LSTOPO_SHOW_LEGEND_NO_DEFAULT;
            printf("switched to no default legend lines\n");
          } else if (loutput->show_legend == LSTOPO_SHOW_LEGEND_NO_DEFAULT) {
            loutput->show_legend = LSTOPO_SHOW_LEGEND_ALL;
            printf("switched to all legend lines\n");
          } else {
            abort();
          }
	  disp->needs_redraw = 1;
	  disp->needs_resize = 1;
	  move_x11(disp);
	  break;
	case XK_E:
	  lstopo_show_interactive_cli_options(loutput);
	  break;
	}
      }
    }
  }
  x11_destroy(disp);
  XDestroyWindow(disp->dpy, disp->top);
  XFreeCursor(disp->dpy, disp->hand);
  XCloseDisplay(disp->dpy);

  destroy_colors(loutput);
  return 0;
}
#endif /* LSTOPO_HAVE_X11 */


#ifdef CAIRO_HAS_PNG_FUNCTIONS
/* PNG back-end */

static struct draw_methods png_draw_methods = {
  NULL,
  NULL,
  topo_cairo_box,
  topo_cairo_line,
  topo_cairo_text,
  topo_cairo_textsize,
};

int
output_png(struct lstopo_output *loutput, const char *filename)
{
  struct lstopo_cairo_output coutput;
  FILE *output;
  cairo_surface_t *fakecs, *cs;

  output = open_output(filename, loutput->overwrite);
  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    return -1;
  }

  memset(&coutput, 0, sizeof(coutput));
  coutput.loutput = loutput;
  loutput->backend_data = &coutput;
  loutput->methods = &png_draw_methods;
  loutput->file = output;

  /* create a fake surface */
  fakecs = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1, 1);
  coutput.surface = fakecs;

  /* recurse once for preparing sizes and positions using the fake surface */
  loutput->drawing = LSTOPO_DRAWING_PREPARE;
  topo_cairo_paint(&coutput);
  loutput->drawing = LSTOPO_DRAWING_DRAW;
  cairo_surface_destroy(fakecs);

  /* create the actual surface with the right size */
  cs = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, loutput->width, loutput->height);
  coutput.surface = cs;

  /* ready */
  declare_colors(loutput);
  lstopo_prepare_custom_styles(loutput);

  topo_cairo_paint(&coutput);
  cairo_surface_write_to_png_stream(coutput.surface, topo_cairo_write, output);
  cairo_surface_destroy(coutput.surface);

  if (output != stdout)
    fclose(output);

  destroy_colors(loutput);
  return 0;
}
#endif /* CAIRO_HAS_PNG_FUNCTIONS */


#ifdef CAIRO_HAS_PDF_SURFACE
/* PDF back-end */

static struct draw_methods pdf_draw_methods = {
  NULL,
  NULL,
  topo_cairo_box,
  topo_cairo_line,
  topo_cairo_text,
  topo_cairo_textsize,
};

int
output_pdf(struct lstopo_output *loutput, const char *filename)
{
  struct lstopo_cairo_output coutput;
  FILE *output;
  cairo_surface_t *fakecs, *cs;

  output = open_output(filename, loutput->overwrite);
  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    return -1;
  }

  memset(&coutput, 0, sizeof(coutput));
  coutput.loutput = loutput;
  loutput->backend_data = &coutput;
  loutput->methods = &pdf_draw_methods;
  loutput->file = output;

  /* create a fake surface */
  fakecs = cairo_pdf_surface_create_for_stream(NULL, NULL, 1, 1);
  coutput.surface = fakecs;

  /* recurse once for preparing sizes and positions using the fake surface */
  loutput->drawing = LSTOPO_DRAWING_PREPARE;
  topo_cairo_paint(&coutput);
  loutput->drawing = LSTOPO_DRAWING_DRAW;
  cairo_surface_destroy(fakecs);

  /* create the actual surface with the right size */
  cs = cairo_pdf_surface_create_for_stream(topo_cairo_write, loutput->file, loutput->width, loutput->height);
  coutput.surface = cs;

  /* ready */
  declare_colors(loutput);
  lstopo_prepare_custom_styles(loutput);

  topo_cairo_paint(&coutput);
  cairo_surface_flush(coutput.surface);
  cairo_surface_destroy(coutput.surface);

  if (output != stdout)
    fclose(output);

  destroy_colors(loutput);
  return 0;
}
#endif /* CAIRO_HAS_PDF_SURFACE */


#ifdef CAIRO_HAS_PS_SURFACE
/* PS back-end */

static struct draw_methods ps_draw_methods = {
  NULL,
  NULL,
  topo_cairo_box,
  topo_cairo_line,
  topo_cairo_text,
  topo_cairo_textsize,
};

int
output_ps(struct lstopo_output *loutput, const char *filename)
{
  struct lstopo_cairo_output coutput;
  FILE *output;
  cairo_surface_t *fakecs, *cs;

  output = open_output(filename, loutput->overwrite);
  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    return -1;
  }

  memset(&coutput, 0, sizeof(coutput));
  coutput.loutput = loutput;
  loutput->backend_data = &coutput;
  loutput->methods = &ps_draw_methods;
  loutput->file = output;

  /* create a fake surface */
  fakecs = cairo_ps_surface_create_for_stream(NULL, NULL, 1, 1);
  coutput.surface = fakecs;

  /* recurse once for preparing sizes and positions using the fake surface */
  loutput->drawing = LSTOPO_DRAWING_PREPARE;
  topo_cairo_paint(&coutput);
  loutput->drawing = LSTOPO_DRAWING_DRAW;
  cairo_surface_destroy(fakecs);

  /* create the actual surface with the right size */
  cs = cairo_ps_surface_create_for_stream(topo_cairo_write, loutput->file, loutput->width, loutput->height);
  coutput.surface = cs;

  /* ready */
  declare_colors(loutput);
  lstopo_prepare_custom_styles(loutput);

  topo_cairo_paint(&coutput);
  cairo_surface_flush(coutput.surface);
  cairo_surface_destroy(coutput.surface);

  if (output != stdout)
    fclose(output);

  destroy_colors(loutput);
  return 0;
}
#endif /* CAIRO_HAS_PS_SURFACE */


#ifdef CAIRO_HAS_SVG_SURFACE
/* SVG back-end */

static struct draw_methods svg_draw_methods = {
  NULL,
  NULL,
  topo_cairo_box,
  topo_cairo_line,
  topo_cairo_text,
  topo_cairo_textsize,
};

int
output_cairosvg(struct lstopo_output *loutput, const char *filename)
{
  struct lstopo_cairo_output coutput;
  FILE *output;
  cairo_surface_t *fakecs, *cs;

  output = open_output(filename, loutput->overwrite);
  if (!output) {
    fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
    return -1;
  }

  memset(&coutput, 0, sizeof(coutput));
  coutput.loutput = loutput;
  loutput->backend_data = &coutput;
  loutput->methods = &svg_draw_methods;
  loutput->file = output;

  /* create a fake surface */
  fakecs = cairo_svg_surface_create_for_stream(NULL, NULL, 1, 1);
  coutput.surface = fakecs;

  /* recurse once for preparing sizes and positions using the fake surface */
  loutput->drawing = LSTOPO_DRAWING_PREPARE;
  topo_cairo_paint(&coutput);
  loutput->drawing = LSTOPO_DRAWING_DRAW;
  cairo_surface_destroy(fakecs);

  /* create the actual surface with the right size */
  cs = cairo_svg_surface_create_for_stream(topo_cairo_write, loutput->file, loutput->width, loutput->height);
  coutput.surface = cs;

  /* ready */
  declare_colors(loutput);
  lstopo_prepare_custom_styles(loutput);

  topo_cairo_paint(&coutput);
  cairo_surface_flush(coutput.surface);
  cairo_surface_destroy(coutput.surface);

  if (output != stdout)
    fclose(output);

  destroy_colors(loutput);
  return 0;
}
#endif /* CAIRO_HAS_SVG_SURFACE */
