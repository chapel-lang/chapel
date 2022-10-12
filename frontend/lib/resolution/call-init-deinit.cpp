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

#include "call-init-deinit.h"

#include "split-init.h"
#include "Resolver.h"

#include "chpl/resolution/ResolvedVisitor.h"
#include "chpl/resolution/resolution-types.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/uast/all-uast.h"

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;


// TODO -- figure out where to store copy (associatedFns?)
//         and where to store deinit (associatedFns not so good).
//         For now it just prints these.

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

  // Which variables are declared in this scope?
  std::set<const VarLikeDecl*> declaredVars;

  // Which variables are initialized in this scope
  // (possibly including outer variables)?
  std::set<const VarLikeDecl*> initedVars;

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



void CallInitDeinit::process(Resolver& resolver) {
  CallInitDeinit uv(resolver);
  MutatingResolvedVisitor<CallInitDeinit> rv(resolver.context,
                                             resolver.symbol,
                                             uv,
                                             resolver.byPostorder);

  resolver.symbol->traverse(rv);
}

CallInitDeinit::CallInitDeinit(Resolver& resolver)
  : context(resolver.context), resolver(resolver)
{
}

void CallInitDeinit::printActions(const std::vector<Action>& actions) {
  for (auto act : actions) {
    switch (act.action) {
      case Action::COPY_INIT:
        printf("copy-init %s\n", act.id.str().c_str());
        break;
      case Action::WRITE_BACK:
        printf("writeback %s\n", act.id.str().c_str());
        break;
      case Action::DEINIT:
        printf("deinit %s\n", act.id.str().c_str());
        break;
    }
  }
}

void CallInitDeinit::enterScope(const AstNode* ast) {
  if (createsScope(ast->tag())) {
    printf("ENTER SCOPE %s\n", ast->id().str().c_str());
    const Scope* scope = scopeForId(context, ast->id());
    scopeStack.push_back(ScopeFrame(scope));
  }
  /*if (auto d = ast->toDecl()) {
    declStack.push_back(d);
  }*/
}
void CallInitDeinit::exitScope(const AstNode* ast) {
  if (createsScope(ast->tag())) {
    printf("EXIT SCOPE %s\n", ast->id().str().c_str());
    ScopeFrame& frame = scopeStack.back();
    printActions(frame.endOfScopeActions);

    assert(!scopeStack.empty());
    scopeStack.pop_back();
  }
  /*if (ast->isDecl()) {
    assert(!declStack.empty());
    declStack.pop_back();
  }*/
}

bool CallInitDeinit::enter(const VarLikeDecl* ast, RV& rv) {

  enterScope(ast);

  return true;
}
void CallInitDeinit::exit(const VarLikeDecl* ast, RV& rv) {
  assert(!scopeStack.empty());
  if (!scopeStack.empty()) {
    printf("ADDING VARIABLE %s\n", ast->id().str().c_str());
    ScopeFrame& frame = scopeStack.back();
    frame.declaredVars.insert(ast);
  }

  exitScope(ast);
}

// TODO: visit nested calls & record their IDs
// in InitDeinitState to record required deinit actions

bool CallInitDeinit::enter(const Call* ast, RV& rv) {

  if (auto op = ast->toOpCall()) {
    if (op->op() == USTR("=")) {
      // TODO: determine which of these cases it really is:
      //  * assignment
      //  * copy initialization
      //  * move initialization
    }
  }

  callStack.push_back(ast);

  return true;
}
void CallInitDeinit::exit(const Call* ast, RV& rv) {
  // handle in/out/inout temporaries for nested calls by adding
  // to the call's CallFrame (which is currently at callStack.back()).
  callStack.pop_back();
}


bool CallInitDeinit::enter(const AstNode* ast, RV& rv) {
  enterScope(ast);

  printf("IN CALLINITDEINIT %s\n", ast->id().str().c_str());
  if (rv.hasAst(ast)) {
    rv.byAst(ast).dump();
    printf("\n");
  }
  return true;
}
void CallInitDeinit::exit(const AstNode* ast, RV& rv) {
  exitScope(ast);
}


void callInitDeinit(Resolver& resolver) {
  std::set<ID> splitInitedVars = computeSplitInits(resolver);
  printf("SPLIT INIT ANALYSIS RESULTS\n");
  for (auto id : splitInitedVars) {
    printf("SPLIT INITED %s\n", id.str().c_str());
  }

  CallInitDeinit::process(resolver);
}

} // end namespace resolution
} // end namespace chpl
