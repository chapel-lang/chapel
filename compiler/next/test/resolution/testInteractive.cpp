/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

static const char* nameForAst(const ASTNode* ast) {
  if (auto ident = ast->toIdentifier()) {
    return ident->name().c_str();
  } else if (auto decl = ast->toNamedDecl()) {
    return decl->name().c_str();
  } else if (auto call = ast->toCall()) {
    return nameForAst(call->calledExpression());
  }

  return "";
}

static void printId(const ASTNode* ast) {
  printf("%-16s %-8s", ast->id().toString().c_str(), nameForAst(ast));
}

static const ResolvedExpression*
resolvedExpressionForAst(Context* context, const ASTNode* ast,
                         const TypedFnSignature* inFn) {
  if (!(ast->isLoop() || ast->isBlock())) {
    // compute the parent module or function
    int postorder = ast->id().postOrderId();
    if (postorder >= 0) {
      ID parentId = ast->id().parentSymbolId(context);
      auto parentAst = idToAst(context, parentId);
      if (parentAst != nullptr) {
        if (parentAst->isModule()) {
          const auto& byId = resolveModule(context, parentAst->id());
          assert(0 <= postorder && postorder < byId.size());
          return &byId[postorder];
        } else if (parentAst->isFunction()) {
          auto untyped = untypedSignature(context, parentAst->id());
          auto typed = typedSignatureInitial(context, untyped);
          // use inFn if it matches
          if (inFn && inFn->untypedSignature == untyped) {
            typed = inFn;
          }
          if (!typed->needsInstantiation) {
            auto rFn = resolvedFunction(context, typed, typed->poiInfo.poiScope);
            assert(0 <= postorder && postorder < rFn->resolutionById.size());
            return &rFn->resolutionById[postorder];
          }
        }
      }
    }
  }

  return nullptr;
}

static void
computeAndPrintStuff(Context* context,
                     const ASTNode* ast,
                     const TypedFnSignature* inFn,
                     std::set<const TypedFnSignature*>& calledFns) {

  for (const ASTNode* child : ast->children()) {
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
      printf("%-32s ", m.id.toString().c_str());
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
    if (r->mostSpecific.bestRef)
      calledFns.insert(r->mostSpecific.bestRef);
    if (r->mostSpecific.bestConstRef)
      calledFns.insert(r->mostSpecific.bestConstRef);
    if (r->mostSpecific.bestValue)
      calledFns.insert(r->mostSpecific.bestValue);

    printId(ast);
    printf("%-35s ", r->toString().c_str());
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
    printf("%-32s ", t.toString().c_str());

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

int main(int argc, char** argv) {

  if (argc == 1) {
    printf("Usage: %s file.chpl otherFile.chpl ...\n", argv[0]);
    return 0; // need this to return 0 for testing to be happy
  }

  bool gc = false;
  Context context;
  Context* ctx = &context;

  while (true) {
    ctx->advanceToNextRevision(gc);

    std::set<const TypedFnSignature*> calledFns;

    for (int i = 1; i < argc; i++) {
      auto filepath = UniqueString::build(ctx, argv[i]);

      const ModuleVec& mods = parse(ctx, filepath);
      for (const auto mod : mods) {
        ASTNode::dump(mod);
        printf("\n");

        computeAndPrintStuff(ctx, mod, nullptr, calledFns);
        printf("\n");
      }
    }

    printf("Instantiations:\n");

    std::set<const TypedFnSignature*> printed;

    // Gather all instantiations, transitively
    while (true) {

      std::set<const TypedFnSignature*> iterCalledFns = calledFns;
      size_t startCount = calledFns.size();

      for (auto calledFn : iterCalledFns) {
        if (calledFn->instantiatedFrom != nullptr) {
          calledFns.insert(calledFn);
          auto pair = printed.insert(calledFn);
          bool added = pair.second;
          if (added) {
            auto ast = idToAst(ctx, calledFn->untypedSignature->functionId);
            auto uSig = untypedSignature(ctx, ast->id());
            auto initialType = typedSignatureInitial(ctx, uSig);
            printf("Instantiation of %s\n", initialType->toString().c_str());
            printf("Instantiation is %s\n", calledFn->toString().c_str());
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
