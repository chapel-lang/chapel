
#include "chplvis.h"
#include "ViewField.h"
#include "math.h"
#include <FL/fl_draw.H>

static Fl_Color heatColor ( int val, int max ) {
  // if (val == 0) return FL_WHITE;
  return fl_rgb_color( 255*((0.0+val)/max), 255 * ((-1.0+max-val)/max), 0);
}

// Private methods ...

void ViewField::allocArrays()
{
  int ix;
  int ix2;

  //printf ("allocArrays\n");
  // Dealloc anything current
  if (theLocales != NULL)  delete [] theLocales;
  if (numGets != NULL) {
    for (ix = 0; ix < getSize; ix++)  delete [] numGets[ix];
    delete [] numGets;
  }

  // Alloc new space
  theLocales = new localeInfo [numlocales];
  numGets = new  int* [numlocales];
  if (theLocales == NULL || numGets == NULL) {
    fprintf (stderr, "chplvis: out of memory.\n");
    exit(1);
  }
  getSize = numlocales;
  for (ix = 0; ix < getSize; ix++) {
    numGets[ix] = new int[numlocales];
    if (numGets[ix] == NULL) {
      fprintf (stderr, "chplvis: out of memory.\n");
      exit(1);
    }
    for (ix2 = 0; ix2 < numlocales; ix2++)
      numGets[ix][ix2] = 0;
  }

  for (ix = 0; ix < numlocales; ix++)
    theLocales[ix].numTasks = 0;
}


// extern DataModel VisData;

  ViewField::ViewField (int bx, int by, int bw, int bh, const char *label)
   : Fl_Box (bx, by, bw, bh, 0) 
    {
      // printf ("ViewField init. h=%d, w=%d, numlocales is %d\n",bh,bw, VisData.NumLocales());
      numlocales = 0;
      theLocales = NULL;
      if (VisData.NumLocales() > 0) {
        setNumLocales(VisData.NumLocales());
      }
    };

  ViewField::ViewField (Fl_Boxtype b, int bx, int by, int bw, int bh,
             const char *label)
    : Fl_Box (b, bx, by, bw, bh, 0)
    {
      // printf ("ViewField init with boxtype. h=%d, w=%d\n",bh,bw);
      numlocales = 0;
      theLocales = NULL;
      numGets = NULL;
      if (VisData.NumLocales() > 0) {
        setNumLocales(VisData.NumLocales());
      }
    };

void ViewField::drawLocale ( int ix, Fl_Color col)
{
  //printf ("drawLocale %d\n", ix);
    char ixchars[10];
    if (theLocales != NULL) {
      localeInfo *loc = &theLocales[ix];
      
      // Draw a locale ...
      loc->x = cx + (int) rint(rx * sin (angle * ix - start));
      loc->y = cy - (int) rint(ry * cos (angle * ix - start));
      snprintf (ixchars, 10, "%d", ix);
      loc->w = 30;
      loc->h = 30;
      
      fl_color(col);
      fl_rectf(loc->x-loc->w/2, loc->y-loc->h/2, loc->w, loc->h);
      fl_color(FL_BLACK);
      fl_rect(loc->x-loc->w/2, loc->y-loc->h/2, loc->w, loc->h);
      fl_draw(ixchars, loc->x+3-loc->w/2, loc->y+3-loc->h/2,
	      loc->w-6, loc->h-6, FL_ALIGN_CENTER, NULL, 0);
    }
}

