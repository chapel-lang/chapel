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
  static bool              globalsAllocate(size_t initialSize);
  static void              globalsDeallocate();

private:
  static char*             sStoreData;
  static int               sStoreTail;
  static int               sStoreSize;

  //
  // Instance interface
  //
public:
                           IpeEnv(IpeEnv*   parent,
                                  IpeScope* scope);

                           IpeEnv(IpeEnv*   parent,
                                  IpeScope* scope,
                                  int       frameSize);

                           IpeEnv(IpeEnv*   parent,
                                  IpeScope* scope,
                                  int       frameSize,
                                  void*     frameData);

                          ~IpeEnv();

  IpeScope*                scopeGet()                                         const;

  bool                     isEnvForBlockStmt()                                const;
  bool                     isEnvForModule()                                   const;

  int                      frameSize()                                        const;
  int                      depth()                                            const;

  void                     argAdd(ArgSymbol* arg)                             const;

  void                     varAdd(VarSymbol* var)                             const;
  int                      varCount()                                         const;
  LcnSymbol*               varGet(int index)                                  const;

  void                     useAdd(IpeModule* use);
  int                      useCount()                                         const;
  IpeModule*               useGet(int index)                                  const;

  LcnSymbol*               findLocal(const char* name)                        const;
  LcnSymbol*               findVariable(const char* name)                     const;

  IpeValue                 fetch           (LcnSymbol* sym)                   const;
  void*                    fetchPtr        (LcnSymbol* sym)                   const;

  int                      allocateValue(void* value);
  void                     extend          (LcnSymbol* sym, IpeValue value);
  void                     allocate        (LcnSymbol* sym, IpeValue value);

  void                     store           (LcnSymbol* sym, IpeValue value)   const;

  IpeValue                 addrOf          (LcnSymbol* sym)                   const;

  void                     initializeUsedModules()                            const;

  void                     describe(int offset)                               const;

private:
                           IpeEnv();

  LcnSymbol*               findVariable(const char* name, bool localOnly)     const;

  bool                     fetchBool   (LcnSymbol* sym)                       const;
  long                     fetchInteger(LcnSymbol* sym)                       const;
  double                   fetchReal   (LcnSymbol* sym)                       const;

  void                     describe(const char* pad,
                                    int         index,
                                    LcnSymbol*  var)                          const;


#if 0
  void                     assign          (ArgSymbol* sym, IpeValue value);

  int                      locationSet(ArgSymbol* arg)                        const;
  int                      locationSet(VarSymbol* var)                        const;
  int                      locationCurr()                                     const;

  void                     allocateSpace(VarSymbol* var)                      const;


  IpeValue                 valueForVariable(LcnSymbol* lcn)                   const;
  IpeValue                 valueFetch      (LcnSymbol* sym)                   const;

  void                     assign          (VarSymbol* sym, IpeValue value);

  int                      reserveSpace(VarSymbol* var);

  int                      allocateValue(bool  value);
  int                      allocateValue(long  value);

  void                     storeBool   (LcnSymbol* sym, IpeValue value);
  void                     storeInteger(LcnSymbol* sym, IpeValue value);
  void                     storeReal   (LcnSymbol* sym, IpeValue value);

  IpeValue                 fetchFoo (LcnSymbol* sym)                          const;
  void                     storeFoo (LcnSymbol* sym, IpeValue value);
  IpeValue                 addrOfFoo(VarSymbol* sym)                          const;

  void                     assign(LcnSymbol* sym, IpeValue value);
#endif

  IpeEnv*                  mParent;
  int                      mDepth;

  IpeScope*                mScope;

  char*                    mFrameData;
  int                      mFrameTail;
  int                      mFrameSize;
};

#endif
