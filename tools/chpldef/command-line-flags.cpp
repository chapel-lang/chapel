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

namespace chpldef {
namespace cmd {

llvm::cl::opt<std::string> logFile("log-file",
  llvm::cl::desc("Specify log file path"),
  llvm::cl::value_desc("filename"));

llvm::cl::opt<Logger::Level> logLevel(
  llvm::cl::desc("Specify log verbosity level"),
    llvm::cl::values(
      clEnumValN(Logger::OFF, "off", "No logging output"),
      clEnumValN(Logger::MESSAGES, "messages", "Log messages only"),
      clEnumValN(Logger::VERBOSE, "verbose", "Messages and details"),
      clEnumValN(Logger::TRACE, "trace", "Full trace of execution")));

} // end namespace 'cmd'
} // end namespace 'chpldef'
