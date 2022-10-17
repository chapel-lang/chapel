/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

template <typename InputMap>
llvm::ErrorOr<ChplEnvMap>
getChplEnvImpl(const InputMap& varMap,
           const char* chplHome) {
  // Run printchplenv script, passing currently known CHPL_vars as well
  std::string command;

  // Pass known variables in varMap into printchplenv by prepending to command
  for (auto& ii : varMap)
    command += ii.first + "=" + ii.second + " ";

  command += "CHPLENV_SKIP_HOST=true ";
  command += "CHPLENV_SUPPRESS_WARNINGS=true ";
  command += std::string(chplHome) + "/util/printchplenv --all --internal --no-tidy --simple";

  auto commandOutput = getCommandOutput(command);
  if (auto err = commandOutput.getError()) {
    // forward error code
    return err;
  }
  ChplEnvMap result;
  parseChplEnv(commandOutput.get(), result);
  return result;
}

llvm::ErrorOr<ChplEnvMap>
getChplEnv(const std::map<std::string, const char*>& varMap,
           const char* chplHome) {
  return getChplEnvImpl(varMap, chplHome);
}

llvm::ErrorOr<ChplEnvMap>
getChplEnv(const std::unordered_map<std::string, std::string>& varMap,
           const char* chplHome) {
  return getChplEnvImpl(varMap, chplHome);
}

bool isMaybeChplHome(std::string path) {
  // assume if we find subfolder util/ containing chplenv, we might be home
  path += "/util/chplenv";
  return llvm::sys::fs::exists(path);
}

std::error_code findChplHome(char* argv0, void* mainAddr,
                             std::string& chplHomeOut,
                             bool& installed, bool& fromEnv,
                             std::string& warningMessage) {
  std::string versionString = getMajorMinorVersion();
  std::string guess = getExecutablePath(argv0, mainAddr);

  const char* chpl_home = getenv("CHPL_HOME");

  if (!guess.empty()) {
    // truncate path at /bin
    char* tmp_guess = strdup(guess.c_str());
    if ( tmp_guess[0] ) {
      int j = strlen(tmp_guess) - 5; // /bin and '\0'
      for ( ; j >= 0; j-- ) {
        if ( tmp_guess[j] == '/' &&
            tmp_guess[j+1] == 'b' &&
            tmp_guess[j+2] == 'i' &&
            tmp_guess[j+3] == 'n' ) {
          tmp_guess[j] = '\0';
          break;
        }
      }
    }
    guess = std::string(tmp_guess);
    if (isMaybeChplHome(guess)) {
      chplHomeOut = guess;
    } else {
      guess = "";
    }
  }

  if (chpl_home) {
    fromEnv = true;
    if(strlen(chpl_home) > FILENAME_MAX)
      // USR_FATAL("$CHPL_HOME=%s path too long", chpl_home);
      // error_state
      // TODO: customize error message?
      return std::make_error_code(std::errc::filename_too_long);
    if (guess.empty()) {
      // Could not find exe path, but have a env var set
      chplHomeOut = std::string(chpl_home);
    } else {
      // We have env var and found exe path.
      // Check that they match and emit a warning if not.
      if ( ! isSameFile(chpl_home, guess.c_str()) ) {
        // Not the same. Emit warning.
        //USR_WARN("$CHPL_HOME=%s mismatched with executable home=%s",
        //         chpl_home, guess);
        warningMessage = "$CHPL_HOME=" + std::string(chpl_home) + " is mismatched with executable home=" + guess;
      }
      // Since we have an enviro var, always use that.
      chplHomeOut = std::string(chpl_home);
    }
  } else {
    // Check in a default location too
    if (guess.empty()) {
      char TEST_HOME[FILENAME_MAX+1] = "";

      // Check for Chapel libraries at installed prefix
      // e.g. /usr/share/chapel/<vers>
      int rc;
      rc = snprintf(TEST_HOME, FILENAME_MAX, "%s/%s/%s",
                    getConfiguredPrefix(), // e.g. /usr
                    "share/chapel",
                    versionString.c_str());
      if (rc >= FILENAME_MAX) {
        // USR_FATAL("Installed pathname too long");
        // TODO: return an error here
        return std::make_error_code(std::errc::filename_too_long);
      }

      if (isMaybeChplHome(TEST_HOME)) {
        guess = strdup(TEST_HOME);
        installed = true;
       }
    }

    if (guess.empty()) {
      // Could not find enviro var, and could not
      // guess at exe's path name.
      // USR_FATAL("$CHPL_HOME must be set to run chpl");
      // TODO: customize the error message
      return std::make_error_code(std::errc::no_such_file_or_directory);
    } else {
      int rc;

      if (guess.length() > FILENAME_MAX) {
      // USR_FATAL("chpl guessed home %s too long", guess);
        return std::make_error_code(std::errc::filename_too_long);
      }
      // Determined exe path, but don't have a env var set
        rc = setenv("CHPL_HOME", guess.c_str(), 0);
        if ( rc ) {
          // USR_FATAL("Could not setenv CHPL_HOME");
          // TODO: customize the error message
          return std::make_error_code(std::errc::no_such_file_or_directory);
         }
         chplHomeOut = guess;
    }
  }
  // Check that the resulting path is a Chapel distribution.
  if (!isMaybeChplHome(chplHomeOut.c_str())) {
    // Bad enviro var.
    //USR_WARN("CHPL_HOME=%s is not a Chapel distribution", CHPL_HOME);
    warningMessage = "CHPL_HOME=" + chplHomeOut + " is not a Chapel distribution";
  }
  return std::error_code();
}

} // namespace chpl
