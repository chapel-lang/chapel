/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2016 Inria.  All rights reserved.
 * Copyright © 2009-2013, 2015 Université Bordeaux
 * Copyright © 2009-2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <private/autogen/config.h>
#include <private/private.h>
#include <hwloc.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "lstopo.h"

#define EPOXY_R_COLOR 0xe7
#define EPOXY_G_COLOR 0xff
#define EPOXY_B_COLOR 0xb5

#define DARK_EPOXY_R_COLOR ((EPOXY_R_COLOR * 100) / 110)
#define DARK_EPOXY_G_COLOR ((EPOXY_G_COLOR * 100) / 110)
#define DARK_EPOXY_B_COLOR ((EPOXY_B_COLOR * 100) / 110)

#define DARKER_EPOXY_R_COLOR ((DARK_EPOXY_R_COLOR * 100) / 110)
#define DARKER_EPOXY_G_COLOR ((DARK_EPOXY_G_COLOR * 100) / 110)
#define DARKER_EPOXY_B_COLOR ((DARK_EPOXY_B_COLOR * 100) / 110)

#define PACKAGE_R_COLOR 0xde
#define PACKAGE_G_COLOR 0xde
#define PACKAGE_B_COLOR 0xde

#define MEMORY_R_COLOR 0xef
#define MEMORY_G_COLOR 0xdf
#define MEMORY_B_COLOR 0xde

#define CORE_R_COLOR 0xbe
#define CORE_G_COLOR 0xbe
#define CORE_B_COLOR 0xbe

#define THREAD_R_COLOR 0xff
#define THREAD_G_COLOR 0xff
#define THREAD_B_COLOR 0xff

#define RUNNING_R_COLOR 0
#define RUNNING_G_COLOR 0xff
#define RUNNING_B_COLOR 0

#define FORBIDDEN_R_COLOR 0xff
#define FORBIDDEN_G_COLOR 0
#define FORBIDDEN_B_COLOR 0

#define OFFLINE_R_COLOR 0
#define OFFLINE_G_COLOR 0
#define OFFLINE_B_COLOR 0

#define CACHE_R_COLOR 0xff
#define CACHE_G_COLOR 0xff
#define CACHE_B_COLOR 0xff

#define MACHINE_R_COLOR EPOXY_R_COLOR
#define MACHINE_G_COLOR EPOXY_G_COLOR
#define MACHINE_B_COLOR EPOXY_B_COLOR

#define NODE_R_COLOR DARK_EPOXY_R_COLOR
#define NODE_G_COLOR DARK_EPOXY_G_COLOR
#define NODE_B_COLOR DARK_EPOXY_B_COLOR

#define SYSTEM_R_COLOR 0xff
#define SYSTEM_G_COLOR 0xff
#define SYSTEM_B_COLOR 0xff

#define MISC_R_COLOR 0xff
#define MISC_G_COLOR 0xff
#define MISC_B_COLOR 0xff

#define PCI_DEVICE_R_COLOR DARKER_EPOXY_R_COLOR
#define PCI_DEVICE_G_COLOR DARKER_EPOXY_G_COLOR
#define PCI_DEVICE_B_COLOR DARKER_EPOXY_B_COLOR

#define OS_DEVICE_R_COLOR 0xde
#define OS_DEVICE_G_COLOR 0xde
#define OS_DEVICE_B_COLOR 0xde

#define BRIDGE_R_COLOR 0xff
#define BRIDGE_G_COLOR 0xff
#define BRIDGE_B_COLOR 0xff

unsigned get_textwidth(void *output,
		       const char *text, unsigned length,
		       unsigned fontsize, unsigned gridsize)
{
  struct lstopo_output *loutput = output;
  if (loutput->methods->textsize) {
    unsigned width;
    loutput->methods->textsize(output, text, length, fontsize, &width);
    return width + gridsize;
  }
  return (length * fontsize * 3) / 4 + gridsize;
}

/* preferred width/height compromise */
#define RATIO (4.f/3.f)

/* PCI object height: just a box */
#define PCI_HEIGHT (fontsize ? gridsize + fontsize + gridsize : gridsize)

/* do we prefer ratio1 over ratio2? */
static int prefer_ratio(float ratio1, float ratio2) {
  float _ratio1 = (ratio1) / RATIO;
  float _ratio2 = (ratio2) / RATIO;
  if (_ratio1 < 1)
    _ratio1 = 1/_ratio1;
  if (_ratio2 < 1)
    _ratio2 = 1/_ratio2;
  return _ratio1 < _ratio2;
}

static void null_declare_color(void *output __hwloc_attribute_unused, int r __hwloc_attribute_unused, int g __hwloc_attribute_unused, int b __hwloc_attribute_unused) { }
static void null_box(void *output __hwloc_attribute_unused, int r __hwloc_attribute_unused, int g __hwloc_attribute_unused, int b __hwloc_attribute_unused, unsigned depth __hwloc_attribute_unused, unsigned x __hwloc_attribute_unused, unsigned width __hwloc_attribute_unused, unsigned y __hwloc_attribute_unused, unsigned height __hwloc_attribute_unused) { }
static void null_line(void *output __hwloc_attribute_unused, int r __hwloc_attribute_unused, int g __hwloc_attribute_unused, int b __hwloc_attribute_unused, unsigned depth __hwloc_attribute_unused, unsigned x1 __hwloc_attribute_unused, unsigned y1_arg __hwloc_attribute_unused, unsigned x2 __hwloc_attribute_unused, unsigned y2 __hwloc_attribute_unused) { }
static void null_text(void *output __hwloc_attribute_unused, int r __hwloc_attribute_unused, int g __hwloc_attribute_unused, int b __hwloc_attribute_unused, int size __hwloc_attribute_unused, unsigned depth __hwloc_attribute_unused, unsigned x __hwloc_attribute_unused, unsigned y __hwloc_attribute_unused, const char *text __hwloc_attribute_unused) { }

static struct draw_methods null_draw_methods = {
  NULL, /* init */
  null_declare_color,
  null_box,
  null_line,
  null_text,
  NULL, /* no textsize needed, only native methods' textsize is used */
};

/*
 * foo_draw functions take a OBJ, computes which size it needs, recurse into
 * sublevels with null_draw_methods to recursively compute the needed size
 * without actually drawing anything, then draw things about OBJ (chip draw,
 * cache size information etc) at (X,Y), recurse into sublevels again to
 * actually draw things, and return in RETWIDTH and RETHEIGHT the amount of
 * space that the drawing took.
 *
 * For generic detailed comments, see the node_draw function.
 *
 * border is added around the objects
 * separator is added between objects
 */

typedef void (*foo_draw)(hwloc_topology_t topology, struct draw_methods *methods, int logical, hwloc_obj_t obj, void *output, unsigned depth, unsigned x, unsigned *retwidth, unsigned y, unsigned *retheight);

static foo_draw get_type_fun(hwloc_obj_type_t type);

/*
 * Helper to recurse into sublevels, either horizontally or vertically
 * Updates caller's totwidth/myheight and maxwidth/maxheight
 * Needs textwidth, topology, output, depth, x and y
 */

