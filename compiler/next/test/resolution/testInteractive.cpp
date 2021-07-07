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
#include "chpl/resolution/resolution-queries.h"
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

/*
static void printAllScopes(Context* context, const ASTNode* ast) {
  // ignore comments
  if (ast->id().isEmpty())
    return;

  printf("\n");
  printf("computing scope for id %s ast %p\n",
         ast->id().toString().c_str(),
         ast);

  const Scope* scope = scopeForId(context, ast->id());
  assert(scope != nullptr);

  printf("\n");
  printf("%s -> scope ptr %p -- parent scope ptr %p\n",
         ast->id().toString().c_str(), scope, scope->parentScope);
  printf("  scope id %s -- parent scope id %s\n",
         scope->id.toString().c_str(),
        scope->parentScope->id.toString().c_str());
  for (const auto& pair : scope->declared) {
    printf("  declarations with name %s:\n", pair.first.c_str());
    const OwnedIdsWithName& ids = pair.second;
    if (ids.moreIds.get() != nullptr) {
      for (const auto& id : *ids.moreIds.get()) {
        printf("    %s\n", id.toString().c_str());
      }
    } else {
      printf("    %s\n", ids.id.toString().c_str());
    }
  }
  if (scope->containsUseImport) {
    printf("  contains use/import\n");
  }
  printf("\n");

  for (const ASTNode* child : ast->children()) {
    printAllScopes(context, child);
  }
}
*/

static void findInnermostDecls(Context* context, const ASTNode* ast) {
  if (auto ident = ast->toIdentifier()) {
    printf("%s %s refers to: ",
           ident->id().toString().c_str(),
           ident->name().c_str());

    const Scope* scope = scopeForId(context, ast->id());
    assert(scope != nullptr);

    const auto& pair = findInnermostDecl(context, scope, ident->name());
    if (pair.second == 0) {
      printf("no such name found\n");
    } else if (pair.second == 1) {
      printf("%s\n", pair.first.toString().c_str());
    } else {
      printf("ambiguity\n");
    }
  }

  for (const ASTNode* child : ast->children()) {
    findInnermostDecls(context, child);
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

        //printAllScopes(ctx, mod);
        //printf("\n");

        findInnermostDecls(ctx, mod);
        printf("\n");
      }

      /*
      const DefinedTopLevelNamesVec& vec = moduleLevelDeclNames(ctx, filepath);

      for (const auto& elt : vec) {
        const Module* module = elt.module;
        const std::vector<UniqueString>& topLevelNames = elt.topLevelNames;

        printf("Module %s:\n", module->name().c_str());
        ASTNode::dump(module);

        printf("Defines these toplevel names:\n");
        for (const UniqueString& name : topLevelNames) {
          printf("%s\n", name.c_str());
        }
      }*/

      /*
      const ResolvedSymbolVec& rmods = resolveFile(ctx, filepath);
      for (const auto& elt : rmods) {
        const Module* module = elt->decl->toModule();

        printf("Module %s:\n", module->name().c_str());
        ASTNode::dump(module);
        printf("\n");

        const ResolutionResultByPostorderID& resolution = elt->resolutionById;
        for (const auto& rr : resolution) {
          if (rr.expr != nullptr && rr.decl != nullptr) {
            printf("Resolved:\n");
            ASTNode::dump(rr.expr, 2);
            printf("to:\n");
            ASTNode::dump(rr.decl, 2);
            printf("\n");
          }
        }
      }*/
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
