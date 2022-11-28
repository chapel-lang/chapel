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

#include <cstdio>
#include "driver.h"
#include "files.h"
#include "version.h"

// this include sets BUILD_VERSION
#include "version_num.h"

// this include sets CONFIGURED_PREFIX
#include "configured_prefix.h"

void get_version(char* v, size_t bufsize) {
  size_t len = 0;
  len += snprintf(v + len, bufsize - len, "%d.%d.%d", MAJOR_VERSION,
                  MINOR_VERSION, UPDATE_VERSION);
  if (!officialRelease) {
    len += snprintf(v + len, bufsize - len, " pre-release (%s)", BUILD_VERSION);
  } else {
    // It's is an official release.
    // Try to decide whether or not to include the BUILD_VERSION
    // based on its string length. A short git sha is 10 characters.
    if (strlen(BUILD_VERSION) > 2 && !developer) {
      // assume it is a sha, so don't include it
    } else if (strcmp(BUILD_VERSION, "0") == 0) {
      // no need to append a .0
    } else {
      // include the BUILD_VERSION contents to add e.g. a .1
      len += snprintf(v + len, bufsize - len, ".%s", BUILD_VERSION);
    }
  }
}

void
get_major_minor_version(char *v, size_t bufsize) {
  snprintf(v, bufsize, "%d.%d", MAJOR_VERSION, MINOR_VERSION);
}

const char*
get_configured_prefix() {
  return CONFIGURED_PREFIX;
}

int get_major_version() {
  return MAJOR_VERSION;
}
int get_minor_version() {
  return MINOR_VERSION;
}
int get_update_version() {
  return UPDATE_VERSION;
}
const char* get_build_version() {
  return BUILD_VERSION;
}
bool get_is_official_release() {
  return officialRelease;
}
