/*
 * Copyright 2025 Hewlett Packard Enterprise Development LP
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

#include "ErrorGuard.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/util/filesystem.h"
#include <csignal>
#include <cstdlib>
#include <llvm/Support/Path.h>
#include <llvm/Support/FileSystem.h>

static ErrorGuard* currentGuard = nullptr;
static int guardCount = 0;


// check CHPL_DEBUG_DYNO_DUMP_SYMLINK for a directory to symlink to the
// directory created by makeTempDir
static const char* symlinkTarget() {
  static const char* target = std::getenv("CHPL_DEBUG_DYNO_DUMP_SYMLINK");
  return target;
}

extern "C" {

static void errorGuardSignalHandler(int sig) {
  debuggerBreakHere();

  auto target = symlinkTarget();
  if (!target) return;

  if (currentGuard) {
    std::string path;
    if (chpl::makeTempDir("state-dump", path)) return;

    auto files = chpl::parsing::introspectParsedFiles(currentGuard->context());
    for (auto file : files) {
      // standard modules are in CHPL_HOME, don't copy them out.
      if (chpl::parsing::filePathIsInBundledModule(currentGuard->context(), file)) continue;

      // copy file into temporary directory.
      auto destPath = path + "/" + file.str();

      // create any needed subdirectories
      auto destDir = llvm::sys::path::parent_path(destPath);
      std::ignore = llvm::sys::fs::create_directories(destDir);

      auto fileText = chpl::parsing::fileText(currentGuard->context(), file);
      std::ignore = chpl::writeFile(destPath.c_str(), fileText.text());
    }

    if (std::remove(target) != 0) {
      printf("failed to remove symlink target %s\n", symlinkTarget());
    }

    std::ignore = llvm::sys::fs::create_link(path, target);

    printf("created Chapel file dump directory %s\n", path.c_str());
    printf("symlinked to %s\n", symlinkTarget());
  }
}

}

void ErrorGuard::installSignalHandler() {
  if (!symlinkTarget()) return;

  currentGuard = this;
  guardCount++;

  assert(guardCount == 1 && "ErrorGuard::installSignalHandler called more than once");
  std::signal(SIGSEGV, errorGuardSignalHandler);
  std::signal(SIGABRT, errorGuardSignalHandler);
}

void ErrorGuard::removeSignalHandler() {
  if (!symlinkTarget()) return;

  guardCount--;
  currentGuard = nullptr;

  std::signal(SIGSEGV, SIG_DFL);
  std::signal(SIGABRT, SIG_DFL);
}
