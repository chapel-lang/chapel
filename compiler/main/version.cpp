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

#include <cstdio>
#include "driver.h"
#include "files.h"
#include "version.h"

#include "chpl/util/version-info.h"

void get_version(char* buf, size_t bufsize) {
  std::string ret = chpl::getVersion();
  strcpy(buf, ret.c_str());
}

void get_major_minor_version(char* buf, size_t bufsize) {
  snprintf(buf, bufsize, "%d.%d", get_major_version(), get_minor_version());
}

const char* get_configured_prefix() {
  return chpl::getConfiguredPrefix();
}

int get_major_version() {
  return chpl::getMajorVersion();
}
int get_minor_version() {
  return chpl::getMinorVersion();
}
int get_update_version() {
  return chpl::getUpdateVersion();
}
bool get_is_official_release() {
  return chpl::getIsOfficialRelease();
}
