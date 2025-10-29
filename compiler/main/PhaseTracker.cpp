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

#include "PhaseTracker.h"

#include "baseAST.h"
#include "driver.h"

#include <cstdlib>
#include <cstring>
#include <algorithm>

struct SortByTime {
  bool operator()(Pass const& a, Pass const& b) const {
    return a.CompareByTime(b);
  }
};
struct SortByMemory {
  bool operator()(Pass const& a, Pass const& b) const {
    return a.CompareByMemory(b);
  }
};

static void PassesReport(const std::vector<Pass>& passes, PhaseData total);

static void
PassesReport(FILE* fp, const std::vector<Pass>& passes, PhaseData total);

/************************************* | **************************************
 *                                                                             *
 * Implementation of PhaseTracker                                              *
 *                                                                             *
 ************************************** | *************************************/

PhaseTracker::PhaseTracker() {
  mPhaseId = 0;

  mTimer.start();
  mMemoryTracker.start();
  StartPhase("startup");
}

PhaseTracker::~PhaseTracker() {
  for (size_t i = 0; i < mPhases.size(); i++) delete mPhases[i];
}

void PhaseTracker::StartPhase(const char* name) {
  StartPhase(name, 0, kPrimary);
}

void PhaseTracker::StartPhase(const char*            name,
                              PhaseTracker::SubPhase subPhase) {
  if (subPhase == kPrimary) mPhaseId = mPhaseId + 1;

  StartPhase(name, mPhaseId, subPhase);
}

void PhaseTracker::StartPhase(const char* name, int passId, SubPhase subPhase) {
  Phase* phase = new Phase(
    name,
    passId,
    subPhase,
    PhaseData(mTimer.elapsedUsecs(), mMemoryTracker.usedBytes())
  );

  mPhases.push_back(phase);
}

void PhaseTracker::Stop() {
  mTimer.stop();
  mMemoryTracker.stop();
}

void PhaseTracker::Resume() {
  mTimer.start();
  mMemoryTracker.start();
}

void PhaseTracker::ReportPass() const {
  int index = mPhases.size() - 1;

  while (index >= 0 && !mPhases[index]->IsStartOfPass())
    index = index - 1;

  mPhases[index]->ReportPass(
    PhaseData(mTimer.elapsedUsecs(),
              mMemoryTracker.usedBytes()));
}

static auto passGroups = std::array{
  std::make_pair("(front end)", "parallel"),
  std::make_pair("(middle end)", "denormalize"),
  std::make_pair("(back end)", "driverCleanup"),
};

void PhaseTracker::ReportPassGroupTotals(
  std::vector<PhaseData>* groupTimes) const {
  // Capture total up to this point
  const auto total =
    PhaseData(mTimer.elapsedUsecs(), mMemoryTracker.usedBytes());

  // Were we provided previously-saved values to report out?
  const bool useSaved = groupTimes && !groupTimes->empty();
  // Should we save values to the provided list?
  const bool saveToList = groupTimes && groupTimes->empty();

  PhaseData lastStart;
  PhaseData passData;
  for (size_t i = 0; i < passGroups.size(); i++) {
    // can't capture structured bindings in the lambda without C++20
    // auto [groupName, groupLastPhase] = passGroups[i];
    auto groupName      = passGroups[i].first;
    auto groupLastPhase = passGroups[i].second;
    if (!useSaved) {
      // No times provided, so calculate them.

      auto currentPass = std::find_if(
        mPhases.begin(), mPhases.end(), [&groupLastPhase](auto pass) {
          if (pass->mName == nullptr) return false;
          return strcmp(pass->mName, groupLastPhase) == 0;
        });

      // No such pass, we might've exited early, or begun late from driver mode.
      if (currentPass == mPhases.end()) {
        if (fDriverMakeBinaryPhase) {
          // We began after the current pass group, check the next.
          continue;
        } else {
          // Compilation exited early, no further reporting.
          break;
        }
      }

      auto nextPass = currentPass + 1;

      if (nextPass != mPhases.end()) {
        passData = (*nextPass)->mStart - lastStart;
        lastStart = (*nextPass)->mStart;
      } else {
        passData = total - lastStart;
      }
    } else {
      // Just used saved time value

      // If there is no value for this pass group, it did not occur due to early
      // exit, so skip it. We can exit the loop since there can't be any groups
      // following either.
      if (groupTimes->size() <= i) break;

      passData = (*groupTimes)[i];
    }

    // Save or report out result
    if (saveToList) {
      groupTimes->emplace_back(passData);
    } else {
      // No out-parameter to save into, report time normally
      // (whether it was calculated or retrieved from list).

      Phase::ReportPassGroup(groupName, passData);
    }
  }
}

