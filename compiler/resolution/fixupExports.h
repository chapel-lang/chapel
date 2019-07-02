/*
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

#ifndef _RESOLUTION_FIXUP_EXPORTS_H_
#define _RESOLUTION_FIXUP_EXPORTS_H_

#include <vector>

class FnSymbol;
class Type;

FnSymbol* getUnwrappedFunction(FnSymbol* wrapper);
void fixupExportedFunctions(const std::vector<FnSymbol*>& fns);
void fixupExportedFunction(FnSymbol* fn);

// This may only be called _after_ resolution has occurred.
Type* getCharPtrType(void);

#endif
