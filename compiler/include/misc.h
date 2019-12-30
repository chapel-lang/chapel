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

#ifndef _MISC_H_
#define _MISC_H_

#include <cstdio>
#include <cstdlib>

#ifdef HAVE_LLVM
#define exit(x) clean_exit(x)
#else
// This interferes with uses of exit() in LLVM header files.
#define exit(x) dont_use_exit_use_clean_exit_instead
#endif

#if defined(__GNUC__) && __GNUC__ >= 3
#define chpl_noreturn __attribute__((__noreturn__))
#else
#define chpl_noreturn
#endif

#ifndef COMPILER_SUBDIR
#define COMPILER_SUBDIR
#endif

#define DO_TOSTRING(tok) #tok
#define TOSTRING(tok) DO_TOSTRING(tok)

// INT_FATAL(ast, format, ...)
//   where ast         == BaseAST* or NULL
//         format, ... == normal printf stuff
// results in something like:
// INTERNAL ERROR in compilerSrc.c (lineno): your text here (usrSrc:usrLineno)

#define INT_FATAL      gdbShouldBreakHere(), \
                       setupError(TOSTRING(COMPILER_SUBDIR), __FILE__, __LINE__, 1), handleError

#define USR_FATAL      gdbShouldBreakHere(), \
                       setupError(TOSTRING(COMPILER_SUBDIR), __FILE__, __LINE__, 2), handleError

#define USR_FATAL_CONT gdbShouldBreakHere(), \
                       setupError(TOSTRING(COMPILER_SUBDIR), __FILE__, __LINE__, 3), handleError

#define USR_WARN       setupError(TOSTRING(COMPILER_SUBDIR), __FILE__, __LINE__, 4), handleError

#define USR_PRINT      setupError(TOSTRING(COMPILER_SUBDIR), __FILE__, __LINE__, 5), handleError

#define USR_STOP       exitIfFatalErrorsEncountered

// INT_ASSERT is intended to become no-op in production builds of compiler
#define INT_ASSERT(x) do { if (!(x)) INT_FATAL("assertion error"); } while (0)

#define iterKindTypename          "iterKind"
#define iterKindLeaderTagname     "leader"
#define iterKindFollowerTagname   "follower"
#define iterKindStandaloneTagname "standalone"
#define iterFollowthisArgname     "followThis"

#define tupleInitName "chpl__init_tuple"

class BaseAST;

bool        forceWidePtrsForLocal();
bool        requireWideReferences();
bool        requireOutlinedOn();

const char* cleanFilename(const BaseAST* ast);
const char* cleanFilename(const char*    name);

void        setupError(const char* subdir, const char* filename, int lineno, int tag);

void        handleError(const char* fmt, ...);
void        handleError(const BaseAST* ast, const char* fmt, ...);
void        handleError(FILE* file, const BaseAST* ast, const char* fmt, ...);

void        exitIfFatalErrorsEncountered();

void        considerExitingEndOfPass();

void        printCallStack(bool force, bool shortModule, FILE* out);

void        startCatchingSignals();
void        stopCatchingSignals();

void        clean_exit(int status) chpl_noreturn;

void        printCallStack();
void        printCallStackCalls();

bool        fatalErrorsEncountered();
void        clearFatalErrors();

// Returns true if an error/warning at this location
// (e.g. with USR_FATAL(ast, ...)) would print out
// a user line number.
bool        printsUserLocation(const BaseAST* ast);

// must be exported to avoid dead-code elimination by C++ compiler
void        gdbShouldBreakHere();

#endif
