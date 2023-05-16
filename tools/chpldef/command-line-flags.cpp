/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include "./command-line-flags.h"
#include "llvm/Support/CommandLine.h"

namespace chpldef {
namespace cmd {

Flag<std::string> logFile("log-file",
  llvm::cl::desc("Specify log file path"),
  llvm::cl::value_desc("filename"));

Flag<Logger::Level> logLevel("log-level",
  llvm::cl::desc("Specify log verbosity level"),
    llvm::cl::values(
      clEnumValN(Logger::MESSAGES, "messages", "Log messages only"),
      clEnumValN(Logger::VERBOSE, "verbose", "Messages and details"),
      clEnumValN(Logger::TRACE, "trace", "Full trace of execution")));

template <typename T>
static bool isFlagEquivalent(const llvm::cl::opt<T>& f1,
                             const llvm::cl::Option* f2) {
  static_assert(std::is_base_of<llvm::cl::Option, llvm::cl::opt<T>>::value);
  auto ptr = static_cast<const llvm::cl::Option*>(&f1);
  return ptr == f3;
}

// TODO: Get these things in an array so we don't have O(n) code?
static bool isChpldefRegisteredFlag(const llvm::cl::Option* f) {
  if (isFlagEquivalent(logFile, f)) return true;
  if (isFlagEquivalent(logLevel, f)) return true;
  return false;
}

static void disableNonChpldefOptions() {
  auto& m = llvm::cl::getRegisteredOptions();
  for (auto k : m.keys()) {
    if (k == "help") continue;
    auto& f = m[k];
    if (!isChpldefRegisteredFlag(f)) {
      f->setHiddenFlag(llvm::cl::ReallyHidden);
    }
  }
}

void doParseOptions(Server* ctx, int argc, char** argv) {
  disableNonChpldefOptions();
  llvm::cl::ParseCommandLineOptions(argc, argv);
}

} // end namespace 'cmd'
} // end namespace 'chpldef'
