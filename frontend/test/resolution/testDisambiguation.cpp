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
#include "chpl/resolution/resolution-queries.h"
#include "chpl/uast/all-uast.h"

#include <string>

struct GatherStuff {
  std::vector<const Function*> fns;
  std::vector<const FnCall*> fnCalls;

  // traverse into everything
  bool enter(const AstNode* ast) { return true; }
  void exit(const AstNode* ast) { }

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

  const ModuleVec& vec = parseToplevel(context, path);
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
      if (s.only() && s.only().fn()->id() == fn->id()) {
        foundOnlyIdx = i;
      } else {
        if (s.bestRef() && s.bestRef().fn()->id() == fn->id()) {
          foundBestRef = i;
        }
        if (s.bestConstRef() && s.bestConstRef().fn()->id() == fn->id()) {
          foundBestConstRef = i;
        }
        if (s.bestValue() && s.bestValue().fn()->id() == fn->id()) {
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

  checkCalledIndex(context,
      R""""(
        proc f(arg: numeric) { }  // 1
        proc f(arg: int) { }      // 2
        proc f(arg) { }           // 3
        var x: string;
        f(x);
      )"""",
    3);

}

static void test3() {
  Context ctx;
  Context* context = &ctx;

  checkCalledIndex(context,
      R""""(
        var g: int;
        proc f(arg: int) ref { return g; }        // 1
        proc f(arg: int) const ref { return g; }  // 2
        proc f(arg: int) { return g; }            // 3
        proc f(arg: numeric) { }                  // 4
        proc f(arg: real) { }                     // 5
        var x: int;
        f(x);
      )"""",
    0, 1, 2, 3);

  checkCalledIndex(context,
      R""""(
        var g: int;
        proc f(arg: real) { }                     // 1
        proc f(arg: numeric) { }                  // 2
        proc f(arg: int) { return g; }            // 3
        proc f(arg: int) const ref { return g; }  // 4
        proc f(arg: int) ref { return g; }        // 5
        var x: int;
        f(x);
      )"""",
    0, 5, 4, 3);

  checkCalledIndex(context,
      R""""(
        var g: int;
        proc f(arg: int) ref { return g; }        // 1
        proc f(arg: int) const ref { return g; }  // 2
        proc f(arg: int) { return g; }            // 3
        proc f(arg: numeric) { }                  // 4
        proc f(arg: real) { }                     // 5
        var x: real;
        f(x);
      )"""",
    5);

  checkCalledIndex(context,
      R""""(
        var g: int;
        proc f(arg: real) { }                     // 1
        proc f(arg: numeric) { }                  // 2
        proc f(arg: int) { return g; }            // 3
        proc f(arg: int) const ref { return g; }  // 4
        proc f(arg: int) ref { return g; }        // 5
        var x: real;
        f(x);
      )"""",
    1);

  // updated disambiguation based on visibility of proc
  checkCalledIndex(context,
      R""""(
        proc f(arg: int) { }                     // 1
        {
          proc f(arg) { }                        // 2

          var x = 1;
          f(x);
        }
      )"""",
    2);
}

// updated disambiguation based on width of argument
static void test4() {
  Context ctx;
  Context* context = &ctx;

  std::string program =
    R""""(
      proc f(arg: int(8)) { }                   // 1
      proc f(arg: uint(64)) { }                 // 2
      var x: int(64);
      f(x);
    )"""";

  auto name = std::string("test4.chpl");

  printf("%s\n", name.c_str());

  auto path = UniqueString::get(context, name);

  setFileText(context, path, program);

  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);

  // gather the Function and FnCalls
  GatherStuff stuff;
  m->traverse(stuff);

  assert(stuff.fns.size() > 1); // > 1 needed for disambiguation
  assert(stuff.fnCalls.size() == 4); // we have 3 calls defining types prior to
                                      // the call to f that we care about

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  auto fnCall = stuff.fnCalls[3];
  assert(fnCall->calledExpression()->toIdentifier()->name().compare("f") == 0);
  const ResolvedExpression& re = rr.byAst(fnCall);
  const MostSpecificCandidates& s = re.mostSpecific();

  // disambiguation tests should either have a best candidate or be ambiguous.
  assert (!s.isEmpty());

  int foundOnlyIdx = 0;
  int i = 1;
  for (auto fn : stuff.fns) {
    if (s.only() && s.only().fn()->id() == fn->id()) {
      foundOnlyIdx = i;
    }
    i++;
  }
  // should pick the uint(64) overload
  assert(foundOnlyIdx == 2);
}

static void test5() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  std::string program = R"""(
    proc foo(arg) where arg.type == int {
      return 42;
    }

    proc foo(arg) {
      return "hello";
    }

    var x = foo(42);
    )""";

  auto qt = resolveQualifiedTypeOfX(context, program);
  assert(qt.type()->isIntType());
}

static void test6() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  std::string program = R"""(
    proc helper(param a : int(64), param b : int(64)) param {
      return true;
    }

    proc helper(param a : uint(64), param b : uint(64)) param {
      return false;
    }

    param val : uint = 1;
    var x = if helper(val, 0) then 5 else "42";
  )""";

  auto qt = resolveQualifiedTypeOfX(context, program);
  assert(qt.type()->isStringType());
}

int main() {

  test1();
  test2();
  test3();
  test4();
  test5();
  test6();

  return 0;
}
