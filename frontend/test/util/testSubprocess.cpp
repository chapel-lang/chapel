/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#include "test-common.h"

#include "chpl/util/subprocess.h"

using namespace chpl;

// check that trying to run a nonexistant command returns an error
static void checkNonexistent() {
  int rc;
  std::vector<std::string> cmd;

  cmd.clear();
  cmd.push_back("./nonexistent-command");
  rc = executeAndWait(cmd, "nonexistent-command description", true);
  assert(rc != 0);

  cmd.clear();
  cmd.push_back("./nonexistent-command");
  cmd.push_back("arg");
  rc = executeAndWait(cmd, "nonexistent-command description", true);
  assert(rc != 0);

  cmd.clear();
  cmd.push_back("/this/path/does/not/exist");
  rc = executeAndWait(cmd, "path does not exist description", true);
  assert(rc != 0);

  cmd.clear();
  cmd.push_back("/this/path/does/not/exist");
  cmd.push_back("arg");
  rc = executeAndWait(cmd, "path does not exist description", true);
  assert(rc != 0);
}

// check that we can run /usr/bin/env and get 0 return code from it
static void checkEnv() {
  int rc;
  std::vector<std::string> cmd;

  cmd.clear();
  cmd.push_back("/usr/bin/env");
  rc = executeAndWait(cmd, "testing that env can be run", true);
  assert(rc == 0);
}

// check that running an empty command or a comment returns 0
static void checkEmptyOrComment() {
  int rc;
  std::vector<std::string> cmd;

  cmd.clear();
  rc = executeAndWait(cmd, "empty command 1", true);
  assert(rc == 0);

  cmd.clear();
  cmd.push_back("");
  rc = executeAndWait(cmd, "empty command 2", true);
  assert(rc == 0);

  cmd.clear();
  cmd.push_back("# this is a comment");
  rc = executeAndWait(cmd, "comment command", true);
  assert(rc == 0);
}


int main(int argc, char** argv) {

  checkNonexistent();
  checkEnv();
  checkEmptyOrComment();

  return 0;
}
