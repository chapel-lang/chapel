/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#include "try-catch-analysis.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/ResolvedVisitor.h"
#include "chpl/resolution/BranchSensitiveVisitor.h"
#include "chpl/resolution/resolution-types.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/uast/all-uast.h"
#include "chpl/framework/compiler-configuration.h"
#include "chpl/resolution/can-pass.h"
#include "Resolver.h"


namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;


enum class ErrorCheckingMode {
  UNKNOWN,
  FATAL,
  RELAXED,
};

struct TryCatchState {
  const Try* tryNode;
  int taskFunctionDepth;
  bool catchesAreExhaustive;
};

struct TryCatchAnalyzer : BranchSensitiveVisitor<DefaultFrame, ResolvedVisitor<TryCatchAnalyzer>&> {
  using RV = ResolvedVisitor<TryCatchAnalyzer>;

  // input
  ResolutionContext* rc;
  Context* context = rc ? rc->context() : nullptr;
  const AstNode* astForErr;
  ErrorCheckingMode mode;
  // need to keep a stack of try nodes and some meta info
  std::vector<TryCatchState> tryStack;
  const Function* currentFn = nullptr;
  // output
  // TODO: figure out the output - errors when something doesn't pass?
  // maybe no output?
  std::vector<QualifiedType> returnedTypes;

  TryCatchAnalyzer(ResolutionContext* rc,
                   const AstNode* symbol,
                   ErrorCheckingMode mode)
    : rc(rc),
      astForErr(symbol),
      mode(mode) {
  }

  void checkCatchallIsLast(const Try* node, RV& rv) {
    bool hasCatchAll = false;
    for (auto handler : node->handlers()) {
      if (hasCatchAll) {
        context->error(node, "catchall placed before the end of a catch list");
      }
      if (isCatchAll(handler, rv)) {
        hasCatchAll = true;
      }
    }
  }

  static const  ClassType*
  getClassTypeForError(Context* context, const Variable* error, RV& rv) {
      auto errType = CompositeType::getErrorType(context);
      auto dec = ClassTypeDecorator(ClassTypeDecorator::MANAGED_NONNIL);
      auto manager = AnyOwnedType::get(context);
      CHPL_ASSERT(errType->basicClassType());
      return ClassType::get(context, errType->basicClassType(), manager, dec);
  }

  // Returns true if the catch is a catchall statement.
  bool isCatchAll(const Catch* node, RV& rv) {
    bool catchAll = false;
    if (auto err = node->error()) {
      // we validate that the type of error is a subtype of Error or Error
      // itself resolving the AST.
      // So here we just need to check for the base Errors.Error to know if
      // this is a catchall.
      const ResolvedExpression& r = rv.byAst(err);
      auto resType = getClassTypeForError(context, err, rv);
      auto qt = QualifiedType(QualifiedType::VAR, resType);
      auto passResult = canPass(context, r.type(), qt);
      using CPR = CanPassResult::ConversionKind;
      if (passResult.passes() && passResult.conversionKind() == CPR::NONE) {
        catchAll = true;
      }
    } else {
      // this is a catchall since it has no error associated with it
      catchAll = true;
    }
    return catchAll;
  }

  // Returns true if the catches cover all of the cases.
  bool catchesExhaustive(const Try* node, RV& rv) {
    if (node->isTryBang()) {
      return true;
    }

    bool hasCatchAll = false;
    for (auto handler : node->handlers()) {
      if (isCatchAll(handler, rv)) {
        hasCatchAll = true;
        break;
      }
    }

    return hasCatchAll;
  }

  bool canThrow() {
    return currentFn && currentFn->throws();
  }

  void enterTry(const Try* ast, RV& rv) {
    TryCatchState tca;
    tca.tryNode = ast;
    tca.catchesAreExhaustive = catchesExhaustive(ast, rv);
    tca.taskFunctionDepth = 0;
    tryStack.push_back(tca);
  }

  void exitTry() {
    tryStack.pop_back();
  }

  const types::Param* determineWhenCaseValue(const uast::AstNode* ast, RV& rv) override;
  const types::Param* determineIfValue(const uast::AstNode* ast, RV& rv) override;
  void traverseNode(const uast::AstNode* ast, RV& rv) override;

  void process(const uast::AstNode* symbol,
               ResolutionResultByPostorderID& byPostorder);

  bool enter(const Try* ast, RV& rv);
  void exit(const Try* ast, RV& rv);

  bool enter(const FnCall* fn, RV& rv);
  void exit(const FnCall* fn, RV& rv);

  // add pairs of methods for Comment, Module, TypeDecl

  bool enter(const Comment* ast, RV& rv);
  void exit(const Comment* ast, RV& rv);

  bool enter(const Module* ast, RV& rv);
  void exit(const Module* ast, RV& rv);

  bool enter(const TypeDecl* ast, RV& rv);
  void exit(const TypeDecl* ast, RV& rv);

