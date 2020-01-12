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

#include "PhaseTracker.h"

#include "baseAST.h"
#include "driver.h"

#include <cstdlib>
#include <cstring>
#include <algorithm>

// Used to collect the times as the program runs
class Phase
{
public:
                           Phase(const char*            name,
                                 int                    passId,
                                 PhaseTracker::SubPhase subPhase,
                                 unsigned long          startTime);
                          ~Phase();

  bool                     IsStartOfPass()                            const;

  void                     ReportPass (unsigned long now)   const;
  static void              ReportTotal(unsigned long totalTime);

  static void              ReportTime(const char* name, double secs);
  static void              ReportText(const char* text);

  char*                    mName;       // Only set for kPrimary
  int                      mPassId;
  PhaseTracker::SubPhase   mSubPhase;
  unsigned long            mStartTime;  // Elapsed time from main() usecs

private:
  Phase();
};

// Group the phases in to passes and report on passes
class Pass
{
public:
                 Pass();
                ~Pass();

  static void    Header(FILE* fp);
  static void    Footer(FILE*         fp, 
                        unsigned long mainTime,
                        unsigned long checkTime,
                        unsigned long cleanTime,
                        unsigned long totalTime);

  bool           CompareByTime(Pass const& ref)      const;

  void           Reset();
  unsigned long  TotalTime()                         const;

  void           Print(FILE*         fp,
                       unsigned long accumTime, 
                       unsigned long totalTime)      const;

  char*          mName;
  int            mPassId;
  int            mIndex;
  unsigned long  mPrimary;          // usecs()
  unsigned long  mVerify;           // usecs()
  unsigned long  mCleanAst;         // usecs()
};

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
  Phase* phase = new Phase(name, passId, subPhase, mTimer.elapsedUsecs());

  mPhases.push_back(phase);
}

void PhaseTracker::Stop()
{
  mTimer.stop();
}

void PhaseTracker::ReportPass() const
{
  int index = mPhases.size() - 1;

  while (index >= 0 && mPhases[index]->IsStartOfPass() == false)
    index = index - 1;

  mPhases[index]->ReportPass(mTimer.elapsedUsecs());
}

void PhaseTracker::ReportTotal() const
{
  Phase::ReportTotal(mTimer.elapsedUsecs());
}

void PhaseTracker::ReportRollup() const
{
  std::vector<Pass> passes;
  unsigned long     totalTime = mTimer.elapsedUsecs();

  PassesCollect(passes);
  PassesReport(passes, totalTime);
  
  Phase::ReportText("\n\n\n");

  PassesSortByTime(passes);
  PassesReport(passes, totalTime);
}

