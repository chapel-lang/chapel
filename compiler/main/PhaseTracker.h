/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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
#include "MemoryTracker.h"

#include <cstdio>
#include <vector>
#include <optional>
#include <string>
#include <string_view>

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
* which consists of three phases;                                             *
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

struct PhaseData {
  unsigned long                timeInUsecs;
  MemoryTracker::MemoryInBytes memory;

  PhaseData() : timeInUsecs(0), memory(0) {}
  PhaseData(unsigned long time, MemoryTracker::MemoryInBytes memory)
      : timeInUsecs(time), memory(memory) {}

  std::string serialize() const {
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "%lu %lld", timeInUsecs, memory);
    return std::string(buffer);
  }
  static PhaseData deserialize(std::string_view str) {
    PhaseData result;
    sscanf(str.data(), "%lu %lld", &result.timeInUsecs, &result.memory);
    return result;
  }

  PhaseData operator-(const PhaseData& other) const {
    auto ret = *this;
    return ret -= other;
  }
  PhaseData& operator-=(const PhaseData& other) {
    this->timeInUsecs -= other.timeInUsecs;
    this->memory -= other.memory;
    return *this;
  }
  PhaseData operator+(const PhaseData& other) const {
    auto ret = *this;
    return ret += other;
  }
  PhaseData& operator+=(const PhaseData& other) {
    this->timeInUsecs += other.timeInUsecs;
    this->memory += other.memory;
    return *this;
  }
};

class PhaseTracker {
public:
  enum SubPhase { kPrimary, kVerify, kCleanAst };

  PhaseTracker();
  ~PhaseTracker();

  void StartPhase(const char* phaseName);
  void StartPhase(const char* passName, SubPhase subPhase);

  void Stop();
  void Resume();

  void ReportPass() const;
  // Report out total times by pass group, with differing behavior based on the
  // provided argument:
  // - nullptr: Ignore it and report as normal.
  // - empty list: Instead insert times into it and skip outputting.
  // - non-empty list: Take the values as already-recorded times and report
  // them out. They are assumed to represent pass group times in order, with
  // missing values meaning that pass group did not occur (early exit).
  void
  ReportPassGroupTotals(std::vector<PhaseData>* groupTimes = nullptr) const;
  // Report out total overall time for the compiler. If provided, add in
  // the overhead time.
  void ReportOverallTotal(
    std::optional<PhaseData> overheadTime = std::nullopt) const;

  void ReportRollup() const;

  static bool   shouldReportPasses();
  static FILE*& passesOutputFile();

private:
  void PassesCollect(std::vector<Pass>& passes) const;

  void StartPhase(const char* phaseName, int passId, SubPhase subPhase);

  Timer               mTimer;
  MemoryTracker       mMemoryTracker;
  int                 mPhaseId;
  std::vector<Phase*> mPhases;
};

// Used to collect the times as the program runs
class Phase {
public:
  Phase(const char*            name,
        int                    passId,
        PhaseTracker::SubPhase subPhase,
        PhaseData              start);
  ~Phase();

  bool IsStartOfPass() const;

  void        ReportPass(PhaseData now) const;
  static void ReportPassGroup(const char* suffix, PhaseData total);

  static void ReportTime(const char* name, double secs);
  static void ReportMemory(const char* name, MemoryTracker::MemoryInMB MB);
  static void ReportData(const char* name, PhaseData data);
  static void ReportText(const char* text);

  char*                  mName; // Only set for kPrimary
  int                    mPassId;
  PhaseTracker::SubPhase mSubPhase;
  PhaseData              mStart; // Elapsed from main()

private:
  Phase();
};

// Group the phases in to passes and report on passes
class Pass {
public:
  Pass();
  ~Pass();

  static void Header(FILE* fp);
  static void Footer(FILE*     fp,
                     PhaseData main,
                     PhaseData check,
                     PhaseData clean,
                     PhaseData total);

  bool                         CompareByTime(Pass const& ref) const;
  bool                         CompareByMemory(Pass const& ref) const;
  void                         Reset();
  unsigned long                TotalTime() const;
  MemoryTracker::MemoryInBytes TotalMemory() const;

  void Print(FILE* fp, PhaseData accum, PhaseData total) const;

  char*     mName;
  int       mPassId;
  int       mIndex;
  PhaseData mPrimary;
  PhaseData mVerify;
  PhaseData mCleanAst;
};

#endif
