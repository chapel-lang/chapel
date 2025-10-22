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

struct SortByTime
{
  bool operator() (Pass const& a, Pass const& b) const
  {
    return a.CompareByTime(b);
  }
};

static void PassesSortByTime(std::vector<Pass>& passes);

static void PassesReport(const std::vector<Pass>& passes,
                         unsigned long            totalTime);

static void PassesReport(FILE*                    fp,
                         const std::vector<Pass>& passes,
                         unsigned long            totalTime);

/************************************* | **************************************
*                                                                             *
* Implementation of PhaseTracker                                              *
*                                                                             *
************************************** | *************************************/

PhaseTracker::PhaseTracker()
{
  mPhaseId = 0;

  mTimer.start();
  mMemoryTracker.start();
  StartPhase("startup");
}

PhaseTracker::~PhaseTracker()
{
  for (size_t i = 0; i < mPhases.size(); i++)
    delete mPhases[i];
}

void PhaseTracker::StartPhase(const char* name)
{
  StartPhase(name, 0, kPrimary);
}

void PhaseTracker::StartPhase(const char*            name,
                              PhaseTracker::SubPhase subPhase)
{
  if (subPhase == kPrimary)
    mPhaseId = mPhaseId + 1;

  StartPhase(name, mPhaseId, subPhase);
}


void PhaseTracker::StartPhase(const char* name,
                              int         passId,
                              SubPhase    subPhase)
{
  Phase* phase = new Phase(name, passId, subPhase,
                           mTimer.elapsedUsecs(),
                           mMemoryTracker.elapsedBytes());

  mPhases.push_back(phase);
}

void PhaseTracker::Stop()
{
  mTimer.stop();
  mMemoryTracker.stop();
}

void PhaseTracker::Resume()
{
  mTimer.start();
  mMemoryTracker.start();
}

void PhaseTracker::ReportPass() const
{
  int index = mPhases.size() - 1;

  while (index >= 0 && mPhases[index]->IsStartOfPass() == false)
    index = index - 1;

  mPhases[index]->ReportPass(mTimer.elapsedUsecs(),
                             mMemoryTracker.elapsedBytes());
}

static const char* passGroups[][2] = {
  {"total time (front end)", "parallel"},
  {"total time (middle end)", "denormalize"},
  {"total time (back end)", "driverCleanup"},
};

