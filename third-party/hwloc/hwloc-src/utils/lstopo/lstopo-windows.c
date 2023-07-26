/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2021 Inria.  All rights reserved.
 * Copyright © 2009-2010, 2012 Université Bordeaux
 * Copyright © 2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "private/autogen/config.h"
#include "hwloc.h"

#include <stdlib.h>
#include <stdio.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>

#include "private/windows.h" /* must be before windows.h */

#include <windows.h>
#include <windowsx.h>

#include "lstopo.h"

/* windows back-end.  */

struct lstopo_windows_output {
  struct lstopo_output *loutput;
  PAINTSTRUCT ps;
};

static HFONT font_default;
static HFONT font_bold;
static HPEN pen_default;
static HPEN pen_style[4];

static struct lstopo_windows_output the_output;
static int state, control;
static int the_x, the_y, x_delta, y_delta;
static int finish;
static int the_width, the_height;
static int win_width, win_height;
static unsigned int the_fontsize, the_gridsize;
static float the_scale;
static int auto_resize;
static int needs_resize;
static int ignore_wm_size;

static void
windows_box(struct lstopo_output *loutput, const struct lstopo_color *lcolor, unsigned depth __hwloc_attribute_unused, unsigned x, unsigned width, unsigned y, unsigned height, hwloc_obj_t obj __hwloc_attribute_unused, unsigned box_id __hwloc_attribute_unused);

