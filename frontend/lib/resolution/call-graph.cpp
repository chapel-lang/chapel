/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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
  int depth = 0;

  // output
  CalledFnsSet& called;

  CalledFnCollector(Context* context,
                    const AstNode* symbol,
                    const ResolvedFunction* resolvedFunction,
                    int depth,
                    CalledFnsSet& called)
    : context(context),
      symbol(symbol),
      resolvedFunction(resolvedFunction),
      depth(depth),
      called(called)
  {
  }

  // helper to run the visitor on something
  void process();
  // collect calls from the ResolvedExpression
  void collect(const ResolvedExpression* re);

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
      collect(re);
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
    const ResolutionResultByPostorderID& byPostorder =
      resolvedFunction->resolutionById();
    ResolvedVisitor<CalledFnCollector> rv(context, symbol, *this, byPostorder);
    symbol->traverse(rv);
  } else {
    CHPL_ASSERT(symbol && symbol->isModule());
    const ResolutionResultByPostorderID& byPostorder =
      resolveModule(context, symbol->id());
    ResolvedVisitor<CalledFnCollector> rv(context, symbol, *this, byPostorder);
    symbol->traverse(rv);
  }
}

void CalledFnCollector::collect(const ResolvedExpression* re) {
  const PoiScope* poiScope = re->poiScope();

  // consider the return-intent overloads
  for (const auto& candidate : re->mostSpecific()) {
    if (const TypedFnSignature* sig = candidate.fn()) {
      if (sig->untyped()->idIsFunction()) {
        const ResolvedFunction* fn = resolveFunction(context, sig, poiScope);
        called.insert({fn, depth});
      }
    }
  }

  // consider the associated actions
  for (const auto& action : re->associatedActions()) {
    // TODO: handle copy-init called by default copy-init, etc.
    // Ideally, that would work by generating uAST for them.
    if (const TypedFnSignature* sig = action.fn()) {
      if (sig->untyped()->idIsFunction()) {
        const ResolvedFunction* fn = resolveFunction(context, sig, poiScope);
        called.insert({fn, depth});
      }
    }
  }
}

void gatherFnsCalledByFn(Context* context,
                         const ResolvedFunction* fn,
                         int depth,
                         CalledFnsSet& called) {
  const AstNode* symbol = parsing::idToAst(context, fn->id());
  CHPL_ASSERT(symbol && symbol->isFunction());
  auto v = CalledFnCollector(context, symbol, fn, depth, called);
  v.process();
}

void gatherTransitiveFnsCalledByFn(Context* context,
                                   const ResolvedFunction* fn,
                                   int depth,
                                   CalledFnsSet& called) {
  CalledFnsSet directCalls;
  gatherFnsCalledByFn(context, fn, depth, directCalls);

  // Now, consider each direct call. Add it to 'called' and
  // also handle it recursively, if we added it.
  for (auto kv : directCalls) {
    auto pair = called.insert(kv);
    if (pair.second) {
      // The insertion took place, so it is the first time handling this fn.
      // Visit it recursively.
      gatherTransitiveFnsCalledByFn(context, /* fn */ kv.first,
                                    /* depth */ kv.second + 1, called);
    }
  }
}

void gatherFnsCalledByModInit(Context* context,
                              ID moduleId,
                              CalledFnsSet& called) {
  const AstNode* symbol = parsing::idToAst(context, moduleId);
  CHPL_ASSERT(symbol && symbol->isModule());
  auto v = CalledFnCollector(context, symbol,
                             /* resolvedFunction */ nullptr,
                             /* depth */ 0,
                             called);
  v.process();
}

void gatherTransitiveFnsCalledByModInit(Context* context,
                                        ID moduleId,
                                        CalledFnsSet& called) {
  CalledFnsSet directCalls;
  gatherFnsCalledByModInit(context, moduleId, directCalls);

  // Now, consider each direct call. Add it to 'called' and
  // also handle it recursively, if we added it.
  for (auto kv : directCalls) {
    auto pair = called.insert(kv);
    if (pair.second) {
      // The insertion took place, so it is the first time handling this fn.
      // Visit it recursively.
      gatherTransitiveFnsCalledByFn(context, /* fn */ kv.first,
                                    /* depth */ kv.second + 1, called);
    }
  }
}


} // end namespace resolution
} // end namespace chpl
