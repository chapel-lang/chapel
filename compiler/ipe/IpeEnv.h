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

#ifndef _IPE_ENV_H_
#define _IPE_ENV_H_

class ArgSymbol;
class VarSymbol;
class LcnSymbol;

class IpeModule;
class IpeScope;
class IpeValue;

class Type;

class UnresolvedSymExpr;

#include <cstddef>

class IpeEnv
{
  //
  // Class interface
  //
public:
  static char*             gMemoryData;
  static size_t            gMemoryTail;
  static size_t            gMemorySize;

  static bool              globalsAllocate(size_t initialSize);
  static void              globalsDeallocate();

  static int               reserveSpace(VarSymbol* var);




  static int               allocateValue(bool  value);
  static int               allocateValue(long  value);
  static int               allocateValue(void* value);

  static bool              fetchBool   (LcnSymbol* sym);
  static long              fetchInteger(LcnSymbol* sym);
  static double            fetchReal   (LcnSymbol* sym);
  static void*             fetchPtr    (LcnSymbol* sym);

  static void              storeBool   (LcnSymbol* sym, IpeValue value);
  static void              storeInteger(LcnSymbol* sym, IpeValue value);
  static void              storeReal   (LcnSymbol* sym, IpeValue value);

  static IpeValue          addrOfFoo(VarSymbol* sym);
  static IpeValue          fetchFoo (LcnSymbol* sym);
  static void              storeFoo (LcnSymbol* sym, IpeValue value);


  //
  // Instance interface
  //
public:
                           IpeEnv(IpeScope* scope);
                           IpeEnv(IpeScope* scope, void* data);

                          ~IpeEnv();

  IpeScope*                scopeGet()                                         const;

  bool                     isEnvForBlockStmt()                                const;
  bool                     isEnvForModule()                                   const;
  bool                     isDefinedLocally(const char* name)                 const;

  int                      frameSize()                                        const;

  int                      locationSet(ArgSymbol* arg)                        const;
  int                      locationSet(VarSymbol* var)                        const;
  int                      locationCurr()                                     const;

  void                     allocateSpace(VarSymbol* var)                      const;

  void                     varAdd(VarSymbol* var)                             const;
  void                     argAdd(ArgSymbol* arg)                             const;

  LcnSymbol*               findLocal(const char* name)                        const;

  LcnSymbol*               findVariable(UnresolvedSymExpr* expr,
                                        bool               localOnly = false) const;

  LcnSymbol*               findVariable(const char*        name,
                                        bool               localOnly = false) const;

  void                     useAdd(IpeModule* use);
  void                     initializeUsedModules()                            const;

  IpeValue                 addrOf(VarSymbol* sym)                             const;

  IpeValue                 fetch (LcnSymbol* sym)                             const;

  void                     assign(ArgSymbol* sym, IpeValue value);
  void                     assign(VarSymbol* sym, IpeValue value);

  IpeValue                 valueForVariable(LcnSymbol* lcn)                   const;
  IpeValue                 valueFetch      (LcnSymbol* sym)                   const;
  void                     valueStore      (LcnSymbol* sym, IpeValue value)   const;

  void                     describe(int offset)                               const;

private:
                           IpeEnv();

  void                     assign(LcnSymbol* sym, IpeValue value);

  void                     describe(const char* pad,
                                    int         index,
                                    LcnSymbol*  var)                          const;

  IpeEnv*                  mParent;
  IpeScope*                mScope;
  void*                    mData;
};

#endif