#define RECURSE_BEGIN(obj, border) do { \
  hwloc_obj_t *subobjs = obj->children; \
  unsigned numsubobjs = obj->arity; \
  unsigned numignoredsubobjs = 0; \
  unsigned width, height; \
  unsigned maxwidth __hwloc_attribute_unused, maxheight __hwloc_attribute_unused; \
  unsigned i; \
  maxwidth = maxheight = 0; \
  totwidth = (border) + mywidth; \
  totheight = (border) + myheight; \
  /* Count objects to ignore */ \
  if (lstopo_ignore_pus) { \
    for (i = 0; i < numsubobjs; i++) \
      if (subobjs[i]->type == HWLOC_OBJ_PU) \
        numignoredsubobjs++; \
  } \
  if (obj->type == HWLOC_OBJ_BRIDGE && lstopo_collapse) { \
    for (i = 0; i < numsubobjs; i++) \
      if (subobjs[i]->type == HWLOC_OBJ_PCI_DEVICE) { \
	const char *collapsestr = hwloc_obj_get_info_by_name(subobjs[i], "lstopoCollapse"); \
	if (collapsestr && !strcmp(collapsestr, "0")) \
	  numignoredsubobjs++; \
      } \
  } \
  numsubobjs -= numignoredsubobjs; \
  if (numsubobjs) { \

#define RECURSE_FOR() \
    /* Iterate over subobjects */ \
    for (i = 0; i < numsubobjs+numignoredsubobjs; i++) { \
      if (lstopo_ignore_pus && subobjs[i]->type == HWLOC_OBJ_PU) \
	continue; \
      if (lstopo_collapse && subobjs[i]->type == HWLOC_OBJ_PCI_DEVICE) { \
	const char *collapsestr = hwloc_obj_get_info_by_name(subobjs[i], "lstopoCollapse"); \
	if (collapsestr && !strcmp(collapsestr, "0")) \
	  continue; \
      }

      /* Recursive call */
#define RECURSE_CALL_FUN(methods) \
      get_type_fun(subobjs[i]->type)(topology, methods, logical, subobjs[i], output, depth-1, x + totwidth, &width, y + totheight, &height) \

#define RECURSE_END_HORIZ(separator, border) \
      /* Add the subobject's width and separator */ \
      totwidth += width + (separator); \
      /* Update maximum height */ \
      if (height > maxheight) \
	maxheight = height; \
    } \
    \
    /* Remove spurious separator on the right */ \
    totwidth -= (separator); \
    \
    /* Make sure there is width for the heading text */ \
    /* Add subobjects height */ \
    totheight += maxheight; \
    /* And add border below */ \
    totheight += (border); \
    /* Add border on the right */ \
    totwidth += (border); \
  } \
  if (totwidth < (border) + textwidth) \
    totwidth = (border) + textwidth; \
  /* Update returned values */ \
  *retwidth = totwidth; \
  *retheight = totheight; \
} while(0)

#define RECURSE_END_VERT(separator, border) \
      /* Add the subobject's height and separator */ \
      totheight += height + (separator); \
      if (width > maxwidth) \
      /* Update maximum width */ \
	maxwidth = width; \
    } \
    /* Remove spurious separator at the bottom */ \
    totheight -= (separator); \
    /* Add subobjects width */ \
    totwidth += maxwidth; \
    /* And add border on the right */ \
    totwidth += (border); \
    /* Add border at the bottom */ \
    totheight = totheight + (border); \
  } \
  \
  /* Make sure there is width for the heading text */ \
  if (totwidth < (border) + textwidth) \
    totwidth = (border) + textwidth; \
  /* Update returned values */ \
  *retwidth = totwidth; \
  *retheight = totheight; \
} while(0)

/* Pack objects horizontally */
#define RECURSE_HORIZ(obj, methods, separator, border) \
  RECURSE_BEGIN(obj, border) \
  RECURSE_FOR() \
    RECURSE_CALL_FUN(methods); \
  RECURSE_END_HORIZ(separator, border)

/* Pack objects vertically */
#define RECURSE_VERT(obj, methods, separator, border) \
  RECURSE_BEGIN(obj, border) \
  RECURSE_FOR() \
    RECURSE_CALL_FUN(methods); \
  RECURSE_END_VERT(separator, border)

#define RECURSE_RECT_BEGIN(obj, methods, separator, border) \
RECURSE_BEGIN(obj, border) \
    /* Total width for subobjects */ \
    unsigned obj_totwidth = 0, obj_totheight = 0; \
    /* Total area for subobjects */ \
    unsigned area = 0; \
    unsigned rows, columns; \
    RECURSE_FOR() \
      RECURSE_CALL_FUN(&null_draw_methods); \
      obj_totwidth += width + (separator); \
      obj_totheight += height + (separator); \
      area += (width + (separator)) * (height + (separator)); \
    } \
    if (force_orient[obj->type] == LSTOPO_ORIENT_HORIZ) { \
      rows =  1; \
      columns = numsubobjs; \
    } else if (force_orient[obj->type] == LSTOPO_ORIENT_VERT) { \
      columns =  1; \
      rows = numsubobjs; \
    } else { \
      unsigned found = 0; \
      /* Try to find a fitting rectangle */ \
      for (rows = (unsigned) (float) floor(sqrt(numsubobjs)); \
	   rows >= (unsigned) (float) ceil(pow(numsubobjs,0.33)) && rows > 1; \
	   rows--) { \
        columns = numsubobjs / rows; \
        if (columns > 1 && columns * rows == numsubobjs) { \
          found = 1; \
          break; \
        } \
      } \
      if (!found) { \
        /* Average object size */ \
        unsigned obj_avgwidth = obj_totwidth / numsubobjs; \
        unsigned obj_avgheight = obj_totheight / numsubobjs; \
        /* Ideal total height for spreading that area with RATIO */ \
        float idealtotheight = (float) sqrt(area/RATIO); \
        float under_ratio, over_ratio; \
        /* approximation of number of rows */ \
        rows = (unsigned) (idealtotheight / obj_avgheight); \
        columns = rows ? (numsubobjs + rows - 1) / rows : 1; \
        /* Ratio obtained by underestimation */ \
        under_ratio = (float) (columns * obj_avgwidth) / (rows * obj_avgheight); \
        \
        /* try to overestimate too */ \
        rows++; \
        columns = (numsubobjs + rows - 1) / rows; \
        /* Ratio obtained by overestimation */ \
        over_ratio = (float) (columns * obj_avgwidth) / (rows * obj_avgheight); \
        /* Did we actually preferred underestimation? (good row/column fit or good ratio) */ \
        if (rows > 1 && prefer_ratio(under_ratio, over_ratio)) { \
          rows--; \
          columns = (numsubobjs + rows - 1) / rows; \
        } \
      } \
    } \
    \
    maxheight = 0; \
    RECURSE_FOR() \
      /* Newline? */ \
      if (i && i%columns == 0) { \
        totwidth = (border) + mywidth; \
        /* Add the same height to all rows */ \
        totheight += maxheight + (separator); \
        maxheight = 0; \
      } \

#define RECURSE_RECT_END(obj, methods, separator, border) \
      if (totwidth + width + (separator) > maxwidth) \
        maxwidth = totwidth + width + (separator); \
      totwidth += width + (separator); \
      /* Update maximum height */ \
      if (height > maxheight) \
	maxheight = height; \
    } \
    /* Remove spurious separator on the right */ \
    maxwidth -= (separator); \
    /* Compute total width */ \
    totwidth = maxwidth + (border); \
    /* Add the last row's height and border at the bottom */ \
    totheight += maxheight + (border); \
  } \
  /* Make sure there is width for the heading text */ \
  if (totwidth < (border) + textwidth) \
    totwidth = (border) + textwidth; \
  /* Update returned values */ \
  *retwidth = totwidth; \
  *retheight = totheight; \
} while(0)

/* Pack objects in a grid */
#define RECURSE_RECT(obj, methods, separator, border) do {\
  RECURSE_RECT_BEGIN(obj, methods, separator, border) \
  RECURSE_CALL_FUN(methods); \
  RECURSE_RECT_END(obj, methods, separator, border); \
} while (0)

