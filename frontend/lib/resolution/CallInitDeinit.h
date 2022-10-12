/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#ifndef CALL_INIT_DEINIT_H
#define CALL_INIT_DEINIT_H

#include "chpl/resolution/ResolvedVisitor.h"
#include "chpl/resolution/resolution-types.h"
#include "chpl/uast/all-uast.h"

namespace chpl {
namespace resolution {

class Resolver;

  // TODO -- this is just for split init
  // TOOD -- use ResolvedVisitor.h for copy and deinit
  // TODO -- figure out where to store copy (associatedFns?)
  //         and where to store deinit (associatedFns not so good)
/*struct InitDeinitState {
  bool inited = false;
  bool lastIsCopy = false;
  bool inScope = false;
};*/

struct Action {
  enum ActionKind {
    COPY_INIT, // for 'in'
    WRITE_BACK, // for 'out' and 'inout'
    DEINIT,
  };
  ActionKind action; // which action?
  ID id;             // which ID?
  Action(ActionKind action, ID id) : action(action), id(id) { }
};

// for blocks / things that behave like blocks
struct ScopeFrame {
  const Scope* scope = nullptr;

  // localsAndDefers contains both VarSymbol and DeferStmt in
  // order to create a single stack for cleanup operations to be executed.
  // In particular, the ordering between defer blocks and locals matters,
  // in addition to the ordering within each group.
  std::vector<const AstNode*> localsAndDefers;

  // Which variables are initialized in this scope
  // (possibly including outer variables)?
  std::set<const VarLikeDecl*> declaredVars;

  // Which outer variables have been initialized in this scope?
  // This vector lists them in initialization order.
  std::vector<const VarLikeDecl*> initedOuterVars;

  // Which variables have been deinitialized early in this scope?
  std::set<const VarLikeDecl*> deinitedVars;

  // What actions should be taken at the end of the scope?
  std::vector<Action> endOfScopeActions;

  // Stores the state for variables that are currently
  //std::map<ID, InitDeinitState> varState;

  ScopeFrame(const Scope* scope) : scope(scope) { }
};

// Resolves init, deinit, and assign
// TODO: should it be renamed to include Assign?
struct CallInitDeinit {
  using RV = MutatingResolvedVisitor<CallInitDeinit>;

  // inputs to the process
  Context* context = nullptr;
  Resolver& resolver;

  // internal variables

  // for handling calls, nested calls, end of statement actions 
  std::vector<const Call*> callStack;
  std::vector<Action> endOfStatementActions;

  // for handling end of block / end of scope actions
  std::vector<ScopeFrame> scopeStack;

  // main entry point to this code
  // updates the ResolutionResultsByPostorderID
  static void process(Resolver& resolver);

  CallInitDeinit(Resolver& resolver);

  void printActions(const std::vector<Action>& actions);

  void enterScope(const uast::AstNode* ast);
  void exitScope(const uast::AstNode* ast);

  bool enter(const VarLikeDecl* ast, RV& rv);
  void exit(const VarLikeDecl* ast, RV& rv);

  bool enter(const Call* ast, RV& rv);
  void exit(const Call* ast, RV& rv);

  bool enter(const uast::AstNode* node, RV& rv);
  void exit(const uast::AstNode* node, RV& rv);
};



} // end namespace resolution
} // end namespace chpl

#endif
