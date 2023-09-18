/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#include "test-resolution.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/framework/query-impl.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/uast/Call.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif


using namespace chpl;
using namespace parsing;
using namespace resolution;
using namespace uast;

static void
computeAndPrintStuff(Context* context,
                     const AstNode* ast,
                     const ResolvedFunction* inFn,
                     std::set<const ResolvedFunction*>& calledFns,
                     bool scopeResolveOnly) {

  // Scope resolve / resolve concrete functions before printing
  if (auto fn = ast->toFunction()) {
    if (scopeResolveOnly) {
      inFn = scopeResolveFunction(context, fn->id());
    } else {
      auto untyped = UntypedFnSignature::get(context, fn);
      auto typed = typedSignatureInitial(context, untyped);
      if (!typed->needsInstantiation()) {
        inFn = resolveFunction(context, typed, nullptr);
      }
    }
  }

  for (const AstNode* child : ast->children()) {
    computeAndPrintStuff(context, child, inFn, calledFns, scopeResolveOnly);
  }

  const ResolvedExpression* r =
    resolvedExpressionForAst(context, ast, inFn, scopeResolveOnly);
  if (r != nullptr) {
    for (const TypedFnSignature* sig : r->mostSpecific()) {
      if (sig != nullptr) {
        if (sig->untyped()->idIsFunction()) {
          auto fn = resolveFunction(context, sig, r->poiScope());
          calledFns.insert(fn);
        }
      }
    }
  }
}

static void setupSearchPaths(Context* ctx,
                             const std::vector<std::string>& cmdLinePaths,
                             const std::vector<std::string>& files) {
  setupModuleSearchPaths(ctx, false, false, cmdLinePaths, files);
}

int main(int argc, char** argv) {
  bool brief = false;
  (void) brief;
  std::string chpl_home;
  std::vector<std::string> cmdLinePaths;
  std::vector<std::string> files;
  for (int i = 1; i < argc; i++) {
    if (0 == strcmp(argv[i], "--brief")) {
      brief = true;
    } else {
      files.push_back(argv[i]);
    }
  }

  if (const char* chpl_home_env = getenv("CHPL_HOME")) {
    chpl_home = chpl_home_env;
  } else {
    printf("CHPL_HOME must be set");
    exit(1);
  }

  Context::Configuration config;
  config.chplHome = chpl_home;
  Context context(config);
  Context* ctx = &context;
  context.setDetailedErrorOutput(!brief);

  if (files.size() == 0) {
    return 0; // need this to return 0 for testing to be happy
  }

  ctx->advanceToNextRevision(false);

  setupSearchPaths(ctx, cmdLinePaths, files);
  std::set<const ResolvedFunction*> calledFns;

  for (auto p: files) {
    auto filepath = UniqueString::get(ctx, p);

    const ModuleVec& mods = parseToplevel(ctx, filepath);
    for (const auto mod : mods) {
      computeAndPrintStuff(ctx, mod, nullptr, calledFns, false);
    }
  }

  std::set<const ResolvedFunction*> printed;

  // Gather all instantiations, transitively
  while (true) {

    std::set<const ResolvedFunction*> iterCalledFns = calledFns;
    size_t startCount = calledFns.size();

    for (auto calledFn : iterCalledFns) {
      const TypedFnSignature* sig = calledFn->signature();
      if (sig->instantiatedFrom() != nullptr) {
        calledFns.insert(calledFn);
        auto pair = printed.insert(calledFn);
        bool added = pair.second;
        if (added) {
          auto ast = idToAst(ctx, sig->id());
          computeAndPrintStuff(ctx, ast, calledFn, calledFns, false);
        }
      }
    }

    size_t endCount = calledFns.size();

    if (startCount == endCount) break;
  }

  return 0;
}