void PhaseTracker::ReportOverallTotal(
  std::optional<PhaseData> overheadTime) const {
  // Measure so far
  auto total = PhaseData(mTimer.elapsedUsecs(), mMemoryTracker.usedBytes());
  // Add overhead if used
  if (overheadTime.has_value()) {
    total += overheadTime.value();
  }

  Phase::ReportTime("total time", total.timeInUsecs / 1e6);
  if (printPassesMemory) {
    Phase::ReportText("\n");
    Phase::ReportMemory("total memory", MemoryTracker::toMB(total.memory));
  }
  Phase::ReportText("\n\n\n\n");
}

void PhaseTracker::ReportRollup() const {
  std::vector<Pass> passes;
  auto total = PhaseData(mTimer.elapsedUsecs(), mMemoryTracker.usedBytes());

  PassesCollect(passes);
  PassesReport(passes, total);

  // Repeat the information but sorted by time, for monolithic mode or driver
  // compilation phase.
  // Skipped for driver overhead time or makeBinary phase as they contain
  // very little information and the sort isn't that informative.
  if (fDriverDoMonolithic || fDriverCompilationPhase) {
    Phase::ReportText("\n\n\n");

    std::sort(passes.begin(), passes.end(), SortByTime());
    PassesReport(passes, total);

    if (printPassesMemory) {
      Phase::ReportText("\n\n\n");

      std::sort(passes.begin(), passes.end(), SortByMemory());
      PassesReport(passes, total);
    }
    Phase::ReportText("\n");
  }
}

void PhaseTracker::PassesCollect(std::vector<Pass>& passes) const {
  auto total = PhaseData(mTimer.elapsedUsecs(), mMemoryTracker.usedBytes());

  if (mPhases.size() > 0) {
    Pass pass;

    for (size_t i = 0; i < mPhases.size(); i++) {
      auto start = mPhases[i]->mStart;
      PhaseData elapsed;

      // Check if it's time to push an completed pass
      if (i > 0 && mPhases[i]->mSubPhase == PhaseTracker::kPrimary) {
        passes.push_back(pass);
        pass.Reset();
      }

      if (i < mPhases.size() - 1) {
        elapsed = mPhases[i + 1]->mStart - start;
      } else {
        elapsed = total - start;
      }

      switch (mPhases[i]->mSubPhase) {
        case PhaseTracker::kPrimary:
          pass.mName    = mPhases[i]->mName;
          pass.mPassId  = mPhases[i]->mPassId;
          pass.mIndex   = (int)passes.size();
          pass.mPrimary = elapsed;
          break;
        case PhaseTracker::kVerify:   pass.mVerify = elapsed; break;
        case PhaseTracker::kCleanAst: pass.mCleanAst = elapsed; break;
      }
    }

    passes.push_back(pass);
  }
}

static void PassesReport(const std::vector<Pass>& passes, PhaseData total) {
  if (PhaseTracker::shouldReportPasses())
    PassesReport(PhaseTracker::passesOutputFile(), passes, total);
}

static void
PassesReport(FILE* fp, const std::vector<Pass>& passes, PhaseData total) {
  PhaseData accum;
  PhaseData main;
  PhaseData check;
  PhaseData clean;

  Pass::Header(fp);

  for (auto p: passes) {
    accum += PhaseData(p.TotalTime(), p.TotalMemory());
    p.Print(fp, accum, total);
  }

  for (auto p: passes) {
    main += p.mPrimary;
    check += p.mVerify;
    clean += p.mCleanAst;
  }

  Pass::Footer(fp, main, check, clean, total);
}

