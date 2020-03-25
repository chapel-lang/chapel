/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2016-2019 Cray Inc.
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

#ifndef LOCCOMMBOX_H
#define LOCCOMMBOX_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include "DataModel.h"

class LocCommBox : public Fl_Group {

  // Locale information
  int L_Num;
  int L_Tasks;
  int L_Conc;
  double L_Sys;
  double L_User;
  double L_Total;
  double L_Clock;

  // Comm information
  int C_L1;
  int C_L2;
  int C_Total;
  int C_Bytes;
  int C_Gets;
  int C_Puts;
  int C_Forks;

  Fl_Button *X_box;

 public:

  enum LCBoxKind { I_None, I_Locale, I_Comm } what;
  
  LocCommBox (int X, int Y, int W, int H, char *l = 0);

  enum LCBoxKind boxKind (void) { return what; }

  void setLocale (int num, localeData* data)
    {
      // printf ("setLocale ...\n");
      L_Num = num;
      L_Tasks = data->numTasks;
      L_Conc = data->maxConc;
      L_Sys = data->sysCpu;
      L_User = data->userCpu;
      L_Total = data->Cpu;
      L_Clock = data->clockTime;
      what = I_Locale;
    }

  void setComm (int L1, int L2, commData *comm)
    {
      C_L1 = L1;
      C_L2 = L2;
      C_Total = comm->numComms;
      C_Bytes = comm->commSize;
      C_Gets = comm->numGets;
      C_Puts = comm->numPuts;
      C_Forks = comm->numForks;
      what = I_Comm;
    }

  void addXButton();

  void draw();

  void resize(int X, int Y, int W, int H);

  int getLoc(void) { return L_Num; }

  void getCommLocs(int &loc1, int &loc2) { loc1 = C_L1; loc2 = C_L2; }

};

#endif
