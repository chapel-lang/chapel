/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

#ifndef COMPILER_INCLUDE_ERROR_LOGGING_H
#define COMPILER_INCLUDE_ERROR_LOGGING_H

#include <cstdio>
#include <cstdlib>

#include "astlocs.h"
#include "baseAST.h"
#include "chpl/util/break.h"
#include "chpl/framework/ErrorBase.h"

//
// User facing error logging hooks. These are printf-style macros that make
// calls to the underlying implementation.
//
// INT_FATAL(ast, format, ...)
//   where ast         == BaseAST* or NULL
//         format, ... == normal printf stuff
// results in something like:
// INTERNAL ERROR in compilerSrc.c (lineno): your text here (usrSrc:usrLineno)
//

#ifndef COMPILER_SUBDIR
  #define COMPILER_SUBDIR
#endif

#define DO_TOSTRING(tok) #tok
#define TOSTRING(tok) DO_TOSTRING(tok)

enum ErrorLevel {
  ERR_INTERNAL_FATAL = 1,
  ERR_USER_FATAL,
  ERR_USER_FATAL_CONTINUE,
  ERR_USER_WARN,
  ERR_USER_PRINT
};

#define INT_FATAL gdbShouldBreakHere(), \
  setupError(TOSTRING(COMPILER_SUBDIR), __FILE__, __LINE__, \
             ERR_INTERNAL_FATAL), \
  handleError

#define USR_FATAL gdbShouldBreakHere(), \
  setupError(TOSTRING(COMPILER_SUBDIR), __FILE__, __LINE__, \
             ERR_USER_FATAL), \
  handleError

#define USR_FATAL_CONT gdbShouldBreakHere(), \
  setupError(TOSTRING(COMPILER_SUBDIR), __FILE__, __LINE__, \
             ERR_USER_FATAL_CONTINUE), \
  handleError

#define USR_WARN gdbShouldBreakHere(), \
  setupError(TOSTRING(COMPILER_SUBDIR), __FILE__, __LINE__, \
             ERR_USER_WARN), \
  handleError

#define USR_PRINT gdbShouldBreakHere(), \
  setupError(TOSTRING(COMPILER_SUBDIR), __FILE__, __LINE__, \
             ERR_USER_PRINT), \
  handleError

#define USR_STOP exitIfFatalErrorsEncountered

// INT_ASSERT is intended to become no-op in production builds of compiler
#define SELECT_ASSERT(_1, _2, NAME, ...) NAME
#define INT_ASSERT(...) SELECT_ASSERT(__VA_ARGS__, INT_ASSERT2, INT_ASSERT1)(__VA_ARGS__)
#define INT_ASSERT1(x) do { if (!(x)) INT_FATAL("assertion error"); } while (0)
#define INT_ASSERT2(s, x) do { if (!(x)) INT_FATAL((s), "assertion error"); } while (0)

void setupError(const char* subdir, const char* filename, int lineno,
                ErrorLevel tag);

// This function mostly exists as a convenience to set exit_immediately and
// exit_eventually, so both production and dyno errors can share the same
// exit-on-error logic.
void dynoSetupError(chpl::ErrorBase::Kind errKind);

// TODO: The __attribute__ stuff can't be portable, can it?!?!
void handleError(const char* fmt, ...) __attribute__ ((format (printf, 1, 2)));
void handleError(const BaseAST* ast, const char* fmt, ...) __attribute__ ((format (printf, 2, 3)));
void handleError(astlocT astloc, const char* fmt, ...) __attribute__ ((format (printf, 2, 3)));
void handleError(chpl::Location, const char* fmt, ...)__attribute__ ((format (printf, 2, 3)));

bool fatalErrorsEncountered();
void clearFatalErrors();
void exitIfFatalErrorsEncountered();
void considerExitingEndOfPass();
void clearLastErrorLocation();

bool printsSameLocationAsLastError(const BaseAST* ast);

// Supporting bold / colorful output to terminals
// These are "" if stderr is not a tty we think supports them
// Otherwise they are format codes that can be print in error messages.
const char* boldErrorFormat();
const char* underlineErrorFormat();
const char* clearErrorFormat();

// Unpacks a dyno syntax error and prints it. A hold-over from how the old
// parser used to print syntax errors.
void dynoPrintSyntaxError(const chpl::ErrorMessage& err);

#endif