/************************************* | **************************************
*                                                                             *
* Implementation of Phase                                                     *
*                                                                             *
************************************** | *************************************/

Phase::Phase(const char*            name,
             int                    passId,
             PhaseTracker::SubPhase subPhase,
             PhaseData              start) {
  mName     = (subPhase == PhaseTracker::kPrimary) ? strdup(name) : 0;
  mPassId   = passId;
  mSubPhase = subPhase;
  mStart    = start;
}

Phase::~Phase() {
  if (mName) free(mName);
}

bool Phase::IsStartOfPass() const {
  return (mSubPhase == PhaseTracker::kPrimary) ? true : false;
}

void Phase::ReportPass(PhaseData now) const {
  // clock-skew can cause now < startTime, just report 0 in that case
  unsigned long phaseTime = 0;
  if (now.timeInUsecs > mStart.timeInUsecs)
    phaseTime = now.timeInUsecs - mStart.timeInUsecs;
  auto phaseMemory = now.memory - mStart.memory;

  ReportData(mName, PhaseData(phaseTime, phaseMemory));

  if (developer) {
    char text[32];

    snprintf(text, sizeof(text), "  [%9d]", lastNodeIDUsed());

    ReportText(text);
  }

  ReportText("\n");
}

void Phase::ReportPassGroup(const char* suffix, PhaseData data) {
  ReportTime((std::string("total time ") + suffix).c_str(),
             data.timeInUsecs / 1e6);
  if (printPassesMemory) {
    Phase::ReportText("\n");
    ReportMemory((std::string("total memory ") + suffix).c_str(),
                 MemoryTracker::toMB(data.memory));
  }
  ReportText("\n");
}

void Phase::ReportTime(const char* name, double secs) {
  if (PhaseTracker::shouldReportPasses())
    fprintf(PhaseTracker::passesOutputFile(), "%32s :%8.3f seconds", name, secs);
}

void Phase::ReportMemory(const char* name, MemoryTracker::MemoryInMB MB) {
  if (PhaseTracker::shouldReportPasses() && printPassesMemory)
    fprintf(PhaseTracker::passesOutputFile(), "%32s :%8.3f MB", name, MB);
}

void Phase::ReportData(const char* name, PhaseData data) {
  if (PhaseTracker::shouldReportPasses()) {
    fprintf(PhaseTracker::passesOutputFile(), "%32s :%8.3f seconds", name, data.timeInUsecs / 1e6);
    if (printPassesMemory)
      fprintf(PhaseTracker::passesOutputFile(), "  %8.3f MB", MemoryTracker::toMB(data.memory));
  }
}

void Phase::ReportText(const char* text) {
  if (PhaseTracker::shouldReportPasses())
    fputs(text, PhaseTracker::passesOutputFile());
}

/************************************* | **************************************
*                                                                             *
* Implementation of Pass                                                      *
*                                                                             *
************************************** | *************************************/

Pass::Pass() { Reset(); }

Pass::~Pass() {}

void Pass::Reset() {
  mName     = 0;
  mPassId   = 0;
  mIndex    = 0;
  mPrimary  = PhaseData();
  mVerify   = PhaseData();
  mCleanAst = PhaseData();
}

unsigned long Pass::TotalTime() const {
  return mPrimary.timeInUsecs + mVerify.timeInUsecs + mCleanAst.timeInUsecs;
}

MemoryTracker::MemoryInBytes Pass::TotalMemory() const {
  return mPrimary.memory + mVerify.memory + mCleanAst.memory;
}

bool Pass::CompareByTime(Pass const& ref) const {
  if (TotalTime() > ref.TotalTime())
    return true;
  else if (TotalTime() < ref.TotalTime())
    return false;
  else
    return mIndex < ref.mIndex;
}
bool Pass::CompareByMemory(Pass const& ref) const {
  if (TotalMemory() > ref.TotalMemory())
    return true;
  else if (TotalMemory() < ref.TotalMemory())
    return false;
  else
    return mIndex < ref.mIndex;
}