/* Dynamic programming */

/* Save the computed size */
#define DYNA_SAVE() do { \
    struct lstopo_obj_userdata *save = level->userdata; \
    save->width = *retwidth; \
    save->height = *retheight; \
    save->fontsize = fontsize; \
    save->gridsize = gridsize; \
} while (0)

/* Check whether we already computed the size and we are not actually drawing, in that case return it */
#define DYNA_CHECK() do { \
  if (methods == &null_draw_methods) { \
    struct lstopo_obj_userdata *save = level->userdata; \
    if (save->fontsize == fontsize && save->gridsize == gridsize) { \
      *retwidth = save->width; \
      *retheight = save->height; \
      return; \
    } \
  } \
} while (0)

static int
prefer_vert(hwloc_topology_t topology, int logical, hwloc_obj_t level, void *output, unsigned depth, unsigned x, unsigned y, unsigned separator)
{
  float horiz_ratio, vert_ratio;
  unsigned textwidth = 0;
  unsigned mywidth = 0, myheight = 0;
  unsigned totwidth, *retwidth = &totwidth, totheight, *retheight = &totheight;
  RECURSE_HORIZ(level, &null_draw_methods, separator, 0);
  horiz_ratio = (float)totwidth / totheight;
  RECURSE_VERT(level, &null_draw_methods, separator, 0);
  vert_ratio = (float)totwidth / totheight;
  return force_orient[level->type] == LSTOPO_ORIENT_VERT || (force_orient[level->type] != LSTOPO_ORIENT_HORIZ && prefer_ratio(vert_ratio, horiz_ratio));
}

static int
lstopo_obj_snprintf(char *text, size_t textlen, hwloc_obj_t obj, int logical)
{
  unsigned idx = logical ? obj->logical_index : obj->os_index;
  const char *indexprefix = logical ? " L#" : " P#";
  const char *value;
  char typestr[32];
  char indexstr[32]= "";
  char attrstr[256];
  char totmemstr[64] = "";
  int attrlen;

  /* For OSDev, Misc and Group, name replaces type+index+attrs */
  if (obj->name && (obj->type == HWLOC_OBJ_OS_DEVICE || obj->type == HWLOC_OBJ_MISC || obj->type == HWLOC_OBJ_GROUP)) {
    return snprintf(text, textlen, "%s", obj->name);
  }

  /* Type replaces the basic type name */
  if ((value = hwloc_obj_get_info_by_name(obj, "Type")) != NULL) {
    snprintf(typestr, sizeof(typestr), "%s", value);
  } else {
    hwloc_obj_type_snprintf(typestr, sizeof(typestr), obj, 0);
  }

  if (idx != (unsigned)-1 && obj->depth != 0
      && obj->type != HWLOC_OBJ_PCI_DEVICE
      && (obj->type != HWLOC_OBJ_BRIDGE || obj->attr->bridge.upstream_type == HWLOC_OBJ_BRIDGE_HOST))
    snprintf(indexstr, sizeof(indexstr), "%s%u", indexprefix, idx);
  attrlen = hwloc_obj_attr_snprintf(attrstr, sizeof(attrstr), obj, " ", 0);
  /* display the root total_memory if different from the local_memory (already shown) */
  if (!obj->parent && obj->memory.total_memory > obj->memory.local_memory)
    snprintf(totmemstr, sizeof(totmemstr), " (%lu%s total)",
             (unsigned long) hwloc_memory_size_printf_value(obj->memory.total_memory, 0),
             hwloc_memory_size_printf_unit(obj->memory.total_memory, 0));
  if (attrlen > 0)
    return snprintf(text, textlen, "%s%s (%s)%s", typestr, indexstr, attrstr, totmemstr);
  else
    return snprintf(text, textlen, "%s%s%s", typestr, indexstr, totmemstr);
}

static void
lstopo_prepare_custom_styles(struct lstopo_output *loutput, hwloc_obj_t obj)
{
  struct lstopo_obj_userdata *lud = obj->userdata;
  struct style *s = &lud->style;
  hwloc_obj_t child;
  unsigned forcer, forceg, forceb;
  const char *stylestr;

  lud->style_set = 0;

  stylestr = hwloc_obj_get_info_by_name(obj, "lstopoStyle");
  if (stylestr) {
    while (*stylestr != '\0') {
      if (sscanf(stylestr, "%02x%02x%02x", &forcer, &forceg, &forceb) == 3
	  || sscanf(stylestr, "Background=#%02x%02x%02x", &forcer, &forceg, &forceb) == 3) {
	s->bg.r = forcer & 255;
	s->bg.g = forceg & 255;
	s->bg.b = forceb & 255;
	lud->style_set |= 0x1;
	loutput->methods->declare_color(loutput, s->bg.r, s->bg.g, s->bg.b);
	s->t.r = s->t.g = s->t.b = (s->bg.r + s->bg.g + s->bg.b < 0xff) ? 0xff : 0;
    } else if (sscanf(stylestr, "Background2=#%02x%02x%02x", &forcer, &forceg, &forceb) == 3) {
	s->bg2.r = forcer & 255;
	s->bg2.g = forceg & 255;
	s->bg2.b = forceb & 255;
	lud->style_set |= 0x2;
	loutput->methods->declare_color(loutput, s->bg2.r, s->bg2.g, s->bg2.b);
	s->t2.r = s->t2.g = s->t2.b = (s->bg2.r + s->bg2.g + s->bg2.b < 0xff) ? 0xff : 0;
      } else if (sscanf(stylestr, "Text=#%02x%02x%02x", &forcer, &forceg, &forceb) == 3) {
	s->t.r = forcer & 255;
	s->t.g = forceg & 255;
	s->t.b = forceb & 255;
	lud->style_set |= 0x4;
	loutput->methods->declare_color(loutput, s->t.r, s->t.g, s->t.b);
      } else if (sscanf(stylestr, "Text2=#%02x%02x%02x", &forcer, &forceg, &forceb) == 3) {
	s->t2.r = forcer & 255;
	s->t2.g = forceg & 255;
	s->t2.b = forceb & 255;
	lud->style_set |= 0x8;
	loutput->methods->declare_color(loutput, s->t2.r, s->t2.g, s->t2.b);
      }
      stylestr = strchr(stylestr, ';');
      if (!stylestr)
	break;
      stylestr++;
    }
  }

  for(child = obj->first_child; child; child = child->next_sibling)
    lstopo_prepare_custom_styles(loutput, child);
}

static void
lstopo_set_object_color(struct draw_methods *methods,
			hwloc_topology_t topology __hwloc_attribute_unused,
			hwloc_obj_t obj, int arg, /* PU status (0=normal, 1=running, 2=forbidden, 3=offline)
						   * Machine status (0=normal, 1=displayed as a root/System) */
			struct style *s)
{
  struct lstopo_obj_userdata *lud = obj->userdata;

  /* no need to deal with colors when computing max sizes */
  if (methods == &null_draw_methods)
    return;

  memset(s, 0, sizeof(*s));

