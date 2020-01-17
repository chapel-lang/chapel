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

#ifndef _FIXUP_EXPORTS_H_
#define _FIXUP_EXPORTS_H_

#include <vector>

class ArgSymbol;
class FnSymbol;
class Type;

//
// These are only initialized _after_ "fixupExportedFunctions" has been called
// during resolution.
//
extern Type* exportTypeChplByteBuffer;

FnSymbol* getUnwrappedFunction(FnSymbol* wrapper);
ArgSymbol* getUnwrappedArg(ArgSymbol* arg);

// Some return types can change after lowering (i.e., callDestructors).
Type* getUnwrappedRetType(FnSymbol* wrapper);

void fixupExportedFunctions(const std::vector<FnSymbol*>& fns);
void fixupExportedFunction(FnSymbol* fn);

#endif
