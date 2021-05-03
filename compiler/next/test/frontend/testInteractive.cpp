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

#include "chpl/frontend/frontend-queries.h"

#include "chpl/ast/ModuleDecl.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace uast;
using namespace frontend;

int main(int argc, char** argv) {

  if (argc == 1) {
    printf("Usage: %s file.chpl otherFile.chpl ...\n", argv[0]);
    return 0; // need this to return 0 for testing to be happy
  }

  bool gc = false;
  auto context = Context::build();
  Context* ctx = context.get();
  while (true) {
    ctx->advanceToNextRevision(gc);
    for (int i = 1; i < argc; i++) {
      auto filepath = UniqueString::build(ctx, argv[i]);
      //const frontend::ModuleDeclVec& mods
      //  = frontend::parse(ctx, filepath);

      // update fileText and parseFile so that
      // the later query can reuse results.
      fileText(ctx, filepath);
      //parseFile(ctx, filepath);

      /*
      const frontend::DefinedTopLevelNamesVec& vec =
        frontend::moduleLevelDeclNames(ctx, filepath);

      for (const auto& elt : vec) {
        const Module* module = elt.module;
        const std::vector<UniqueString>& topLevelNames = elt.topLevelNames;

        printf("Module %s:\n", module->name().c_str());
        ASTBase::dump(module);

        printf("Defines these toplevel names:\n");
        for (const UniqueString& name : topLevelNames) {
          printf("%s\n", name.c_str());
        }
      }*/

      const ResolvedModuleVec& rmods = resolveFile(ctx, filepath);
      for (const auto& elt : rmods) {
        const Module* module = elt.module;

        printf("Module %s:\n", module->name().c_str());
        ASTBase::dump(module);
        printf("\n");

        const ResolutionResultByPostorderID& resolution = *elt.resolution;
        for (const auto& rr : resolution) {
          if (rr.exp != nullptr && rr.decl != nullptr) {
            printf("Resolved:\n");
            ASTBase::dump(rr.exp, 2);
            printf("to:\n");
            ASTBase::dump(rr.decl, 2);
            printf("\n");
          }
        }
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
