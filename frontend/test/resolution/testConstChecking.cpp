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
#include "chpl/resolution/scope-queries.h"
#include "chpl/resolution/split-init.h"
#include "chpl/uast/Call.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Variable.h"

static void
testConstChecking(const char* test,
                  const char* program,
                  // vector of line numbers with an error expected
                  std::vector<int> expectedErrorLines) {
  printf("\n### %s\n", test);

  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  std::string testname = test;
  testname += ".chpl";
  auto path = UniqueString::get(context, testname);
  std::string contents = program;
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);
  const Module* M = vec[0]->toModule();
  assert(M);
  assert(M->numStmts() >= 1);

  M->dump();

  resolveModule(context, M->id());

  // and make sure to resolve any concrete functions
  for (auto stmt : M->stmts()) {
    if (auto fn = stmt->toFunction()) {
      // resolve concrete functions (ignore generics)
      resolveConcreteFunction(context, fn->id());
    }
  }

  guard.printErrors();

  std::set<int> expectedSet;
  std::set<int> gotSet;
  for (auto line : expectedErrorLines) {
    expectedSet.insert(line);
  }

  for (const auto& err : guard.errors()) {
    Location errLoc = err->location(context);
    gotSet.insert(errLoc.line());
  }

  if (expectedSet != gotSet) {
    assert(false && "Error locations do not match expected locations");
  }

  guard.clearErrors();
}

static void test1a() {
  testConstChecking("test1a",
    R""""(
      module M {
        proc acceptsRef(ref arg) { }
        proc test() {
          const x: int;
          acceptsRef(x);
        }
      }
    )"""",
    {6});
}
static void test1b() {
  testConstChecking("test1b",
    R""""(
      module M {
        proc acceptsRef(ref arg) { }
        proc test() {
          var x: int;
          acceptsRef(x);
        }
      }
    )"""",
    {});
}
static void test1c() {
  testConstChecking("test1c",
    R""""(
      module M {
        proc acceptsConstRef(const ref arg) { }
        proc test() {
          const x: int;
          acceptsConstRef(x);
        }
      }
    )"""",
    {});
}

static void test2a() {
  testConstChecking("test2a",
    R""""(
      module M {
        proc acceptsRef(ref arg) { }
        proc test() {
          const x: int;
          const ref y = x;
          acceptsRef(y);
        }
      }
    )"""",
    {7});
}

static void test3a() {
  testConstChecking("test3a",
    R""""(
      module M {
        proc acceptsRef(ref arg) { }
        var global: int;
        proc foo() ref { return global; }
        proc test() {
          acceptsRef(foo());
        }
      }
    )"""",
    {});
}

static void test3b() {
  testConstChecking("test3b",
    R""""(
      module M {
        proc acceptsRef(ref arg) { }
        var global: int;
        proc foo() const ref { return global; }
        proc test() {
          acceptsRef(foo());
        }
      }
    )"""",
    {7});
}

static void test3c() {
  testConstChecking("test3c",
    R""""(
      module M {
        proc acceptsRef(ref arg) { }
        var global: int;
        proc foo() { return global; }
        proc test() {
          acceptsRef(foo());
        }
      }
    )"""",
    {7});
}
static void test3d() {
  testConstChecking("test3d",
    R""""(
      module M {
        proc acceptsRef(ref arg) { }
        var global: int;
        proc foo() : int { return global; }
        proc test() {
          acceptsRef(foo());
        }
      }
    )"""",
    {7});
}

static void test4a() {
  testConstChecking("test4a",
    R""""(
      module M {
        proc acceptsRef(ref arg) { }
        var global: int;
        proc foo() ref { return global; }
        proc foo() const ref { return global; }
        proc test() {
          acceptsRef(foo());
        }
      }
    )"""",
    {});
}

static void test4b() {
  testConstChecking("test4b",
    R""""(
      module M {
        proc acceptsRef(ref arg) { }
        var global: int;
        proc foo() { return global; }
        proc foo() ref { return global; }
        proc test() {
          acceptsRef(foo());
        }
      }
    )"""",
    {});
}

static void test4c() {
  testConstChecking("test4c",
    R""""(
      module M {
        proc acceptsRef(ref arg) { }
        var global: int;
        proc foo() { return global; }
        proc foo() const ref { return global; }
        proc test() {
          acceptsRef(foo());
        }
      }
    )"""",
    {8});
}

static void test4d() {
  testConstChecking("test4d",
    R""""(
      module M {
        proc acceptsRef(ref arg) { }
        var global: int;
        proc foo() { return global; }
        proc foo() ref { return global; }
        proc foo() const ref { return global; }
        proc test() {
          acceptsRef(foo());
        }
      }
    )"""",
    {});
}

static void test5a() {
  testConstChecking("test5a",
    R""""(
      module M {
        proc acceptsOut(out arg: int) { }
        proc test() {
          const x: int;
          acceptsOut(x);
        }
      }
    )"""",
    {6});
}
static void test5b() {
  testConstChecking("test5b",
    R""""(
      module M {
        proc acceptsInout(inout arg: int) { }
        proc test() {
          const x: int;
          acceptsInout(x);
        }
      }
    )"""",
    {6});
}

static void test6a() {
  testConstChecking("test6a",
    R""""(
      module M {
        operator =(ref lhs: int, const rhs: int) {}
        proc test() {
          const x: int = 0;
          x = 34;
        }
      }
    )"""",
    {6});
}
static void test6b() {
  testConstChecking("test6b",
    R""""(
      module M {
        operator =(ref lhs: int, const rhs: int) {}
        proc test() {
          const x: int = 0;
          const ref y = x;
          y = 34;
        }
      }
    )"""",
    {7});
}
static void test6c() {
  testConstChecking("test6c",
    R""""(
      module M {
        operator =(ref lhs: int, const rhs: int) {}
        proc test() {
          const x: int = 0;
          ref y = x;
          y = 34;
        }
      }
    )"""",
    {6});
}

static void test7a() {
  // This test exists because of a past bug in which the call to 'dummy' was
  // incorrectly evaluated due to an incorrectly-formed CallInfo, caused by a
  // failure to account for the implicit receiver. The CallInfo lacked an
  // argument for 'this', leading to const-checking being off by one for
  // arguments 'x' and 'constThing'. This resulted in a "failure" as the
  // frontend thought we were passing 'constThing' to 'ref A'.
  testConstChecking("test7a",
    R"""(
      module M {
        record R {
          var x : int;

          proc dummy(ref A, const B) {
          }

          proc ref wrapper() {
            const constThing : int;
            dummy(x, constThing);
          }
        }

        proc main() {
          var r : R;
          r.wrapper();
        }
      }
    )""",
  {});
}

// TODO: test const checking for associated functions


int main() {
  test1a();
  test1b();
  test1c();

  test2a();

  test3a();
  test3b();
  test3c();
  test3d();

  test4a();
  test4b();
  test4c();
  test4d();

  test5a();
  test5b();

  test6a();
  test6b();
  test6c();

  test7a();

  return 0;
}