static LRESULT CALLBACK
WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
  struct lstopo_output *loutput = the_output.loutput;
  int redraw = 0;

  switch (message) {
    case WM_CHAR:  {
      switch (wparam) {
      case '+':
	the_scale *= 1.2f;
	redraw = 1;
	if (auto_resize)
	  needs_resize = 1;
	break;
      case '-':
	the_scale /= 1.2f;
	redraw = 1;
	if (auto_resize)
	  needs_resize = 1;
	break;
      case 'F': {
	/* fit drawing to window, don't resize the window */
	float wscale, hscale;
	wscale = win_width / (float)the_width;
	hscale = win_height / (float)the_height;
	the_scale *= wscale > hscale ? hscale : wscale;
	redraw = 1;
	break;
      }
      case '1':
	the_scale = 1.0;
	redraw = 1;
	if (auto_resize)
	  needs_resize = 1;
	break;
      case 'r':
	needs_resize = 1; /* forced */
	break;
      case 'R':
	auto_resize ^= 1;
	if (auto_resize)
	  needs_resize = 1;
	printf("%s window autoresizing\n", auto_resize ? "enabled" : "disabled");
	break;
      case 'a':
	loutput->show_attrs_enabled ^= 1;
	printf("%s object attributes\n", loutput->show_attrs_enabled ? "enabled" : "disabled");
	redraw = 1;
	if (auto_resize)
	  needs_resize = 1;
	break;
      case 't':
	loutput->show_text_enabled ^= 1;
	printf("%s object text\n", loutput->show_text_enabled ? "enabled" : "disabled");
	redraw = 1;
	if (auto_resize)
	  needs_resize = 1;
	break;
      case 'i':
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
	redraw = 1;
	if (auto_resize)
	  needs_resize = 1;
	break;
      case 'k':
	loutput->show_cpukinds ^= 1;
	printf("%s displaying of CPU kinds\n", loutput->show_cpukinds ? "enabled" : "disabled");
	redraw = 1;
	break;
      case 'd':
	loutput->show_disallowed ^= 1;
	printf("%s coloring of disallowed resources\n", loutput->show_disallowed ? "enabled" : "disabled");
	redraw = 1;
	break;
      case 'b':
	loutput->show_binding ^= 1;
	printf("%s coloring of binding resources\n", loutput->show_binding ? "enabled" : "disabled");
	redraw = 1;
	break;
      case 'l':
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
	redraw = 1;
	if (auto_resize)
	  needs_resize = 1;
	break;
      case 'f':
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
	redraw = 1;
	if (auto_resize)
	  needs_resize = 1;
	break;
      case 'E':
	lstopo_show_interactive_cli_options(loutput);
	fflush(stdout);
	break;
      case 'h':
      case 'H':
      case '?':
	lstopo_show_interactive_help();
	break;
      case 'q':
      case 'Q':
	finish = 1;
	break;
      }
      break;
    }

    case WM_PAINT: {
#ifdef HWLOC_HAVE_GCC_W_MISSING_FIELD_INITIALIZERS
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
      struct lstopo_color white = {0xff, 0xff, 0xff};
#ifdef HWLOC_HAVE_GCC_W_MISSING_FIELD_INITIALIZERS
#pragma GCC diagnostic warning "-Wmissing-field-initializers"
#endif
      BeginPaint(hwnd, &the_output.ps);
      font_default = CreateFont(loutput->fontsize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, NULL);
      SelectObject(the_output.ps.hdc, (HGDIOBJ) font_default);
      pen_default = CreatePen(PS_SOLID, loutput->thickness, RGB(0,0,0));
      SelectObject(the_output.ps.hdc, pen_default);
      if (loutput->nr_cpukind_styles > 1) {
        font_bold = CreateFont(loutput->fontsize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, NULL);
        /* only 1-pixel width is supported for dash and dots */
        pen_style[0] = CreatePen(PS_DASH, 1, RGB(0,0,0));
        pen_style[1] = CreatePen(PS_DOT, 1, RGB(0,0,0));
        pen_style[2] = CreatePen(PS_DASHDOT, 1, RGB(0,0,0));
        pen_style[3] = CreatePen(PS_DASHDOTDOT, 1, RGB(0,0,0));
      }
      SetBkMode(the_output.ps.hdc, TRANSPARENT);
      loutput->drawing = LSTOPO_DRAWING_PREPARE;
      output_draw(loutput);
      the_width = loutput->width;
      the_height = loutput->height;

      /* now that we computed the new drawing size, resize the window if needed */
      if (needs_resize) {
	RECT rect;
	x_delta = 0;
	y_delta = 0;
	rect.top = 0;
	rect.left = 0;
	rect.right = the_width;
	rect.bottom = the_height;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	/* SetWindowPos() generates a WM_SIZE event, we don't want to scale the drawing there */
	ignore_wm_size = 1;
	SetWindowPos(hwnd, HWND_TOP, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOCOPYBITS|SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOZORDER);
	ignore_wm_size = 0;

	needs_resize = 0;
      }

      loutput->drawing = LSTOPO_DRAWING_DRAW;
      windows_box(loutput, &white, 0, 0, win_width, 0, win_height, NULL, 0);
      output_draw(loutput);
      DeleteObject(pen_default);
      DeleteObject(font_default);
      if (loutput->nr_cpukind_styles > 1) {
        DeleteObject(font_bold);
        DeleteObject(pen_style[0]);
        DeleteObject(pen_style[1]);
        DeleteObject(pen_style[2]);
        DeleteObject(pen_style[3]);
      }
      EndPaint(hwnd, &the_output.ps);
      break;
    }
    case WM_LBUTTONDOWN:
      state = 1;
      the_x = GET_X_LPARAM(lparam);
      the_y = GET_Y_LPARAM(lparam);
      break;
    case WM_LBUTTONUP:
      state = 0;
      break;
    case WM_MOUSEMOVE:
      if (!(wparam & MK_LBUTTON))
        state = 0;
      if (state) {
        int new_x = GET_X_LPARAM(lparam);
        int new_y = GET_Y_LPARAM(lparam);
        x_delta -= new_x - the_x;
        y_delta -= new_y - the_y;
        the_x = new_x;
        the_y = new_y;
        redraw = 1;
      }
      break;
    case WM_KEYDOWN:
      switch (wparam) {
      case VK_ESCAPE:
        finish = 1;
        break;
      case VK_LEFT:
        x_delta -= win_width/10;
        redraw = 1;
        break;
      case VK_RIGHT:
        x_delta += win_width/10;
        redraw = 1;
        break;
      case VK_UP:
        y_delta -= win_height/10;
        redraw = 1;
        break;
      case VK_DOWN:
        y_delta += win_height/10;
        redraw = 1;
        break;
      case VK_PRIOR:
        if (control) {
          x_delta -= win_width;
          redraw = 1;
        } else {
          y_delta -= win_height;
          redraw = 1;
        }
        break;
      case VK_NEXT:
        if (control) {
          x_delta += win_width;
          redraw = 1;
        } else {
          y_delta += win_height;
          redraw = 1;
        }
        break;
      case VK_HOME:
        x_delta = 0;
        y_delta = 0;
        redraw = 1;
        break;
      case VK_END:
        x_delta = INT_MAX;
        y_delta = INT_MAX;
        redraw = 1;
        break;
      case VK_CONTROL:
        control = 1;
        break;
      case VK_F5:
        loutput->needs_topology_refresh = 1;
        break;
      }
      break;
    case WM_KEYUP:
      switch (wparam) {
      case VK_CONTROL:
        control = 0;
        break;
      }
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    case WM_SIZE: {
      win_width = LOWORD(lparam);
      win_height = HIWORD(lparam);
      if (!ignore_wm_size) {
	float wscale, hscale;
	wscale = win_width / (float)the_width;
	hscale = win_height / (float)the_height;
	the_scale *= wscale > hscale ? hscale : wscale;
	if (the_scale < 1.0f)
	  the_scale = 1.0f;
	redraw = 1;
      }
      break;
    }
  }
  if (needs_resize) {
    /* force a redraw since resizing occurs during redraw */
    redraw = 1;
  }
  if (redraw) {
    if (x_delta > the_width - win_width)
      x_delta = the_width - win_width;
    if (y_delta > the_height - win_height)
      y_delta = the_height - win_height;
    if (x_delta < 0)
      x_delta = 0;
    if (y_delta < 0)
      y_delta = 0;
    loutput->fontsize = (unsigned)(the_fontsize * the_scale);
    loutput->gridsize = (unsigned)(the_gridsize * the_scale);
    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
  }
  return DefWindowProc(hwnd, message, wparam, lparam);
}

