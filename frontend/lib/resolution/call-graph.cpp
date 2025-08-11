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

#include "chpl/resolution/call-graph.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/ResolvedVisitor.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/resolution-types.h"
#include "chpl/uast/Function.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/TypeDecl.h"

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;

bool operator==(const CalledFnOrder& x, const CalledFnOrder& y) {
  return x.depth == y.depth && x.index == y.index;
}

struct CalledFnCollector {
  using RV = ResolvedVisitor<CalledFnCollector>;

  // input
  Context* context;
  const AstNode* symbol = nullptr; // Module* or Function*
  const ResolvedFunction* resolvedFunction = nullptr; // set if not module
  CalledFnOrder order;

  // output
  CalledFnsSet& called;

  CalledFnCollector(Context* context,
                    const AstNode* symbol,
                    const ResolvedFunction* resolvedFunction,
                    CalledFnOrder order,
                    CalledFnsSet& called)
    : context(context),
      symbol(symbol),
      resolvedFunction(resolvedFunction),
      order(order),
      called(called)
  {
  }

  // helper to run the visitor on something
  void process();
  // collect a single call
  void collect(const ResolvedFunction* fn);
  // collect calls from the ResolvedExpression
  void collectCalls(const ResolvedExpression* re);

  bool enter(const Module* mod, RV& rv) {
    return mod == symbol; // only proceed if it's the module requested
  }
  void exit(const Module* mod, RV& rv) {
  }

  bool enter(const TypeDecl* ast, RV& rv) {
    return false;
  }
  void exit(const TypeDecl* ast, RV& rv) {
  }

  bool enter(const Function* fn, RV& rv) {
    // only proceed if it's the function requested
    if (fn == symbol) {
      return true;
    }

    // If it's an export function, we should queue it up
    if (fn->linkage() == Function::Linkage::EXPORT) {
      collect(resolveConcreteFunction(context, fn->id()));
    }

    return false;
  }
  void exit(const Function* fn, RV& rv) {
  }

  bool enter(const AstNode* ast, RV& rv) {
    if (auto re = rv.byPostorder().byAstOrNull(ast)) {
      collectCalls(re);

      // Look for uses of module-scope variables in internal/standard modules,
      // which we will need to convert.
      //
      // TODO: this is a temporary workaround while the typed resolver is
      // still in development.
      if (ast->isIdentifier() &&
          !re->toId().isEmpty() &&
          parsing::idIsModuleScopeVar(context, re->toId()) &&
          parsing::idIsInBundledModule(context, re->toId())) {
        auto var = parsing::idToAst(context, re->toId())->toVariable();
        CHPL_ASSERT(var && var->isVariable());

        if (var->kind() != Variable::Kind::TYPE &&
            var->kind() != Variable::Kind::PARAM) {
          CalledFnOrder newOrder = {order.depth + 1, 0};
          auto v = CalledFnCollector(context, var, nullptr, newOrder, called);
          v.process();

          order.index += v.order.index;
        }
      }
    }
    return true;
  }
  void exit(const AstNode* ast, RV& rv) {
  }

  // TODO: How can we make this work through ``resolveFunction``, rather than
  // relying on the cached map in ``ResolvedFunction``? The problem appears to
  // be that we never use the 'global cache' when storing queries for
  // nested functions, so the results only live on in ``ResolvedFunction``.
  const ResolvedFunction* getResolvedFunction(const TypedFnSignature* sig,
                           const PoiScope* poiScope) {
    chpl::resolution::ResolutionContext rcval(context);
    const ResolvedFunction* fn = nullptr;
    if (resolvedFunction) {
      if (auto stored = resolvedFunction->getNestedResult(sig, poiScope)) {
        fn = stored;
      }
    }

    if (fn == nullptr) {
      fn = resolveFunction(&rcval, sig, poiScope);
    }

    return fn;
  }
};


void CalledFnCollector::process() {
  if (resolvedFunction) {
    CHPL_ASSERT(symbol && symbol->isFunction());
    // we are handling a function, so visit based on the ResolvedFunction.
    chpl::resolution::ResolutionContext rcval(context);
    const ResolutionResultByPostorderID& byPostorder =
      resolvedFunction->resolutionById();
    ResolvedVisitor<CalledFnCollector> rv(&rcval, symbol, *this, byPostorder);
    symbol->traverse(rv);
  } else if (symbol && symbol->isVariable()) {
    CHPL_ASSERT(parsing::idIsModuleScopeVar(context, symbol->id()));
    chpl::resolution::ResolutionContext rcval(context);
    const ResolutionResultByPostorderID& byPostorder =
      resolveModuleStmt(context, symbol->id());
    ResolvedVisitor<CalledFnCollector> rv(&rcval, symbol, *this, byPostorder);
    symbol->traverse(rv);
  } else {
    CHPL_ASSERT(symbol && symbol->isModule());
    chpl::resolution::ResolutionContext rcval(context);
    const ResolutionResultByPostorderID& byPostorder =
      resolveModule(context, symbol->id());
    ResolvedVisitor<CalledFnCollector> rv(&rcval, symbol, *this, byPostorder);
    symbol->traverse(rv);
  }
}

