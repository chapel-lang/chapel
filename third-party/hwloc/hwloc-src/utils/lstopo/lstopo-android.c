/*
 * Copyright © 2019-2021 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <private/autogen/config.h>
#include <hwloc.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "lstopo.h"

extern void JNIbox(int r, int g, int b, int x, int y, int width, int height, unsigned style, int gp_index, char *info);
extern void JNItext(char *text, int gp_index, int x, int y, int fontsize, int bold);
extern void JNIline(unsigned x1, unsigned y1, unsigned x2, unsigned y2);
extern void JNIprepare(int width, int height, int fontsize);

#define ANDROID_TEXT_WIDTH(length, fontsize) (((length) * (fontsize))/2)
#define ANDROID_FONTSIZE_SCALE(size) (((size) * 11) / 9)

static void native_android_box(struct lstopo_output *loutput, const struct lstopo_color *lcolor, unsigned depth __hwloc_attribute_unused, unsigned x, unsigned width, unsigned y, unsigned height, hwloc_obj_t obj, unsigned box_id __hwloc_attribute_unused)
{
    unsigned cpukind_style = lstopo_obj_cpukind_style(loutput, obj);
    int gp_index = -1;
    int r = lcolor->r, g = lcolor->g, b = lcolor->b;
    char * info = malloc(1096);
    const char * sep = " ";
    unsigned style = 0;

    if(obj){
        gp_index = obj->gp_index;
        hwloc_obj_attr_snprintf(info, 1096, obj, sep, 1);
    }

    if (cpukind_style)
        style = cpukind_style;

    JNIbox(r, g, b, x, y, width, height, style, gp_index, info);
    //Creating a usable java string from char * may trigger an UTF-8 error
    //This code creates a byte array from the char * variable before creating the java string
}


static void
native_android_line(struct lstopo_output *loutput __hwloc_attribute_unused, unsigned depth __hwloc_attribute_unused, unsigned x1, unsigned y1, unsigned x2, unsigned y2, hwloc_obj_t obj __hwloc_attribute_unused, unsigned line_id __hwloc_attribute_unused)
{
    JNIline(x1, y1, x2, y2);
}

static void
native_android_textsize(struct lstopo_output *loutput __hwloc_attribute_unused, const char *text __hwloc_attribute_unused, unsigned textlength, unsigned fontsize, unsigned *width)
{
    fontsize = ANDROID_FONTSIZE_SCALE(fontsize);
    *width = ANDROID_TEXT_WIDTH(textlength, fontsize);
}


static void
native_android_text(struct lstopo_output *loutput, const struct lstopo_color *lcolor __hwloc_attribute_unused, int size __hwloc_attribute_unused, unsigned depth __hwloc_attribute_unused, unsigned x, unsigned y, const char *text, hwloc_obj_t obj, unsigned text_id __hwloc_attribute_unused)
{
    unsigned cpukind_style = lstopo_obj_cpukind_style(loutput, obj);
    int gp_index = -1;
    int bold = 0;

    if(obj)
        gp_index = obj->gp_index;

    if (cpukind_style % 2)
        bold = 1;

    JNItext((char *)text, gp_index, x, y, loutput->fontsize, bold);
}

static struct draw_methods native_android_draw_methods = {
        NULL,
        NULL,
        native_android_box,
        native_android_line,
        native_android_text,
        native_android_textsize,
};

int output_android(struct lstopo_output * loutput, const char *filename __hwloc_attribute_unused)
{
    loutput->methods = &native_android_draw_methods;
    loutput->backend_flags |= LSTOPO_BACKEND_FLAG_APPROXIMATIVE_TEXTWIDTH;

    /* recurse once for preparing sizes and positions */
    loutput->drawing = LSTOPO_DRAWING_PREPARE;
    output_draw(loutput);
    loutput->drawing = LSTOPO_DRAWING_DRAW;

    /* ready */
    declare_colors(loutput);
    lstopo_prepare_custom_styles(loutput);
    JNIprepare(loutput->height, loutput->width, loutput->fontsize);

    output_draw(loutput);

    return 0;
}
