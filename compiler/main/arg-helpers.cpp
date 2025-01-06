/*
 * Copyright 2022-2025 Hewlett Packard Enterprise Development LP
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

#include "arg-helpers.h"
#include "driver.h"
#include "files.h"
#include "misc.h"
#include "stringutil.h"

void arg_fatalError(const char *msg) {
  INT_FATAL("%s", msg);
}

void arg_fatalUserError(const char *msg) {
  USR_FATAL("%s", msg);
}

const char *arg_programLoc(const char *argv0, void *mainAddr) {
  return findProgramPath(argv0);
}

void arg_fatalErrorCont(const char *msg) {
  USR_FATAL_CONT("%s", msg);
}

void arg_warn(const char *msg, const char* path) {
  USR_WARN(msg, path, nullptr);
}
