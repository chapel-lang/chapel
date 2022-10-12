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

#include "Resolver.h"

#include "chpl/resolution/scope-queries.h"

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;


// TODO -- figure out where to store copy (associatedFns?)
//         and where to store deinit (associatedFns not so good).
//         For now it just prints these.

// -------- split init ----------

struct SplitInitFrame {
  const Scope* scope = nullptr;

  // Which variables are declared in this scope?
  std::set<ID> declaredVars;

  // Which variables are initialized in this scope before
  // being mentioned, throwing or returning?
  std::set<ID> initedVars;

  // Which variables are mentioned in this scope before
  // being mentioned, throwing or returning?
  std::set<ID> mentionedVars;

  // has the block already encountered a return?
  bool returns = false;

  // has the block already encountered a throw?
  bool throws = false;

  // When processing a conditional,
  // instead of popping the SplitInitFrame for the if/else Blocks,
  // store them here, for use in exit(Conditional).
  const Block* thenBlock = nullptr;
  const Block* elseBlock = nullptr;
  owned<SplitInitFrame> thenBlockFrame;
  owned<SplitInitFrame> elseBlockFrame;

  SplitInitFrame(const Scope* scope) : scope(scope) { }
};

struct FindSplitInits {
  using RV = MutatingResolvedVisitor<FindSplitInits>;

  // inputs to the process
  Context* context = nullptr;
  Resolver& resolver;

  // result of the process
  std::set<ID> splitInitedVars;

  // internal variables
  std::vector<owned<SplitInitFrame>> scopeStack;

  // main entry point to this code
  static void process(Resolver& resolver, std::set<ID>& splitInitedVars);

  FindSplitInits(Resolver& resolver);

  void handleMention(ID dstId);
  void handleInitOrAssign(ID dstId);
  void handleExitConditional(const uast::Conditional* cond);
  void handleExitScope(const uast::AstNode* ast);

  void enterScope(const uast::AstNode* ast);
  void exitScope(const uast::AstNode* ast);

  bool enter(const VarLikeDecl* ast, RV& rv);
  void exit(const VarLikeDecl* ast, RV& rv);

  bool enter(const OpCall* ast, RV& rv);
  void exit(const OpCall* ast, RV& rv);

  bool enter(const Return* ast, RV& rv);
  void exit(const Return* ast, RV& rv);

  bool enter(const Throw* ast, RV& rv);
  void exit(const Throw* ast, RV& rv);

  bool enter(const Identifier* ast, RV& rv);
  void exit(const Identifier* ast, RV& rv);

  bool enter(const uast::AstNode* node, RV& rv);
  void exit(const uast::AstNode* node, RV& rv);
};

void FindSplitInits::process(Resolver& resolver,
                             std::set<ID>& splitInitedVars) {
  FindSplitInits uv(resolver);
  MutatingResolvedVisitor<FindSplitInits> rv(resolver.context,
                                             resolver.symbol,
                                             uv,
                                             resolver.byPostorder);

  resolver.symbol->traverse(rv);

  // swap the result into place
  splitInitedVars.swap(uv.splitInitedVars);
}

FindSplitInits::FindSplitInits(Resolver& resolver)
  : context(resolver.context), resolver(resolver)
{
}

void FindSplitInits::handleMention(ID dstId) {
  if (scopeStack.empty()) return;

  bool foundReturnsThrows = false;
  bool foundInitedVar = false;
  bool foundMentionedVar = false;
  bool foundDeclaredVar = false;
  for (ssize_t i = scopeStack.size() - 1; i >= 0; i--) {
    SplitInitFrame* frame = scopeStack[i].get();
    if (frame->returns || frame->throws) {
      foundReturnsThrows = true;
    }
    if (frame->initedVars.count(dstId) > 0) {
      foundInitedVar = true;
    }
    if (frame->mentionedVars.count(dstId) > 0) {
      foundMentionedVar = true;
    }
    if (frame->declaredVars.count(dstId) > 0) {
      foundDeclaredVar = true;
      break;
    }
  }

  if (!foundDeclaredVar) return;

  // if it wasn't already initialized or mentioned, we mark it mentioned
  // in this frame.
  if (!foundReturnsThrows && !foundInitedVar && !foundMentionedVar) {
    SplitInitFrame* frame = scopeStack.back().get();
    frame->mentionedVars.insert(dstId);
  }
}

void FindSplitInits::handleInitOrAssign(ID dstId) {
  if (scopeStack.empty()) return;

  bool foundReturnsThrows = false;
  bool foundInitedVar = false;
  bool foundMentionedVar = false;
  bool foundDeclaredVar = false;
  for (ssize_t i = scopeStack.size() - 1; i >= 0; i--) {
    SplitInitFrame* frame = scopeStack[i].get();
    if (frame->returns || frame->throws) {
      foundReturnsThrows = true;
    }
    if (frame->initedVars.count(dstId) > 0) {
      foundInitedVar = true;
    }
    if (frame->mentionedVars.count(dstId) > 0) {
      foundMentionedVar = true;
    }
    if (frame->declaredVars.count(dstId) > 0) {
      foundDeclaredVar = true;
      break;
    }
  }

  if (!foundDeclaredVar) return;

  // if it wasn't already initialized or mentioned, we mark it initialized
  // in this frame.
  if (!foundReturnsThrows && !foundInitedVar && !foundMentionedVar) {
    SplitInitFrame* frame = scopeStack.back().get();
    frame->initedVars.insert(dstId);
  }
}