void PhaseTracker::ReportPassGroupTotals(
  std::vector<std::pair<unsigned long, MemoryTracker::MemoryInBytes>>* groupTimes
) const {
  // Capture total time up to this point
  const unsigned long totalTime = mTimer.elapsedUsecs();
  const auto totalMemory = mMemoryTracker.elapsedBytes();

  // Were we provided previously-saved values to report out?
  const bool useSaved = groupTimes && !groupTimes->empty();
  // Should we save values to the provided list?
  const bool saveToList = groupTimes && groupTimes->empty();

  static const size_t numMetapasses =
      sizeof(passGroups) / sizeof(passGroups[0]);
  unsigned long lastStart = 0;
  unsigned long passTime;
  MemoryTracker::MemoryInBytes lastMemory = 0;
  MemoryTracker::MemoryInBytes passMemory;
  for (size_t i = 0; i < numMetapasses; i++) {
    if (!useSaved) {
      // No times provided, so calculate them.

      const char* groupLastPhase = passGroups[i][1];
      auto currentPass = mPhases.begin();
      currentPass = std::find_if(currentPass, mPhases.end(), [&](auto pass) {
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
        passTime = (*nextPass)->mStartTime - lastStart;
        lastStart = (*nextPass)->mStartTime;

        passMemory = (*nextPass)->mStartMemory - lastMemory;
        lastMemory = (*nextPass)->mStartMemory;
      } else {
        passTime = totalTime - lastStart;
        lastStart = totalTime;

        passMemory = totalMemory - lastMemory;
        lastMemory = totalMemory;
      }
    } else {
      // Just used saved time value

      // If there is no value for this pass group, it did not occur due to early
      // exit, so skip it. We can exit the loop since there can't be any groups
      // following either.
      if (groupTimes->size() <= i) break;

      passTime = (*groupTimes)[i].first;
      passMemory = (*groupTimes)[i].second;
    }

    // Save or report out result
    if (saveToList) {
      groupTimes->emplace_back(std::make_pair(passTime, passMemory));
    } else {
      // No out-parameter to save into, report time normally
      // (whether it was calculated or retrieved from list).

      const char* groupName = passGroups[i][0];
      Phase::ReportPassGroup(groupName, passTime, passMemory);
    }
  }
}

void PhaseTracker::ReportOverallTotal(long long overheadTime) const {
  // Measure time so far
  unsigned long totalTime = mTimer.elapsedUsecs();
  // Add overhead if used
  if (overheadTime >= 0) {
    totalTime += overheadTime;
  }

  Phase::ReportTime("total time", totalTime / 1e6);
  Phase::ReportText("\n\n\n\n");
}

void PhaseTracker::ReportRollup() const
{
  std::vector<Pass> passes;
  unsigned long     totalTime = mTimer.elapsedUsecs();
  auto              totalMemory = mMemoryTracker.usedBytes();

  PassesCollect(passes);
  PassesReport(passes, totalTime, totalMemory);

  // Repeat the information but sorted by time, for monolithic mode or driver
  // compilation phase.
  // Skipped for driver overhead time or makeBinary phase as they contain
  // very little information and the sort isn't that informative.
  if (fDriverDoMonolithic || fDriverCompilationPhase) {
    Phase::ReportText("\n\n\n");

    PassesSortByTime(passes);
    PassesReport(passes, totalTime, totalMemory);
  }
}

void PhaseTracker::PassesCollect(std::vector<Pass>& passes) const
{
  unsigned long totalTime = mTimer.elapsedUsecs();
  auto totalMemory = mMemoryTracker.usedBytes();

  if (mPhases.size() > 0)
  {
    Pass pass;

    for (size_t i = 0; i < mPhases.size(); i++)
    {
      unsigned long start   = mPhases[i]->mStartTime;
      unsigned long elapsed = 0;
      auto startMem = mPhases[i]->startMemory;
      auto elapsedMem = MemoryTracker::MemoryInBytes(0);

      // Check if it's time to push an completed pass
      if (i > 0 && mPhases[i]->mSubPhase == PhaseTracker::kPrimary)
      {
        passes.push_back(pass);
        pass.Reset();
      }

      if (i < mPhases.size() - 1) {
        elapsed = mPhases[i + 1]->mStartTime - start;
        elapsedMem = mPhases[i + 1]->startMemory - startMem;
      } else {
        elapsed = totalTime - start;
        elapsedMem = totalMemory - startMem;
      }

      switch (mPhases[i]->mSubPhase)
      {
        case PhaseTracker::kPrimary:
          pass.mName     = mPhases[i]->mName;
          pass.mPassId   = mPhases[i]->mPassId;
          pass.mIndex    = (int) passes.size();
          pass.mPrimary  = std::make_pair(elapsed, elapsedMem);
          break;

        case PhaseTracker::kVerify:
          pass.mVerify   = std::make_pair(elapsed, elapsedMem);
          break;

        case PhaseTracker::kCleanAst:
          pass.mCleanAst = std::make_pair(elapsed, elapsedMem);
          break;
      }
    }

    passes.push_back(pass);
  }
}

static void PassesSortByTime(std::vector<Pass>& passes)
{
  std::sort(passes.begin(), passes.end(), SortByTime());
}

static void PassesReport(const std::vector<Pass>&     passes,
                         unsigned long                totalTime,
                         MemoryTracker::MemoryInBytes totalMemory)
{
  if (printPasses     == true)
    PassesReport(stderr, passes, totalTime, totalMemory);

  if (printPassesFile != 0)
    PassesReport(printPassesFile, passes, totalTime, totalMemory);
}

static void PassesReport(FILE*                        fp,
                         const std::vector<Pass>&     passes,
                         unsigned long                totalTime,
                         MemoryTracker::MemoryInBytes totalMemory)
{
  unsigned long accumTime = 0;
  auto accumMemory = MemoryTracker::MemoryInBytes(0);

  unsigned long mainTime  = 0;
  unsigned long checkTime = 0;
  unsigned long cleanTime = 0;

  auto mainMemory  = MemoryTracker::MemoryInBytes(0);
  auto checkMemory = MemoryTracker::MemoryInBytes(0);
  auto cleanMemory = MemoryTracker::MemoryInBytes(0);

  Pass::Header(fp);

  for (size_t i = 0; i < passes.size(); i++)
  {
    accumTime = accumTime + passes[i].TotalTime();
    accumMemory = accumMemory + passes[i].TotalMemory();

    passes[i].Print(fp, accumTime, totalTime, accumMemory, totalMemory);
  }

  for (size_t i = 0; i < passes.size(); i++)
  {
    mainTime  = mainTime  + passes[i].mPrimary.first;
    checkTime = checkTime + passes[i].mVerify.first;
    cleanTime = cleanTime + passes[i].mCleanAst.first;

    mainMemory  = mainMemory  + passes[i].mPrimary.second;
    checkMemory = checkMemory + passes[i].mVerify.second;
    cleanMemory = cleanMemory + passes[i].mCleanAst.second;
  }

  Pass::Footer(fp, mainTime, checkTime, cleanTime, totalTime,
                   mainMemory, checkMemory, cleanMemory, totalMemory);
}

/************************************* | **************************************
*                                                                             *
* Implementation of Phase                                                     *
*                                                                             *
************************************** | *************************************/

Phase::Phase(const char*                  name,
             int                          passId,
             PhaseTracker::SubPhase       subPhase,
             unsigned long                startTime,
             MemoryTracker::MemoryInBytes startMemory) {
  mName      = (subPhase == PhaseTracker::kPrimary) ? strdup(name) : 0;
  mPassId    = passId;
  mSubPhase  = subPhase;
  mStartTime = startTime;
  mStartMemory = startMemory;
}

Phase::~Phase() {
  if (mName)
    free(mName);
}

bool Phase::IsStartOfPass() const
{
  return (mSubPhase == PhaseTracker::kPrimary) ? true : false;
}

void Phase::ReportPass(unsigned long now,
                       MemoryTracker::MemoryInBytes nowMem) const {
  // clock-skew can cause now < startTime, just report 0 in that case
  unsigned long phaseTime = 0;
  if (now > mStartTime)
    phaseTime = now - mStartTime;
  MemoryTracker::MemoryInBytes phaseMemory = 0;
  if (nowMem > mStartMemory)
    phaseMemory = nowMem - mStartMemory;

  ReportTime(mName, phaseTime / 1e6);
  ReportMemory(mName, phaseMemory);

  if (developer) {
    char text[32];

    snprintf(text, sizeof(text), "  [%9d]", lastNodeIDUsed());

    ReportText(text);
  }

  ReportText("\n");
}

void Phase::ReportPassGroup(const char* label,
                            unsigned long totalTime,
                            MemoryTracker::MemoryInBytes totalMemory) {
  ReportTime(label, totalTime / 1e6);
  ReportText("\n");
}

void Phase::ReportTime(const char* name, double secs) {
  if (printPasses     == true)
    fprintf(stderr, "%32s :%8.3f seconds", name, secs);

  if (printPassesFile != 0)
    fprintf(printPassesFile, "%32s :%8.3f seconds", name, secs);
}

void Phase::ReportMemory(const char* name, MemoryTracker::MemoryInBytes bytes) {
  if (printPasses     == true)
    fprintf(stderr, "%32s :%8.3f MB", name, bytes / (1024.0 * 1024.0));

  if (printPassesFile != 0)
    fprintf(printPassesFile, "%32s :%8.3f MB", name, bytes / (1024.0 * 1024.0));
}

void Phase::ReportText(const char* text)
{
  if (printPasses     == true)
    fputs(text, stderr);

  if (printPassesFile != 0)
    fputs(text, printPassesFile);
}

/************************************* | **************************************
*                                                                             *
* Implementation of Pass                                                      *
*                                                                             *
************************************** | *************************************/

Pass::Pass()
{
  Reset();
}

Pass::~Pass()
{

}

void Pass::Reset()
{
  mName     = 0;
  mPassId   = 0;
  mIndex    = 0;
  mPrimary  = std::make_pair(0, MemoryTracker::MemoryInBytes(0));
  mVerify   = std::make_pair(0, MemoryTracker::MemoryInBytes(0));
  mCleanAst = std::make_pair(0, MemoryTracker::MemoryInBytes(0));
}

unsigned long Pass::TotalTime() const
{
  return mPrimary.first + mVerify.first + mCleanAst.first;
}

MemoryTracker::MemoryInBytes Pass::TotalMemory() const
{
  return mPrimary.second + mVerify.second + mCleanAst.second;
}

bool Pass::CompareByTime(Pass const& ref) const
{
  bool retval = false;

  if (TotalTime() > ref.TotalTime())
    retval = true;

  else if (TotalTime() < ref.TotalTime())
    retval = false;

  else
    retval = mIndex < ref.mIndex;

  return retval;
}

void Pass::Header(FILE* fp)
{
  // Print column headers
  fprintf(fp, "Pass               Name               ");

  fprintf(fp, "    Main ");
  fprintf(fp, "   Check ");
  fprintf(fp, "   Clean ");

  fprintf(fp, "    Time    %%  ");
  fprintf(fp, "   Accum    %%  ");
  fprintf(fp, "\n");


  // Print column underlines
  fprintf(fp, "---- ---------------------------------");

  fprintf(fp, "  -------");
  fprintf(fp, "  -------");
  fprintf(fp, "  -------");

  fprintf(fp, "  ------- -----");
  fprintf(fp, "  ------- -----");
  fprintf(fp, "\n");
}

void Pass::Print(FILE*                        fp,
                 unsigned long                accumTime,
                 unsigned long                totalTime,
                 MemoryTracker::MemoryInBytes accumMemory,
                 MemoryTracker::MemoryInBytes totalMemory) const {

  unsigned long passTime  = TotalTime();
  double passFrac  = (100.0 * passTime)  / totalTime;
  double accumFrac = (100.0 * accumTime) / totalTime;

  double primary   = mPrimary.first  / 1e6;
  double verify    = mVerify.first   / 1e6;
  double clean     = mCleanAst.first / 1e6;

  auto passMemory  = TotalMemory();
  auto passMemoryFrac  = (100.0 * passMemory)  / totalMemory;
  auto accumMemoryFrac = (100.0 * accumMemory) / totalMemory;

  auto primaryMem = mPrimary.second / (1024.0 * 1024.0);
  auto verifyMem  = mVerify.second  / (1024.0 * 1024.0);
  auto cleanMem   = mCleanAst.second / (1024.0 * 1024.0);

  if (mPassId > 0)
    fprintf(fp, "%4d ", mPassId);
  else
    fprintf(fp, "     ");

  fprintf(fp, "%-33s", mName);
  fprintf(fp, "  %7.3f  %7.3f  %7.3f", primary, verify, clean);
  fprintf(fp, "  %7.3f %5.1f", passTime  / 1e6, passFrac );
  fprintf(fp, "  %7.3f %5.1f", accumTime / 1e6, accumFrac);
  fprintf(fp, "  %7.3f %5.1f", passMemory / (1024.0 * 1024.0), passMemoryFrac);
  fprintf(fp, "  %7.3f %5.1f", accumMemory / (1024.0 * 1024.0), accumMemoryFrac);
  fprintf(fp, "\n");
}

void Pass::Footer(FILE*         fp,
                  unsigned long mainTime,
                  unsigned long checkTime,
                  unsigned long cleanTime,
                  unsigned long totalTime,
                  MemoryTracker::MemoryInBytes mainMemory,
                  MemoryTracker::MemoryInBytes checkMemory,
                  MemoryTracker::MemoryInBytes cleanMemory,
                  MemoryTracker::MemoryInBytes totalMemory) {

  fprintf(fp,
          "\n     %-33s  %7.3f  %7.3f  %7.3f  %7.3f \n     %-33s  %7.3f  %7.3f  %7.3f  %7.3f \n",
          "total time",
          mainTime / 1e6,
          checkTime / 1e6,
          cleanTime / 1e6,
          totalTime / 1e6,
          mainMemory / (1024.0 * 1024.0),
          checkMemory / (1024.0 * 1024.0),
          cleanMemory / (1024.0 * 1024.0),
          totalMemory / (1024.0 * 1024.0)
      );
}


