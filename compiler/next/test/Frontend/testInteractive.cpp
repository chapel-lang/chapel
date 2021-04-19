#include "chpl/Frontend/FrontendQueries.h"

#include "chpl/AST/ModuleDecl.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace ast;

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
      //const FrontendQueries::ModuleDeclVec& mods
      //  = FrontendQueries::parse(ctx, filepath);

      const FrontendQueries::DefinedTopLevelNamesVec& vec =
        FrontendQueries::moduleLevelDeclNames(ctx, filepath);

      for (const auto& elt : vec) {
        const ast::Module* module = elt.module;
        const std::vector<UniqueString>& topLevelNames = elt.topLevelNames;

        printf("Module %s:\n", module->name().c_str());
        BaseAST::dump(module);

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