static bool allowsSplitInit(const uast::AstNode* ast) {
  return ast->isBlock() ||
         ast->isConditional() ||
         ast->isLocal() ||
         ast->isSerial() ||
         ast->isTry();
}

// updates the back frame with the combined result from
// the then/else blocks from the conditional
void FindSplitInits::handleExitConditional(const uast::Conditional* cond) {
  SplitInitFrame* frame = scopeStack.back().get();
  SplitInitFrame* thenFrame = frame->thenBlockFrame.get();
  SplitInitFrame* elseFrame = frame->elseBlockFrame.get();

  // save results for vars declared in then/else
  // gather the set of variables to consider
  std::set<ID> initedVars;
  for (auto id : thenFrame->initedVars) {
    if (thenFrame->declaredVars.count(id) > 0) {
      // variable declared in this scope, so save the result
      splitInitedVars.insert(id);
    } else {
      initedVars.insert(id);
    }
  }
  if (elseFrame != nullptr) {
    for (auto id : elseFrame->initedVars) {
      if (elseFrame->declaredVars.count(id) > 0) {
        // variable declared in this scope, so save the result
        splitInitedVars.insert(id);
      } else {
        initedVars.insert(id);
      }
    }
  }

  bool thenReturns = thenFrame->returns;
  bool thenThrows = thenFrame->throws;
  bool thenReturnsThrows = thenReturns || thenThrows;
  bool elseReturns = false;
  bool elseThrows = false;
  bool elseReturnsThrows = false;
  if (elseFrame != nullptr) {
    elseReturns = elseFrame->returns;
    elseThrows = elseFrame->throws;
    elseReturnsThrows = elseReturns || elseThrows;
  }

  // now consider the initialized 'vars' && propogate them
  // split init is OK if:
  //   both then & else blocks initialize it before mentioning it
  //   one initializes it and the other returns
  for (auto id : initedVars) {
    bool thenInits = thenFrame->initedVars.count(id) > 0;
    bool elseInits = false;
    if (elseFrame) {
      elseInits = elseFrame->initedVars.count(id) > 0;
    }

    if ((thenInits         && elseInits) ||
        (thenInits         && elseReturnsThrows) ||
        (thenReturnsThrows && elseInits)) {
      frame->initedVars.insert(id);
    } else {
      frame->mentionedVars.insert(id);
    }
  }
  // propagate the mentioned variables
  for (auto id : thenFrame->mentionedVars) {
    if (thenFrame->declaredVars.count(id) == 0) {
      frame->mentionedVars.insert(id);
    }
  }
  if (elseFrame != nullptr) {
    for (auto id : elseFrame->mentionedVars) {
      if (elseFrame->declaredVars.count(id) == 0) {
        frame->mentionedVars.insert(id);
      }
    }
  }
}

void FindSplitInits::handleExitScope(const uast::AstNode* ast) {
  size_t n = scopeStack.size();
  SplitInitFrame* frame = scopeStack.back().get();
  SplitInitFrame* parent = nullptr;
  if (n >= 2) {
    parent = scopeStack[n-2].get();
  }

  if (allowsSplitInit(ast)) {
    // a scope that allows split init (e.g. a regular { } block)

    // merge conditionals
    if (auto cond = ast->toConditional()) {
      // note: updates 'frame'
      handleExitConditional(cond);
    }

    // propagate initedVars and update global result
    for (auto id : frame->initedVars) {
      if (frame->declaredVars.count(id) > 0) {
        // variable declared in this scope, so save the result
        splitInitedVars.insert(id);
      } else if (parent != nullptr) {
        // variable declared in a parent scope, so update parent scope
        parent->initedVars.insert(id);
      }
    }

    if (parent != nullptr) {
      // propagate mentionedVars
      for (auto id : frame->mentionedVars) {
        if (frame->declaredVars.count(id) == 0) {
          parent->mentionedVars.insert(id);
        }
      }
      // propagate return/throw-ness
      if (frame->returns) {
        parent->returns = true;
      }
      if (frame->throws) {
        parent->throws = true;
      }
    }
  } else {
    // some kind of scope that does not allow split init
    // (e.g., a loop)
    SplitInitFrame* frame = scopeStack.back().get();
    if (parent != nullptr) {
      // propagate initedVars and mentionedVars
      // to mentionedVars in the parent
      for (auto id : frame->initedVars) {
        if (frame->declaredVars.count(id) == 0) {
          parent->mentionedVars.insert(id);
        }
      }
      for (auto id : frame->mentionedVars) {
        if (frame->declaredVars.count(id) == 0) {
          parent->mentionedVars.insert(id);
        }
      }
    }
  }
}

