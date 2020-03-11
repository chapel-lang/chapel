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

#ifndef _PARTIAL_COPY_DATA_H_
#define _PARTIAL_COPY_DATA_H_

//
// Used to pass information from FnSymbol::partialCopy() to
// FnSymbol::finalizeCopy().
//

#include "baseAST.h"

#include <vector>

class ArgSymbol;
class FnSymbol;

class PartialCopyData {
public:
                          PartialCopyData();
                         ~PartialCopyData();

  SymbolMap               partialCopyMap;

  FnSymbol*               partialCopySource;

  ArgSymbol*              varargOldFormal;
  std::vector<ArgSymbol*> varargNewFormals;

};

bool             hasPartialCopyData(const FnSymbol* fn);

PartialCopyData* getPartialCopyData(FnSymbol* fn);

PartialCopyData& addPartialCopyData(FnSymbol* fn);

void             clearPartialCopyData(FnSymbol* fn);

void             clearPartialCopyDataFnMap();

void             checkEmptyPartialCopyDataFnMap();

#endif
