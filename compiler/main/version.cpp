/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#include <cstdio>
#include "driver.h"
#include "files.h"
#include "version.h"

// this include sets BUILD_VERSION
#include "version_num.h"

// this include sets CONFIGURED_PREFIX
#include "configured_prefix.h"

// this include sets CLANG_SETTINGS
#include "clang_settings.h"

void
get_version(char *v) {
  v += sprintf(v, "%d.%s.%s", MAJOR_VERSION, MINOR_VERSION, UPDATE_VERSION);
  if (strcmp(BUILD_VERSION, "0") != 0 || developer)
    sprintf(v, ".%s", BUILD_VERSION);
}

void
get_major_minor_version(char *v) {
  sprintf(v, "%d.%s", MAJOR_VERSION, MINOR_VERSION);
}

const char*
get_configured_prefix() {
  return CONFIGURED_PREFIX;
}

std::string
get_clang_cc() {
  std::string ret = CLANG_SETTINGS[0];
  expandInstallationPaths(ret);
  return ret;
}

std::string
get_clang_cxx() {
  std::string ret = CLANG_SETTINGS[1];
  expandInstallationPaths(ret);
  return ret;
}

std::string
get_clang_sysroot_args() {
  std::string ret = CLANG_SETTINGS[2];
  expandInstallationPaths(ret);
  return ret;
}
