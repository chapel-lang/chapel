/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_RT_PRGINFO_PROGRAM_ONLY_DECLS_H
#define CHPL_RT_PRGINFO_PROGRAM_ONLY_DECLS_H

#include "chpl-prginfo.h"

//
// DO NOT INCLUDE THIS HEADER WITHIN RUNTIME CODE!
//

//
// This header fixes issues with '--incremental' compilation.
//
// This header is a workaround for the fact that the compiler cannot tell
// the types of certain global compiler-generated symbols such as
// 'CreateConfigVarTable' or 'mainHasArgs' because it previously relied
// on them being visible via 'chplcgfns.h'.
//
// We are trying to remove 'chplcgfns.h' so that the runtime cannot access
// the symbol declarations in it (because it should not be linking against
// those symbols at all).
//
// This header is a workaround, but eventually we _should remove it too_.
// It will be easier to remove after 'chplcgfns.h' is gone.

// Manually declare rather than rely on macro-expansion since the signature
// for some callbacks can differ from machine to machine (e.g., they use
// Chapel C types).
//
// Also, the declarations in 'chpl-prginfo-data-macro.h' tend to omit
// 'const' qualifiers which makes the backend C compiler unhappy.

extern void CreateConfigVarTable(void);
extern const int mainHasArgs;
extern const int mainPreserveDelimiter;
extern const char* chpl_compileCommand;
extern char* chpl_executionCommand;
extern const char* chpl_compileDirectory;
extern const char* chpl_saveCDir;
extern const int warnUnstable;
extern const char* CHPL_LOCALE_MODEL;
extern const char* CHPL_TARGET_CPU;
extern const char* CHPL_TARGET_PLATFORM;
extern const char* CHPL_UNWIND;
extern const char* CHPL_HOME;
extern const char* CHPL_COMM;
extern const char* CHPL_LAUNCHER;
extern const char* CHPL_GASNET_SEGMENT;
extern const char* CHPL_LLVM_BIN_DIR;
extern const char* CHPL_TARGET_MEM;
extern const char* CHPL_THIRD_PARTY;
extern const int CHPL_INTERLEAVE_MEM;
extern const int CHPL_STACK_CHECKS;
extern const int CHPL_CACHE_REMOTE;
extern const int chpl_sizeSymTable;
extern const int chpl_filenumSymTable[];
extern const c_string chpl_funSymTable[];
extern const c_string chpl_filenameTable[];
extern const int chpl_filenameTableSize;


#endif
