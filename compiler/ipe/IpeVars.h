/*
 * Copyright 2004-2015 Cray Inc.
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

#ifndef _IPE_VARS_H_
#define _IPE_VARS_H_

#include "IpeValue.h"
#include <vector>

class IpeScope;
class LcnSymbol;
class VarSymbol;

class IpeVars
{
  //
  // The class interface
  //
  // This provides
  //   1) operations for allocating/reading/writing the root environment
  //   2) operations for allocating/reading/writing "stack frames"
  //
public:
  static IpeVars*                  rootAllocate(int size);

  static IpeVars*                  allocate(int dataSize);
  static void                      deallocate(IpeVars* vars);

  //
  // If sym->depth() == 0 then
  //    the operation is on the root env and env must be NULL
  // else
  //    the operation is on an allocated "stack frame"
  //

  // This is only valid for the root environment
  static void                      bind  (VarSymbol* sym,    IpeValue value, IpeVars* vars);

  // Valid for all environments
  static IpeValue                  addrOf(LcnSymbol* sym,                    IpeVars* vars);

  static IpeValue                  fetch (LcnSymbol* sym,                    IpeVars* vars);
  static void                      store (LcnSymbol* sym,    IpeValue value, IpeVars* vars);

  static void                      describe(IpeVars* env, int offset = 0);

private:
  static void                      ensureSpaceToExtend(int size);

  static IpeValue*                 sRootData;
  static int                       sRootSize;
  static int                       sRootTail;

  static std::vector<LcnSymbol*>   sSymbols;

  //
  // The instance interface
  //
  // These represent "stack frames".
  // These are, effectively,  dynamically sized "structs".
  // No virtual functions etc.
  //
public:
  IpeValue                         valueGet(int index)                                const;

private:
                                   IpeVars();
                                  ~IpeVars();

  IpeVars*                         mLexicalParent;
  IpeScope*                        mScope;
  IpeValue                         mValues[1];
};

#endif
