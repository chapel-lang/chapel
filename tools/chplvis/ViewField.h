#ifndef VIEWFIELD_H
#define VIEWFIELD_H

#include <FL/Fl.H>
//#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>

#include "DataModel.h"

// extern Fl_Double_Window *MainWin;

class ViewField : public Fl_Box {

  private:

    int numlocales;

  public:

  ViewField (int bx, int by, int bw, int bh, const char *label = 0)
   : Fl_Box (bx, by, bw, bh, 0) 
    {
      printf ("ViewField init. h=%d, w=%d\n",bh,bw);
      numlocales = 1;
    };

  ViewField (Fl_Boxtype b, int bx, int by, int bw, int bh,
             const char *label = 0)
    : Fl_Box (b, bx, by, bw, bh, 0)
    {
      printf ("ViewField init with boxtype. h=%d, w=%d\n",bh,bw);
      numlocales = 1;
    };

  //  Virtual methods to override
  void draw(void);

  int handle(int event);

  // Processing routines

  void setNumLocales(int n) { numlocales = n; draw(); }

  int  getNumLocales(void) { return numlocales; }

};

#endif