void PhaseTracker::PassesCollect(std::vector<Pass>& passes) const
{
  unsigned long totalTime = mTimer.elapsedUsecs();

  if (mPhases.size() > 0)
  {
    Pass pass;

    for (size_t i = 0; i < mPhases.size(); i++)
    {
      unsigned long start   = mPhases[i]->mStartTime;
      unsigned long elapsed = 0;

      // Check if it's time to push an completed pass
      if (i > 0 && mPhases[i]->mSubPhase == PhaseTracker::kPrimary)
      {
        passes.push_back(pass);
        pass.Reset();
      }

      if (i < mPhases.size() - 1)
        elapsed = mPhases[i + 1]->mStartTime - start;
      else
        elapsed = totalTime                 - start;

      switch (mPhases[i]->mSubPhase)
      {
        case PhaseTracker::kPrimary:
          pass.mName     = mPhases[i]->mName;
          pass.mPassId   = mPhases[i]->mPassId;
          pass.mIndex    = (int) passes.size();
          pass.mPrimary  = elapsed;
          break;

        case PhaseTracker::kVerify:
          pass.mVerify   = elapsed;
          break;

        case PhaseTracker::kCleanAst:
          pass.mCleanAst = elapsed;
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

static void PassesReport(const std::vector<Pass>& passes,
                         unsigned long            totalTime)
{
  if (printPasses     == true)
    PassesReport(stderr, passes, totalTime);

  if (printPassesFile != 0)
    PassesReport(printPassesFile, passes, totalTime);
}

static void PassesReport(FILE*                    fp,
                         const std::vector<Pass>& passes,
                         unsigned long            totalTime)
{
  unsigned long accumTime = 0;

  unsigned long mainTime  = 0;
  unsigned long checkTime = 0;
  unsigned long cleanTime = 0;

  Pass::Header(fp);

  for (size_t i = 0; i < passes.size(); i++)
  {
    accumTime = accumTime + passes[i].TotalTime();

    passes[i].Print(fp, accumTime, totalTime);
  }

  for (size_t i = 0; i < passes.size(); i++)
  {
    mainTime  = mainTime  + passes[i].mPrimary;
    checkTime = checkTime + passes[i].mVerify;
    cleanTime = cleanTime + passes[i].mCleanAst;
  }

  Pass::Footer(fp, mainTime, checkTime, cleanTime, totalTime);
}

/************************************* | **************************************
*                                                                             *
* Implementation of Phase                                                     *
*                                                                             *
************************************** | *************************************/

Phase::Phase(const char*            name,
             int                    passId,
             PhaseTracker::SubPhase subPhase,
             unsigned long          startTime)
{
  mName      = (subPhase == PhaseTracker::kPrimary) ? strdup(name) : 0;
  mPassId    = passId;
  mSubPhase  = subPhase;
  mStartTime = startTime;
}

Phase::~Phase()
{
  if (mName)
    free(mName);
}

bool Phase::IsStartOfPass() const
{
  return (mSubPhase == PhaseTracker::kPrimary) ? true : false;
}

void Phase::ReportPass(unsigned long now) const
{
  // clock-skew can cause now < startTime, just report 0 in that case
  unsigned long phaseTime = 0;
  if (now > mStartTime)
    phaseTime = now - mStartTime;

  ReportTime(mName, phaseTime / 1e6);

  if (developer == true)
  {
    char text[32];

    sprintf(text, "  [%9d]", lastNodeIDUsed());

    ReportText(text);
  }

  ReportText("\n");
}

void Phase::ReportTotal(unsigned long totalTime)
{
  ReportTime("total time", totalTime / 1e6);
  ReportText("\n\n\n\n");
}

void Phase::ReportTime(const char* name, double secs)
{
  if (printPasses     == true)
    fprintf(stderr, "%32s :%8.3f seconds", name, secs);

  if (printPassesFile != 0)
    fprintf(printPassesFile, "%32s :%8.3f seconds", name, secs);
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
  mPrimary  = 0;
  mVerify   = 0;
  mCleanAst = 0;
}

unsigned long Pass::TotalTime() const
{
  return mPrimary + mVerify + mCleanAst;
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

void Pass::Print(FILE*         fp, 
                 unsigned long accumTime, 
                 unsigned long totalTime) const
{
  unsigned long passTime  = TotalTime();
  double        passFrac  = (100.0 * passTime)  / totalTime;
  double        accumFrac = (100.0 * accumTime) / totalTime;

  double        primary   = mPrimary  / 1e6;
  double        verify    = mVerify   / 1e6;
  double        clean     = mCleanAst / 1e6;

  if (mPassId > 0)
    fprintf(fp, "%4d ", mPassId);
  else
    fprintf(fp, "     ");

  fprintf(fp, "%-33s", mName);
  fprintf(fp, "  %7.3f  %7.3f  %7.3f", primary, verify, clean);
  fprintf(fp, "  %7.3f %5.1f", passTime  / 1e6, passFrac );
  fprintf(fp, "  %7.3f %5.1f", accumTime / 1e6, accumFrac);
  fprintf(fp, "\n");
}

void Pass::Footer(FILE*         fp, 
                  unsigned long mainTime,
                  unsigned long checkTime,
                  unsigned long cleanTime,
                  unsigned long totalTime)
{
  fprintf(fp,
          "\n     %-33s  %7.3f  %7.3f  %7.3f  %7.3f\n",
          "total time",
          mainTime / 1e6,
          checkTime / 1e6,
          cleanTime / 1e6,
          totalTime / 1e6);
}



