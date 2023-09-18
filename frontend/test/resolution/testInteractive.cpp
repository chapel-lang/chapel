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
#include "chpl/framework/compiler-configuration.h"
#include "chpl/framework/query-impl.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/uast/Call.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"

#include <iomanip>

static std::string nameForAst(const AstNode* ast) {
  if (ast == nullptr) {
    return "";
  } else if (auto ident = ast->toIdentifier()) {
    return ident->name().str();
  } else if (auto decl = ast->toNamedDecl()) {
    return decl->name().str();
  } else if (auto call = ast->toCall()) {
    return nameForAst(call->calledExpression());
  }

  return "";
}

static const char* tagToString(const AstNode* ast) {
  if (ast == nullptr) {
    return "null";
  } else {
    return asttags::tagToString(ast->tag());
  }
}

static const ResolvedExpression*
resolvedExpressionForAstInteractive(Context* context, const AstNode* ast,
                         const ResolvedFunction* inFn,
                         bool scopeResolveOnly) {
  if (!(ast->isLoop() || ast->isBlock())) {
    // compute the parent module or function
    int postorder = ast->id().postOrderId();
    if (postorder >= 0) {
      ID parentId = ast->id().parentSymbolId(context);
      auto parentAst = idToAst(context, parentId);
      if (parentAst != nullptr) {
        if (parentAst->isModule()) {
          if (scopeResolveOnly) {
            const auto& byId = scopeResolveModule(context, parentAst->id());
            return byId.byAstOrNull(ast);
          } else {
            const auto& byId = resolveModule(context, parentAst->id());
            return byId.byAstOrNull(ast);
          }
        } else if (auto parentFn = parentAst->toFunction()) {
          auto untyped = UntypedFnSignature::get(context, parentFn);
          // use inFn if it matches
          if (inFn && inFn->signature()->untyped() == untyped) {
            return inFn->byAstOrNull(ast);
          } else {
            if (scopeResolveOnly) {
              auto rFn = scopeResolveFunction(context, parentFn->id());
              return rFn->byAstOrNull(ast);
            } else {
              auto typed = typedSignatureInitial(context, untyped);
              if (!typed->needsInstantiation()) {
                auto rFn = resolveFunction(context, typed, nullptr);
                return rFn->byAstOrNull(ast);
              }
            }
          }
        }
      }
    }
  }

  if (ast->id().postOrderId() < 0) {
    // It's a symbol with a different path, e.g. a nested Function.
    // Don't try to resolve it now in this
    // traversal. Instead, resolve it separately.
    return nullptr;
  }

  if (inFn != nullptr && inFn->id() != ast->id()) {
    return inFn->byAstOrNull(ast);
  }
  return nullptr;
}

static void
computeAndPrintStuff(Context* context,
                     const AstNode* ast,
                     const ResolvedFunction* inFn,
                     std::set<const ResolvedFunction*>& calledFns,
                     bool scopeResolveOnly,
                     int maxIdWidth) {
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
    computeAndPrintStuff(context, child, inFn, calledFns,
                         scopeResolveOnly, maxIdWidth);
    if (child->isModule() || child->isFunction()) {
      std::cout << "\n";
    }
  }

  int beforeCount = context->numQueriesRunThisRevision();
  const ResolvedExpression* r =
    resolvedExpressionForAstInteractive(context, ast, inFn, scopeResolveOnly);
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
    for (auto a : r->associatedActions()) {
      auto sig = a.fn();
      if (sig != nullptr) {
        if (sig->untyped()->idIsFunction()) {
          auto fn = resolveFunction(context, sig, r->poiScope());
          calledFns.insert(fn);
        }
      }
    }

    std::string idStr = ast->id().str();
    std::string tagStr = tagToString(ast);
    std::string nameStr = nameForAst(ast);

    // output the ID
    std::cout << std::setw(maxIdWidth) << std::left << idStr;
    // restore format to default
    std::cout.copyfmt(std::ios(NULL));

    // output the tag and name (if any name)
    std::string tagNameStr = " " + tagStr;
    if (!nameStr.empty()) {
      tagNameStr += " " + nameStr;
    }
    std::cout << std::setw(16) << tagNameStr << std::setw(0);

    // output the resolution result
    r->stringify(std::cout, chpl::StringifyKind::CHPL_SYNTAX);

    if (afterCount > beforeCount) {
      std::cout << " (ran " << (afterCount - beforeCount) << " queries)";
    }
    std::cout << "\n";

  }
}

