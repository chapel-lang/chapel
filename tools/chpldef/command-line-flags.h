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

#ifndef CHPL_TOOLS_CHPLDEF_COMMAND_LINE_FLAGS_H
#define CHPL_TOOLS_CHPLDEF_COMMAND_LINE_FLAGS_H

#include "./Logger.h"
#include "./Server.h"
#include "llvm/Support/CommandLine.h"

/** This header provides a thin wrapper around LLVM's CommandLine support
    header. When you are ready to parse command-line options, call the
    'doParseOptions' function. Note that this will interpret the options
    (e.g., if '--help' was thrown, the program will exit). */
namespace chpldef {
namespace cmd {

/** Call this in 'main' to parse command-line options. Only the options
    added by this program will be shown. */
void doParseOptions(int argc, char** argv);

/** Alias for LLVM's command-line option type. */
template <typename T> using Flag = llvm::cl::opt<T>;

/** A string representing a log file name. Defaults to the empty string. */
extern Flag<std::string> logFile;

/** The log level. Defaults to 'Logger::OFF'. */
extern Flag<Logger::Level> logLevel;

/** The path to 'CHPL_HOME'. Overridden by the 'CHPL_HOME' env var. */
extern Flag<std::string> chplHome;

/** Whether or not unstable warnings should be emitted. */
extern Flag<bool> warnUnstable;

/** The GC frequency. Defaults to a server-decided value. */
extern Flag<int> garbageCollectionFrequency;

/** If the standard library should be enabled. Defaults to 'false'. */
extern Flag<bool> enableStandardLibrary;

/** Enable debug tracing for the compiler. Defaults to 'false'. */
extern Flag<bool> compilerDebugTrace;

} // end namespace 'cmd'
} // end namespace 'chpldef'

#endif
