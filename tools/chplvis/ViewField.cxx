
#include "ViewField.h"
#include "chplvis.h"
#include "math.h"
#include <FL/fl_draw.H>

extern DataModel VisData;

void ViewField::draw()
{

  int cx = x() + w()/2;
  int cy = y() + h()/2;

  double rx = cx * 0.75;
  double ry = cy * 0.75;

  int numLocales = VisData.NumLocales();

  Fl_Box::draw();

  if (numLocales > 0 ) {

    double angle = 6.28318530717958647688 / numLocales;
    double start = ( numLocales % 2 == 0 ? angle / 2 : 0 );

    int ix;

    for (ix = 0; ix < numLocales; ix++) {
      // Draw a locale ...
      int lx = cx - (int) rint( rx * sin (angle * ix - start) );
      int ly = cy - (int) rint( ry * cos (angle * ix - start) );

      fl_color(FL_GREEN);
      fl_rectf(lx-15, ly-15, 30, 30);
      fl_color(FL_BLACK);
      fl_rect(lx-15, ly-15, 30, 30);

    }

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