  bool enter(const Function* fn, RV& rv);
  void exit(const Function* fn, RV& rv);

  bool enter(const Throw* ast, RV& rv);
  void exit(const Throw* ast, RV& rv);

  bool enter(const Break* brk, RV& rv);
  void exit(const Break* brk, RV& rv);

  bool enter(const Continue* cont, RV& rv);
  void exit(const Continue* cont, RV& rv);

  bool enter(const Return* ret, RV& rv);
  void exit(const Return* ret, RV& rv);

  bool enter(const Conditional* cond, RV& rv);
  void exit(const Conditional* cond, RV& rv);

  bool enter(const Select* sel, RV& rv);
  void exit(const Select* sel, RV& rv);

  bool enter(const AstNode* ast, RV& rv);
  void exit(const AstNode* ast, RV& rv);
};

} // end namespace resolution

namespace uast {
template <>
struct AstVisitorPrecondition<resolution::TryCatchAnalyzer> {
  static bool skipSubtree(const AstNode* node, resolution::TryCatchAnalyzer& rv) {
    return rv.isDoneExecuting();
  }
};

};

namespace resolution {

  const types::Param* TryCatchAnalyzer::determineWhenCaseValue(const uast::AstNode* ast, RV& rv) {
    if (auto action = rv.byAst(ast).getAction(AssociatedAction::COMPARE)) {
      return action->type().param();
    } else {
      return nullptr;
    }
  }
  const types::Param* TryCatchAnalyzer::determineIfValue(const uast::AstNode* ast, RV& rv) {
    return rv.byAst(ast).type().param();
  }
  void TryCatchAnalyzer::traverseNode(const uast::AstNode* ast, RV& rv) {
    ast->traverse(rv);
  }

  bool TryCatchAnalyzer::enter(const Try* ast, RV& rv) {
    enterScope(ast, rv);
    enterTry(ast, rv);
    return true;
  }

  void TryCatchAnalyzer::exit(const Try* ast, RV& rv) {
    checkCatchallIsLast(ast, rv);
    // is it an exhaustive catch?
    bool exhaustive = tryStack.back().catchesAreExhaustive;
    exitTry();
    exitScope(ast, rv);
    // check for a catchall, or this is a try! or the parent function throws
    if (tryStack.size() == 0 && !exhaustive && !this->canThrow()) {
      context->error(ast, "Try without a catchall in a non-throwing function");
    }
  }

  bool TryCatchAnalyzer::enter(const Function* fn, RV& rv) {
    return false;
  }
  void TryCatchAnalyzer::exit(const Function* fn, RV& rv) {
  }

  bool TryCatchAnalyzer::enter(const FnCall* ast, RV& rv) {
    // sometimes we don't have the resolved expression for the ast,
    // For now we just skip those
    // TODO: determine what to do in the cases skipped by this check
    if (rv.hasAst(ast)) {
      const ResolvedExpression& r = rv.byAst(ast);
      if (auto bestResMsc = r.mostSpecific().only()) {
        auto bestResFn = bestResMsc.fn();
        if (bestResFn->untyped()->throws()) {
          // are we in a throwing function or a try?
          if (!this->canThrow() && tryStack.size() == 0) {
            if (mode == ErrorCheckingMode::RELAXED) {
              context->error(ast, "call to throwing function '%s' without "
                                  "throws, try, or try! (relaxed mode)",
                                  bestResFn->untyped()->name().c_str());
            } else if (mode == ErrorCheckingMode::FATAL) {
              // do nothing - we'll let the program halt if it throws
            } else {
              context->error(ast, "call to throwing function from "
                                  "non-throwing function");
            }
          } else if (tryStack.size() > 0) {
            // check if any of the enclosing try blocks have a catchall
            bool foundCatchAll = false;
            for (auto it = tryStack.rbegin(); it != tryStack.rend(); it++) {
              if (it->catchesAreExhaustive) {
                foundCatchAll = true;
                break;
              }
            }
            if (!foundCatchAll && !canThrow()) {
              context->error(ast, "call to throwing function '%s' is "
                                  "in a try but not handled",
                                  bestResFn->untyped()->name().c_str());
            }
          }
        }
      }
      if (r.causedFatalError()) markFatalError();
    }
    return true;
  }
  void TryCatchAnalyzer::exit(const FnCall* ast, RV& rv) {
  }


  bool TryCatchAnalyzer::enter(const Comment* ast, RV& rv) {
    return false;
  }
  void TryCatchAnalyzer::exit(const Comment* ast, RV& rv) {
  }

  bool TryCatchAnalyzer::enter(const Module* ast, RV& rv) {
    return false;
  }

  void TryCatchAnalyzer::exit(const Module* ast, RV& rv) {
  }

  bool TryCatchAnalyzer::enter(const TypeDecl* ast, RV& rv) {
    return false;
  }

