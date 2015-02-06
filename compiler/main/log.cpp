/*
 * Copyright 2004-2015 Cray Inc.
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

#include "files.h"
#include "misc.h"
#include "runpasses.h"

#include <cstdarg>
#include <cstring>
#include <sys/stat.h>

char             log_dir   [FILENAME_MAX + 1]           = "./log";
char             log_module[FILENAME_MAX + 1]           =      "";

bool             fLogIds                                =   false;

int              fdump_html                             =       0;
char             fdump_html_chpl_home[FILENAME_MAX + 1] =      "";
bool             fdump_html_include_system_modules      =    true;
bool             fdump_html_wrap_lines                  =    true;
bool             fdump_html_print_block_IDs             =   false;

FILE*            deletedIdHandle                        =    NULL;
char             deletedIdFilename[FILENAME_MAX + 1]    =      "";

static Vec<char> valid_log_flags;
static Vec<char> log_flags;

void log_flags_arg(const ArgumentState* state, const char* arg) {
  if (valid_log_flags.count() == 0)
    initLogFlags(valid_log_flags);

  // --log or --log= means "log every pass"
  // Actually, passes whose log letter is NUL are skipped.
  if (*arg == '\0') {   // empty
    log_flags.set_union(valid_log_flags);   // Set all.

  } else {

    // Parse the argument of --log=<arg> or -d<arg> for specific log flags.
    // Each flag is a single letter; see log.h for the correspondence.
    while (*arg) {
      if (valid_log_flags.set_in(*arg))
        log_flags.set_add(*arg);
      else {
        fprintf(stderr, "Unrecognized log flag: '%c'\n", *arg);
        clean_exit(1);
      }

      arg++;
    }
  }
}

void setupLogfiles() {
  if (log_flags.count() > 0 || fdump_html || *deletedIdFilename) {
    ensureDirExists(log_dir, "ensuring directory for log files exists");
  }

  if (log_dir[strlen(log_dir) - 1] != '/') {
    strcat(log_dir, "/");
  }

  if (fdump_html) {
    AstDumpToHtml::init();
  }

  if (deletedIdFilename[0] != '\0') {
    if ((deletedIdHandle = fopen(deletedIdFilename, "w")) == 0) {
      USR_FATAL("cannot open file \"%s\", to log deleted AST ids, for writing", deletedIdFilename);
    }
  }
}

void teardownLogfiles() {
  if (fdump_html) {
    AstDumpToHtml::done();
  }

  if (deletedIdFilename[0] != '\0') {
    fclose(deletedIdHandle);
    deletedIdHandle = NULL;
  }
}

void log_writeLog(const char* passName, int passNum, char logTag) {
  if (fdump_html) {
    AstDumpToHtml::view(passName);
  }

  if (log_flags.set_in(logTag) != 0) {
    if (fUseIPE == false)
      AstDump::view(passName, passNum);
    else
      AstDumpToNode::view(passName, passNum);
  }
}

bool deletedIdON() {
  return (deletedIdFilename[0] != '\0') ? true : false;
}
