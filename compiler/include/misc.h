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

#ifndef _misc_H_
#define _misc_H_

#include "driver.h"

#include <cstdio>

#define exit(x) dont_use_exit_use_clean_exit_instead

// INT_FATAL(ast, format, ...)
//   where ast         == BaseAST* or NULL
//         format, ... == normal printf stuff
// results in something like:
// INTERNAL ERROR in compilerSrc.c (lineno): your text here (usrSrc:usrLineno)

#define INT_FATAL      setupError(__FILE__, __LINE__, 1), handleError
#define USR_FATAL      setupError(__FILE__, __LINE__, 2), handleError
#define USR_FATAL_CONT setupError(__FILE__, __LINE__, 3), handleError
#define USR_WARN       setupError(__FILE__, __LINE__, 4), handleError
#define USR_PRINT      setupError(__FILE__, __LINE__, 5), handleError

#define USR_STOP       exitIfFatalErrorsEncountered

// INT_ASSERT is intended to become no-op in production builds of compiler
#define INT_ASSERT(x) do { if (!(x)) INT_FATAL("assertion error"); } while (0)

#define iterKindTypename          "iterKind"
#define iterKindLeaderTagname     "leader"
#define iterKindFollowerTagname   "follower"
#define iterKindStandaloneTagname "standalone"
#define iterFollowthisArgname     "followThis"

class BaseAST;

bool forceWidePtrs();
bool forceWidePtrsForLocal();
bool requireWideReferences();
bool requireOutlinedOn();

const char* cleanFilename(BaseAST*    ast);
const char* cleanFilename(const char* name);

void setupError(const char* filename, int lineno, int tag);
void handleError(const char* fmt, ...);
void handleError(BaseAST* ast, const char* fmt, ...);
void handleError(FILE* file, BaseAST* ast, const char* fmt, ...);
void exitIfFatalErrorsEncountered(void);
void considerExitingEndOfPass(void);
void printCallStack(bool force, bool shortModule, FILE* out);

void startCatchingSignals(void);
void stopCatchingSignals(void);

void clean_exit(int status);

void gdbShouldBreakHere(void); // must be exposed to avoid dead-code elim.

#endif
