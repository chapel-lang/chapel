/*
 * Copyright 2026 Hewlett Packard Enterprise Development LP
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

// Flags uses of std::less on pointers to compiler AST nodes (BaseAST and its
// subclasses), including the implicit std::less in std::set<Symbol*>,
// std::map<FnSymbol*, V>, etc. Such containers must use AstIdLess instead so
// that their iteration order is deterministic (by AST id, not by address).
//
// Usage: astIdLess -p <build dir> [-j N] [files...]
// With no files, every compiler source in the compilation database is checked.

#include "clang/AST/DeclCXX.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/Type.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Lex/Lexer.h"
#include "clang/Tooling/AllTUsExecution.h"
#include "clang/Tooling/ArgumentsAdjusters.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Signals.h"
#include <mutex>
#include <set>
#include <string>

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;

static bool isAstClass(const CXXRecordDecl* rd) {
  if (!rd || !(rd = rd->getDefinition())) return false;
  if (rd->getName() == "BaseAST" &&
      rd->getDeclContext()->getRedeclContext()->isTranslationUnit())
    return true;
  for (const CXXBaseSpecifier& base : rd->bases()) {
    if (isAstClass(base.getType()->getAsCXXRecordDecl())) return true;
  }
  return false;
}

static bool isAstPointer(QualType t) {
  const auto* pt = t.getCanonicalType()->getAs<clang::PointerType>();
  return pt && isAstClass(pt->getPointeeType()->getAsCXXRecordDecl());
}

static const ClassTemplateSpecializationDecl* asSpecialization(QualType t) {
  return dyn_cast_or_null<ClassTemplateSpecializationDecl>(
      t.getCanonicalType()->getAsCXXRecordDecl());
}

static bool isStdLessOfAstPointer(QualType t) {
  const auto* spec = asSpecialization(t);
  if (!spec || !spec->isInStdNamespace() || spec->getName() != "less")
    return false;
  const TemplateArgumentList& args = spec->getTemplateArgs();
  return args.size() == 1 && args[0].getKind() == TemplateArgument::Type &&
         isAstPointer(args[0].getAsType());
}

static bool hasStdLessOfAstPointerArg(QualType t) {
  const auto* spec = asSpecialization(t);
  if (!spec) return false;
  for (const TemplateArgument& arg : spec->getTemplateArgs().asArray()) {
    if (arg.getKind() == TemplateArgument::Type &&
        isStdLessOfAstPointer(arg.getAsType()))
      return true;
  }
  return false;
}

class Reports {
 public:
  void add(std::string report) {
    std::lock_guard<std::mutex> lock(mutex_);
    reports_.insert(std::move(report));
  }
  const std::set<std::string>& get() const { return reports_; }

 private:
  std::mutex mutex_;
  std::set<std::string> reports_;
};

class StdLessCallback : public MatchFinder::MatchCallback {
 public:
  explicit StdLessCallback(Reports& reports) : reports_(reports) {}

  void run(const MatchFinder::MatchResult& result) override {
    const auto* tl = result.Nodes.getNodeAs<TypeLoc>("tl");
    QualType t = tl->getType();

    const char* problem = nullptr;
    if (isStdLessOfAstPointer(t)) {
      problem = "uses std::less on an AST pointer; use AstIdLess instead";
    } else if (hasStdLessOfAstPointerArg(t)) {
      problem = "compares AST pointers with std::less; "
                "pass AstIdLess as the comparator";
    } else {
      return;
    }

    const SourceManager& sm = *result.SourceManager;
    SourceLocation loc = sm.getExpansionLoc(tl->getBeginLoc());
    StringRef text = Lexer::getSourceText(
        CharSourceRange::getTokenRange(tl->getSourceRange()), sm,
        result.Context->getLangOpts());

    reports_.add(loc.printToString(sm) + ": " + text.str() + " " + problem);
  }

 private:
  Reports& reports_;
};

class LintAction : public ASTFrontendAction {
 public:
  explicit LintAction(Reports& reports) : callback_(reports) {}

  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance&,
                                                 StringRef) override {
    finder_.addMatcher(
        templateSpecializationTypeLoc(unless(isExpansionInSystemHeader()))
            .bind("tl"),
        &callback_);
    return finder_.newASTConsumer();
  }

 private:
  StdLessCallback callback_;
  MatchFinder finder_;
};

class LintActionFactory : public FrontendActionFactory {
 public:
  explicit LintActionFactory(Reports& reports) : reports_(reports) {}
  std::unique_ptr<FrontendAction> create() override {
    return std::make_unique<LintAction>(reports_);
  }

 private:
  Reports& reports_;
};

// Restricts a compilation database to a fixed set of files
class FilteredCompilationDatabase : public CompilationDatabase {
 public:
  FilteredCompilationDatabase(const CompilationDatabase& db,
                              std::vector<std::string> files)
      : db_(db), files_(std::move(files)) {}

  std::vector<CompileCommand>
  getCompileCommands(StringRef file) const override {
    return db_.getCompileCommands(file);
  }
  std::vector<std::string> getAllFiles() const override { return files_; }

 private:
  const CompilationDatabase& db_;
  std::vector<std::string> files_;
};

static cl::OptionCategory toolCategory("astIdLess options");
static cl::opt<std::string> buildPath(
    "p", cl::desc("Build directory containing compile_commands.json"),
    cl::Required, cl::cat(toolCategory));
static cl::opt<unsigned> jobs("j", cl::desc("Number of files to check in "
                                            "parallel (0 = all cores)"),
                              cl::init(1), cl::cat(toolCategory));
static cl::list<std::string> sourcePaths(
    cl::Positional, cl::desc("[files...] (default: all compiler sources)"),
    cl::cat(toolCategory));

int main(int argc, const char** argv) {
  llvm::sys::PrintStackTraceOnErrorSignal(argv[0]);
  cl::HideUnrelatedOptions(toolCategory);
  cl::ParseCommandLineOptions(argc, argv);

  std::string loadError;
  auto db = CompilationDatabase::loadFromDirectory(buildPath, loadError);
  if (!db) {
    llvm::errs() << loadError << "\n";
    return 1;
  }

  std::vector<std::string> files;
  if (!sourcePaths.empty()) {
    for (const std::string& file : sourcePaths)
      files.push_back(getAbsolutePath(file));
  } else {
    for (const std::string& file : db->getAllFiles()) {
      if (StringRef(file).starts_with(CHPL_COMPILER_SOURCE_DIR "/") &&
          !StringRef(file).starts_with(CHPL_LINTERS_SOURCE_DIR "/"))
        files.push_back(file);
    }
  }

  ArgumentsAdjuster adjuster =
      getInsertArgumentAdjuster("-w", ArgumentInsertPosition::END);
#ifdef CHPL_CLANG_RESOURCE_DIR
  adjuster = combineAdjusters(
      adjuster, getInsertArgumentAdjuster(
                    "-resource-dir=" CHPL_CLANG_RESOURCE_DIR,
                    ArgumentInsertPosition::END));
#endif
#ifdef CHPL_SYSROOT
  adjuster = combineAdjusters(
      adjuster, getInsertArgumentAdjuster({"-isysroot", CHPL_SYSROOT},
                                          ArgumentInsertPosition::END));
#endif

  Reports reports;
  FilteredCompilationDatabase filtered(*db, files);
  AllTUsToolExecutor executor(filtered, jobs);
  auto err = executor.execute(std::make_unique<LintActionFactory>(reports),
                              adjuster);

  for (const std::string& report : reports.get())
    llvm::outs() << report << "\n";

  if (err) {
    llvm::errs() << llvm::toString(std::move(err)) << "\n";
    return 1;
  }
  return reports.get().empty() ? 0 : 1;
}
