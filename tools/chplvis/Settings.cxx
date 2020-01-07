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


#include "chplvis.h"
#include <FL/Fl_Color_Chooser.H>
#include <FL/fl_ask.H>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// Color Reference location and height
static const int cr_x = 180;
static const int cr_y = 35;  
static const int cr_h = 20;


Settings::Settings (int x, int y, int W, int H, const char *l)
  :  Fl_Group (x, y, W, H, l) {
  VisSettings.readFromFile();
  copyIn();
}

void Settings::copyIn(void)
{
  minR = VisSettings.minR/255;
  minG = VisSettings.minG/255;
  minB = VisSettings.minB/255;
  maxR = VisSettings.maxR/255;
  maxG = VisSettings.maxG/255;
  maxB = VisSettings.maxB/255;
  save_WH = VisSettings.save_WH;
}

void Settings::copyOut(void)
{
  VisSettings.minR = minR*255;
  VisSettings.minG = minG*255;
  VisSettings.minB = minB*255;
  VisSettings.maxR = maxR*255;
  VisSettings.maxG = maxG*255;
  VisSettings.maxB = maxB*255;
  VisSettings.save_WH = save_WH;
}

void Settings::reset(void)
{
    minR = 0;
    minG = 180.0/255;
    minB = 1;
    maxR = 1;
    maxG = 0;
    maxB = 0;
    redraw();
}

Fl_Color Settings::heatColor ( double val, double max ) {
  if (val <= 0) return FL_WHITE;
  if (val > max) return fl_rgb_color(maxR, maxG, maxB);
  // if (val < 0) return fl_rgb_color(minR, minG, minB);
  return fl_rgb_color( 255 * (minR + (maxR-minR)*(val/max)),
                       255 * (minG + (maxG-minG)*(val/max)),
                       255 * (minB + (maxB-minB)*(val/max)));
}


void Settings::setCoolColor(void)
{
  //printf ("%f, %f, %f\n", minR, minG, minB);
  fl_color_chooser("Choose cool color", minR, minG, minB, 2);
  //printf ("%f, %f, %f\n", minR, minG, minB);
  redraw();
}

void Settings::setHotColor(void)
{
  fl_color_chooser("Choose hot color", maxR, maxG, maxB, 2);
  redraw();
}

void Settings::draw(void)
{
  Fl_Group::draw();

  // Color Reference
  int ix;
  for (ix = 0; ix <= 30; ix++) {
    fl_color(heatColor(ix+1,31));
    fl_line_style(FL_SOLID,3,NULL);
    fl_line(x()+cr_x+3*ix, y()+cr_y, x()+cr_x+3*ix, y()+cr_y+cr_h);
    if ((ix%3) == 0) {
      fl_color(FL_BLACK);
      fl_line_style(FL_SOLID,1,NULL);
      fl_line(x()+cr_x+3*ix, y()+cr_y+cr_h+2, x()+cr_x+3*ix, y()+cr_y+cr_h+6);
    }
  }

}

// Saving to and reading settings data from a file
//
//  Initially:  ~/.cache/chplvis
//  Format:  Single line, text data
//           minR minG minB maxR maxG maxB save_WH
//

void SettingsData::saveToFile (bool saveAll)
{
  char *home = getenv("HOME");
  if (!home) {
    fl_alert ("HOME environment variable not set, can't save settings.\n");
    return;
  }
  int namelen = strlen(home)+20;
  char name[namelen];
  // First make sure .cache is there
  snprintf (name, namelen, "%s/.cache", home);
  if (access(name, W_OK) < 0) {
    // Create .cache
    if (mkdir(name,0777) < 0) {
      fl_alert ("Could not make %s: %s.\n", name, strerror(errno));
      return;
    }
  }
  snprintf (name, namelen, "%s/.cache/chplvis", home);
  (void) unlink(name);
  mainW = MainWindow->w();
  mainH = MainWindow->h();

  FILE *setF = fopen(name, "w");
  if (setF) {
    if (saveAll) {
      file_minG = minG;
      file_minB = minB;
      file_minR = minR;
      file_maxG = maxG;
      file_maxB = maxB;
      file_maxR = maxR;
    } else if (!didReadFile) {
      // Set the defaults to write to the file.
      file_minR = 0;
      file_minG = 180;
      file_minB = 255;
      file_maxR = 255;
      file_maxG = 0;
      file_maxB = 0;
    }

    // Write the data
    fprintf (setF, "%lf %lf %lf %lf %lf %lf %d %d %d\n",
             file_minR, file_minG, file_minB, file_maxR, file_maxG, file_maxB,
             save_WH, mainW, mainH);
    fclose(setF);
  } else {
    fl_alert ("Could not make %s: %s.\n", name, strerror(errno));
  }
}

void SettingsData::readFromFile (void)
{
  char *home = getenv("HOME");
  if (!home) {
    fl_alert ("HOME environment variable not set, can't read saved settings.\n");
    return;
  }
  int namelen = strlen(home)+20;
  char name[namelen];
  snprintf (name, namelen, "%s/.cache/chplvis", home);
  didReadFile = false;

  FILE *setF;
  if (access(name, R_OK) == 0) {
    setF = fopen(name, "r");
    if (setF) {
      int sWH;
      int nr = fscanf (setF, "%lf %lf %lf %lf %lf %lf %d %d %d", &file_minR,
                       &file_minG, &file_minB, &file_maxR, &file_maxG,
                       &file_maxB, &sWH, &mainW, &mainH);
      if (nr != 9) {
        fl_alert ("Settings corrupt, reverting to default settings.");
        setDefaults();
      } else {
        save_WH = sWH == 1;
        minG = file_minG;
        minB = file_minB;
        minR = file_minR;
        maxG = file_maxG;
        maxB = file_maxB;
        maxR = file_maxR;
        if (!save_WH) {
          // Ignore file's W and H if not "saved"
          mainW = 560;
          mainH = 600;
        }
        didReadFile = true;
      }
      fclose(setF);
    }
  } else
    setDefaults();
}