  void TryCatchAnalyzer::exit(const TypeDecl* ast, RV& rv) {
  }


  bool TryCatchAnalyzer::enter(const Throw* ast, RV& rv) {
    // check that we are in a function that is marked as throwing or inside
    // a try
    enterScope(ast, rv);
    bool canThrow = tryStack.size() > 0 || this->canThrow();
    if (!canThrow) {
      context->error(ast, "cannot throw in a non-throwing function");
    }
    return true;
  }
  void TryCatchAnalyzer::exit(const Throw* ast, RV& rv) {
    exitScope(ast, rv);
  }

  bool TryCatchAnalyzer::enter(const Break* brk, RV& rv) {
    markBreak(rv.getBreakOrContinueTarget(brk));
    return false;
  }
  void TryCatchAnalyzer::exit(const Break* brk, RV& rv) {}

  bool TryCatchAnalyzer::enter(const Continue* cont, RV& rv) {
    markContinue(rv.getBreakOrContinueTarget(cont));
    return false;
  }
  void TryCatchAnalyzer::exit(const Continue* cont, RV& rv) {}

  bool TryCatchAnalyzer::enter(const Return* ret, RV& rv) {
    return true;
  }
  void TryCatchAnalyzer::exit(const Return* ret, RV& rv) {
    markReturn();
  }

  bool TryCatchAnalyzer::enter(const Conditional* cond, RV& rv) {
    enterScope(cond, rv);
    return branchSensitivelyTraverse(cond, rv);
  }
  void TryCatchAnalyzer::exit(const Conditional* cond, RV& rv) {
    exitScope(cond, rv);
  }

  bool TryCatchAnalyzer::enter(const Select* sel, RV& rv) {
    enterScope(sel, rv);
    return branchSensitivelyTraverse(sel, rv);
  }
  void TryCatchAnalyzer::exit(const Select* sel, RV& rv) {
    exitScope(sel, rv);
  }

  bool TryCatchAnalyzer::enter(const AstNode* ast, RV& rv) {
    enterScope(ast, rv);
    return true;
  }
  void TryCatchAnalyzer::exit(const AstNode* ast, RV& rv) {
    exitScope(ast, rv);
  }

  void TryCatchAnalyzer::process(const uast::AstNode* symbol,
                                 ResolutionResultByPostorderID& byPostorder) {
    ResolvedVisitor<TryCatchAnalyzer> rv(rc, symbol, *this, byPostorder);
    // Traverse formals and then the body. This is done here rather
    // than in enter(Function) because nested functions will have
    // 'process' called on them separately.
    if (auto fn = symbol->toFunction()) {
      currentFn = fn;
      if (fn->throws() && fn->name() == UniqueString::get(context,"deinit")) {
        context->error(fn, "deinit is not permitted to throw");
      }
      if (auto body = fn->body()) {
        // traverse the formals
        for (auto formal : fn->formals()) {
          formal->traverse(rv);
        }
        // traverse the real body
        body->traverse(rv);
      }
    } else if (auto mod = symbol->toModule()) {
      // module, comment, typedecl do same thing
      // probably need a helper/adapter that we can re-use for this purpose
      for (auto stmt : mod->stmts()) {
        stmt->traverse(rv);
      }
    }
  }

  // Compute the error handling mode to use by looking for modules whose kind is
  // prototype or implicit. If neither of those apply, look for the value of the
  // PERMIT_UNHANDLED_MODULE_ERRORS compiler flag to determine the handling mode
  static ErrorCheckingMode computeErrorCheckingMode(Context* context,
                                                    const AstNode* node) {
    auto mode = ErrorCheckingMode::UNKNOWN;
    auto moduleKind = parsing::idToModuleKind(context, node->id());
    if (moduleKind == Module::Kind::PROTOTYPE ||
        moduleKind == Module::Kind::IMPLICIT) {
      mode = ErrorCheckingMode::FATAL;
    }

    if (mode == ErrorCheckingMode::UNKNOWN) {
      // No mode was chosen explicitly, see if the compiler flag sets the
      // handling mode or use the default.
      if (isCompilerFlagSet(context,
                            CompilerFlags::PERMIT_UNHANDLED_MODULE_ERRORS)) {
        mode = ErrorCheckingMode::FATAL;
      } else {
        mode = ErrorCheckingMode::RELAXED;
      }
    }
    CHPL_ASSERT(mode != ErrorCheckingMode::UNKNOWN);
    return mode;
  }

  void checkThrows(ResolutionContext* rc,
                   ResolutionResultByPostorderID& result,
                   const AstNode* symbol) {
    if (symbol->isFunction() || symbol->isModule()) {
      auto mode = computeErrorCheckingMode(rc->context(), symbol);
      auto v = TryCatchAnalyzer(rc, symbol, mode);
      v.process(symbol, result);
    }
  }


} // end namespace resolution


} // end namespace chpl
