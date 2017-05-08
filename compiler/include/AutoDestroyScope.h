/*
 * Copyright 2004-2017 Cray Inc.
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

#ifndef _AUTO_DESTROY_SCOPE_H_
#define _AUTO_DESTROY_SCOPE_H_

class BlockStmt;
class Expr;
class FnSymbol;
class VarSymbol;

#include <vector>

// Track status of auto destroy variables within lexical scopes
class AutoDestroyScope {
public:
                           AutoDestroyScope(const AutoDestroyScope* parent,
                                            const BlockStmt*        block);

  void                     variableAdd(VarSymbol* var);

  bool                     handlingFormalTemps(const Expr* stmt) const;

  void                     insertAutoDestroys(FnSymbol* fn,
                                              Expr*     refStmt);

private:
  void                     variablesDestroy(Expr*      refStmt,
                                            VarSymbol* excludeVar)     const;

  const AutoDestroyScope*  mParent;
  const BlockStmt*         mBlock;

  bool                     mLocalsHandled;     // Manage function epilogue
  std::vector<VarSymbol*>  mFormalTemps;       // Temps for out/inout formals
  std::vector<VarSymbol*>  mLocals;
};

#endif