void Pass::Header(FILE* fp) {
  // Print column headers
  fprintf(fp, "Pass               Name               ");

  fprintf(fp, "    Main ");
  fprintf(fp, "   Check ");
  fprintf(fp, "   Clean ");

  fprintf(fp, "    Time    %%  ");
  fprintf(fp, "   Accum    %%  ");

  if (printPassesMemory) {
    fprintf(fp, "  ");

    fprintf(fp, "    Main (MB) ");
    fprintf(fp, "   Check (MB) ");
    fprintf(fp, "   Clean (MB) ");

    fprintf(fp, "   Memory   %%  ");
    fprintf(fp, "   Accum    %%  ");
  }

  fprintf(fp, "\n");

  // Print column underlines
  fprintf(fp, "---- ---------------------------------");

  fprintf(fp, "  -------");
  fprintf(fp, "  -------");
  fprintf(fp, "  -------");

  fprintf(fp, "  ------- -----");
  fprintf(fp, "  ------- -----");

  if (printPassesMemory) {
    fprintf(fp, "  ");

    fprintf(fp, "  ------------");
    fprintf(fp, "  ------------");
    fprintf(fp, "  ------------");

    fprintf(fp, "  ------- -----");
    fprintf(fp, "  ------- -----");
  }

  fprintf(fp, "\n");
}

void Pass::Print(FILE* fp, PhaseData accum, PhaseData total) const {

  unsigned long passTime  = TotalTime();
  double        passFrac  = (100.0 * passTime) / total.timeInUsecs;
  double        accumFrac = (100.0 * accum.timeInUsecs) / total.timeInUsecs;

  double primary = mPrimary.timeInUsecs / 1e6;
  double verify  = mVerify.timeInUsecs / 1e6;
  double clean   = mCleanAst.timeInUsecs / 1e6;

  auto passMemory      = TotalMemory();
  auto passMemoryFrac  = (100.0 * passMemory) / total.memory;
  auto accumMemoryFrac = (100.0 * accum.memory) / total.memory;

  auto primaryMem = MemoryTracker::toMB(mPrimary.memory);
  auto verifyMem  = MemoryTracker::toMB(mVerify.memory);
  auto cleanMem   = MemoryTracker::toMB(mCleanAst.memory);

  if (mPassId > 0)
    fprintf(fp, "%4d ", mPassId);
  else
    fprintf(fp, "     ");

  fprintf(fp, "%-33s", mName);
  fprintf(fp, "  %7.3f  %7.3f  %7.3f", primary, verify, clean);
  fprintf(fp, "  %7.3f %5.1f", passTime  / 1e6, passFrac );
  fprintf(fp, "  %7.3f %5.1f", accum.timeInUsecs / 1e6, accumFrac);
  if (printPassesMemory) {
    fprintf(fp, "  ");
    fprintf(fp, "  %12.3f  %12.3f  %12.3f", primaryMem, verifyMem, cleanMem);
    fprintf(fp, "  %7.3f %5.1f", MemoryTracker::toMB(passMemory), passMemoryFrac);
    fprintf(fp, "  %7.3f %5.1f", MemoryTracker::toMB(accum.memory), accumMemoryFrac);
  }
  fprintf(fp, "\n");
}

void Pass::Footer(FILE*     fp,
                  PhaseData main,
                  PhaseData check,
                  PhaseData clean,
                  PhaseData total) {

  fprintf(fp,
          "\n     %-33s  %7.3f  %7.3f  %7.3f  %7.3f ",
          "total",
          main.timeInUsecs / 1e6,
          check.timeInUsecs / 1e6,
          clean.timeInUsecs / 1e6,
          total.timeInUsecs / 1e6);
  if (printPassesMemory) {
    fprintf(fp, "%28s", "");
    fprintf(fp,
            " %7.3f  %7.3f  %7.3f  %7.3f ",
            MemoryTracker::toMB(main.memory),
            MemoryTracker::toMB(check.memory),
            MemoryTracker::toMB(clean.memory),
            MemoryTracker::toMB(total.memory));
  }
  fprintf(fp, "\n");
}

bool PhaseTracker::shouldReportPasses() {
  return printPasses || printPassesFile != nullptr || printPassesMemory;
}
FILE*& PhaseTracker::passesOutputFile() {
  return printPassesFile != nullptr ? printPassesFile : stderr;
}
