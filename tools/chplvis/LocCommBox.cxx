/*
 * Copyright 2016-2020 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include "LocCommBox.h"
#include "chplvis.h"

void LocCom_cb_delete(Fl_Widget*w, void*) {
  Fl_Group *p = w->parent();
  if (p->parent() == Info) {
    // printf ("LocCom delete on Info\n");
    Info->delLocOrComm((LocCommBox *)p);
  } else {
    printf ("LocCom delete on elsewhere, shouldn't happen!\n");
    //p->remove(w->parent());
    //delete w->parent();
  }
}

LocCommBox::LocCommBox ( int X, int Y, int W, int H, char *l)
    : Fl_Group (X, Y, W, H, l)
    {
      // printf ("New LocCommBox (%d,%d) %d X %d\n", X, Y, W, H);
      what = I_None;
      box(FL_BORDER_FRAME);
      X_box = NULL;
    }

void LocCommBox::addXButton()
  {
    if (!X_box) {
      X_box = new Fl_Button(x()+w()-14, y()+2, 12, 12, "X");
      X_box->box(FL_NO_BOX);
      X_box->clear_visible_focus();
      X_box->callback((Fl_Callback*)LocCom_cb_delete);
      redraw();
    }
  }

void LocCommBox::draw()
  {
    char str[60];

    // printf ("LocCommBox::draw(: ");
    Fl_Group::draw();

    switch (what) {
      case I_Locale:
        // printf ("drawing a locale\n");
        snprintf (str, 60, "Locale %d, tasks %d", L_Num, L_Tasks);
        fl_draw(str, x()+2, y()+1, 200, 19, FL_ALIGN_LEFT, NULL, 0);
        if (L_Clock > 60) {
          snprintf (str, 60, " Clock: %ldm, %.3lfs", (long)L_Clock/60,
                    L_Clock - (double)((long)L_Clock/60)*60);
        } else {
          snprintf (str, 60, " Clock: %lfs", L_Clock);
        }
        fl_draw(str, x()+2, y()+21, 200, 19, FL_ALIGN_LEFT, NULL, 0);
        snprintf (str, 60, " CPU: %.4lfU,%.4lfS", L_User, L_Sys);
        fl_draw(str, x()+2, y()+41, 200, 19, FL_ALIGN_LEFT, NULL, 0);
        snprintf (str, 60, " TCPU: %.4lf, Conc: %d", L_Total, L_Conc);
        fl_draw(str, x()+2, y()+61, 200, 19, FL_ALIGN_LEFT, NULL, 0);
        break;

    case I_Comm:
        // printf ("drawing a comm\n");
        snprintf (str, 60, "Com %d->%d, Size %d", C_L1, C_L2, C_Bytes);
        fl_draw(str, x()+2, y()+1, 200, 19, FL_ALIGN_LEFT, NULL, 0);
        snprintf (str, 60, " Total: %d", C_Total);
        fl_draw(str, x()+2, y()+21, 200, 19, FL_ALIGN_LEFT, NULL, 0);
        snprintf (str, 60, " Gets: %d, Puts: %d", C_Gets, C_Puts);
        fl_draw(str, x()+2, y()+41, 200, 19, FL_ALIGN_LEFT, NULL, 0);
        snprintf (str, 60, " On Calls: %d", C_Forks);
        fl_draw(str, x()+2, y()+61, 200, 19, FL_ALIGN_LEFT, NULL, 0);
        break;

    default:
        // Do nothing ...
        // printf ("drawing nothing\n");
        break;
    }
  }

void LocCommBox::resize(int X, int Y, int W, int H)
  {
    if (X_box) {
      // Move the X also!
      X_box->resize(X+W-12, Y+1, 12, 12);
    }
    Fl_Widget::resize(X,Y,W,H);
  }
