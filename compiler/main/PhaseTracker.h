/*
 * Copyright 2004-2020 Cray Inc.
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

#ifndef _PHASE_TRACKER_H_
#define _PHASE_TRACKER_H_

#include "timer.h"

#include <cstdio>
#include <vector>

/************************************* | **************************************
*                                                                             *
* This class is designed to provide an accurate and reasonably detailed view  *
* of the way time is being spent within the compiler.  The intent is to       *
* account for all of the time between the entry point to main() and the final *
* call to clean_exit(0) in driver.cpp.                                        *
*                                                                             *
* a) Create a Timer object that will record the wall time as early in the     *
*    compiler as possible                                                     *
*                                                                             *
* b) Use the Timer to determine the elapsed time between the start of the     *
*    program and the start of each phase                                      *
*                                                                             *
* c) Compute the elapsed time for each phase                                  *
*                                                                             *
* d) Stop the timer and generate one or more reports on the time spent in     *
*    each phase.
*                                                                             *
* Every phase has a descriptive name that will be used by the reports.        *
*                                                                             *
* The main loop for the compiler, in runpasses.cpp, has a notion of a "pass"  *
* consists of three phases;                                                   *
*                                                                             *
*    1) The primary computation                                               *
*    2) An optional verify / check phase                                      *
*    3) An AST cleanup phase                                                  *
*                                                                             *
* The tracker collects the sub-phases and reports the overall time in terms   *
* of these passes.  Phases that occur before and after the Passes ignore      *
* the check and clean phases.                                                 *
*                                                                             *
************************************** | *************************************/

class Phase;
class Pass;

class PhaseTracker
{
public:
  enum SubPhase
  {
    kPrimary,
    kVerify,
    kCleanAst
  };

                       PhaseTracker();
                      ~PhaseTracker();

  void                 StartPhase(const char* phaseName);
  void                 StartPhase(const char* passName, SubPhase subPhase);

  void                 Stop();

  void                 ReportPass  ()                                const;
  void                 ReportTotal ()                                const;

  void                 ReportRollup()                                const;

private:
  void                 PassesCollect(std::vector<Pass>& passes) const;
  
  void                 StartPhase(const char* phaseName,
                                  int         passId,
                                  SubPhase    subPhase);

  Timer                mTimer;
  int                  mPhaseId;
  std::vector<Phase*>  mPhases;
};

#endif
