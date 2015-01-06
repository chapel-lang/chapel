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

#ifndef _log_H_
#define _log_H_

#include <cstdio>

/*
  To add a new log type, add a new LOG_XXX with a unique letter:
     #define LOG_AST 'a'

  To write to the new log:
     log(LOG_AST, "starting AST #%d\n", 1);

  To test if a log should be written:
     if (logging(LOG_AST)) printf("writing AST log");

  To write based on a "log level":
     log_level(LOG_AST, 3, "starting level 3 AST log\n");

  To test based on a "log level":
     if (logging_level(LOG_AST, 3)) printf("starting level 3 AST log\n");

  To get the FILE* for a log:
     FILE *fp = log_fp(LOG_AST);

  log levels start at 1
  logs appear in the log_dir directory.
  one level of old logs is stored in log_dir/save.

  To specify logs on the command line:
    chpl -laai
  specifies AST log level 2 and IF1 log level 1
 */

struct ArgumentState;

#ifndef NUL
 #define NUL '\0'
#endif

// Driver uses this to configure the logger
void  log_flags_arg(const ArgumentState* state, const char* arg);

void  setupLogfiles();
void  teardownLogfiles();

void  log_writeLog(const char* passName, int passNum, char logTag);

bool  deletedIdON();

extern char  log_dir   [FILENAME_MAX + 1];
extern char  log_module[FILENAME_MAX + 1];

extern bool  fLogIds;

extern int   fdump_html;
extern char  fdump_html_chpl_home[FILENAME_MAX + 1];
extern bool  fdump_html_include_system_modules;
extern bool  fdump_html_wrap_lines;
extern bool  fdump_html_print_block_IDs;

extern FILE* deletedIdHandle;
extern char  deletedIdFilename[FILENAME_MAX + 1];

#endif
