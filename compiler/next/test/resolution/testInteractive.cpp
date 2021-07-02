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
    const owned<std::vector<ID>>& vec = pair.second;
    for (const auto& id : *vec.get()) {
      printf("    %s\n", id.toString().c_str());
    }
  }
  for (const auto& elt : scope->usesAndImports) {
    printf("  use/import %s\n",
           elt.toString().c_str());
  }
  printf("\n");

  for (const ASTNode* child : ast->children()) {
    printAllScopes(context, child);
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

        printAllScopes(ctx, mod);
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