void FindSplitInits::enterScope(const AstNode* ast) {
  if (createsScope(ast->tag())) {
    printf("SPLIT INIT ENTER SCOPE %s\n", ast->id().str().c_str());
    const Scope* scope = scopeForId(context, ast->id());
    scopeStack.push_back(toOwned(new SplitInitFrame(scope)));
  }
  if (auto c = ast->toConditional()) {
    // note thenBlock / elseBlock
    SplitInitFrame* condFrame = scopeStack.back().get();
    condFrame->thenBlock = c->thenBlock();
    condFrame->elseBlock = c->elseBlock();
  }
  /*if (auto d = ast->toDecl()) {
    declStack.push_back(d);
  }*/
}
void FindSplitInits::exitScope(const AstNode* ast) {
  if (createsScope(ast->tag())) {
    printf("SPLIT INIT EXIT SCOPE %s\n", ast->id().str().c_str());
    assert(!scopeStack.empty());
    size_t n = scopeStack.size();
    owned<SplitInitFrame>& curFrame = scopeStack[n-1];
    SplitInitFrame* condFrame = nullptr;
    if (ast->isBlock() && n >= 2) {
      // Are we in the situation of just finishing a Block
      // that is within a Conditional?
      // If so, save it as the if or else block.
      condFrame = scopeStack[n-2].get();
    }
    if (condFrame && ast == condFrame->thenBlock) {
      // will be handled with Conditional
      condFrame->thenBlockFrame.swap(curFrame);
    } else if (condFrame && ast == condFrame->elseBlock) {
      // will be handled with Conditional
      condFrame->elseBlockFrame.swap(curFrame);
    } else {
      handleExitScope(ast);
    }
    scopeStack.pop_back();
  }
  /*if (ast->isDecl()) {
    assert(!declStack.empty());
    declStack.pop_back();
  }*/
}

bool FindSplitInits::enter(const VarLikeDecl* ast, RV& rv) {

  enterScope(ast);

  return true;
}
void FindSplitInits::exit(const VarLikeDecl* ast, RV& rv) {
  assert(!scopeStack.empty());
  if (!scopeStack.empty()) {
    printf("SPLIT INIT ADDING VARIABLE %s\n", ast->id().str().c_str());
    SplitInitFrame* frame = scopeStack.back().get();
    frame->declaredVars.insert(ast->id());
  }

  exitScope(ast);
}

bool FindSplitInits::enter(const OpCall* ast, RV& rv) {

  if (ast->op() == USTR("=")) {
    // What is the RHS and LHS of the '=' call?
    auto lhsAst = ast->actual(0);
    auto rhsAst = ast->actual(1);

    // visit the RHS first
    rhsAst->traverse(rv);

    // now consider the LHS
    ID toId;
    if (rv.hasAst(lhsAst)) {
      toId = rv.byAst(lhsAst).toId();
    }

    if (!toId.isEmpty()) {
      handleInitOrAssign(toId);
    } else {
      // visit the LHS to check for mentions
      lhsAst->traverse(rv);
    }

    // TODO: determine which of these cases it really is:
    //  * assignment
    //  * copy initialization
    //  * move initialization
  }

  return false;
}

void FindSplitInits::exit(const OpCall* ast, RV& rv) {
}

bool FindSplitInits::enter(const Return* ast, RV& rv) {
  return true;
}
void FindSplitInits::exit(const Return* ast, RV& rv) {
  if (!scopeStack.empty()) {
    SplitInitFrame* frame = scopeStack.back().get();
    frame->returns = true;
  }
}

bool FindSplitInits::enter(const Throw* ast, RV& rv) {
  return true;
}
void FindSplitInits::exit(const Throw* ast, RV& rv) {
  if (!scopeStack.empty()) {
    SplitInitFrame* frame = scopeStack.back().get();
    frame->throws = true;
  }
}

bool FindSplitInits::enter(const Identifier* ast, RV& rv) {
  return true;
}
void FindSplitInits::exit(const Identifier* ast, RV& rv) {
  if (!scopeStack.empty()) {
    ID toId;
    if (rv.hasAst(ast)) {
      toId = rv.byAst(ast).toId();
    }
    if (!toId.isEmpty()) {
      handleMention(toId);
    }
  }
}



bool FindSplitInits::enter(const AstNode* ast, RV& rv) {
  enterScope(ast);

  printf("SPLIT INIT IN %s\n", ast->id().str().c_str());
  if (rv.hasAst(ast)) {
    rv.byAst(ast).dump();
    printf("\n");
  }
  return true;
}
void FindSplitInits::exit(const AstNode* ast, RV& rv) {
  exitScope(ast);
}

// -------- copy elision and deinit ---------

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
  std::set<ID> splitInitedVars;
  FindSplitInits::process(resolver, splitInitedVars);

  printf("SPLIT INIT ANALYSIS RESULTS\n");
  for (auto id : splitInitedVars) {
    printf("SPLIT INITED %s\n", id.str().c_str());
  }

  CallInitDeinit::process(resolver);
}

} // end namespace resolution
} // end namespace chpl
