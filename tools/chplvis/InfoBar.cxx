

#include "InfoBar.h"
#include "ViewField.h"  // To get access to heatColor

#include <FL/fl_draw.H>

void InfoBar::draw(void)
{
  Fl_Box::draw();

  fl_color(FL_BLACK);
  fl_line_style(FL_SOLID, 2, NULL);
  fl_line(x(),y()+h(),x()+w(),y()+h());

  // Color Reference
  int ix;
  for (ix = 0; ix < 30; ix++) {
    fl_color(heatColor(ix+1,30));
    fl_line_style(FL_SOLID,3,NULL);
    fl_line(x()+40+3*ix,y()+5,x()+40+3*ix,y()+h()-10);
    if ((ix%3) == 0 && ix != 0) {
      fl_color(FL_BLACK);
      fl_line_style(FL_SOLID,1,NULL);
      fl_line(x()+40+3*ix, y()+h()-9, x()+40+3*ix, y()+h()-5);
    }
  }
  fl_color(FL_BLACK);
  fl_line_style(FL_SOLID,1,NULL);
  fl_draw("Min", x()+10, y()+5, 15, h()-10, FL_ALIGN_CENTER, NULL, 0);
  fl_draw("Max", x()+140, y()+5, 15, h()-10, FL_ALIGN_CENTER, NULL, 0);

  // Messages on max counts
  char mesg[150];
  if (maxTasks > 0) {
    snprintf (mesg, 150, "maxTasks: %d", maxTasks);
    fl_draw(mesg, x()+170, y()+5, 120, h()-10, FL_ALIGN_CENTER, NULL, 0);
    snprintf (mesg, 150, "maxComms: %d", maxComms);
    fl_draw(mesg, x()+320, y()+5, 120, h()-10, FL_ALIGN_CENTER, NULL, 0);
  }
}


