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

// This file is used in LLVM backend compiles to compile the header
// declarations for the Chapel runtime into an LLVM module.

// Reduce our exposure to overloads of symbols with the same name
// when parsing runtime headers in C++ mode during our LLVM codegen.
// https://github.com/chapel-lang/chapel/issues/25506
#define CHPL_AVOID_CPP_CODE 1

#include "stdchpl.h"

#include "chpl-gen-includes.h"
