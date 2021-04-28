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

#include "chpl/Frontend/FrontendQueries.h"

#include "chpl/AST/ModuleDecl.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace uast;

int main(int argc, char** argv) {

  if (argc == 1) {
    printf("Usage: %s file.chpl otherFile.chpl ...\n", argv[0]);
    return 0; // need this to return 0 for testing to be happy
  }

  auto context = Context::build();
  Context* ctx = context.get();
  while (true) {
    ctx->advanceToNextRevision(true);
    for (int i = 1; i < argc; i++) {
      auto filepath = UniqueString::build(ctx, argv[i]);
      //const frontend::ModuleDeclVec& mods
      //  = frontend::parse(ctx, filepath);

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
      }
    }
    ctx->collectGarbage();

    // ask the user if they want to run it again
    printf ("Would you like to incrementally parse again? [Y]: ");
    int ch = getc(stdin);
    if (!(ch == 'Y' || ch == 'y' || ch == '\n'))
      break;
    printf("\n");
  }

  return 0;
}
