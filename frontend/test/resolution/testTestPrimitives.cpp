/*
 * Copyright 2024-2025 Hewlett Packard Enterprise Development LP
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

#include <vector>
#include "test-resolution.h"
#include "chpl/resolution/resolution-queries.h"

static void testGatherTests(const std::vector<std::pair<std::string, std::string>>& files, std::vector<std::string> expectedTestNames) {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  struct GatherFinder {
    const PrimCall* gatherPrimCall = nullptr;

    bool enter(const PrimCall* prim) {
      if (prim->prim() == chpl::uast::primtags::PRIM_GATHER_TESTS) {
        gatherPrimCall = prim;
        return false;
      }
      return true;
    }

    bool enter(const AstNode* node) { return true; }
    void exit(const AstNode* node) {}
  } gatherFinder;

  // Parse all of the files first. This is how the compiler does it, and this
  // is required for introspection to work, since we can't know about test
  // functions we haven't even parsed first.
  for (auto file : files) {
    setFileText(context, file.first, file.second);
    auto& topLevel = parseToplevel(context, UniqueString::get(context, file.first));
    for (auto mod : topLevel) {
      mod->traverse(gatherFinder);
    }
  }

  assert(gatherFinder.gatherPrimCall);

  // Resolve all the files in the order they were given.
  for (auto& file : files) {
    auto modVec = parseToplevel(context, UniqueString::get(context, file.first));
    for (auto mod : modVec) {
      auto resolved = resolveModule(context, mod->id());

      // If this was the module that had the gather call, check that the
      // gather resolved as expected.
      if (resolved.hasAst(gatherFinder.gatherPrimCall)) {
        auto& gatherResult = resolved.byAst(gatherFinder.gatherPrimCall);
        assert(gatherResult.type().type());
        assert(gatherResult.type().type()->isIntType());
        assert(gatherResult.type().type()->toIntType()->isDefaultWidth());
        assert(gatherResult.type().isParam());
        assert(gatherResult.type().param());
        assert(gatherResult.type().param()->isIntParam());
        assert((size_t)(gatherResult.type().param()->toIntParam()->value()) == expectedTestNames.size());
      }
    }
  }

  std::vector<std::string> actualTestNames;
  for (auto fn : getTestsGatheredViaPrimitive(context)) {
    actualTestNames.push_back(fn->name().str());
  }

  std::sort(expectedTestNames.begin(), expectedTestNames.end());
  std::sort(actualTestNames.begin(), actualTestNames.end());

  assert(expectedTestNames.size() == expectedTestNames.size());
  for (size_t i = 0; i < expectedTestNames.size(); i++) {
    assert(expectedTestNames[i] == actualTestNames[i]);
  }
}

static void testAllPermutations(std::vector<std::pair<std::string, std::string>> files, std::vector<std::string> expectedTestNames) {
  std::sort(files.begin(), files.end());

  do {
    testGatherTests(files, expectedTestNames);
  } while (std::next_permutation(files.begin(), files.end()));
}

static void test1() {
  const char* M1 =
      R"""(
      record testRec {

      }

      proc aTest(test: testRec) throws {}
      proc notATest1(test: testRec, arg: int) throws {}
      proc notATest2(test: testRec, arg: int) {}
      proc notATest3(test: testRec) {}

      record someOtherRecord {
        proc notATest4(test: testRec) throws {}
        proc notATest5(test: testRec, arg: int) throws {}
        proc notATest6(test: testRec, arg: int) {}
        proc notATest7(test: testRec) {}
      }

      var tr: testRec;
      __primitive("gather tests", tr);
      )""";
  testGatherTests(
    { {"M1", M1} },
    { "aTest" }
  );
}

static void test2() {
  const char* M1 =
      R"""(
      record testRec {

      }

      proc testInM1(test: testRec) throws {}
      )""";
  const char* M2 =
      R"""(
      use M1;

      proc testInM2(test: testRec) throws {}
      var tr: testRec;
      __primitive("gather tests", tr);

      use M3;
      )""";
  const char* M3 =
      R"""(
      use M1;

      proc testInM3(test: testRec) throws {}
      )""";

  testAllPermutations(
    { {"M1", M1}, {"M2", M2}, {"M3", M3} },
    { "testInM1", "testInM2", "testInM3" }
  );
}

int main() {
  test1();
  test2();
}
