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
    return fn == symbol; // only proceed if it's the function requested
  }
  void exit(const Function* fn, RV& rv) {
  }

  bool enter(const AstNode* ast, RV& rv) {
    if (auto re = rv.byPostorder().byAstOrNull(ast)) {
      collectCalls(re);
    }
    return true;
  }
  void exit(const AstNode* ast, RV& rv) {
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
        chpl::resolution::ResolutionContext rcval(context);
        const ResolvedFunction* fn = resolveFunction(&rcval, sig, poiScope);
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
        chpl::resolution::ResolutionContext rcval(context);
        const ResolvedFunction* fn = resolveFunction(&rcval, sig, poiScope);
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

  // used for recording the order value for transitive calls
  CalledFnOrder newOrder = {order.depth + 1, directCount};

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
