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
  }

  return "";
}

static void printId(const ASTNode* ast) {
  printf("%-8s %-8s", ast->id().toString().c_str(), nameForAst(ast));
}

static void computeAndPrintStuff(Context* context, const ASTNode* ast) {
  for (const ASTNode* child : ast->children()) {
    computeAndPrintStuff(context, child);
  }

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
  }

  // check the type
  if (ast->isIdentifier() || ast->isNamedDecl()) {
    const auto& t = typeForSymbol(context, ast->id());

    printId(ast);
    printf(" has type:  ");
    printf("%-32s ", t.toString().c_str());

    auto status = context->queryStatus(typeForSymbol,
                                       std::make_tuple(ast->id()));

    if (status == Context::NOT_CHECKED_NOT_CHANGED) {
      printf("(not checked)");
    } else if (status == Context::REUSED) {
      printf("(reused)");
    } else if (status == Context::CHANGED) {
      printf("(changed)");
    }
    printf("\n");
  }
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
    for (int i = 1; i < argc; i++) {
      auto filepath = UniqueString::build(ctx, argv[i]);

      const ModuleVec& mods = parse(ctx, filepath);
      for (const auto mod : mods) {
        ASTNode::dump(mod);
        printf("\n");

        computeAndPrintStuff(ctx, mod);
        printf("\n");
      }
    }
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