  switch (obj->type) {

  case HWLOC_OBJ_MACHINE:
    if (arg == 0) {
      s->bg.r = MACHINE_R_COLOR;
      s->bg.g = MACHINE_G_COLOR;
      s->bg.b = MACHINE_B_COLOR;
      break;
    }
    assert(arg == 1); /* Machine printed as a System (when root) */
    /* fallthrough */
  case HWLOC_OBJ_SYSTEM:
    s->bg.r = SYSTEM_R_COLOR;
    s->bg.g = SYSTEM_G_COLOR;
    s->bg.b = SYSTEM_B_COLOR;
    break;

  case HWLOC_OBJ_GROUP:
    s->bg.r = MISC_R_COLOR;
    s->bg.g = MISC_G_COLOR;
    s->bg.b = MISC_B_COLOR;
    break;

  case HWLOC_OBJ_MISC:
    s->bg.r = MISC_R_COLOR;
    s->bg.g = MISC_G_COLOR;
    s->bg.b = MISC_B_COLOR;
    break;

  case HWLOC_OBJ_NUMANODE:
    s->bg.r = NODE_R_COLOR;
    s->bg.g = NODE_G_COLOR;
    s->bg.b = NODE_B_COLOR;
    s->bg2.r = MEMORY_R_COLOR;
    s->bg2.g = MEMORY_G_COLOR;
    s->bg2.b = MEMORY_B_COLOR;
    break;

  case HWLOC_OBJ_PACKAGE:
    s->bg.r = PACKAGE_R_COLOR;
    s->bg.g = PACKAGE_G_COLOR;
    s->bg.b = PACKAGE_B_COLOR;
    break;

  case HWLOC_OBJ_CORE:
    s->bg.r = CORE_R_COLOR;
    s->bg.g = CORE_G_COLOR;
    s->bg.b = CORE_B_COLOR;
    break;

  case HWLOC_OBJ_CACHE:
    s->bg.r = CACHE_R_COLOR;
    s->bg.g = CACHE_G_COLOR;
    s->bg.b = CACHE_B_COLOR;
    break;

  case HWLOC_OBJ_PU:
    switch (arg) {
    case 0:
      s->bg.r = THREAD_R_COLOR;
      s->bg.g = THREAD_G_COLOR;
      s->bg.b = THREAD_B_COLOR;
      break;
    case 1:
      s->bg.r = RUNNING_R_COLOR;
      s->bg.g = RUNNING_G_COLOR;
      s->bg.b = RUNNING_B_COLOR;
      break;
    case 2:
      s->bg.r = FORBIDDEN_R_COLOR;
      s->bg.g = FORBIDDEN_G_COLOR;
      s->bg.b = FORBIDDEN_B_COLOR;
      break;
    case 3:
      s->bg.r = OFFLINE_R_COLOR;
      s->bg.g = OFFLINE_G_COLOR;
      s->bg.b = OFFLINE_B_COLOR;
      s->t.r = s->t.g = s->t.b = 0xff;
      break;
    default:
      assert(0);
    }
    break;

  case HWLOC_OBJ_BRIDGE:
    s->bg.r = BRIDGE_R_COLOR;
    s->bg.g = BRIDGE_G_COLOR;
    s->bg.b = BRIDGE_B_COLOR;
    break;

  case HWLOC_OBJ_PCI_DEVICE:
    s->bg.r = PCI_DEVICE_R_COLOR;
    s->bg.g = PCI_DEVICE_G_COLOR;
    s->bg.b = PCI_DEVICE_B_COLOR;
    break;

  case HWLOC_OBJ_OS_DEVICE:
    s->bg.r = OS_DEVICE_R_COLOR;
    s->bg.g = OS_DEVICE_G_COLOR;
    s->bg.b = OS_DEVICE_B_COLOR;
    break;

  default:
    assert(0);
  }

  if (lud->style_set & 0x1)
    memcpy(&s->bg, &lud->style.bg, sizeof(struct stylecolor));
  if (lud->style_set & 0x2)
    memcpy(&s->t, &lud->style.t, sizeof(struct stylecolor));
  if (lud->style_set & 0x4)
    memcpy(&s->bg2, &lud->style.bg2, sizeof(struct stylecolor));
  if (lud->style_set & 0x8)
    memcpy(&s->t2, &lud->style.t2, sizeof(struct stylecolor));
}

static void
pci_device_draw(hwloc_topology_t topology __hwloc_attribute_unused, struct draw_methods *methods, int logical, hwloc_obj_t level, void *output, unsigned depth, unsigned x, unsigned *retwidth, unsigned y, unsigned *retheight)
{
  unsigned textwidth = gridsize;
  unsigned textheight = (fontsize ? fontsize + gridsize : 0);
  unsigned myheight = textheight;
  unsigned mywidth = 0;
  unsigned totwidth, totheight;
  unsigned overlaidoffset = 0;
  struct style style;
  char text[64], _text[64];
  const char *collapsestr = hwloc_obj_get_info_by_name(level, "lstopoCollapse");
  unsigned collapse = collapsestr ? atoi(collapsestr) : 1;
  int n;

  DYNA_CHECK();

  if (fontsize) {
    if (collapse > 1) {
      n = lstopo_obj_snprintf(_text, sizeof(_text), level, logical);
      n = snprintf(text, sizeof(text), "%u x { %s }", collapse, _text);
    } else {
      n = lstopo_obj_snprintf(text, sizeof(text), level, logical);
    }
    textwidth = get_textwidth(output, text, n, fontsize, gridsize);
  }

  if (collapse > 1) {
    /* additional depths and height for overlaid boxes */
    depth -= 2;
    if (collapse > 2) {
      overlaidoffset = gridsize;
    } else {
      overlaidoffset = gridsize/2;
    }
    textwidth += overlaidoffset;
    textheight += overlaidoffset;
    myheight = textheight;
  }

  RECURSE_RECT(level, &null_draw_methods, gridsize, gridsize);

  lstopo_set_object_color(methods, topology, level, 0, &style);

  if (collapse > 1) {
    methods->box(output, style.bg.r, style.bg.g, style.bg.b, depth+2, x + overlaidoffset, *retwidth - overlaidoffset, y + overlaidoffset, *retheight - overlaidoffset);
    if (collapse > 2)
      methods->box(output, style.bg.r, style.bg.g, style.bg.b, depth+1, x + overlaidoffset/2, *retwidth - overlaidoffset, y + overlaidoffset/2, *retheight - overlaidoffset);
    methods->box(output, style.bg.r, style.bg.g, style.bg.b, depth, x, *retwidth - overlaidoffset, y, *retheight - overlaidoffset);
  } else {
    methods->box(output, style.bg.r, style.bg.g, style.bg.b, depth, x, *retwidth, y, *retheight);
  }

  if (fontsize)
    methods->text(output, style.t.r, style.t.g, style.t.b, fontsize, depth-1, x + gridsize, y + gridsize, text);

  RECURSE_RECT(level, methods, gridsize, gridsize);

  DYNA_SAVE();
}

