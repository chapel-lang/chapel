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

#include "test-resolution.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/call-graph.h"
#include "chpl/uast/Module.h"

static void checkCalledSetMatches(CalledFnsSet& called,
                                  std::vector<std::string> expectSymPaths) {
  std::vector<std::string> gotSymPaths;

  // note: this loop runs in a nondeterministic order
  for (auto pair : called) {
    const ResolvedFunction* fn = pair.first;
    gotSymPaths.push_back(fn->id().symbolPath().str());
  }
  std::sort(gotSymPaths.begin(), gotSymPaths.end());
  std::sort(expectSymPaths.begin(), expectSymPaths.end());

  if (gotSymPaths.size() != expectSymPaths.size()) {
    assert(false && "number got does not match number expected");
  }

  size_t n = expectSymPaths.size();
  for (size_t i = 0; i < n; i++) {
    assert(gotSymPaths[i] == expectSymPaths[i]);
  }
}

static void test1() {
  Context ctx;
  auto context = &ctx;
  ErrorGuard guard(context);

  auto path = TEST_NAME_FROM_FN_NAME(context);
  std::string contents =
    R""""(
    module M {
      proc foo() { }
      proc bar() { foo(); }
      bar();
    }
    )"""";
  setFileText(context, path, contents);

  auto& br = parseAndReportErrors(context, path);
  auto mod = br.singleModule();
  assert(mod);

  CalledFnsSet directCalls;
  gatherFnsCalledByModInit(context, mod->id(), directCalls);
  checkCalledSetMatches(directCalls, {"M.bar"});

  CalledFnsSet transitiveCalls;
  gatherTransitiveFnsCalledByModInit(context, mod->id(), transitiveCalls);
  checkCalledSetMatches(transitiveCalls, {"M.bar", "M.foo"});
}

static void test2() {
  Context ctx;
  auto context = &ctx;
  ErrorGuard guard(context);

  auto path = TEST_NAME_FROM_FN_NAME(context);
  std::string contents =
    R""""(
    module M {
      proc foo() { bar(); }
      proc bar() { foo(); }
      bar();
    }
    )"""";
  setFileText(context, path, contents);

  auto& br = parseAndReportErrors(context, path);
  auto mod = br.singleModule();
  assert(mod);

  CalledFnsSet directCalls;
  gatherFnsCalledByModInit(context, mod->id(), directCalls);
  checkCalledSetMatches(directCalls, {"M.bar"});

  CalledFnsSet transitiveCalls;
  gatherTransitiveFnsCalledByModInit(context, mod->id(), transitiveCalls);
  checkCalledSetMatches(transitiveCalls, {"M.bar", "M.foo"});
}


int main() {
  test1();
  test2();

  printf("OK\n");

  return 0;
}
