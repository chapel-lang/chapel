/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/uast/all-uast.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <string>

using namespace chpl;
using namespace parsing;
using namespace resolution;
using namespace types;
using namespace uast;

struct GatherStuff {
  std::vector<const Function*> fns;
  std::vector<const FnCall*> fnCalls;

  // traverse into everything
  bool enter(const ASTNode* ast) { return true; }
  void exit(const ASTNode* ast) { }

  bool enter(const Function* ast) {
    fns.push_back(ast);
    return true;
  }

  bool enter(const FnCall* ast) {
    fnCalls.push_back(ast);
    return true;
  }
};

// function indexes count functions from 1
//   0 means nothing found
//  -1 means ambiguous 
static void checkCalledIndex(Context* context,
                             std::string contents,
                             int expectOnlyIdx,
                             int expectBestRef = 0,
                             int expectBestConstRef = 0,
                             int expectBestValue = 0) {

  // create a new filename every time this is called
  static int countInput = 1;
  auto name = std::string("input") + std::to_string(countInput) + ".chpl";
  countInput++;

  printf("%s\n", name.c_str());

  auto path = UniqueString::get(context, name);

  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);

  // gather the Function and FnCalls
  GatherStuff stuff;
  m->traverse(stuff);

  assert(stuff.fns.size() > 1); // > 1 needed for disambiguation
  assert(stuff.fnCalls.size() == 1); // need just one to disambiguate

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  const ResolvedExpression& re = rr.byAst(stuff.fnCalls[0]);
  const MostSpecificCandidates& s = re.mostSpecific();

  // disambiguation tests should either have a best candidate or be ambiguous.
  assert (!s.isEmpty());

  int foundOnlyIdx = 0;
  int foundBestRef = 0;
  int foundBestConstRef = 0;
  int foundBestValue = 0;

  if (s.isAmbiguous()) {
    foundOnlyIdx = -1;
    foundBestRef = -1;
    foundBestConstRef = -1;
    foundBestValue = -1;
  } else {
    int i = 1;
    for (auto fn : stuff.fns) {
      if (s.only() && s.only()->id() == fn->id()) {
        foundOnlyIdx = i;
      } else {
        if (s.bestRef() && s.bestRef()->id() == fn->id()) {
          foundBestRef = i;
        }
        if (s.bestConstRef() && s.bestConstRef()->id() == fn->id()) {
          foundBestConstRef = i;
        }
        if (s.bestValue() && s.bestValue()->id() == fn->id()) {
          foundBestValue = i;
        }
      }
      i++;
    }
  }

  if (expectOnlyIdx != 0) {
    assert(foundOnlyIdx == expectOnlyIdx);
  } else {
    assert(foundBestRef == expectBestRef);
    assert(foundBestConstRef == expectBestConstRef);
    assert(foundBestValue == expectBestValue);
  }
}

static void test1() {
  Context ctx;
  Context* context = &ctx;

  checkCalledIndex(context,
      R""""(
        proc f(arg: int) { }   // 1
        proc f(arg: real) { }  // 2
        var x: int;
        f(x);
      )"""",
    1);

  checkCalledIndex(context,
      R""""(
        proc f(arg: real) { }  // 1
        proc f(arg: int) { }   // 2
        var x: int;
        f(x);
      )"""",
    2);

  checkCalledIndex(context,
      R""""(
        proc f(param arg: int) { }   // 1
        proc f(param arg: real) { }  // 2
        param x = 1;
        f(x);
      )"""",
    1);

  checkCalledIndex(context,
      R""""(
        proc f(param arg: real) { }  // 1
        proc f(param arg: int) { }   // 2
        param x = 1;
        f(x);
      )"""",
    2);
}

static void test2() {
  Context ctx;
  Context* context = &ctx;

  checkCalledIndex(context,
      R""""(
        proc f(arg) { }           // 1
        proc f(arg: numeric) { }  // 2
        proc f(arg: int) { }      // 3
        var x: int;
        f(x);
      )"""",
    3);

  checkCalledIndex(context,
      R""""(
        proc f(arg) { }           // 1
        proc f(arg: numeric) { }  // 2
        proc f(arg: int) { }      // 3
        var x: real;
        f(x);
      )"""",
    2);

  checkCalledIndex(context,
      R""""(
        proc f(arg) { }           // 1
        proc f(arg: numeric) { }  // 2
        proc f(arg: int) { }      // 3
        var x: string;
        f(x);
      )"""",
    1);
}

int main() {

  test1();
  test2();

  return 0;
}