static void
os_device_draw(hwloc_topology_t topology __hwloc_attribute_unused, struct draw_methods *methods, int logical __hwloc_attribute_unused, hwloc_obj_t level, void *output, unsigned depth, unsigned x, unsigned *retwidth, unsigned y, unsigned *retheight)
{
  unsigned myheight = 0, totheight;
  unsigned mywidth = 0, totwidth;
  unsigned textwidth = gridsize;
  struct style style;
  char text[64];
  int n;
  unsigned nmorelines = 0, i;
  char morelines[3][64];

  DYNA_CHECK();

  if (fontsize) {
    const char *coproctype;

    if (HWLOC_OBJ_OSDEV_COPROC == level->attr->osdev.type
        && (coproctype = hwloc_obj_get_info_by_name(level, "CoProcType")) != NULL) {

      if (!strcmp(coproctype, "CUDA")) {
	const char *value, *value2, *value3;

	value = hwloc_obj_get_info_by_name(level, "CUDAGlobalMemorySize");
	if (value) {
	  unsigned long long mb = strtoull(value, NULL, 10) / 1024;
	  snprintf(morelines[nmorelines], sizeof(morelines[0]),
		   mb >= 10240 ? "%llu GB" : "%llu MB",
		   mb >= 10240 ? mb/1024 : mb);
	  nmorelines++;
	}

	value = hwloc_obj_get_info_by_name(level, "CUDAL2CacheSize");
	if (value) {
	  unsigned long long kb = strtoull(value, NULL, 10);
	  snprintf(morelines[nmorelines], sizeof(morelines[0]),
		   kb >= 10240 ? "L2 (%llu MB)" : "L2 (%llu kB)",
		   kb >= 10240 ? kb/1024 : kb);
	  nmorelines++;
	}

	value = hwloc_obj_get_info_by_name(level, "CUDAMultiProcessors");
	value2 = hwloc_obj_get_info_by_name(level, "CUDACoresPerMP");
	value3 = hwloc_obj_get_info_by_name(level, "CUDASharedMemorySizePerMP");
	if (value && value2 && value3) {
	  snprintf(morelines[nmorelines], sizeof(morelines[0]), "%s MP x (%s cores + %s kB)", value, value2, value3);
	  nmorelines++;
	}

      } else if (!strcmp(coproctype, "MIC")) {
	const char *value;
	value = hwloc_obj_get_info_by_name(level, "MICActiveCores");
	if (value) {
	  snprintf(morelines[nmorelines], sizeof(morelines[0]), "%s cores", value);
	  nmorelines++;
	}
	value = hwloc_obj_get_info_by_name(level, "MICMemorySize");
	if (value) {
	  unsigned long long mb = strtoull(value, NULL, 10) / 1024;
	  snprintf(morelines[nmorelines], sizeof(morelines[0]),
		   mb >= 10240 ? "%llu GB" : "%llu MB",
		   mb >= 10240 ? mb/1024 : mb);
	  nmorelines++;
	}

      } else if (!strcmp(coproctype, "OpenCL")) {
	const char *value;
	value = hwloc_obj_get_info_by_name(level, "OpenCLComputeUnits");
	if (value) {
	  unsigned long long cu = strtoull(value, NULL, 10);
	  snprintf(morelines[nmorelines], sizeof(morelines[0]), "%llu compute units", cu);
	  nmorelines++;
	}
	value = hwloc_obj_get_info_by_name(level, "OpenCLGlobalMemorySize");
	if (value) {
	  unsigned long long mb = strtoull(value, NULL, 10) / 1024;
	  snprintf(morelines[nmorelines], sizeof(morelines[0]),
		   mb >= 10240 ? "%llu GB" : "%llu MB",
		   mb >= 10240 ? mb/1024 : mb);
	  nmorelines++;
	}
      }
    }

    n = lstopo_obj_snprintf(text, sizeof(text), level, logical);
    textwidth = get_textwidth(output, text, n, fontsize, gridsize);
    for(i=0; i<nmorelines; i++) {
      unsigned nn = (unsigned)strlen(morelines[i]);
      unsigned ntextwidth = get_textwidth(output, morelines[i], nn, fontsize, gridsize);
      if (ntextwidth > textwidth)
	textwidth = ntextwidth;
    }
    myheight = (fontsize + gridsize)*(nmorelines+1);
    mywidth = 0;
  }

  RECURSE_RECT(level, &null_draw_methods, gridsize, gridsize);

  lstopo_set_object_color(methods, topology, level, 0, &style);
  methods->box(output, style.bg.r, style.bg.g, style.bg.b, depth, x, *retwidth, y, *retheight);

  if (fontsize) {
    methods->text(output, style.t.r, style.t.g, style.t.b, fontsize, depth-1, x + gridsize, y + gridsize, text);
    for(i=0; i<nmorelines; i++)
      methods->text(output, style.t.r, style.t.g, style.t.b, fontsize, depth-1, x + gridsize, y + (i+2)*gridsize + (i+1)*fontsize, morelines[i]);
  }

  RECURSE_RECT(level, methods, gridsize, gridsize);

  DYNA_SAVE();
}

static void
bridge_draw(hwloc_topology_t topology, struct draw_methods *methods, int logical, hwloc_obj_t level, void *output, unsigned depth, unsigned x, unsigned *retwidth, unsigned y, unsigned *retheight)
{
  unsigned textwidth = 0;
  unsigned myheight = 0;
  /* Room for square, left link and speed */
  unsigned speedwidth = fontsize ? fontsize + gridsize : 0;
  unsigned mywidth = 2*gridsize + gridsize + speedwidth;
  unsigned totwidth, totheight;
  struct style style;
  unsigned center;

  DYNA_CHECK();

  RECURSE_VERT(level, &null_draw_methods, gridsize, 0);

  /* Square and left link */
  lstopo_set_object_color(methods, topology, level, 0, &style);
  methods->box(output, style.bg.r, style.bg.g, style.bg.b, depth, x, gridsize, y + PCI_HEIGHT/2 - gridsize/2, gridsize);
  methods->line(output, 0, 0, 0, depth, x + gridsize, y + PCI_HEIGHT/2, x + 2*gridsize, y + PCI_HEIGHT/2);

  if (level->arity > 0) {
    unsigned bottom = 0, top = 0;
    RECURSE_BEGIN(level, 0);
    RECURSE_FOR()
      RECURSE_CALL_FUN(methods);

      /* Line to PCI device */
      center = y + totheight + PCI_HEIGHT / 2;
      if (!top)
        top = center;
      bottom = center;
      methods->line(output, 0, 0, 0, depth, x + 2*gridsize, center, x + 2*gridsize + gridsize + speedwidth, center);

      /* Negotiated link speed */
      if (fontsize) {
        float speed = 0.;
        if (subobjs[i]->type == HWLOC_OBJ_PCI_DEVICE)
          speed = subobjs[i]->attr->pcidev.linkspeed;
        if (subobjs[i]->type == HWLOC_OBJ_BRIDGE && subobjs[i]->attr->bridge.upstream_type == HWLOC_OBJ_BRIDGE_PCI)
          speed = subobjs[i]->attr->bridge.upstream.pci.linkspeed;
        if (speed != 0.) {
          char text[4];
          if (speed >= 10.)
	    snprintf(text, sizeof(text), "%.0f", subobjs[i]->attr->pcidev.linkspeed);
	  else
	    snprintf(text, sizeof(text), "%0.1f", subobjs[i]->attr->pcidev.linkspeed);
          methods->text(output, style.t2.r, style.t2.g, style.t2.b, fontsize, depth-1, x + 2*gridsize + gridsize, y + totheight, text);
        }
      }
    RECURSE_END_VERT(gridsize, 0);

    /* Vertical line */
    methods->line(output, 0, 0, 0, depth, x + 2*gridsize, top, x + 2*gridsize, bottom);
  } else
    RECURSE_VERT(level, methods, gridsize, 0);

  /* Empty bridges still need some room */
  if (*retheight < PCI_HEIGHT)
    *retheight = PCI_HEIGHT;

  DYNA_SAVE();
}

