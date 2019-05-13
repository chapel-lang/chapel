//===- ClangFnMapGen.cpp -----------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===--------------------------------------------------------------------===//
//
// Clang tool which creates a list of defined functions and the files in which
// they are defined.
//
//===--------------------------------------------------------------------===//

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"
#include "clang/CrossTU/CrossTranslationUnit.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Signals.h"
#include <sstream>
#include <string>

using namespace llvm;
using namespace clang;
using namespace clang::cross_tu;
using namespace clang::tooling;

static cl::OptionCategory ClangExtDefMapGenCategory("clang-extdefmapgen options");

class MapExtDefNamesConsumer : public ASTConsumer {
public:
  MapExtDefNamesConsumer(ASTContext &Context)
      : SM(Context.getSourceManager()) {}

  ~MapExtDefNamesConsumer() {
    // Flush results to standard output.
    llvm::outs() << createCrossTUIndexString(Index);
  }

  void HandleTranslationUnit(ASTContext &Ctx) override {
    handleDecl(Ctx.getTranslationUnitDecl());
  }

private:
  void handleDecl(const Decl *D);

  SourceManager &SM;
  llvm::StringMap<std::string> Index;
  std::string CurrentFileName;
};

void MapExtDefNamesConsumer::handleDecl(const Decl *D) {
  if (!D)
    return;

  if (const auto *FD = dyn_cast<FunctionDecl>(D)) {
    if (FD->isThisDeclarationADefinition()) {
      if (const Stmt *Body = FD->getBody()) {
        if (CurrentFileName.empty()) {
          CurrentFileName =
              SM.getFileEntryForID(SM.getMainFileID())->tryGetRealPathName();
          if (CurrentFileName.empty())
            CurrentFileName = "invalid_file";
        }

        switch (FD->getLinkageInternal()) {
        case ExternalLinkage:
        case VisibleNoLinkage:
        case UniqueExternalLinkage:
          if (SM.isInMainFile(Body->getBeginLoc())) {
            std::string LookupName =
                CrossTranslationUnitContext::getLookupName(FD);
            Index[LookupName] = CurrentFileName;
          }
          break;
        default:
          break;
        }
      }
    }
  }

  if (const auto *DC = dyn_cast<DeclContext>(D))
    for (const Decl *D : DC->decls())
      handleDecl(D);
}

class MapExtDefNamesAction : public ASTFrontendAction {
protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 llvm::StringRef) {
    return llvm::make_unique<MapExtDefNamesConsumer>(CI.getASTContext());
  }
};

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

int main(int argc, const char **argv) {
  // Print a stack trace if we signal out.
  sys::PrintStackTraceOnErrorSignal(argv[0], false);
  PrettyStackTraceProgram X(argc, argv);

  const char *Overview = "\nThis tool collects the USR name and location "
                         "of external definitions in the source files "
                         "(excluding headers).\n";
  CommonOptionsParser OptionsParser(argc, argv, ClangExtDefMapGenCategory,
                                    cl::ZeroOrMore, Overview);

  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  return Tool.run(newFrontendActionFactory<MapExtDefNamesAction>().get());
}
