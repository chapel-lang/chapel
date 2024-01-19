/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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
#include <set>

namespace chpldef {
namespace cmd {

Flag<std::string> logFile("log-file",
  llvm::cl::desc("Specify log file path"),
  llvm::cl::value_desc("filename"));

Flag<Logger::Level> logLevel("log-level",
  llvm::cl::desc("Specify log verbosity level"),
  llvm::cl::init(Logger::OFF),
  llvm::cl::values(
    clEnumValN(Logger::MESSAGES, "messages", "Log messages only"),
    clEnumValN(Logger::VERBOSE, "verbose", "Messages and details"),
    clEnumValN(Logger::TRACE, "trace", "Full trace of execution")));

Flag<std::string> chplHome("chpl-home",
  llvm::cl::desc("Specify the location of Chapel home"),
  llvm::cl::value_desc("A string specifying a path"));

Flag<bool> warnUnstable("warn-unstable",
  llvm::cl::desc("Set to have the Chapel compiler emit unstable warnings"));

Flag<int> garbageCollectionFrequency("gc-frequency",
  llvm::cl::init(Server::DEFAULT_GC_FREQUENCY),
  llvm::cl::desc("Set the garbage collection frequency"),
  llvm::cl::value_desc("An integer specifying a revision interval"));

Flag<bool> enableStandardLibrary("enable-std",
  llvm::cl::init(false),
  llvm::cl::desc("Set to enable use of the standard library"));

Flag<bool> compilerDebugTrace("debug-trace",
  llvm::cl::init(false),
  llvm::cl::desc("Set to enable high verbosity query debug tracing"));

namespace {
static std::set<const llvm::cl::Option*> flagAddresses = {
  &logFile, &logLevel, &chplHome, &warnUnstable, &garbageCollectionFrequency,
  &enableStandardLibrary,
  &compilerDebugTrace
};

static bool isChpldefRegisteredFlag(const llvm::cl::Option* f) {
  return flagAddresses.find(f) != flagAddresses.end();
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

} // end anonymous namespace

void doParseOptions(int argc, char** argv) {
  disableNonChpldefOptions();
  llvm::cl::ParseCommandLineOptions(argc, argv);
}

} // end namespace 'cmd'
} // end namespace 'chpldef'