static void usage(int argc, char** argv) {
  printf("Usage: %s [args] file.chpl otherFile.chpl ...\n"
         "  --std enables the standard library\n"
         "  --scope only performs scope resolution\n"
         "  --trace enables query tracing\n"
         "  --time <outputFile> outputs query timing information to outputFile\n"
         "  --searchPath <path> adds to the module search path\n"
         "  --warn-unstable turns on unstable warnings\n",
         argv[0]);
}

static void setupSearchPaths(Context* ctx, bool enableStdLib,
                             const std::vector<std::string>& cmdLinePaths,
                             const std::vector<std::string>& files) {
  if (enableStdLib) {
    setupModuleSearchPaths(ctx, false, false, cmdLinePaths, files);
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
  bool trace = false;
  bool scopeResolveOnly = false;
  bool brief = false;
  std::string chpl_home;
  std::vector<std::string> cmdLinePaths;
  std::vector<std::string> files;
  bool enableStdLib = false;
  bool warnUnstable = false;
  const char* timing = nullptr;
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
    } else if (0 == strcmp(argv[i], "--scope")) {
      scopeResolveOnly = true;
    } else if (0 == strcmp(argv[i], "--time")) {
      if (i+1 >= argc) {
        usage(argc, argv);
        return 1;
      }
      timing = argv[i+1];
      i++;
    } else if (0 == strcmp(argv[i], "--brief")) {
      brief = true;
    } else if (0 == strcmp(argv[i], "--warn-unstable")) {
      warnUnstable = true;
    } else {
      files.push_back(argv[i]);
    }
  }

  if (const char* chpl_home_env = getenv("CHPL_HOME")) {
    chpl_home = chpl_home_env;
    printf("# CHPL_HOME is set, so using it\n");
  } else {
    printf("# CHPL_HOME not set so running without one\n");
  }

  if (enableStdLib && chpl_home.empty()) {
    printf("--std only works when CHPL_HOME is set\n");
    exit(1);
  }

  Context::Configuration config;
  config.chplHome = chpl_home;
  Context context(config);
  Context* ctx = &context;
  context.setDetailedErrorOutput(!brief);

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
    if (timing) ctx->beginQueryTimingTrace(timing);

    CompilerFlags flags;
    flags.set(CompilerFlags::WARN_UNSTABLE, warnUnstable);
    setCompilerFlags(ctx, std::move(flags));

    setupSearchPaths(ctx, enableStdLib, cmdLinePaths, files);

    std::set<const ResolvedFunction*> calledFns;

    for (auto p: files) {
      auto filepath = UniqueString::get(ctx, p);

      const ModuleVec& mods = parseToplevel(ctx, filepath);
      for (const auto mod : mods) {
        mod->stringify(std::cout, chpl::StringifyKind::DEBUG_DETAIL);
        printf("\n");

        int maxIdWidth = mod->computeMaxIdStringWidth();
        computeAndPrintStuff(ctx, mod, nullptr, calledFns,
                             scopeResolveOnly, maxIdWidth);
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
            int maxIdWidth = ast->computeMaxIdStringWidth();
            computeAndPrintStuff(ctx, ast, calledFn, calledFns,
                                 scopeResolveOnly, maxIdWidth);
            printf("\n");
          }
        }
      }

      size_t endCount = calledFns.size();

      if (startCount == endCount) break;
    }

    printf("Ran %i queries to compute the above\n\n",
           ctx->numQueriesRunThisRevision());
    if (timing) ctx->endQueryTimingTrace();

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
