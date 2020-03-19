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

#ifndef _AUTO_DESTROY_SCOPE_H_
#define _AUTO_DESTROY_SCOPE_H_

#include "baseAST.h"

#include <set>
#include <vector>

// Track status of auto destroy variables within lexical scopes
class AutoDestroyScope {
public:
                           AutoDestroyScope(AutoDestroyScope* parent,
                                            const BlockStmt*  block);

  void                     addFormalTemps();

  // adds a declaration
  void                     variableAdd(VarSymbol* var);

  void                     deferAdd(DeferStmt* var);

  // adds an initialization
  void                     addInitialization(VarSymbol* var);

  void                     addEarlyDeinit(VarSymbol* var);

  VarSymbol*               findVariableUsedBeforeInitialized(Expr* stmt);

  // Forget about initializations for outer variables initialized
  // in this scope. The variables will no longer be considered initialized.
  // This matters for split-init and conditionals.
  void                     forgetOuterVariableInitializations();

  // Returns outer variables initialized in this scope
  std::vector<VarSymbol*>  getInitedOuterVars() const;
  // Returns variables in outer scopes deinited early (from copy elision)
  std::vector<VarSymbol*>  getDeinitedOuterVars() const;

  AutoDestroyScope*        getParentScope() const;

  // Report initialization for outer variables to parent scope
  void                     addInitializationsToParent() const;

  bool                     handlingFormalTemps(const Expr* stmt) const;

  void                     insertAutoDestroys(FnSymbol* fn,
                                              Expr*     refStmt,
                                              const std::set<VarSymbol*>& ignored);

  void                     destroyVariable(Expr* after, VarSymbol* var,
                                           const std::set<VarSymbol*>& ignored);

private:
  void                     variablesDestroy(Expr*      refStmt,
                                            VarSymbol* excludeVar,
                                            const std::set<VarSymbol*>& ignored,
                                            AutoDestroyScope* startingScope) const;

  void                     destroyOuterVariables(Expr* before,
                                                 std::set<VarSymbol*>& ignored) const;

  // Returns true if the variable has already been initialized - and
  // has not already been deinitialized - in this or a parent scope.
  // Returns false otherwise.
  bool                     isVariableInitialized(VarSymbol* var) const;

  // Returns true if the variable has been declared in this or a parent scope.
  bool                     isVariableDeclared(VarSymbol* var) const;

  AutoDestroyScope*        mParent;
  const BlockStmt*         mBlock;

  bool                     mLocalsHandled;     // Manage function epilogue
  std::vector<CallExpr*>   mFormalTempActions; // e.g. = back for inout
  std::vector<BaseAST*>    mLocalsAndDefers;   // VarSymbol* or DeferStmt*
  // note: mLocalsAndDefers contains both VarSymbol and DeferStmt in
  // order to create a single stack for cleanup operations to be executed.
  // In particular, the ordering between defer blocks and locals matters,
  // in addition to the ordering within each group.
  // This vector stores variables declared in this block that have
  // been initialized by this point in the traversal. It stores variables
  // in initialization order.

  // Which variables are declared in this scope?
  std::set<VarSymbol*>     mDeclaredVars;

  // Which variables have been initialized in this scope
  // (possibly including outer variables)?
  std::set<VarSymbol*>     mInitedVars;

  // Which outer variables have been initialized in this scope?
  // This vector lists them in initialization order.
  std::vector<VarSymbol*>  mInitedOuterVars;

  // Which variables have been deinitialized early in this scope
  // (possibly including outer variables)?
  std::set<VarSymbol*>     mDeinitedVars;
};

#endif

