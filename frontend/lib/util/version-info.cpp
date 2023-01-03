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

#include <string>
#include "chpl/util/version-info.h"
#include "chpl/config/config.h"

namespace chpl {

extern const char* GIT_SHA;

const char* gitSHA() {
  return GIT_SHA;
}

int getMajorVersion() {
  return CHPL_COMPILER_LIB_VERSION_MAJOR;
}
int getMinorVersion() {
  return CHPL_COMPILER_LIB_VERSION_MINOR;
}
int getUpdateVersion() {
  return CHPL_COMPILER_LIB_VERSION_PATCH;
}

const char* getConfiguredPrefix() {
  return CHPL_CONFIGURED_PREFIX;
}

std::string getMajorMinorVersion() {
  std::string version = std::to_string(getMajorVersion());
  version += ".";
  version += std::to_string(getMinorVersion());
  return version;
}

std::string getVersion(bool developer) {
  std::string ret;
  ret = std::to_string(getMajorVersion()) + "." +
        std::to_string(getMinorVersion()) + "." +
        std::to_string(getUpdateVersion());
  if (!CHPL_OFFICIAL_RELEASE) {
    ret += " pre-release (" + std::string(getBuildVersion(developer)) + ")";
  } else {
    // It's is an official release.
    // Try to decide whether or not to include the BUILD_VERSION
    // based on its string length. A short git sha is 10 characters.
    std::string build = std::string(getBuildVersion(developer));
    if (build.length() > 2 && !developer) {
      // assume it is a sha, so don't include it
    } else if (build.compare("0") == 0) {
      // no need to append a .0
    } else {
      // include the BUILD_VERSION contents to add e.g. a .1
      ret += "." + build;
    }
  }
  return ret;
}

bool getIsOfficialRelease() {
  return CHPL_OFFICIAL_RELEASE;
}

const char* getBuildVersion(bool developer) {
  if (!CHPL_OFFICIAL_RELEASE || developer) {
    return gitSHA();
  } else {
    return CHPL_BUILD_VERSION;
  }
}

} // namespace chpl
