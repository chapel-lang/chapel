#ifndef VIEWFIELD_H
#define VIEWFIELD_H

class ViewField;  // So we can include chplvis.h

#include <FL/Fl.H>
#include <FL/Fl_Box.H>

#include "DataModel.h"

// extern Fl_Double_Window *MainWin;

class ViewField : public Fl_Box {

  private:

    int numlocales;
    int cx, cy; // center of the ViewField
    double rx, ry; // Radius of the locales
    double angle;
    double start;

  public:

  ViewField (int bx, int by, int bw, int bh, const char *label = 0);

  ViewField (Fl_Boxtype b, int bx, int by, int bw, int bh,
             const char *label = 0);

  //  Virtual methods to override
  void draw(void);

  int handle(int event);

  // Processing routines

  void setNumLocales(int n)
    { 
      printf("NumLocalse set to %d\n", n);
      numlocales = n;
      angle = 6.28318530717958647688 / numlocales;
      start = ( numlocales % 2 == 0 ? angle / 2 : 0 );
    }

  int  getNumLocales(void) { return numlocales; }
  
  void drawLocale(int ix, Fl_Color col);

};

#endif