static void
pu_draw(hwloc_topology_t topology, struct draw_methods *methods, int logical, hwloc_obj_t level, void *output, unsigned depth, unsigned x, unsigned *retwidth, unsigned y, unsigned *retheight)
{
  struct lstopo_output *loutput = output;
  unsigned myheight = (fontsize ? (fontsize + gridsize) : 0), totheight;
  unsigned textwidth = gridsize;
  unsigned mywidth = 0, totwidth;
  unsigned textxoffset = 0;
  char text[64];
  int n;
  struct style style;
  int colorarg;

  DYNA_CHECK();

  if (fontsize) {
    n = lstopo_obj_snprintf(text, sizeof(text), level, logical);
    textwidth = get_textwidth(output, text, n, fontsize, gridsize);
    /* if smaller than other PU, artificially extend/shift it
     * to make PU boxes nicer when vertically stacked.
     */
    if (textwidth < loutput->min_pu_textwidth) {
      textxoffset = (loutput->min_pu_textwidth - textwidth) / 2;
      textwidth = loutput->min_pu_textwidth;
    }
  }

  RECURSE_RECT(level, &null_draw_methods, 0, gridsize);

  if (lstopo_pu_offline(level))
    colorarg = 3;
  else if (lstopo_pu_forbidden(level))
    colorarg = 2;
  else if (lstopo_pu_running(topology, level))
    colorarg = 1;
  else
    colorarg = 0;
  lstopo_set_object_color(methods, topology, level, colorarg, &style);
  methods->box(output, style.bg.r, style.bg.g, style.bg.b, depth, x, *retwidth, y, *retheight);

  if (fontsize) {
    methods->text(output, style.t.r, style.t.g, style.t.b, fontsize, depth-1, x + gridsize + textxoffset, y + gridsize, text);
  }

  RECURSE_RECT(level, methods, 0, gridsize);

  DYNA_SAVE();
}

static void
cache_draw(hwloc_topology_t topology, struct draw_methods *methods, int logical, hwloc_obj_t level, void *output, unsigned depth, unsigned x, unsigned *retwidth, unsigned y, unsigned *retheight)
{
  unsigned myheight = gridsize + (fontsize ? (fontsize + gridsize) : 0) + gridsize, totheight;
  unsigned mywidth = 0, totwidth;
  unsigned textwidth = gridsize;
  /* Do not separate objects when in L1 (SMT) */
  unsigned separator = level->attr->cache.depth > 1 ? gridsize : 0;
  char text[64];
  int n;
  struct style style;

  DYNA_CHECK();

  if (fontsize) {
    n = lstopo_obj_snprintf(text, sizeof(text), level, logical);
    textwidth = get_textwidth(output, text, n, fontsize, gridsize);
    textwidth += gridsize; /* artificially extend the minimal inner size because RECURSE_RECT() uses 0 as border when computing totwidth */
  }

  RECURSE_RECT(level, &null_draw_methods, separator, 0);

  lstopo_set_object_color(methods, topology, level, 0, &style);
  methods->box(output, style.bg.r, style.bg.g, style.bg.b, depth, x, totwidth, y, myheight - gridsize);

  if (fontsize) {
    methods->text(output, style.t.r, style.t.g, style.t.b, fontsize, depth-1, x + gridsize, y + gridsize, text);
  }

  RECURSE_RECT(level, methods, separator, 0);

  DYNA_SAVE();
}

static void
core_draw(hwloc_topology_t topology, struct draw_methods *methods, int logical, hwloc_obj_t level, void *output, unsigned depth, unsigned x, unsigned *retwidth, unsigned y, unsigned *retheight)
{
  unsigned myheight = (fontsize ? (fontsize + gridsize) : 0), totheight;
  unsigned mywidth = 0, totwidth;
  unsigned textwidth = gridsize;
  char text[64];
  int n;
  struct style style;

  DYNA_CHECK();

  if (fontsize) {
    n = lstopo_obj_snprintf(text, sizeof(text), level, logical);
    textwidth = get_textwidth(output, text, n, fontsize, gridsize);
  }

  RECURSE_RECT(level, &null_draw_methods, 0, gridsize);

  lstopo_set_object_color(methods, topology, level, 0, &style);
  methods->box(output, style.bg.r, style.bg.g, style.bg.b, depth, x, totwidth, y, totheight);

  if (fontsize) {
    methods->text(output, style.t.r, style.t.g, style.t.b, fontsize, depth-1, x + gridsize, y + gridsize, text);
  }

  RECURSE_RECT(level, methods, 0, gridsize);

  DYNA_SAVE();
}

static void
package_draw(hwloc_topology_t topology, struct draw_methods *methods, int logical, hwloc_obj_t level, void *output, unsigned depth, unsigned x, unsigned *retwidth, unsigned y, unsigned *retheight)
{
  unsigned myheight = (fontsize ? (fontsize + gridsize) : 0), totheight;
  unsigned mywidth = 0, totwidth;
  unsigned textwidth = gridsize;
  char text[64];
  int n;
  struct style style;

  DYNA_CHECK();

  if (fontsize) {
    n = lstopo_obj_snprintf(text, sizeof(text), level, logical);
    textwidth = get_textwidth(output, text, n, fontsize, gridsize);
  }

  RECURSE_RECT(level, &null_draw_methods, gridsize, gridsize);

  lstopo_set_object_color(methods, topology, level, 0, &style);
  methods->box(output, style.bg.r, style.bg.g, style.bg.b, depth, x, totwidth, y, totheight);

  if (fontsize) {
    methods->text(output, style.t.r, style.t.g, style.t.b, fontsize, depth-1, x + gridsize, y + gridsize, text);
  }

  RECURSE_RECT(level, methods, gridsize, gridsize);

  DYNA_SAVE();
}

static void
node_draw(hwloc_topology_t topology, struct draw_methods *methods, int logical, hwloc_obj_t level, void *output, unsigned depth, unsigned x, unsigned *retwidth, unsigned y, unsigned *retheight)
{
  /* Reserve room for the heading memory box and separator */
  unsigned myheight = (fontsize ? (gridsize + fontsize) : 0) + gridsize + gridsize;
  /* Currently filled height */
  unsigned totheight;
  /* Nothing on the left */
  unsigned mywidth = 0;
  /* Currently filled width */
  unsigned totwidth;
  /* Width of the heading text, thus minimal width */
  unsigned textwidth = gridsize;
  char text[64];
  int n;
  struct style style;

  /* Check whether dynamic programming can save us time */
  DYNA_CHECK();

  if (fontsize) {
    n = lstopo_obj_snprintf(text, sizeof(text), level, logical);
    textwidth = get_textwidth(output, text, n, fontsize, gridsize);
    textwidth += 2*gridsize; /* so artificially extend the minimal inner size to include space between space between epoxy and memory box */
  }

  /* Compute the size needed by sublevels */
  RECURSE_RECT(level, &null_draw_methods, gridsize, gridsize);

  lstopo_set_object_color(methods, topology, level, 0 /* node */, &style);
  /* Draw the epoxy box */
  methods->box(output, style.bg.r, style.bg.g, style.bg.b, depth, x, totwidth, y, totheight);
  /* Draw the memory box */
  methods->box(output, style.bg2.r, style.bg2.g, style.bg2.b, depth-1, x + gridsize, totwidth - 2 * gridsize, y + gridsize, myheight - gridsize);

  if (fontsize) {
    methods->text(output, style.t2.r, style.t2.g, style.t2.b, fontsize, depth-2, x + 2 * gridsize, y + 2 * gridsize, text);
  }

  /* Restart, now really drawing sublevels */
  RECURSE_RECT(level, methods, gridsize, gridsize);

  /* Save result for dynamic programming */
  DYNA_SAVE();
}

static void
machine_draw(hwloc_topology_t topology, struct draw_methods *methods, int logical, hwloc_obj_t level, void *output, unsigned depth, unsigned x, unsigned *retwidth, unsigned y, unsigned *retheight)
{
  unsigned myheight = (fontsize ? (fontsize + gridsize) : 0), totheight;
  unsigned mywidth = 0, totwidth;
  unsigned textwidth = gridsize;
  char text[64];
  int n;
  struct style style;

  DYNA_CHECK();

  if (fontsize) {
    n = lstopo_obj_snprintf(text, sizeof(text), level, logical);
    textwidth = get_textwidth(output, text, n, fontsize, gridsize);
  }

  RECURSE_RECT(level, &null_draw_methods, gridsize, gridsize);

  lstopo_set_object_color(methods, topology, level, 0 /* machine */, &style);
  methods->box(output, style.bg.r, style.bg.g, style.bg.b, depth, x, totwidth, y, totheight);

  if (fontsize) {
    methods->text(output, style.t.r, style.t.g, style.t.b, fontsize, depth-1, x + gridsize, y + gridsize, text);
  }

  RECURSE_RECT(level, methods, gridsize, gridsize);

  DYNA_SAVE();
}

