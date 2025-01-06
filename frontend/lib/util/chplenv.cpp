/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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
#include "chpl/util/chplenv.h"

#include "chpl/util/subprocess.h"
#include "chpl/util/filesystem.h"

#include "llvm/Support/FileSystem.h"

#include "chpl/util/version-info.h"

namespace chpl {

static void parseChplEnv(std::string& output, ChplEnvMap& into) {
  // Lines
  std::string line= "";
  std::string lineDelimiter = "\n";
  size_t linePos = 0;        // Line break position

  // Tokens
  std::string tokenDelimiter = "=";
  size_t delimiterPos = 0;    // Position of delimiter
  size_t valuePos = 0;        // Position of value

  std::string key = "";
  std::string value = "";

  while ((linePos = output.find(lineDelimiter)) != std::string::npos)
  {
    line = output.substr(0, linePos);

    // Key is substring up until "=" on a given line
    delimiterPos = line.find(tokenDelimiter);
    key = line.substr(0, delimiterPos);

    // Value is substring after "=" on a given line
    valuePos = delimiterPos + tokenDelimiter.length();
    value = line.substr(valuePos);

    into[key] = value;
    output.erase(0, linePos + lineDelimiter.length());
  }
}

// Get results of printchplenv script, passing currently known CHPL_vars as well
template <typename InputMap>
llvm::ErrorOr<ChplEnvMap> getChplEnvImpl(const InputMap& varMap,
                                         const char* chplHome,
                                         std::string* printchplenvOutput) {
  ChplEnvMap result;

  if (printchplenvOutput && !printchplenvOutput->empty()) {
    // Just re-use passed-in command output
    parseChplEnv(*printchplenvOutput, result);
  } else {
    // Construct and run printchplenv command
    std::string command;
    // Pass known variables in varMap into printchplenv by prepending to command
    for (auto& ii : varMap) {
      command += ii.first + "=" + ii.second + " ";
    }
    command += "CHPLENV_SKIP_HOST=true ";
    command += "CHPLENV_SUPPRESS_WARNINGS=true ";
    command += std::string(chplHome) +
               "/util/printchplenv --all --internal --no-tidy --simple";

    // Run command
    auto commandOutput = getCommandOutput(command);
    if (!commandOutput) {
      // forward error code
      return commandOutput.getError();
    }

    // Save copy of command output if out-parameter was supplied
    if (printchplenvOutput) {
      assert(printchplenvOutput->empty());
      // This is intentionally copied since parseChplEnv destroys the input
      *printchplenvOutput = commandOutput.get();
    }

    parseChplEnv(commandOutput.get(), result);
  }

  return result;
}

llvm::ErrorOr<ChplEnvMap>
getChplEnv(const std::map<std::string, const char*>& varMap,
           const char* chplHome, std::string* printchplenvOutput) {
  return getChplEnvImpl(varMap, chplHome, printchplenvOutput);
}

llvm::ErrorOr<ChplEnvMap>
getChplEnv(const std::unordered_map<std::string, std::string>& varMap,
           const char* chplHome, std::string* printchplenvOutput) {
  return getChplEnvImpl(varMap, chplHome, printchplenvOutput);
}

bool isMaybeChplHome(std::string path) {
  // assume if we find subfolder util/ containing chplenv, we might be home
  path += "/util/chplenv";
  return llvm::sys::fs::exists(path);
}

std::error_code findChplHome(const char* argv0, void* mainAddr,
                             std::string& chplHomeOut,
                             bool& installed, bool& fromEnv,
                             std::string& diagnosticMessage) {
  std::string versionString = getMajorMinorVersion();
  std::string guessFromBinaryPath = getExecutablePath(argv0, mainAddr);
  chplHomeOut = std::string();

  const char* chplHomeEnv = getenv("CHPL_HOME");

  // First, Try figuring CHPL_HOME out from the binary's location.
  // If we're running from /path/to/folder/bin/darwin/chpl,
  // CHPL_HOME might be /path/to/folder.
  if (!guessFromBinaryPath.empty()) {
    // truncate path at /bin
    auto binIdx = guessFromBinaryPath.rfind("/bin");
    if (binIdx != std::string::npos) {
      guessFromBinaryPath.resize(binIdx);
    }

    if (isMaybeChplHome(guessFromBinaryPath)) {
      chplHomeOut = guessFromBinaryPath;
    } else {
      guessFromBinaryPath.clear();
    }
  }

  // Compute a predefined location based on the prefix. If we find that
  // the CHPL_HOME lies in this location, we can reason that this is
  // a prefix-based installation, and install should be true.
  //
  // Check for Chapel libraries at installed prefix
  // e.g. /usr/share/chapel/<vers>
  std::string guessFromPrefix = std::string()
    + getConfiguredPrefix() + "/"
    + "share/chapel/"
    + versionString;
  if (!isMaybeChplHome(guessFromPrefix)) {
    guessFromPrefix.clear();
  }

  if (chplHomeEnv) {
    // If the CHPL_HOME environment variable is set, that is our source of
    // truth. Do some more work to compare it to other guesses and gather info.

    chplHomeOut = chplHomeEnv;
    fromEnv = true;

    if (isSameFile(chplHomeEnv, guessFromPrefix.c_str())) {
      // The pre-configured prefix path matches the variable in the environment;
      // this indicates that the CHPL_HOME is from a prefix-based installation.
      installed = true;
    }

    // Emit a warning if we could guess the CHPL_HOME from the binary's path,
    // but it's not the same path as the environment variable.
    if (!guessFromBinaryPath.empty() &&
        !isSameFile(chplHomeEnv, guessFromBinaryPath.c_str())) {
      diagnosticMessage = "$CHPL_HOME=" + std::string(chplHomeEnv) +
                       " is mismatched with executable home=" +
                       guessFromBinaryPath;
    }
  } else if (guessFromBinaryPath.empty() && !guessFromPrefix.empty()) {
    // If no environment variable set, and the path-based guess failed,
    // the last resort is a prefix-based guess; in this case, installed = true.

    installed = true;

    if (setenv("CHPL_HOME", guessFromPrefix.c_str(), 0)) {
      return std::error_code(errno, std::system_category());
    }

    chplHomeOut = guessFromPrefix;
  }

  if (chplHomeOut.empty()) {
    diagnosticMessage = "CHPL_HOME must be set";
    return std::make_error_code(std::errc::no_such_file_or_directory);
  }

  // Check that the resulting path is a Chapel distribution.
  if (!isMaybeChplHome(chplHomeOut.c_str())) {
    diagnosticMessage = "CHPL_HOME=" + chplHomeOut + " is not a Chapel distribution";
  }
  return std::error_code();
}

} // namespace chpl