static int
windows_declare_color(struct lstopo_output *loutput __hwloc_attribute_unused, struct lstopo_color *lcolor)
{
  int r = lcolor->r, g = lcolor->g, b = lcolor->b;
  HBRUSH brush;
  COLORREF color;

  color = RGB(r, g, b);
  brush = CreateSolidBrush(color);
  if (!brush) {
    fprintf(stderr,"Could not allocate color %02x%02x%02x\n", r, g, b);
    exit(EXIT_FAILURE);
  }

  lcolor->private.windows.brush = (HGDIOBJ) brush;
  lcolor->private.windows.color = color;
  return 0;
}

static void
windows_destroy_color(struct lstopo_output *loutput __hwloc_attribute_unused, struct lstopo_color *lcolor)
{
  DeleteObject(lcolor->private.windows.brush);
}

static void
windows_box(struct lstopo_output *loutput, const struct lstopo_color *lcolor, unsigned depth __hwloc_attribute_unused, unsigned x, unsigned width, unsigned y, unsigned height, hwloc_obj_t obj __hwloc_attribute_unused, unsigned box_id __hwloc_attribute_unused)
{
  struct lstopo_windows_output *woutput = loutput->backend_data;
  unsigned cpukind_style = lstopo_obj_cpukind_style(loutput, obj);
  PAINTSTRUCT *ps = &woutput->ps;

  SelectObject(ps->hdc, lcolor->private.windows.brush);
  SetBkColor(ps->hdc, lcolor->private.windows.color);
  if (cpukind_style)
    SelectObject(ps->hdc, pen_style[(cpukind_style-1)%4]);
  Rectangle(ps->hdc, x - x_delta, y - y_delta, x + width - x_delta, y + height - y_delta);
  if (cpukind_style)
    SelectObject(ps->hdc, pen_default);
}

static void
windows_line(struct lstopo_output *loutput, unsigned depth __hwloc_attribute_unused, unsigned x1, unsigned y1, unsigned x2, unsigned y2, hwloc_obj_t obj __hwloc_attribute_unused, unsigned line_id __hwloc_attribute_unused)
{
  struct lstopo_windows_output *woutput = loutput->backend_data;
  PAINTSTRUCT *ps = &woutput->ps;

  MoveToEx(ps->hdc, x1 - x_delta, y1 - y_delta, NULL);
  LineTo(ps->hdc, x2 - x_delta, y2 - y_delta);
}

static void
windows_text(struct lstopo_output *loutput, const struct lstopo_color *lcolor, int size __hwloc_attribute_unused, unsigned depth __hwloc_attribute_unused, unsigned x, unsigned y, const char *text, hwloc_obj_t obj __hwloc_attribute_unused, unsigned text_id __hwloc_attribute_unused)
{
  struct lstopo_windows_output *woutput = loutput->backend_data;
  unsigned cpukind_style = lstopo_obj_cpukind_style(loutput, obj);
  PAINTSTRUCT *ps = &woutput->ps;

  SetTextColor(ps->hdc, lcolor->private.windows.color);
  if (cpukind_style % 2)
    SelectObject(ps->hdc, font_bold);
  TextOut(ps->hdc, x - x_delta, y - y_delta, text, (int)strlen(text));
  if (cpukind_style % 2)
    SelectObject(ps->hdc, font_default);
}

