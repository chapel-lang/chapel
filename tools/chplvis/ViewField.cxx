
#include "chplvis.h"
#include "ViewField.h"
#include "math.h"
#include <FL/fl_draw.H>

static Fl_Color heatColor ( int val, int max ) {
  return fl_rgb_color( 255*((0.0+val)/max), 255 * ((-1.0+max-val)/max), 0);
}
		   

// extern DataModel VisData;

  ViewField::ViewField (int bx, int by, int bw, int bh, const char *label)
   : Fl_Box (bx, by, bw, bh, 0) 
    {
      printf ("ViewField init. h=%d, w=%d, numlocales is %d\n",bh,bw, VisData.NumLocales());
      numlocales = 0;
      if (VisData.NumLocales() > 0)
        setNumLocales(VisData.NumLocales());
    };

  ViewField::ViewField (Fl_Boxtype b, int bx, int by, int bw, int bh,
             const char *label)
    : Fl_Box (b, bx, by, bw, bh, 0)
    {
      // printf ("ViewField init with boxtype. h=%d, w=%d\n",bh,bw);
      numlocales = 0;
      if (VisData.NumLocales() > 0)
        setNumLocales(VisData.NumLocales());
    };

void ViewField::drawLocale ( int ix, Fl_Color col)
{
    printf ("drawLocale %d\n", ix);
    char ixchars[10];

    // Draw a locale ...
    int lx = cx + (int) rint( rx * sin (angle * ix - start) );
    int ly = cy - (int) rint( ry * cos (angle * ix - start) );
    snprintf (ixchars, 10, "%d", ix);

    fl_color(col);
    fl_rectf(lx-15, ly-15, 30, 30);
    fl_color(FL_BLACK);
    fl_rect(lx-15, ly-15, 30, 30);
    fl_draw(ixchars, lx-12, ly-12, 24, 24, FL_ALIGN_CENTER, NULL, 0);
}

void ViewField::draw()
{
  printf ("ViewField draw, numlocales is %d\n", numlocales);

  cx = x() + w()/2;
  cy = y() + h()/2;
  rx = cx * 0.75;
  ry = cy * 0.75;

  Fl_Box::draw();

  int ix;
  for (ix = 0; ix < numlocales; ix++) {
     drawLocale(ix, heatColor(ix, numlocales));
  }
}

int ViewField::handle(int event)
{
  switch (event) {
  case FL_PUSH:
    printf ("Push at (%d,%d)\n", Fl::event_x(), Fl::event_y());
    break;
  case FL_RELEASE:
    printf ("Release at (%d,%d)\n", Fl::event_x(), Fl::event_y());
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
