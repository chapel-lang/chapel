/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#include "chpl/util/clang-integration.h"

#include "chpl/framework/ErrorBase.h"
#include "chpl/framework/TemporaryFileResult.h"
#include "chpl/framework/query-impl.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/resolution-types.h"
#include "chpl/uast/ExternBlock.h"

#include "../util/filesystem_help.h"

#ifdef HAVE_LLVM
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Job.h"
#include "clang/Driver/Options.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/TextDiagnosticBuffer.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Serialization/ASTReader.h"

#include "llvm/Config/llvm-config.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/Casting.h"

#if LLVM_VERSION_MAJOR >= 16
#include "llvm/TargetParser/Host.h"
#else
#include "llvm/Support/Host.h"
#endif
#endif

namespace chpl {
namespace util {

using namespace types;
using namespace resolution;

const std::vector<std::string>& clangFlags(Context* context) {
  QUERY_BEGIN_INPUT(clangFlags, context);

  std::vector<std::string> ret;
  ret.push_back("clang");

  if (auto chplEnvRes = context->getChplEnv()) {
    const auto& chplEnv = chplEnvRes.get();
    // Append target compile args
    static std::string compileArgsKeys[] = {"CHPL_LLVM_CLANG_C",
                                            "CHPL_TARGET_BUNDLED_COMPILE_ARGS",
                                            "CHPL_TARGET_SYSTEM_COMPILE_ARGS"};
    for (const auto& key : compileArgsKeys) {
      if (chplEnv.find(key) != chplEnv.end()) {
        auto value = chplEnv.at(key);
        if (!value.empty()) {
          // If CHPL_LLVM_CLANG_C exists use it as the clang to invoke,
          // removing the default value we started with first.
          if (key == "CHPL_LLVM_CLANG_C") {
            ret.clear();
          }
          std::istringstream iss(value);
          std::string arg;
          while (iss >> arg) {
            ret.push_back(arg);
          }
        }
      }
    }
  }

  return QUERY_END(ret);
}

void setClangFlags(Context* context, std::vector<std::string> flags) {
  QUERY_STORE_INPUT_RESULT(clangFlags, context, flags);
}

void initializeLlvmTargets() {
#ifdef HAVE_LLVM
  static bool targetsInited = false;
  if (targetsInited == false) {
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmPrinters();
    llvm::InitializeAllAsmParsers();

    targetsInited = true;
  }
#endif
}

#ifdef HAVE_LLVM
static std::string getChplLocaleModel(Context* context) {
  std::string result = "flat";
  auto chplEnv = context->getChplEnv();
  if (chplEnv) {
    auto it = chplEnv->find("CHPL_LOCALE_MODEL");
    if (it != chplEnv->end()) {
      result = it->second;
    }
  }

  return result;
}

static bool usingGpuLocaleModel(Context* context) {
  return getChplLocaleModel(context) == "gpu";
}
#endif

const std::vector<std::string>& getCC1Arguments(Context* context,
                                                std::vector<std::string> args,
                                                bool forGpuCodegen) {
  QUERY_BEGIN(getCC1Arguments, context, args, forGpuCodegen);

  std::vector<std::string> result;

#ifdef HAVE_LLVM
  CHPL_ASSERT(args.size() > 0 && "clang to use should be arg 0");

  std::vector<const char*> argsCstrs;
  for (const auto& arg : args) {
    argsCstrs.push_back(arg.c_str());
  }


  // TODO: use a different triple when cross compiling
  // TODO: look at CHPL_TARGET_ARCH
  initializeLlvmTargets();


  // Note: considered using ci.generateCC1CommandLine /
  // ci.getCC1CommandLine but this does not produce all of the arguments
  // that the below strategy does.

  std::string triple = llvm::sys::getDefaultTargetTriple();
  // Create a compiler instance to handle the actual work.
  auto diagOptions = new clang::DiagnosticOptions();
  auto diagClient = new clang::TextDiagnosticPrinter(llvm::errs(),
                                                     &*diagOptions);
  auto diagID = new clang::DiagnosticIDs();
  auto diags = new clang::DiagnosticsEngine(diagID, &*diagOptions, diagClient);

  // takes ownership of all of the above
  clang::driver::Driver D(argsCstrs[0], triple, *diags);

  std::unique_ptr<clang::driver::Compilation> C(D.BuildCompilation(argsCstrs));

  clang::driver::Command* job = nullptr;

  if (usingGpuLocaleModel(context) == false) {
    if (!C->getJobs().empty()) {
      // Not a CPU+GPU compilation, so just use first job.
      job = &*C->getJobs().begin();
    }
  } else {
    // CPU+GPU compilation
    //  1st cc1 command is for the GPU
    //  2nd cc1 command is for the CPU
    for (auto &command : C->getJobs()) {
      bool isCC1 = false;
      for (const auto& arg : command.getArguments()) {
        if (0 == strcmp(arg, "-cc1")) {
          isCC1 = true;
          break;
        }
      }
      if (isCC1) {
        if (forGpuCodegen) {
          // For GPU, set job to 1st cc1 command
          if (job == NULL) job = &command;
        } else {
          // For CPU, set job to last cc1 command
          job = &command;
        }
      }
    }
  }

  if (job == nullptr) {
    context->error(Location(), "cannot find cc1 command from clang driver");
  } else {
    for (const char* arg : job->getArguments()) {
      result.push_back(arg);
    }
  }

  delete diags;

#endif

  return QUERY_END(result);
}

#ifdef HAVE_LLVM
// Helper method to store a diagnostic message from Clang compilation into a
// form we can use for error reporting.
static void saveClangDiagnostic(
    const std::pair<clang::SourceLocation, std::string>& diagnostic,
    std::string kind, const clang::SourceManager& sm, Context* context,
    std::vector<std::pair<Location, std::string>>& errorInfo) {
  // Use "presumed" location to report correct line number within .chpl file
  // using #line directives.
  const clang::PresumedLoc presumedLoc = sm.getPresumedLoc(diagnostic.first);
  assert(presumedLoc.isValid());
  const Location externErrorLoc =
      Location(UniqueString::get(context, presumedLoc.getFilename()),
               presumedLoc.getLine(), presumedLoc.getColumn());
  const std::string externErrorMsg = kind + ": " + diagnostic.second;
  // TODO: also output diagnostic options after message ([-Werror] etc)
  errorInfo.emplace_back(externErrorLoc, externErrorMsg);
}
#endif

/* returns the precompiled header file data
   args are the clang driver arguments
   externBlockId is the ID of the extern block containing code to precompile */
const owned<TemporaryFileResult>&
createClangPrecompiledHeader(Context* context, ID externBlockId) {
  QUERY_BEGIN(createClangPrecompiledHeader, context, externBlockId);

  owned<TemporaryFileResult> result;

#ifdef HAVE_LLVM
  bool ok = true;

  // set input and output paths
  std::string idStr = externBlockId.str();
  std::string tmpInput = context->tmpDir() + "/" + idStr + ".h";
  std::string tmpOutput = context->tmpDir() + "/" + idStr + ".ast";

  // check we are working with an extern block
  const uast::AstNode* ast = parsing::idToAst(context, externBlockId);
  const uast::ExternBlock* eb = ast ? ast->toExternBlock() : nullptr;
  if (eb == nullptr) {
    ok = false;
  }

  // put extern C code into input file
  std::error_code err;
  if (ok) {
    err = writeFile(tmpInput.c_str(), eb->code());
    if (err) {
      context->error(Location(), "Could not write to file %s: %s",
                     tmpInput.c_str(), err.message().c_str());
      ok = false;
    }
  }

  if (ok) {
    clang::CompilerInstance* Clang = new clang::CompilerInstance();

    // gather args to clang
    std::vector<std::string> clFlags = clangFlags(context);
    // disable storing timestamps in precompiled headers
    clFlags.push_back("-Xclang");
    clFlags.push_back("-fno-pch-timestamp");
    // ask to generate a precompiled header
    clFlags.push_back("-x");
    clFlags.push_back("c-header");
    // specify input
    clFlags.push_back(tmpInput);
    // specify output
    clFlags.push_back("-o");
    clFlags.push_back(tmpOutput);
    const std::vector<std::string>& cc1args =
        getCC1Arguments(context, clFlags, /* forGpuCodegen */ false);

    std::vector<const char*> cc1argsCstrs;
    for (const auto& arg : cc1args) {
      cc1argsCstrs.push_back(arg.c_str());
    }

    auto diagOptions = clang::CreateAndPopulateDiagOpts(cc1argsCstrs);
    auto diagClient = new clang::TextDiagnosticBuffer();
#if LLVM_VERSION_MAJOR >= 20
      auto clangDiags =
      clang::CompilerInstance::createDiagnostics(*llvm::vfs::getRealFileSystem(),
                                                 diagOptions.release(),
                                                 diagClient,
                                                 /* owned */ true);
#else
    auto clangDiags =
      clang::CompilerInstance::createDiagnostics(diagOptions.release(),
                                                 diagClient,
                                                 /* owned */ true);
#endif
    Clang->setDiagnostics(&*clangDiags);

    // replace current compiler invocation with one including args and diags
    bool success = clang::CompilerInvocation::CreateFromArgs(
        Clang->getInvocation(), cc1argsCstrs, *clangDiags);
    CHPL_ASSERT(success);

    CHPL_ASSERT(Clang->getFrontendOpts().IncludeTimestamps == false);

    // Disables "# errors generated" message to stderr from ExecuteAction.
    // (as well as carets for error locations, which we are excluding anyways)
    Clang->getDiagnosticOpts().ShowCarets = false;

    // create GeneratePCHAction
    clang::GeneratePCHAction* genPchAction = new clang::GeneratePCHAction();
    // run action and capture results
    if (!Clang->ExecuteAction(*genPchAction)) {
      // Report Clang errors and warnings to the Context.
      std::vector<std::pair<Location, std::string>> errorInfo;
      const clang::SourceManager& sm = Clang->getSourceManager();
      for (auto it = diagClient->err_begin(); it != diagClient->err_end();
           it++) {
        saveClangDiagnostic((*it), "error", sm, context, errorInfo);
      }
      for (auto it = diagClient->warn_begin(); it != diagClient->warn_end();
           it++) {
        saveClangDiagnostic((*it), "warning", sm, context, errorInfo);
      }
      // could also gather notes and remarks from the TextDiagnosticBuffer here
      // (using note_begin, remark_begin iterators)
      CHPL_REPORT(context, ExternCCompilation, externBlockId, errorInfo);
      ok = false;
    }

    // delete genPctAction
    delete genPchAction;

    // delete Clang instance to clean up the various components
    delete Clang;
  }

  // rename the generated file to the TemporaryFileResult path
  if (ok) {
    result = TemporaryFileResult::create(context,
                                         externBlockId.str(),
                                         ".ast");
    std::error_code err = llvm::sys::fs::rename(tmpOutput, result->path());
    if (err) {
      context->error(Location(), "Could not rename %s to %s",
                     tmpOutput.c_str(), result->path().c_str());
      ok = false;
      result = nullptr; // remove the incomplete result
    } else {
      // tell TemporaryFileResult we are done creating the file
      result->complete();
    }
  }

#endif

  return QUERY_END(result);
}

#ifdef HAVE_LLVM
static QualifiedType convertClangTypeToChapelType(
    Context* context, const clang::Type* clangType) {
  QualifiedType chapelType;

  auto clangBuiltinType = clangType->getAs<clang::BuiltinType>();
  if (clangBuiltinType) {
#define BUILTIN_TYPE_ENTRY(ClangType, ChapelCTypeString)               \
  case clang::BuiltinType::ClangType:                                  \
    chapelType =                                                       \
        resolution::typeForSysCType(context, USTR(ChapelCTypeString)); \
    break;

    switch (clangBuiltinType->getKind()) {
      case clang::BuiltinType::Void:
        chapelType = QualifiedType(QualifiedType::TYPE,
                                   VoidType::get(context));
        break;
      case clang::BuiltinType::Bool:
        chapelType =
            QualifiedType(QualifiedType::TYPE, BoolType::get(context));
        break;
      BUILTIN_TYPE_ENTRY(Int, "c_int")
      BUILTIN_TYPE_ENTRY(UInt, "c_uint")
      BUILTIN_TYPE_ENTRY(Long, "c_long")
      BUILTIN_TYPE_ENTRY(ULong, "c_ulong")
      BUILTIN_TYPE_ENTRY(LongLong, "c_longlong")
      BUILTIN_TYPE_ENTRY(ULongLong, "c_ulonglong")
      BUILTIN_TYPE_ENTRY(Char_S, "c_schar")
      BUILTIN_TYPE_ENTRY(SChar, "c_schar")
      BUILTIN_TYPE_ENTRY(Char_U, "c_uchar")
      BUILTIN_TYPE_ENTRY(UChar, "c_uchar")
      BUILTIN_TYPE_ENTRY(Short, "c_short")
      BUILTIN_TYPE_ENTRY(UShort, "c_ushort")
      case clang::BuiltinType::Float:
        chapelType =
            QualifiedType(QualifiedType::TYPE, RealType::get(context, 32));
        break;
      case clang::BuiltinType::Double:
        chapelType =
            QualifiedType(QualifiedType::TYPE, RealType::get(context, 64));
        break;
      default:
        auto policy = clang::PrintingPolicy(clang::LangOptions());
        std::string msg = "Unsupported builtin type '" +
                          clangBuiltinType->getName(policy).str() +
                          "' in extern \"C\" block";
        context->error(Location(), "%s", msg.c_str());
    }
#undef BUILTIN_TYPE_ENTRY
  } else {
    CHPL_UNIMPL("Non-builtin type in extern block");
  }

  // adjust type to value of type
  if (!chapelType.isUnknown()) {
    chapelType = QualifiedType(QualifiedType::CONST_VAR, chapelType.type());
  }

  return chapelType;
}
#endif

#ifdef HAVE_LLVM
static owned<clang::CompilerInstance> getCompilerInstanceForReadingPch(
    Context* context) {
  std::vector<std::string> clFlags = clangFlags(context);

  std::string dummyFile = context->chplHome() + "/runtime/etc/rtmain.c";
  clFlags.push_back(dummyFile);

  const std::vector<std::string>& cc1args =
    getCC1Arguments(context, clFlags, /* forGpuCodegen */ false);

  std::vector<const char*> cc1argsCstrs;
  cc1argsCstrs.push_back("clang-cc1");
  for (const auto& arg : cc1args) {
    if (arg != dummyFile) {
      cc1argsCstrs.push_back(arg.c_str());
    }
  }

  clang::CompilerInstance* Clang = new clang::CompilerInstance();
  auto diagOptions = clang::CreateAndPopulateDiagOpts(cc1argsCstrs);
  auto diagClient = new clang::TextDiagnosticPrinter(llvm::errs(),
                                                     &*diagOptions);
#if LLVM_VERSION_MAJOR >= 20
  auto clangDiags =
    clang::CompilerInstance::createDiagnostics(*llvm::vfs::getRealFileSystem(),
                                               diagOptions.release(),
                                               diagClient,
                                               /* owned */ true);
#else
  auto clangDiags =
    clang::CompilerInstance::createDiagnostics(diagOptions.release(),
                                               diagClient,
                                               /* owned */ true);
#endif
  Clang->setDiagnostics(&*clangDiags);

  bool success =
    clang::CompilerInvocation::CreateFromArgs(Clang->getInvocation(),
                                              cc1argsCstrs, *clangDiags);
  CHPL_ASSERT(success);

  Clang->setTarget(clang::TargetInfo::CreateTargetInfo(Clang->getDiagnostics(), Clang->getInvocation().TargetOpts));
  Clang->createFileManager();
  Clang->createSourceManager(Clang->getFileManager());
  Clang->createPreprocessor(clang::TU_Complete);

  return toOwned(Clang);
}
#endif

#ifdef HAVE_LLVM
static void runFuncOnIdent(Context* context, const TemporaryFileResult* pch,
                           UniqueString name,
                           const std::function<void(const clang::Decl*)>& f) {
  if (!pch) return;

  auto Clang = getCompilerInstanceForReadingPch(context);
  Clang->createASTReader();
  clang::ASTReader* astReader = Clang->getASTReader().get();
  CHPL_ASSERT(astReader);

  auto readResult =
      astReader->ReadAST(pch->path(), clang::serialization::MK_PCH,
                         clang::SourceLocation(), clang::ASTReader::ARR_None);
  if ((readResult != clang::ASTReader::Success) || !Clang->hasASTContext())
    return;
  clang::IdentifierInfo* iid = astReader->get(name.c_str());
  if (iid->hasMacroDefinition()) {
    // TODO: implement
  } else if (clang::DeclarationName declName(iid); declName.isIdentifier()) {
    auto tuDecl = Clang->getASTContext().getTranslationUnitDecl();
    auto lookupResult = tuDecl->lookup(declName);
    if (lookupResult.empty()) return;
    // Use the first declaration found, but if it is a function instead use
    // the declaration that Clang considers to have the body definition.
    auto firstDecl = lookupResult.front();
    if (auto fnDecl = llvm::dyn_cast<clang::FunctionDecl>(firstDecl)) {
      const clang::FunctionDecl* fnDeclWithBody = nullptr;
      if (fnDecl->hasBody(fnDeclWithBody)) {
        f(fnDeclWithBody);
      }
    } else {
      f(firstDecl);
    }
  }
}
#endif

const bool& precompiledHeaderContainsName(
    Context* context, const TemporaryFileResult* pch, UniqueString name) {
  QUERY_BEGIN(precompiledHeaderContainsName, context, pch, name);

  bool result = false;

#ifdef HAVE_LLVM
  if (pch) {
    auto Clang = getCompilerInstanceForReadingPch(context);
    Clang->createASTReader();
    clang::ASTReader* astReader = Clang->getASTReader().get();
    CHPL_ASSERT(astReader);

    auto readResult =
        astReader->ReadAST(pch->path(), clang::serialization::MK_PCH,
                           clang::SourceLocation(), clang::ASTReader::ARR_None);
    if (readResult == clang::ASTReader::Success) {
      clang::IdentifierInfo* iid = astReader->get(name.c_str());
      result = (iid != nullptr);
    }
  }
#endif

  return QUERY_END(result);
}

const bool& precompiledHeaderContainsFunction(
    Context* context, const TemporaryFileResult* pch, UniqueString name) {
  QUERY_BEGIN(precompiledHeaderContainsFunction, context, pch, name);

  bool result = false;

#ifdef HAVE_LLVM
  runFuncOnIdent(context, pch, name, [&](const clang::Decl* decl) {
    result = llvm::isa<clang::FunctionDecl>(decl);
  });
#endif

  return QUERY_END(result);
}

const QualifiedType& precompiledHeaderTypeForSymbol(
    Context* context, const TemporaryFileResult* pch, UniqueString name) {
  QUERY_BEGIN(precompiledHeaderTypeForSymbol, context, pch, name);

  QualifiedType result;

#ifdef HAVE_LLVM
  runFuncOnIdent(context, pch, name, [&](const clang::Decl* decl) {
    if (llvm::isa<clang::FunctionDecl>(decl)) {
      result = QualifiedType(QualifiedType::FUNCTION, nullptr);
    } else if (auto varDecl = llvm::dyn_cast<clang::VarDecl>(decl)) {
      if (auto typePtr = varDecl->getType().getTypePtrOrNull()) {
        result = convertClangTypeToChapelType(context, typePtr);
      }
    }
  });
#endif

  return QUERY_END(result);
}

const TypedFnSignature* const& precompiledHeaderSigForFn(
    Context* context, const TemporaryFileResult* pch, ID fnId) {
  QUERY_BEGIN(precompiledHeaderSigForFn, context, pch, fnId);

  const TypedFnSignature* result = nullptr;

#ifdef HAVE_LLVM
  auto name = fnId.symbolName(context);
  runFuncOnIdent(context, pch, name, [&](const clang::Decl* decl) {
    if (auto fnDecl = llvm::dyn_cast<clang::FunctionDecl>(decl)) {
      std::vector<UntypedFnSignature::FormalDetail> formals;
      std::vector<types::QualifiedType> formalTypes;
      for (auto clangFormal : fnDecl->parameters()) {
        auto formalName = UniqueString::get(context, clangFormal->getName());
        formals.emplace_back(formalName, UntypedFnSignature::DK_NO_DEFAULT,
                             /* decl */ nullptr);

        const clang::Type* formalClangType =
            clangFormal->getType().getTypePtr();
        auto formalChplType =
            convertClangTypeToChapelType(context, formalClangType);
        auto intent = formalClangType->isArrayType() ? QualifiedType::REF
                                                     : QualifiedType::IN;
        formalChplType = QualifiedType(intent, formalChplType.type());
        formalTypes.push_back(formalChplType);
      }

      const UntypedFnSignature* untypedSig = UntypedFnSignature::get(
          context, fnId, name,
          /* isMethod */ false,
          /* isTypeConstructor */ false,
          /* isCompilerGenerated */ true,
          /* throws */ false,
          /* idTag */ uast::asttags::Function,
          /* kind */ uast::Function::Kind::PROC, std::move(formals),
          /* whereClause */ nullptr,
          /* compilerGeneratedOrigin */ fnId);

      result = TypedFnSignature::get(
          context, untypedSig, std::move(formalTypes),
          /* whereClauseResult */ TypedFnSignature::WHERE_NONE,
          /* needsInstantiation */ false,
          /* instantiatedFrom */ nullptr,
          /* parentFn */ nullptr,
          /* formalsInstantiated */ Bitmap(),
          /* outerVariables */ OuterVariables());
    }
  });
#endif

  return QUERY_END(result);
}

const QualifiedType& precompiledHeaderRetTypeForFn(
    Context* context, const TemporaryFileResult* pch, UniqueString name) {
  QUERY_BEGIN(precompiledHeaderRetTypeForFn, context, pch, name);

  QualifiedType result;

#ifdef HAVE_LLVM
  runFuncOnIdent(context, pch, name, [&](const clang::Decl* decl) {
    if (auto fnDecl = llvm::dyn_cast<clang::FunctionDecl>(decl)) {
      const clang::Type* clangReturnType = fnDecl->getReturnType().getTypePtr();
      auto chplReturnType =
          convertClangTypeToChapelType(context, clangReturnType);
      result = chplReturnType;
    }
  });
#endif

  return QUERY_END(result);
}

} // namespace util
} // namespace chpl