void ViewField::drawCommLine (int ix1, Fl_Color col1,  int ix2, Fl_Color col2)
{
  int x1, x2, y1, y2, dx, dy, midx, midy;
  if (ix1 < 0 || ix1 >= numlocales || ix2 < 0 || ix2 >= numlocales) {
    fprintf (stderr, "DATA ERROR: drawCommLine\n");
    return;
  }
  localeInfo *loc1 = &theLocales[ix1], *loc2 = &theLocales[ix2];
  double theta, cw, ch;
  
  dx = loc2->x - loc1->x;
  dy = loc2->y - loc1->y;
  if (dx == 0) {
    theta = (dy < 0 ? pi / 2 : pi * 1.5);
  } else {
    if (dx < 0) 
      theta = pi + atan((double)dy/(double)dx);
    else
      theta = atan((double)dy/(double)dx);
  }
  cw = loc1->w / sin(pi/4);
  ch = loc1->h / sin(pi/4);
  //printf ("dx = %d, dy = %d, Theta = %lf\n", dx, dy, theta);
  
  //  Need correct calculations here.
  x1 = rint (cw/2 * cos(theta));
  x1 = loc1->x + (abs(x1) > loc1->w/2 ? (dx < 0 ? -1 : 1)*loc1->w/2 : x1);
  y1 = rint (ch/2 * sin(theta));
  y1 = loc1->y + (abs(y1) > loc1->h/2 ? (dy < 0 ? -1 : 1)*loc1->h/2 : y1);

  x2 = rint (cw/2 * cos(pi+theta));
  x2 = loc2->x + (abs(x2) > loc2->w/2 ? (dx < 0 ? 1 : -1)*loc2->w/2 : x2);
  y2 = rint (ch/2 * sin(pi+theta));
  y2 = loc2->y + (abs(y2) > loc2->h/2 ? (dy < 0 ? 1 : -1)*loc2->h/2 : y2);

  midx = loc1->x + dx/2;
  midy = loc1->y + dy/2;

  fl_color(col1);
  fl_line_style(FL_SOLID, 3, NULL);
  fl_line(x1,y1,midx,midy);
  fl_color(col2);
  fl_line_style(FL_SOLID, 3, NULL);
  fl_line(midx,midy,x2,y2);
  fl_line_style(FL_SOLID, 1, NULL);
}

void ViewField::draw()
{
  // printf ("ViewField draw, numlocales is %d\n", numlocales);

  cx = x() + w()/2;
  cy = y() + h()/2;
  rx = cx * 0.70;
  ry = cy * 0.70;

  Fl_Box::draw();

  int ix;
  for (ix = 0; ix < numlocales; ix++) {
    drawLocale(ix, heatColor(ix, numlocales));
  }

  int iy;
  for (ix = 0; ix < numlocales-1; ix++) {
    for (iy = ix + 1; iy < numlocales; iy++)
      drawCommLine(ix, heatColor(ix,numlocales),
		   iy, heatColor(numlocales-iy,numlocales));
  }
}

int ViewField::handle(int event)
{
  int ix;

  int x = Fl::event_x();
  int y = Fl::event_y();
  
  switch (event) {
  case FL_PUSH:
    printf ("Push at (%d,%d)\n", x, y);
    break;
  case FL_RELEASE:
    printf ("Release at (%d,%d)\n", x, y);
    if (numlocales > 0) {
      for (ix = 0; ix < numlocales; ix++) {
	// See if release is inside a locale
	localeInfo *loc = &theLocales[ix];
	if ( x > loc->x && x <= loc->x + loc->w &&
	     y > loc->y && y <= loc->y + loc->h) {
	  printf ("release inside locale %d.\n", ix);
	}
      }
    }
    break;
  }
  return Fl_Box::handle(event);
}




#if 0

    
    // printf ("Params: x %d, y %d, w %d, h %d\n", x(), y(), w(), h());
    
    fl_color(FL_BLUE);
    
    fl_line(x()+30,y()+30,x()+w()-30,y()+h()-30);
    
    fl_color(FL_RED);
    fl_rectf( x(), y(), 30, 30);     
    fl_color(FL_BLACK);      
    fl_rect( x(), y(), 30, 30);     
    
    //fl_draw_box (FL_UP_FRAME, x(), y(), 30, 30, FL_BLUE);

    fl_color(FL_GRAY);
    fl_rectf(cx-15, cy-15, 30, 30);
    fl_color(FL_BLACK);
    fl_rect(cx-15, cy-15, 30, 30);
    
    
    //fl_draw_box (FL_UP_FRAME, cx-15, cy-15, 30, 30, FL_GREEN);
    
    fl_color(FL_BLUE);
    fl_rectf(x()+w()-30, y()+h()-30, 30, 30);
    fl_color(FL_BLACK);
    fl_rect(x()+w()-30, y()+h()-30, 30, 30);
    
    //fl_draw_box (FL_UP_FRAME, x()+w()-30, y()+h()-30, 30, 30, FL_BLUE);

#endif
