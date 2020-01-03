/*
 * Copyright 2004-2020 Cray Inc.
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

#ifndef _INITIALIZER_RULES_H_
#define _INITIALIZER_RULES_H_

class AggregateType;
class BlockStmt;
class CallExpr;
class FnSymbol;

bool      isSuperInit(CallExpr* stmt);
bool      isThisInit (CallExpr* stmt);
bool      isInitDone (CallExpr* stmt);

bool      hasInitDone(BlockStmt* block);

void      errorOnFieldsInArgList(FnSymbol* fn);

void      preNormalizeFields(AggregateType* at);
void      preNormalizeInitMethod(FnSymbol* fn);
void      preNormalizePostInit(AggregateType* at);

#endif
