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

#ifndef _INITIALIZER_RESOLUTION_H_
#define _INITIALIZER_RESOLUTION_H_

// Defines the resolution strategy for initializers.

// This is different than how normal generic functions are handled
// and how generic constructors are handled, as we want to have the
// this argument for the type in the initializer argument list,
// and want to utilize Phase 1 of the initializer body to determine
//  the generic instantiation of it.

#include "baseAST.h"

class CallExpr;
class FnSymbol;
class Type;

FnSymbol* resolveInitializer(CallExpr* call);
void resolveNewInitializer(CallExpr* call, Type* manager = NULL);

#endif
