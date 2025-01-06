/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "chpl/util/assertions.h"

#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


using namespace chpl;

/*
  Signal handler for when the child process aborts
  Helps ensure that we detect an appropriate signal in test4
*/
static void handler(int sig)
{
  assert(sig == SIGCHLD);
}

// test that we can see initial values for globals and edit them
static void test1() {
  // verify the default status
  assert(assertionsAreOn());
  assert(assertionsAreFatal());

  //update and check the new values
  setAssertions(false);
  assert(!assertionsAreOn());
  setAssertionsFatal(false);
  assert(!assertionsAreFatal());
}

// test that assertions are skipped when asked
static void test2() {
  // verify the state from test1
  assert(!assertionsAreOn());
  assert(!assertionsAreFatal());

  // make assertions fatal again
  setAssertionsFatal(true);
  //expect we didn't crash because this gets skipped
  CHPL_ASSERT(false);
}

// test that fatal assertions pass if condition is true
static void test3() {
  // verify the state from test2
  assert(!assertionsAreOn());
  assert(assertionsAreFatal());

  setAssertions(true);
  assert(assertionsAreOn());
  // expect we don't crash because assertion is true
  CHPL_ASSERT(true);
}

// test that fatal assertions cause halting behavior when set
static void test4() {
  // verify the state from test3
  assert(assertionsAreOn());
  assert(assertionsAreFatal());
  signal(SIGCHLD, handler);
  pid_t childPid = fork();
  int status = -1;
  if (childPid == 0) {
    // in child process
    // expect we crash because assertion is false
    CHPL_ASSERT(false);
  } else if (childPid > 0 ) {
    // in parent process
    waitpid(childPid, &status, 0);
    // filter status down to key bits
    int code = WEXITSTATUS(status);
    // expect return code 0 since child process aborted, fork returns 0 immediately
    assert(code == 0);
  }
}

int main(int argc, char** argv) {
  test1();
  test2();
  test3();
  test4();

  // verify the global state was reset before exiting this test
  assert(assertionsAreOn());
  assert(assertionsAreFatal());
  return 0;
}
