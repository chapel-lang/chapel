/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
#include <string>
#include <set>

struct ArgumentDescription;

#define LOG_NO_SHORT ' '
#define LOG_NEVER    '\0'

// runpasses uses this to configure the logger
void logMakePassAvailable(const char* name, char shortname);

// Driver uses this to configure the logger.
// arg might be a pass name or a 1-character short name (e.g. 'R')
void logSelectPass(const char* arg);


void  setupLogfiles();
void  teardownLogfiles();

void  logWriteLog(const char* passName, int passNum, char logTag);

bool  deletedIdON();

extern char  log_dir   [FILENAME_MAX + 1];
extern char  log_module[FILENAME_MAX + 1];

extern bool  fLogDir; // was --log-dir passed?

extern bool  fLog;
extern bool  fLogNode;
extern bool  fLogIds;

extern int   fdump_html;
extern char  fdump_html_chpl_home[FILENAME_MAX + 1];
extern bool  fdump_html_include_system_modules;
extern bool  fdump_html_wrap_lines;
extern bool  fdump_html_print_block_IDs;

extern FILE* deletedIdHandle;
extern char  deletedIdFilename[FILENAME_MAX + 1];

#endif
