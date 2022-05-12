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

#include "chpl/parsing/parsing-queries.h"
#include "chpl/queries/query-impl.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/uast/Call.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace parsing;
using namespace resolution;
using namespace uast;

static UniqueString nameForAst(const AstNode* ast) {
  UniqueString empty;

  if (ast == nullptr) {
    return empty;
  } else if (auto ident = ast->toIdentifier()) {
    return ident->name();
  } else if (auto decl = ast->toNamedDecl()) {
    return decl->name();
  } else if (auto call = ast->toCall()) {
    return nameForAst(call->calledExpression());
  }

  return empty;
}

static void printId(const AstNode* ast) {
  std::ostringstream ss;
  ast->id().stringify(ss, chpl::StringifyKind::DEBUG_SUMMARY);
  printf("%-16s %-8s", ss.str().c_str(), nameForAst(ast).c_str());
}

static const ResolvedExpression*
resolvedExpressionForAst(Context* context, const AstNode* ast,
                         const ResolvedFunction* inFn) {
  if (!(ast->isLoop() || ast->isBlock())) {
    // compute the parent module or function
    int postorder = ast->id().postOrderId();
    if (postorder >= 0) {
      ID parentId = ast->id().parentSymbolId(context);
      auto parentAst = idToAst(context, parentId);
      if (parentAst != nullptr) {
        if (parentAst->isModule()) {
          const auto& byId = resolveModule(context, parentAst->id());
          return &byId.byAst(ast);
        } else if (auto parentFn = parentAst->toFunction()) {
          auto untyped = UntypedFnSignature::get(context, parentFn);
          // use inFn if it matches
          if (inFn && inFn->signature()->untyped() == untyped) {
            return &inFn->resolutionById().byAst(ast);
          } else {
            auto typed = typedSignatureInitial(context, untyped);
            if (!typed->needsInstantiation()) {
              auto rFn = resolveFunction(context, typed, nullptr);
              return &rFn->resolutionById().byAst(ast);
            }
          }
        }
      }
    }
  }

  return nullptr;
}

static void
computeAndPrintStuff(Context* context,
                     const AstNode* ast,
                     const ResolvedFunction* inFn,
                     std::set<const ResolvedFunction*>& calledFns) {

  for (const AstNode* child : ast->children()) {
    computeAndPrintStuff(context, child, inFn, calledFns);
  }

  /*
  if (auto ident = ast->toIdentifier()) {
    const Scope* scope = scopeForId(context, ast->id());
    assert(scope != nullptr);

    auto name = ident->name();
    const auto& m = findInnermostDecl(context, scope, name);

    auto status = context->queryStatus(findInnermostDecl,
                                       std::make_tuple(scope, name));

    printId(ast);
    printf(" refers to: ");

    if (m.found == InnermostMatch::ZERO) {
      printf("%-32s ", "no such name found");
    } else if (m.found == InnermostMatch::ONE && m.id.isEmpty()) {
      printf("%-32s ", "builtin");
    } else if (m.found == InnermostMatch::ONE) {
      std::ostringstream ss;
      m.id.stringify(ss, chpl::StringifyKind::DEBUG_SUMMARY);
      printf("%-32s ", ss.str().c_str());
    } else {
      printf("%-32s ", "ambiguity");
    }

    if (status == Context::NOT_CHECKED_NOT_CHANGED) {
      printf("(not checked)");
    } else if (status == Context::REUSED) {
      printf("(reused)");
    } else if (status == Context::CHANGED) {
      printf("(changed)");
    }

    printf("\n");
  }*/

  int beforeCount = context->numQueriesRunThisRevision();
  const ResolvedExpression* r = resolvedExpressionForAst(context, ast, inFn);
  int afterCount = context->numQueriesRunThisRevision();
  if (r != nullptr) {
    for (const TypedFnSignature* sig : r->mostSpecific()) {
      if (sig != nullptr) {
        if (sig->untyped()->idIsFunction()) {
          auto fn = resolveFunction(context, sig, r->poiScope());
          calledFns.insert(fn);
        }
      }
    }

    printId(ast);
    std::ostringstream ss;
    r->stringify(ss, chpl::StringifyKind::CHPL_SYNTAX);
    // TODO: Surely we can format when we stringify?
    printf("%-35s ", ss.str().c_str());
    if (afterCount > beforeCount) {
      printf(" (ran %i queries)", afterCount - beforeCount);
    }
    printf("\n");
  }

  // check the type
  /*
  if (!(ast->isLoop() || ast->isBlock())) {
    const auto& t = typeForModuleLevelSymbol(context, ast->id());

    printId(ast);
    printf(" has type:  ");
    std::ostringstream ss;
    t.stringify(ss, chpl::StringifyKind::DEBUG_SUMMARY);
    printf("%-32s ", ss.str().c_str());

    auto status = context->queryStatus(typeForModuleLevelSymbol,
                                       std::make_tuple(ast->id()));

    if (status == Context::NOT_CHECKED_NOT_CHANGED) {
      printf("(not checked)");
    } else if (status == Context::REUSED) {
      printf("(reused)");
    } else if (status == Context::CHANGED) {
      printf("(changed)");
    }
    printf("\n");
  }*/
}

