/*
 * Copyright 2016 Cray Inc.
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

#include <FL/Fl_Double_Window.H>

#ifndef SETTINGS_H
#define SETTINGS_H

class SettingsData {

 public:
  double minR;
  double minG;
  double minB;
  double maxR;
  double maxG;
  double maxB;
  bool   save_WH;
  int    mainW;
  int    mainH;

  SettingsData() { setDefaults(); }

  void setDefaults(void)
    {
    minR = 0;
    minG = 180;
    minB = 255;
    maxR = 255;
    maxG = 0;
    maxB = 0;
    save_WH = false;
    mainW = 500;
    mainH = 590;
  }

  void saveToFile (void);
  void readFromFile (void);
};

class Settings : public Fl_Double_Window {
  public: 
    Settings (int x, int y, int W, int H, const char *l=0);

    void setCoolColor(void);
    void setHotColor(void);
    virtual void draw(void);
    Fl_Color heatColor ( double val, double max );
    void copyIn(void);
    void copyOut(void);
    void reset(void);

    bool getSaveWH() { return save_WH; }
    void toggleSaveWH() { save_WH = ! save_WH; }

 private:
    double minR;
    double minG;
    double minB;
    double maxR;
    double maxG;
    double maxB;
    bool save_WH;
};

#endif