void CalledFnCollector::collect(const ResolvedFunction* fn) {
  auto pair = called.insert({fn, order});
  if (pair.second) {
    // insertion took place, so increment the index
    order.index++;
  }
}

void CalledFnCollector::collectCalls(const ResolvedExpression* re) {
  const PoiScope* poiScope = re->poiScope();

  // consider the return-intent overloads
  for (const auto& candidate : re->mostSpecific()) {
    if (const TypedFnSignature* sig = candidate.fn()) {
      if (sig->untyped()->idIsFunction()) {
        auto fn = getResolvedFunction(sig, poiScope);
        collect(fn);
      }
    }
  }

  // consider the associated actions
  for (const auto& action : re->associatedActions()) {
    // TODO: handle copy-init called by default copy-init, etc.
    // Ideally, that would work by generating uAST for them.
    if (const TypedFnSignature* sig = action.fn()) {
      if (sig->untyped()->idIsFunction()) {
        auto fn = getResolvedFunction(sig, poiScope);
        collect(fn);
      }
    }
  }
}

int gatherFnsCalledByFn(Context* context,
                        const ResolvedFunction* fn,
                        CalledFnOrder order,
                        CalledFnsSet& called) {
  const AstNode* symbol = parsing::idToAst(context, fn->id());
  CHPL_ASSERT(symbol && symbol->isFunction());
  auto v = CalledFnCollector(context, symbol, fn, order, called);
  v.process();
  return v.order.index - order.index;
}

int gatherTransitiveFnsCalledByFn(Context* context,
                                  const ResolvedFunction* fn,
                                  CalledFnOrder order,
                                  CalledFnsSet& called) {
  // gather the direct calls into a set
  CalledFnsSet directCalls;
  int directCount = gatherFnsCalledByFn(context, fn, order, directCalls);

  // Sort the direct calls by their index in the call graph, so that we can
  // more reliably iterate over them later. This helps with debugging in
  // the production compiler by making it easier to break on an AST ID.
  auto sorted = std::vector<std::pair<const ResolvedFunction*, CalledFnOrder>>(
      directCalls.begin(), directCalls.end());
  std::sort(sorted.begin(), sorted.end(), [](auto& a, auto& b) {
    return a.second.index < b.second.index;
  });

  // used for recording the order value for transitive calls
  CalledFnOrder newOrder = {order.depth + 1, directCount};

  // Now, consider each direct call. Add it to 'called' and
  // also handle it recursively, if we added it.
  for (auto kv : sorted) {
    if (kv.first == nullptr) continue;

    auto pair = called.insert(kv);
    if (pair.second) {
      // The insertion took place, so it is the first time handling this fn.
      // Visit it recursively.
      int c = gatherTransitiveFnsCalledByFn(context, /* fn */ kv.first,
                                            newOrder, called);
      // include the count for subsequent calls
      newOrder.index += c;
    }
  }
  return newOrder.index;
}

int gatherTransitiveFnsForFnId(Context* context,
                               ID fnId,
                               CalledFnsSet& called) {
  const ResolvedFunction* fn = resolveConcreteFunction(context, fnId);

  if (fn != nullptr) {
    CalledFnOrder order = {0, 0};
    auto pair = called.insert({fn, order});
    if (pair.second) {
      // insertion took place, so increment the index
      order.index++;
      // gather the transitive calls
      order.depth = 1;
      return 1 + gatherTransitiveFnsCalledByFn(context, fn, order, called);
    }
  }

  return 0;
}

int gatherFnsCalledByModInit(Context* context,
                              ID moduleId,
                              CalledFnsSet& called) {
  const AstNode* symbol = parsing::idToAst(context, moduleId);
  CHPL_ASSERT(symbol && symbol->isModule());
  CalledFnOrder order = {0, 0};
  auto v = CalledFnCollector(context, symbol,
                             /* resolvedFunction */ nullptr,
                             order, called);
  v.process();
  return v.order.index - order.index;
}

int gatherTransitiveFnsCalledByModInit(Context* context,
                                        ID moduleId,
                                        CalledFnsSet& called) {
  CalledFnsSet directCalls;
  int directCount = gatherFnsCalledByModInit(context, moduleId, directCalls);

  // used for recording the order value for transitive calls
  CalledFnOrder newOrder = {1, directCount};

  // Now, consider each direct call. Add it to 'called' and
  // also handle it recursively, if we added it.
  for (auto kv : directCalls) {
    auto pair = called.insert(kv);
    if (pair.second) {
      // The insertion took place, so it is the first time handling this fn.
      // Visit it recursively.
      int c = gatherTransitiveFnsCalledByFn(context, /* fn */ kv.first,
                                            newOrder, called);
      // include the count for subsequent calls
      newOrder.index += c;
    }
  }
  return newOrder.index;
}


} // end namespace resolution
} // end namespace chpl
