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

#ifndef _IPE_MODULE_H_
#define _IPE_MODULE_H_

#include "symbol.h"

#include <cstddef>
#include <vector>

class Expr;
class CallExpr;
class DefExpr;
class SymExpr;

class IpeCallExpr;
class IpeEnv;
class IpeModuleRoot;
class IpeScopeModule;
class IpeValue;

class LcnSymbol;
class ModuleSymbol;

class IpeModule
{
public:
  const char*              name()                                             const;
  IpeEnv*                  environment()                                      const;

  void                     moduleAdd(IpeModule* module);
  IpeModule*               moduleByName(const char* name)                     const;

  void                     moduleResolve(IpeModuleRoot* rootModule);
  void                     initialize();

  IpeValue                 evaluate(Expr* expr)                               const;

  static void              describeAllModules(int offset);

protected:
  enum State
  {
    kLoaded,

    kResolving,
    kResolved,

    kInitializing,
    kInitialized
  };

                           IpeModule(ModuleSymbol* sym);
                           IpeModule();

                          ~IpeModule();

  virtual const char*      moduleTypeAsString()                               const = 0;

  const char*              stateAsString()                                    const;
  bool                     isUseStmt(Expr* expr)                              const;

  void                     ensureInitialized();

  State                    mState;
  ModuleSymbol*            mModuleDecl;
  IpeEnv*                  mEnv;

private:
  void                     describe(int offset)                               const;
};

#endif
