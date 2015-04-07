#ifndef VIEWFIELD_H
#define VIEWFIELD_H

class ViewField;  // So we can include chplvis.h

#include <FL/Fl.H>
#include <FL/Fl_Box.H>

#include "DataModel.h"

// extern Fl_Double_Window *MainWin;

struct localeInfo {
  int x;
  int y;
  int w;
  int h;
  int numTasks;
  Fl_Color heat;
};

static const double twopi = 6.28318530717958647688;
static const double pi = 3.14159265358979323844;

class ViewField : public Fl_Box {

  private:

    int numlocales;
    int cx, cy; // center of the ViewField
    double rx, ry; // Radius of the locales
    double angle;
    double start;

    localeInfo *theLocales;
    int getSize; // need this to deallocate after changing numlocales
    int **numGets;

    void allocArrays();

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
      //printf("NumLocalse set to %d\n", n);
      numlocales = n;
      angle = twopi / numlocales;
      start = ( numlocales % 2 == 0 ? angle / 2 : 0 );
      allocArrays();
    }

  int  getNumLocales(void) { return numlocales; }

  // Draw a "locale box, with ix as the label on it
  void drawLocale(int ix, Fl_Color col);

  // Draw a comm line between loc1 and loc2, color changing in the middle
  void drawCommLine(int ix1, Fl_Color col1,  int ix2, Fl_Color col2);

};

#endif
