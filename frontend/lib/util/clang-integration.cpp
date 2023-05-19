/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#include "chpl/framework/TemporaryFileResult.h"
#include "chpl/framework/query-impl.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/uast/ExternBlock.h"

#include "../util/filesystem_help.h"

#ifdef HAVE_LLVM
#include "clang/Basic/TargetInfo.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Job.h"
#include "clang/Driver/Options.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Serialization/ASTReader.h"

#include "llvm/Config/llvm-config.h"
#include "llvm/Support/TargetSelect.h"

#if LLVM_VERSION_MAJOR >= 16
#include "llvm/TargetParser/Host.h"
#else
#include "llvm/Support/Host.h"
#endif
#endif

namespace chpl {
namespace util {


const std::vector<std::string>& clangFlags(Context* context) {
  QUERY_BEGIN_INPUT(clangFlags, context);
  std::vector<std::string> ret;
  ret.push_back("clang"); // dummy argv[0] to make this callable in C++ tests
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
std::unique_ptr<clang::DiagnosticOptions>
wrapCreateAndPopulateDiagOpts(llvm::ArrayRef<const char *> Argv) {
#if LLVM_VERSION_MAJOR >= 14
  return clang::CreateAndPopulateDiagOpts(Argv);
#else
  auto diagOpts = std::make_unique<clang::DiagnosticOptions>();
  unsigned missingArgIndex, missingArgCount;
  llvm::opt::InputArgList Args =
    clang::driver::getDriverOptTable().ParseArgs(Argv.slice(1),
                                                 missingArgIndex,
                                                 missingArgCount);
  clang::ParseDiagnosticArgs(*diagOpts, Args);
  return diagOpts;
#endif
}
#endif

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

    auto diagOptions = wrapCreateAndPopulateDiagOpts(cc1argsCstrs);
    auto diagClient =
        new clang::TextDiagnosticPrinter(llvm::errs(), &*diagOptions);

    auto clangDiags =
      clang::CompilerInstance::createDiagnostics(diagOptions.release(),
                                                 diagClient,
                                                 /* owned */ true);
    Clang->setDiagnostics(&*clangDiags);

    // replace current compiler invocation with one including args and diags
    bool success = clang::CompilerInvocation::CreateFromArgs(
        Clang->getInvocation(), cc1argsCstrs, *clangDiags);
    CHPL_ASSERT(success);

    CHPL_ASSERT(Clang->getFrontendOpts().IncludeTimestamps == false);

    // create GeneratePCHAction
    clang::GeneratePCHAction* genPchAction = new clang::GeneratePCHAction();
    // run action and capture results
    if (!Clang->ExecuteAction(*genPchAction)) {
      context->error(externBlockId, "error running clang on extern block");
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

static const bool&
precompiledHeaderContainsNameQuery(Context* context,
                                   const TemporaryFileResult* pch,
                                   UniqueString name) {
  QUERY_BEGIN(precompiledHeaderContainsNameQuery, context, pch, name);

  bool result = false;

#ifdef HAVE_LLVM

  if (pch != nullptr) {
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
    auto diagOptions = wrapCreateAndPopulateDiagOpts(cc1argsCstrs);
    auto diagClient = new clang::TextDiagnosticPrinter(llvm::errs(),
                                                       &*diagOptions);
    auto clangDiags =
      clang::CompilerInstance::createDiagnostics(diagOptions.release(),
                                                 diagClient,
                                                 /* owned */ true);
    Clang->setDiagnostics(&*clangDiags);

    bool success =
      clang::CompilerInvocation::CreateFromArgs(Clang->getInvocation(),
                                                cc1argsCstrs, *clangDiags);
    CHPL_ASSERT(success);

    Clang->setTarget(clang::TargetInfo::CreateTargetInfo(Clang->getDiagnostics(), Clang->getInvocation().TargetOpts));
    Clang->createFileManager();
    Clang->createSourceManager(Clang->getFileManager());
    Clang->createPreprocessor(clang::TU_Complete);

    Clang->createASTReader();

    clang::ASTReader* astr = Clang->getASTReader().get();
    CHPL_ASSERT(astr);

    auto readResult = astr->ReadAST(pch->path(),
                                    clang::serialization::MK_PCH,
                                    clang::SourceLocation(),
                                    clang::ASTReader::ARR_None);
    if (readResult == clang::ASTReader::Success) {
      clang::IdentifierInfo* iid = astr->get(name.c_str());
      result = (iid != nullptr);
    }

    delete Clang;
  }
#endif

  return QUERY_END(result);
}

bool precompiledHeaderContainsName(Context* context,
                                   const TemporaryFileResult* pch,
                                   UniqueString name) {
  return precompiledHeaderContainsNameQuery(context, pch, name);
}


} // namespace util
} // namespace chpl
