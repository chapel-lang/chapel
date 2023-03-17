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

#include "chpl/framework/query-impl.h"

#include "../util/filesystem_help.h"

#include "clang/Driver/Compilation.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Job.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"

#include "llvm/Support/Host.h"
// TODO: in future LLVM it is llvm/TargetParser/Host.h
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/VirtualFileSystem.h"


namespace chpl {
namespace util {


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

// Get the current clang executable path from printchplenv
static std::string getClangExe(Context* context) {
  std::string clangExe = "clang";
  auto chplEnv = context->getChplEnv();
  if (chplEnv) {
    auto it = chplEnv->find("CHPL_LLVM_CLANG_C");
    if (it != chplEnv->end()) {
      clangExe = it->second;
    }
  }
  return clangExe;
}

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

const std::vector<std::string>& getCC1Arguments(Context* context,
                                                std::vector<std::string> args,
                                                bool forGpuCodegen) {
  QUERY_BEGIN(getCC1Arguments, context, args, forGpuCodegen);

  std::vector<std::string> result;

#ifdef HAVE_LLVM
  std::string clangExe = getClangExe(context);
  std::vector<const char*> argsCstrs;

  argsCstrs.push_back(clangExe.c_str());
  for (const auto& arg : args) {
    argsCstrs.push_back(arg.c_str());
  }


  // TODO: use a different triple when cross compiling
  // TODO: look at CHPL_TARGET_ARCH
  std::string triple = llvm::sys::getDefaultTargetTriple();

  // Create a compiler instance to handle the actual work.
  auto diagOptions = new clang::DiagnosticOptions();
  auto diagClient = new clang::TextDiagnosticPrinter(llvm::errs(),
                                                     &*diagOptions);
  auto diagID = new clang::DiagnosticIDs();
  auto diags = new clang::DiagnosticsEngine(diagID, &*diagOptions, diagClient);

  // takes ownership of all of the above
  clang::driver::Driver D(clangExe, triple, *diags);

  std::unique_ptr<clang::driver::Compilation> C(D.BuildCompilation(argsCstrs));

  clang::driver::Command* job = nullptr;

  if (usingGpuLocaleModel(context) == false) {
    // Not a CPU+GPU compilation, so just use first job.
    job = &*C->getJobs().begin();
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

#endif

  return QUERY_END(result);
}

/* returns the precompiled header file data
   args are the clang driver arguments
   code is the C code to precompile */
const std::string& createClangPrecompiledHeader(Context* context,
                                                std::vector<std::string> args,
                                                std::string code) {
  QUERY_BEGIN(createClangPrecompiledHeader, context, args, code);

  std::string result;

  bool ok = true;

  std::string clangExe = getClangExe(context);
  std::string tmpInput = context->tmpDir() + "/extern-code.h";
  std::string tmpOutput = context->tmpDir() + "/extern-code.ast";

  // write the code to the file
  std::string openError;
  FILE* fp = openfile(tmpInput.c_str(), "w", openError);
  if (fp == nullptr) {
    context->error(Location(), "Could not open file %s: %s",
                   tmpInput.c_str(), openError.c_str());
    ok = false;
  }

  if (ok) {
    size_t written = fwrite(code.data(), 1, code.size(), fp);

    if (written != code.size()) {
      context->error(Location(), "Could not write to file %s",
                     tmpInput.c_str());
      ok = false;
    }
  }

  if (fp != nullptr) {
    std::string closeError;
    bool closedOk = closefile(fp, tmpInput.c_str(), closeError);
    if (!closedOk) {
      context->error(Location(), "Could not close file %s: %s",
                     tmpInput.c_str(), closeError.c_str());
      ok = false;
    }
  }

  if (ok) {
    // run clang
    std::vector<std::string> command;

    command.push_back(clangExe);
    command.push_back("-x");
    command.push_back("c-header");
    command.push_back(tmpInput);
    command.push_back("-o");
    command.push_back(tmpOutput);
    const char* desc = "create clang precompiled header for extern block";
    int code = executeAndWait(command, desc);

    if (code != 0) {
      std::string cmd;
      for (auto& arg : command) {
        cmd.append(arg);
        cmd.append(" ");
      }
      context->error(Location(), "Could not run clang command %s", cmd.c_str());
      ok = false;
    }
  }

  if (ok) {
    std::string readError;
    bool readOk = readfile(tmpOutput.c_str(), result, readError);
    if (!readOk) {
      context->error(Location(), "Could not read file %s: %s",
                     tmpOutput.c_str(), readError.c_str());
    }
  }

  return QUERY_END(result);
}


} // namespace util
} // namespace chpl