#define NETWORK_DRAW_BEGIN() do { \
  /* network of machines, either horizontal or vertical */ \
  if (vert) { \
    mywidth += gridsize; \
    RECURSE_VERT(level, &null_draw_methods, gridsize, gridsize); \
  } else \
    RECURSE_HORIZ(level, &null_draw_methods, gridsize, gridsize); \
} while(0)

#define NETWORK_DRAW_END() do { \
  if (vert) { \
    unsigned top = 0, bottom = 0; \
    unsigned center; \
    RECURSE_BEGIN(level, gridsize) \
    RECURSE_FOR() \
      RECURSE_CALL_FUN(methods); \
      center = y + totheight + height / 2; \
      if (!top) \
        top = center; \
      bottom = center; \
      methods->line(output, 0, 0, 0, depth, x + mywidth, center, x + mywidth + gridsize, center); \
    RECURSE_END_VERT(gridsize, gridsize); \
 \
    methods->line(output, 0, 0, 0, depth, x + mywidth, top, x + mywidth, bottom); \
  } else { \
    unsigned left = 0, right = 0; \
    unsigned center; \
    RECURSE_BEGIN(level, gridsize) \
    RECURSE_FOR() \
      RECURSE_CALL_FUN(methods); \
      center = x + totwidth + width / 2; \
      if (!left) \
        left = center; \
      right = center; \
      methods->line(output, 0, 0, 0, depth, center, y + myheight, center, y + myheight + gridsize); \
    RECURSE_END_HORIZ(gridsize, gridsize); \
 \
    methods->line(output, 0, 0, 0, depth, left, y + myheight, right, y + myheight); \
  } \
} while(0)

static void
system_draw(hwloc_topology_t topology, struct draw_methods *methods, int logical, hwloc_obj_t level, void *output, unsigned depth, unsigned x, unsigned *retwidth, unsigned y, unsigned *retheight)
{
  unsigned myheight = (fontsize ? (fontsize + gridsize) : 0), totheight;
  unsigned mywidth = 0, totwidth;
  unsigned textwidth = gridsize;
  int vert = prefer_vert(topology, logical, level, output, depth, x, y, gridsize);
  char text[64];
  int n;
  struct style style;

  DYNA_CHECK();

  if (fontsize) {
    n = lstopo_obj_snprintf(text, sizeof(text), level, logical);
    textwidth = get_textwidth(output, text, n, fontsize, gridsize);
  }

  if (level->arity > 1 && (level->children[0]->type == HWLOC_OBJ_MACHINE || !level->children[0]->cpuset))
    NETWORK_DRAW_BEGIN();
  else
    RECURSE_RECT(level, &null_draw_methods, gridsize, gridsize);

  lstopo_set_object_color(methods, topology, level, 1 /* system */, &style);
  methods->box(output, style.bg.r, style.bg.g, style.bg.b, depth, x, totwidth, y, totheight);

  if (fontsize) {
    methods->text(output, style.t.r, style.t.g, style.t.b, fontsize, depth-1, x + gridsize, y + gridsize, text);
  }

  if (level->arity > 1 && (level->children[0]->type == HWLOC_OBJ_MACHINE || !level->children[0]->cpuset))
    NETWORK_DRAW_END();
  else
    RECURSE_RECT(level, methods, gridsize, gridsize);

  DYNA_SAVE();
}

static void
group_draw(hwloc_topology_t topology, struct draw_methods *methods, int logical, hwloc_obj_t level, void *output, unsigned depth, unsigned x, unsigned *retwidth, unsigned y, unsigned *retheight)
{
  unsigned myheight = (fontsize ? (fontsize + gridsize) : 0), totheight;
  unsigned mywidth = 0, totwidth;
  unsigned textwidth = gridsize;
  int vert = prefer_vert(topology, logical, level, output, depth, x, y, gridsize);
  char text[64];
  int n;
  struct style style;

  DYNA_CHECK();

  if (fontsize) {
    n = lstopo_obj_snprintf(text, sizeof(text), level, logical);
    textwidth = get_textwidth(output, text, n, fontsize, gridsize);
  }

  if (level->arity > 1 && (level->children[0]->type == HWLOC_OBJ_MACHINE || !level->children[0]->cpuset))
    NETWORK_DRAW_BEGIN();
  else
    RECURSE_RECT(level, &null_draw_methods, gridsize, gridsize);

  lstopo_set_object_color(methods, topology, level, 0, &style);
  methods->box(output, style.bg.r, style.bg.g, style.bg.b, depth, x, totwidth, y, totheight);

  if (fontsize) {
    methods->text(output, style.t.r, style.t.g, style.t.b, fontsize, depth-1, x + gridsize, y + gridsize, text);
  }

  if (level->arity > 1 && (level->children[0]->type == HWLOC_OBJ_MACHINE || !level->children[0]->cpuset))
    NETWORK_DRAW_END();
  else
    RECURSE_RECT(level, methods, gridsize, gridsize);

  DYNA_SAVE();
}

static void
misc_draw(hwloc_topology_t topology, struct draw_methods *methods, int logical, hwloc_obj_t level, void *output, unsigned depth, unsigned x, unsigned *retwidth, unsigned y, unsigned *retheight)
{
  unsigned boxheight = gridsize + (fontsize ? (fontsize + gridsize) : 0);
  unsigned myheight = boxheight + (level->arity?gridsize:0), totheight;
  unsigned mywidth = 0, totwidth;
  unsigned textwidth = gridsize;
  int vert = prefer_vert(topology, logical, level, output, depth, x, y, gridsize);
  char text[64];
  int n;
  struct style style;

  DYNA_CHECK();

  if (fontsize) {
    n = lstopo_obj_snprintf(text, sizeof(text), level, logical);
    textwidth = get_textwidth(output, text, n, fontsize, gridsize);
    textwidth += gridsize; /* artificially extend the minimal inner size because RECURSE_RECT() uses 0 as border when computing totwidth */
  }

  if (level->arity > 1 && (level->children[0]->type == HWLOC_OBJ_MACHINE || !level->children[0]->cpuset))
    NETWORK_DRAW_BEGIN();
  else
    RECURSE_HORIZ(level, &null_draw_methods, gridsize, 0);

  lstopo_set_object_color(methods, topology, level, 0, &style);
  methods->box(output, style.bg.r, style.bg.g, style.bg.b, depth, x, totwidth, y, boxheight);

  if (fontsize) {
    methods->text(output, style.t.r, style.t.g, style.t.b, fontsize, depth-1, x + gridsize, y + gridsize, text);
  }

  if (level->arity > 1 && (level->children[0]->type == HWLOC_OBJ_MACHINE || !level->children[0]->cpuset))
    NETWORK_DRAW_END();
  else
    RECURSE_HORIZ(level, methods, gridsize, 0);

  DYNA_SAVE();
}

