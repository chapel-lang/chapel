/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#ifndef _BUILD_DEFAULT_FUNCTIONS_H_
#define _BUILD_DEFAULT_FUNCTIONS_H_

#include "symbol.h"
#include "type.h"

// build a hollow `writeThis` FunctionSymbol* for the given class type
// 'ct', returning its file (channel) argument in 'filearg'.  The
// callee is expected to fill in the body of the function and to
// normalize the returned function before continuing.

FnSymbol* buildWriteThisFnSymbol(AggregateType* ct, ArgSymbol** filearg);

#endif