static void usage(int argc, char** argv) {
  printf("Usage: %s [--std] [--search path --search otherPath ...] "
         "file.chpl otherFile.chpl ...\n"
         "  --std enables the standard library\n"
         "  --searchPath adjusts the search path\n", argv[0]);
}

static void setupSearchPaths(Context* ctx, bool enableStdLib,
                             const char* chpl_home,
                             const std::vector<std::string>& cmdLinePaths) {
  if (enableStdLib) {
    setupModuleSearchPaths(ctx,
                           chpl_home,
                           false,
                           "flat",
                           false,
                           "qthreads",
                           "none",
                           "linux64-x86_64-gnu",
                           "",
                           cmdLinePaths);
  } else {
    std::vector<UniqueString> uPaths;
    for (auto p: cmdLinePaths) {
      uPaths.push_back(UniqueString::get(ctx, p));
    }
    setModuleSearchPath(ctx, uPaths);
  }
}

int main(int argc, char** argv) {

  bool gc = false;
  Context context;
  Context* ctx = &context;
  bool trace = false;
  const char* chpl_home = nullptr;
  std::vector<std::string> cmdLinePaths;
  std::vector<std::string> files;
  bool enableStdLib = false;
  for (int i = 1; i < argc; i++) {
    if (0 == strcmp(argv[i], "--std")) {
      enableStdLib = true;
    } else if (0 == strcmp(argv[i], "--search")) {
      if (i+1 >= argc) {
        usage(argc, argv);
        return 1;
      }
      cmdLinePaths.push_back(argv[i+1]);
      i++;
    } else if (0 == strcmp(argv[i], "--trace")) {
      trace = true;
    } else {
      files.push_back(argv[i]);
    }
  }

  if (enableStdLib) {
    if (const char* chpl_home_env  = getenv("CHPL_HOME")) {
      chpl_home = chpl_home_env;
      printf("CHPL_HOME is set, so setting up search paths\n");
    } else {
      printf("--std only works when CHPL_HOME is set\n");
      exit(1);
    }
  }

  if (files.size() == 0) {
    usage(argc, argv);
    return 0; // need this to return 0 for testing to be happy
  }

  while (true) {
    ctx->advanceToNextRevision(gc);

    // Run this query first to make the other output more understandable
    ctx->setDebugTraceFlag(false);
    typeForBuiltin(ctx, UniqueString::get(ctx, "int"));
    ctx->setDebugTraceFlag(trace);

    setupSearchPaths(ctx, enableStdLib, chpl_home, cmdLinePaths);

    std::set<const ResolvedFunction*> calledFns;

    for (auto p: files) {
      auto filepath = UniqueString::get(ctx, p);

      const ModuleVec& mods = parse(ctx, filepath);
      for (const auto mod : mods) {
        mod->stringify(std::cout, chpl::StringifyKind::DEBUG_DETAIL);
        printf("\n");

        computeAndPrintStuff(ctx, mod, nullptr, calledFns);
        printf("\n");
      }
    }

    printf("Instantiations:\n");

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
            auto fn = ast->toFunction();
            auto uSig = UntypedFnSignature::get(ctx, fn);
            auto initialType = typedSignatureInitial(ctx, uSig);
            printf("Instantiation of ");
            initialType->stringify(std::cout, chpl::StringifyKind::CHPL_SYNTAX);
            printf("\n");
            printf("Instantiation is ");
            sig->stringify(std::cout, chpl::StringifyKind::CHPL_SYNTAX);
            printf("\n");
            computeAndPrintStuff(ctx, ast, calledFn, calledFns);
            printf("\n");
          }
        }
      }

      size_t endCount = calledFns.size();

      if (startCount == endCount) break;
    }

    printf("Ran %i queries to compute the above\n\n",
           ctx->numQueriesRunThisRevision());

    if (gc) {
      ctx->collectGarbage();
      gc = false;
    }

    // ask the user if they want to run it again
    printf ("Would you like to incrementally parse again? [Y]: ");
    int ch = 0;
    do {
      ch = getc(stdin);
    } while (ch != 0 && (ch == ' ' || ch == '\n'));

    if (ch == 'g' || ch == 'G') {
      gc = true;
    } else if (!(ch == 'Y' || ch == 'y' || ch == '\n')) {
      break;
    }
    printf("\n");
  }

  return 0;
}