static void
fig(hwloc_topology_t topology, struct draw_methods *methods, int logical, int legend, hwloc_obj_t level, void *output, unsigned depth, unsigned x, unsigned y)
{
  unsigned totwidth, totheight, offset, i;
  time_t t;
  char text[128];
  char hostname[128] = "";
  const char *forcedhostname = NULL;
  unsigned long hostname_size = sizeof(hostname);

  system_draw(topology, methods, logical, level, output, depth, x, &totwidth, y, &totheight);

  if (totwidth < 20*fontsize)
    totwidth = 20*fontsize;

  if (legend) {
      /* Display the hostname, but only if we're showing *this*
         system */
    forcedhostname = hwloc_obj_get_info_by_name(hwloc_get_root_obj(topology), "HostName");
    if (!forcedhostname && hwloc_topology_is_thissystem(topology)) {
#if defined(HWLOC_WIN_SYS) && !defined(__CYGWIN__)
      GetComputerName(hostname, &hostname_size);
#else
      gethostname(hostname, hostname_size);
#endif
    }
    if (forcedhostname || *hostname) {
      if (forcedhostname)
	snprintf(text, sizeof(text), "Host: %s", forcedhostname);
      else
	snprintf(text, sizeof(text), "Host: %s", hostname);
      methods->box(output, 0xff, 0xff, 0xff, depth, 0, totwidth, totheight, gridsize + (gridsize + fontsize)*(3+lstopo_append_legends_nr));
      methods->text(output, 0, 0, 0, fontsize, depth, gridsize, totheight + gridsize, text);
      offset = gridsize + fontsize;
    } else {
      methods->box(output, 0xff, 0xff, 0xff, depth, 0, totwidth, totheight, gridsize + (gridsize + fontsize)*(2+lstopo_append_legends_nr));
      offset = 0;
    }

    /* Display whether we're showing physical or logical IDs */
    snprintf(text, sizeof(text), "Indexes: %s", logical ? "logical" : "physical");
    methods->text(output, 0, 0, 0, fontsize, depth, gridsize, totheight + gridsize + offset, text);

    /* Display timestamp */
    t = time(NULL);
#ifdef HAVE_STRFTIME
    {
      struct tm *tmp;
      tmp = localtime(&t);
      strftime(text, sizeof(text), "Date: %c", tmp);
    }
#else /* HAVE_STRFTIME */
    {
      char *date;
      int n;
      date = ctime(&t);
      n = strlen(date);
      if (n && date[n-1] == '\n') {
        date[n-1] = 0;
      }
      snprintf(text, sizeof(text), "Date: %s", date);
    }
#endif /* HAVE_STRFTIME */
    methods->text(output, 0, 0, 0, fontsize, depth, gridsize, totheight + gridsize + offset + fontsize + gridsize, text);

    offset = totheight + gridsize + offset + (fontsize + gridsize) * 2;
    for(i=0; i<lstopo_append_legends_nr; i++) {
      methods->text(output, 0, 0, 0, fontsize, depth, gridsize, offset, lstopo_append_legends[i]);
      offset += fontsize + gridsize;
    }
  }
}

/*
 * given a type, return a pointer FUN to the function that draws it.
 */
static foo_draw
get_type_fun(hwloc_obj_type_t type)
{
  switch (type) {
    case HWLOC_OBJ_SYSTEM: return system_draw;
    case HWLOC_OBJ_MACHINE: return machine_draw;
    case HWLOC_OBJ_NUMANODE: return node_draw;
    case HWLOC_OBJ_PACKAGE: return package_draw;
    case HWLOC_OBJ_CACHE: return cache_draw;
    case HWLOC_OBJ_CORE: return core_draw;
    case HWLOC_OBJ_PU: return pu_draw;
    case HWLOC_OBJ_GROUP: return group_draw;
    case HWLOC_OBJ_PCI_DEVICE: return pci_device_draw;
    case HWLOC_OBJ_OS_DEVICE: return os_device_draw;
    case HWLOC_OBJ_BRIDGE: return bridge_draw;
    default:
    case HWLOC_OBJ_MISC: return misc_draw;
    case HWLOC_OBJ_TYPE_MAX: assert(0);
  }
  /* for dumb compilers */
  return misc_draw;
}

void
output_draw_start(struct lstopo_output *output)
{
  struct draw_methods *methods = output->methods;
  methods->init(output);
  methods->declare_color(output, 0, 0, 0);
  methods->declare_color(output, NODE_R_COLOR, NODE_G_COLOR, NODE_B_COLOR);
  methods->declare_color(output, PACKAGE_R_COLOR, PACKAGE_G_COLOR, PACKAGE_B_COLOR);
  methods->declare_color(output, MEMORY_R_COLOR, MEMORY_G_COLOR, MEMORY_B_COLOR);
  methods->declare_color(output, CORE_R_COLOR, CORE_G_COLOR, CORE_B_COLOR);
  methods->declare_color(output, THREAD_R_COLOR, THREAD_G_COLOR, THREAD_B_COLOR);
  methods->declare_color(output, RUNNING_R_COLOR, RUNNING_G_COLOR, RUNNING_B_COLOR);
  methods->declare_color(output, FORBIDDEN_R_COLOR, FORBIDDEN_G_COLOR, FORBIDDEN_B_COLOR);
  methods->declare_color(output, OFFLINE_R_COLOR, OFFLINE_G_COLOR, OFFLINE_B_COLOR);
  methods->declare_color(output, CACHE_R_COLOR, CACHE_G_COLOR, CACHE_B_COLOR);
  methods->declare_color(output, MACHINE_R_COLOR, MACHINE_G_COLOR, MACHINE_B_COLOR);
  methods->declare_color(output, SYSTEM_R_COLOR, SYSTEM_G_COLOR, SYSTEM_B_COLOR);
  methods->declare_color(output, MISC_R_COLOR, MISC_G_COLOR, MISC_B_COLOR);
  methods->declare_color(output, PCI_DEVICE_R_COLOR, PCI_DEVICE_G_COLOR, PCI_DEVICE_B_COLOR);
  methods->declare_color(output, BRIDGE_R_COLOR, BRIDGE_G_COLOR, BRIDGE_B_COLOR);
  lstopo_prepare_custom_styles(output, hwloc_get_root_obj(output->topology));
}

static void
output_compute_pu_min_textwidth(struct lstopo_output *output)
{
  char text[64];
  int n;
  hwloc_topology_t topology = output->topology;
  hwloc_obj_t lastpu;

  if (!output->methods->textsize) {
    output->min_pu_textwidth = 0;
    return;
  }

  if (output->logical) {
    unsigned depth = hwloc_get_type_depth(topology, HWLOC_OBJ_PU);
    lastpu = hwloc_get_obj_by_depth(topology, depth, hwloc_get_nbobjs_by_depth(topology, depth)-1);
  } else if (hwloc_topology_get_topology_cpuset(topology)) {
    unsigned lastidx = hwloc_bitmap_last(hwloc_topology_get_topology_cpuset(topology));
    lastpu = hwloc_get_pu_obj_by_os_index(topology, lastidx);
  } else {
    /* no easy way to find the max os_index in custom topologies */
    unsigned depth = hwloc_get_type_depth(topology, HWLOC_OBJ_PU);
    hwloc_obj_t curpu = hwloc_get_obj_by_depth(topology, depth, 0);
    lastpu = curpu;
    while (curpu) {
      if (curpu->os_index > lastpu->os_index)
	lastpu = curpu;
      curpu = curpu->next_cousin;
    }
  }

  n = lstopo_obj_snprintf(text, sizeof(text), lastpu, output->logical);
  output->min_pu_textwidth = get_textwidth(output, text, n, fontsize, gridsize);
}

void
output_draw(struct lstopo_output *output)
{
  output_compute_pu_min_textwidth(output);
  fig(output->topology, output->methods, output->logical, output->legend, hwloc_get_root_obj(output->topology), output, 100, 0, 0);
}