static void
windows_textsize(struct lstopo_output *loutput, const char *text, unsigned textlength, unsigned fontsize __hwloc_attribute_unused, unsigned *width)
{
  struct lstopo_windows_output *woutput = loutput->backend_data;
  PAINTSTRUCT *ps = &woutput->ps;
  SIZE size;

  GetTextExtentPoint32(ps->hdc, text, textlength, &size);
  *width = size.cx;
}

struct draw_methods windows_draw_methods = {
  windows_declare_color,
  windows_destroy_color,
  windows_box,
  windows_line,
  windows_text,
  windows_textsize,
};

static HWND toplevel = NULL;

int
output_windows (struct lstopo_output *loutput, const char *dummy __hwloc_attribute_unused)
{
  unsigned width, height;
  MSG msg;
  RECT rect;

  memset(&the_output, 0, sizeof(the_output));
  the_output.loutput = loutput;
  loutput->methods = &windows_draw_methods;
  loutput->backend_data = &the_output;

  if (!toplevel) {
    /* create the toplevel window, with random size for now */
    WNDCLASS wndclass;
    memset(&wndclass, 0, sizeof(wndclass));
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.hCursor = LoadCursor(NULL, IDC_SIZEALL);
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.lpfnWndProc = WndProc;
    wndclass.lpszClassName = "lstopo";

    RegisterClass(&wndclass);

    toplevel = CreateWindow("lstopo", loutput->title, WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, CW_USEDEFAULT,
                            10, 10, NULL, NULL, NULL, NULL);
    assert(!loutput->refreshing);
  } else {
    assert(loutput->refreshing);
  }

  /* recurse once for preparing sizes and positions using a fake top level window */
  loutput->drawing = LSTOPO_DRAWING_PREPARE;
  BeginPaint(toplevel, &the_output.ps);

  font_default = CreateFont(loutput->fontsize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, NULL);
  if (loutput->nr_cpukind_styles > 1)
    font_bold = CreateFont(loutput->fontsize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, NULL);
  SelectObject(the_output.ps.hdc, (HGDIOBJ) font_default);

  output_draw(loutput);

  DeleteObject(font_default);
  if (loutput->nr_cpukind_styles > 1)
    DeleteObject(font_bold);

  EndPaint(toplevel, &the_output.ps);
  loutput->drawing = LSTOPO_DRAWING_DRAW;

  /* now create the actual toplevel with the sizes */
  width = loutput->width;
  height = loutput->height;

  /* compute the window dimensions with borders/title/... */
  rect.top = 0;
  rect.left = 0;
  rect.right = width;
  rect.bottom = height;
  AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
  win_width = rect.right - rect.left;
  win_height = rect.bottom - rect.top;

  /* don't make it bigger than the screen */
  if (win_width > GetSystemMetrics(SM_CXFULLSCREEN))
    win_width = GetSystemMetrics(SM_CXFULLSCREEN);
  if (win_height > GetSystemMetrics(SM_CYFULLSCREEN))
    win_height = GetSystemMetrics(SM_CYFULLSCREEN);

  ignore_wm_size = 1;
  SetWindowPos(toplevel, HWND_TOP, 0, 0, win_width, win_height, SWP_NOCOPYBITS|SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOZORDER);
  ignore_wm_size = 0;

  the_width = width;
  the_height = height;

  the_scale = 1.0f;

  the_fontsize = loutput->fontsize;
  the_gridsize = loutput->gridsize;

  auto_resize = 1;
  needs_resize = 0;
  ignore_wm_size = 0;

  /* ready */
  declare_colors(loutput);
  lstopo_prepare_custom_styles(loutput);

  if (!loutput->refreshing)
    lstopo_show_interactive_help();

  ShowWindow(toplevel, SW_SHOWDEFAULT);
  InvalidateRect(toplevel, NULL, 1); /* make sure UpdateWindow() will update something when refreshing the topology */
  UpdateWindow(toplevel);

  while (!finish && !loutput->needs_topology_refresh && GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  if (!loutput->needs_topology_refresh)
    DestroyWindow(toplevel);
  destroy_colors(loutput);
  return 0;
}
