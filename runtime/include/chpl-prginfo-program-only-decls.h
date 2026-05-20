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
extern char* chpl_executionCommand;
extern const int warnUnstable;

#endif
