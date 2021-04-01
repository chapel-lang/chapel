/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

#include "log.h"

#include "AstDump.h"
#include "AstDumpToHtml.h"
#include "AstDumpToNode.h"
#include "driver.h"
#include "files.h"
#include "misc.h"
#include "runpasses.h"

#include <cstdarg>
#include <cstring>
#include <set>
#include <string>
#include <sys/stat.h>

std::string      log_dir                                = "./log";
std::string      log_module                             =      "";

bool             fLog                                   =    false;
bool             fLogDir                                =    false;
bool             fLogNode                               =    false;
bool             fLogIds                                =    true;

int              fdump_html                             =       0;
std::string      fdump_html_chpl_home                   =      "";
bool             fdump_html_include_system_modules      =    true;
bool             fdump_html_wrap_lines                  =    true;
bool             fdump_html_print_block_IDs             =   false;

FILE*            deletedIdHandle                        =    NULL;
std::string      deletedIdFilename                      =      "";

// Keeping names of available passes
static bool availableInitialized = false;
static std::vector<std::string> logAvailableNames;
static std::vector<char> logAvailableShortNames;

// Which passes should be logged?
static bool logAll = true;
static std::set<std::string> logOnlyName;

void logMakePassAvailable(const char* name, char shortname)
{
  logAvailableNames.push_back(name);
  logAvailableShortNames.push_back(shortname);
}

void logSelectPass(const char* arg) {
  // The first time this is run, collect the available pass names.
  if (availableInitialized == false) {
    initPassesForLogging(); // calls logMakePassAvailable for each pass
    availableInitialized = true;
  }

  // Make --log optional by setting it if --log-pass= is set
  fLog = true;

  // Since we selected a pass, don't log everything
  logAll = false;

  // Check first for a match in the available pass names
  for( size_t i = 0; i < logAvailableNames.size(); i++ ) {
    if (logAvailableNames[i] == arg) {
      logOnlyName.insert(logAvailableNames[i]);
      return;
    }
  }

  // If none found, check for a match in the short names.
  if (strlen(arg) == 1 &&
      arg[0] != LOG_NO_SHORT &&
      arg[0] != LOG_NEVER) {
    for( size_t i = 0; i < logAvailableShortNames.size(); i++ ) {
      if (logAvailableShortNames[i] == arg[0]) {
        logOnlyName.insert(logAvailableNames[i]);
        return;
      }
    }
  }

  // Otherwise, we don't know what pass this is.
  fprintf(stderr, "Unrecognized log pass name: \"%s\"\n", arg);
  clean_exit(1);
}

void setupLogfiles() {
  // Enable logging if --log-module is passed.
  if (!log_module.empty())
    fLog = true;
  // Enable logging if --log-pass is used
  if (logOnlyName.size() > 0)
    fLog = true;
  // Enable logging if --log-dir is used
  if (fLogDir == true)
    fLog = true;

  if (fLog || fdump_html || !deletedIdFilename.empty()) {
    // Remove the log directory to make sure there is no stale data
    deleteDir(log_dir.c_str());
    ensureDirExists(log_dir.c_str(), "ensuring directory for log files exists");
  }

  if (log_dir.back()!='/') {
    log_dir.push_back('/');
  }

  if (fdump_html) {
    AstDumpToHtml::init();
  }

  if (!deletedIdFilename.empty()) {
    if ((deletedIdHandle = fopen(deletedIdFilename.c_str(), "w")) == 0) {
      USR_FATAL("cannot open file \"%s\", to log deleted AST ids, for writing", deletedIdFilename.c_str());
    }
  }
}

void teardownLogfiles() {
  if (fdump_html) {
    AstDumpToHtml::done();
  }

  if (!deletedIdFilename.empty()) {
    fclose(deletedIdHandle);
    deletedIdHandle = NULL;
  }
}

void logWriteLog(const char* passName, int passNum, char logTag) {
  if (fdump_html) {
    AstDumpToHtml::view(passName);
  }

  if (fLog) {
    if ((logAll == true && logTag != LOG_NEVER) ||
        logOnlyName.count(passName) > 0) {
      bool logNode = (fLogNode);
      if (logNode)
        AstDumpToNode::view(passName, passNum);
      else
        AstDump::view(passName, passNum);
    }
  }
}

bool deletedIdON() {
  return (!deletedIdFilename.empty()) ? true : false;
}
