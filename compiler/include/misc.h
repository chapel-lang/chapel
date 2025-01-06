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

#ifndef _MISC_H_
#define _MISC_H_

#include <cstdio>
#include <cstdlib>

#include "baseAST.h"

#include "astlocs.h"
#include "chpl/util/break.h"
#include "chpl/framework/ErrorBase.h"

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
#define SELECT_ASSERT(_1, _2, NAME, ...) NAME
#define INT_ASSERT(...) SELECT_ASSERT(__VA_ARGS__, INT_ASSERT2, INT_ASSERT1)(__VA_ARGS__)
#define INT_ASSERT1(x) do { if (!(x)) INT_FATAL("assertion error"); } while (0)
#define INT_ASSERT2(s, x) do { if (!(x)) INT_FATAL((s), "assertion error"); } while (0)

#define iterKindTypename          "iterKind"
#define iterKindLeaderTagname     "leader"
#define iterKindFollowerTagname   "follower"
#define iterKindStandaloneTagname "standalone"
#define iterFollowthisArgname     "followThis"

#define tupleInitName "chpl__init_tuple"

class BaseAST;

enum class GpuCodegenType {
  GPU_CG_NVIDIA_CUDA,
  GPU_CG_AMD_HIP,
  GPU_CG_CPU,
};

// this enum controls the behavior of argument passing to the generated GPU
// kernels. See the inline comments for what each bit means.
enum GpuArgKind {
  ADDROF = 1<<0, // if this bit is set, we pass the address of the argument
                 // otherwise, we pass it directly
  OFFLOAD = 1<<1, // if this bit is set, we call a different runtime function
                  // that also takes a size (that we generate with sizeof). That
                  // runtime function offloads given number of bytes to the GPU
                  // memory, and passes the new GPU pointer instead of what we
                  // pass. Note that we can either pass the variable directly
                  // (say, if it was already an address, or add an address-of
                  // using the previous bit)
                  // otherwise, the variable is passed directly
  REDUCE = 1<<2,  // this is a reduction temp
  HOST_REGISTER = 1<<3
};


bool        forceWidePtrsForLocal();
bool        requireWideReferences();
bool        requireOutlinedOn();
bool        usingGpuLocaleModel();
bool        isFullGpuCodegen();
GpuCodegenType getGpuCodegenType();

const char* cleanFilename(const BaseAST* ast);
const char* cleanFilename(const char*    name);

//
// Error tag values:
//
//  1 = INT_FATAL
//  2 = USR_FATAL
//  3 = USR_FATAL_CONT
//  4 = USR_WARN
//  5 = USR_PRINT
//
void        setupError(const char* subdir, const char* filename, int lineno, int tag);
void        setupDynoError(chpl::ErrorBase::Kind errKind);

void        handleError(const char* fmt, ...) __attribute__ ((format (printf, 1, 2)));
void        handleError(const BaseAST* ast, const char* fmt, ...)__attribute__ ((format (printf, 2, 3)));
void        handleError(astlocT astloc, const char* fmt, ...)__attribute__ ((format (printf, 2, 3)));
void        handleError(chpl::Location, const char* fmt, ...)__attribute__ ((format (printf, 2, 3)));

void        exitIfFatalErrorsEncountered();

void        considerExitingEndOfPass();

void        startCatchingSignals();
void        stopCatchingSignals();

void        clean_exit(int status) chpl_noreturn;

void        printCallStack();
void        printCallStackCalls();

bool        fatalErrorsEncountered();
void        clearFatalErrors();

bool printsSameLocationAsLastError(const BaseAST* ast);
void clearLastErrorLocation();

astlocT getUserInstantiationLocation(const BaseAST* ast);

// Returns true if an error/warning at this location
// (e.g. with USR_FATAL(ast, ...)) would print out
// a user line number.
bool        printsUserLocation(const BaseAST* ast);

// Supporting bold / colorful output to terminals
// These are "" if stderr is not a tty we think supports them
// Otherwise they are format codes that can be print in error messages.
const char* boldErrorFormat();
const char* underlineErrorFormat();
const char* clearErrorFormat();

#endif
