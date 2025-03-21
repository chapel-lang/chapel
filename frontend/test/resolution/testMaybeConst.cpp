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
testMaybeRef(const char* test,
             const char* program,
             // vector of ID of Formal, isRef (vs const ref)
             std::vector<std::pair<const char*, bool>> expectedRefs,
             // vector of ID of call, ID of called function  
             std::vector<std::pair<const char*, const char*>> expectedCalls,
             bool expectErrors=false) {
  printf("\n### %s\n", test);

  Context* context = buildStdContext();
  ErrorGuard guard(context);

  ResolutionContext rcval(context);
  auto rc = &rcval;

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

  // resolving functions for all of the IDs mentioned in expected vectors
  // (this assumes that they are concrete functions)
  for (auto pair : expectedRefs) {
    ID id = ID::fromString(context, pair.first);
    auto ast = idToAst(context, id);
    assert(ast && ast->isFormal());
    ID parentId = id.parentSymbolId(context);
    auto parentAst = idToAst(context, parentId);
    assert(parentAst && parentAst->isFunction());
    const ResolvedFunction* r = resolveConcreteFunction(context, parentId);
    auto sig = inferRefMaybeConstFormals(rc,
                                         r->signature(),
                                         /* poiScope */ nullptr);
    auto untyped = sig->untyped();
    for (int i = 0; i < untyped->numFormals(); i++) {
      if (untyped->formalDecl(i) == ast) {
        QualifiedType t = sig->formalType(i);
        // it's formal i
        if (pair.second) { // ref
          if (t.kind() != QualifiedType::REF) {
            printf("For Formal ID %s, expected 'ref' but got '%s'\n",
                    ast->id().str().c_str(),
                    qualifierToString(t.kind()));
            assert(false);
          }
        } else {
          if (t.kind() != QualifiedType::CONST_REF) {
            printf("For Formal ID %s, expected 'const ref' but got '%s'\n",
                    ast->id().str().c_str(),
                    qualifierToString(t.kind()));
            assert(false);
          }
        }
      }
    }
  }

  for (auto pair : expectedCalls) {
    ID id = ID::fromString(context, pair.first);
    ID expectedCalledFnId = ID::fromString(context, pair.second);
    ID parentId = id.parentSymbolId(context);
    auto parentAst = idToAst(context, parentId);
    assert(parentAst && parentAst->isFunction());
    const ResolvedFunction* r = resolveConcreteFunction(context, parentId);
    // what is the called function
    auto ast = idToAst(context, id);
    assert(ast && ast->isCall());
    const ResolvedExpression& re = r->byAst(ast);
    // what function is called?
    const MostSpecificCandidates& candidates = re.mostSpecific();
    assert(candidates.numBest() == 1); // should be resolved by now
    ID calledFnId = candidates.only().fn()->untyped()->id();
    if (expectedCalledFnId != calledFnId) {
      printf("For Call ID %s, expected to call Function ID %s, bot got %s\n",
              ast->id().str().c_str(),
              expectedCalledFnId.str().c_str(),
              calledFnId.str().c_str());
      assert(false);
    }
  }

  size_t errCount = guard.realizeErrors();
  if (expectErrors) {
    assert(errCount > 0);
  } else {
    assert(errCount == 0);
  }
}

static void test1() {
  testMaybeRef("test1",
    R""""(
      module M {
        proc test() {
        }
      }
    )"""",
    {},
    {});
}

static void test2() {
  testMaybeRef("test2",
    R""""(
      module M {
        proc acceptsRef(ref arg) { }
        record R {
          proc method() {
            acceptsRef(this);
          }
        }

      }
    )"""",
    {{"M.R.method@0", true}},
    {});
}

// test ref/const ref return intent overload
static void test3a() {
  testMaybeRef("test3a",
    R""""(
      module M {
        var global: int;
        proc foo() ref { return global; }         // M.foo
        proc foo() const ref { return global; }   // M.foo#1
        proc test() {
          var x = foo();
        }
      }
    )"""",
    {},
    {{"M.test@1", "M.foo#1"}});
}

static void test3b() {
  testMaybeRef("test3b",
    R""""(
      module M {
        var global: int;
        proc foo() const ref { return global; }   // M.foo
        proc foo() ref { return global; }         // M.foo#1
        proc test() {
          var x = foo();
        }
      }
    )"""",
    {},
    {{"M.test@1", "M.foo"}});
}

static void test3c() {
  testMaybeRef("test3c",
    R""""(
      module M {
        var global: int;
        proc foo() const ref { return global; }   // M.foo
        proc foo() ref { return global; }         // M.foo#1
        proc test() {
          const ref x = foo();
        }
      }
    )"""",
    {},
    {{"M.test@1", "M.foo"}});
}

static void test3d() {
  testMaybeRef("test3d",
    R""""(
      module M {
        var global: int;
        proc foo() const ref { return global; }   // M.foo
        proc foo() ref { return global; }         // M.foo#1
        proc test() {
          ref x = foo();
        }
      }
    )"""",
    {},
    {{"M.test@1", "M.foo#1"}});
}

static void test3e() {
  testMaybeRef("test3e",
    R""""(
      module M {
        var global: int;
        proc foo() ref { return global; }         // M.foo
        proc foo() const ref { return global; }   // M.foo#1
        proc acceptsRef(ref arg: int) { }
        proc test() {
          acceptsRef(foo());
        }
      }
    )"""",
    {},
    {{"M.test@2", "M.foo"}});
}

static void test3f() {
  testMaybeRef("test3f",
    R""""(
      module M {
        var global: int;
        proc foo() ref { return global; }         // M.foo
        proc foo() const ref { return global; }   // M.foo#1
        proc acceptsConstRef(const ref arg: int) { }
        proc test() {
          acceptsConstRef(foo());
        }
      }
    )"""",
    {},
    {{"M.test@2", "M.foo#1"}});
}

static void test3g() {
  testMaybeRef("test3g",
    R""""(
      module M {
        var global: int;
        proc foo() ref { return global; }         // M.foo
        proc foo() const ref { return global; }   // M.foo#1
        proc acceptsIn(in arg: int) { }
        proc test() {
          acceptsIn(foo());
        }
      }
    )"""",
    {},
    {{"M.test@2", "M.foo#1"}});
}

static void test3h() {
  testMaybeRef("test3h",
    R""""(
      module M {
        var global: int;
        proc foo() ref { return global; }         // M.foo
        proc foo() const ref { return global; }   // M.foo#1
        proc acceptsOut(out arg: int) { }
        proc test() {
          acceptsOut(foo());
        }
      }
    )"""",
    {},
    {{"M.test@2", "M.foo"}});
}

static void test3i() {
  testMaybeRef("test3i",
    R""""(
      module M {
        var global: int;
        proc foo() ref { return global; }         // M.foo
        proc foo() const ref { return global; }   // M.foo#1
        proc acceptsInout(inout arg: int) { }
        proc test() {
          acceptsInout(foo());
        }
      }
    )"""",
    {},
    {{"M.test@2", "M.foo"}});
}

// test ref/value return intent overload
static void test4a() {
  testMaybeRef("test4a",
    R""""(
      module M {
        var global: int;
        proc foo() ref { return global; }         // M.foo
        proc foo() { return global; }             // M.foo#1
        proc test() {
          var x = foo();
        }
      }
    )"""",
    {},
    {{"M.test@1", "M.foo#1"}});
}

static void test4b() {
  testMaybeRef("test4b",
    R""""(
      module M {
        var global: int;
        proc foo() { return global; }             // M.foo
        proc foo() ref { return global; }         // M.foo#1
        proc test() {
          var x = foo();
        }
      }
    )"""",
    {},
    {{"M.test@1", "M.foo"}});
}

static void test4c() {
  testMaybeRef("test4c",
    R""""(
      module M {
        var global: int;
        proc foo() { return global; }             // M.foo
        proc foo() ref { return global; }         // M.foo#1
        proc test() {
          const ref x = foo();
        }
      }
    )"""",
    {},
    {{"M.test@1", "M.foo"}});
}

static void test4d() {
  testMaybeRef("test4d",
    R""""(
      module M {
        var global: int;
        proc foo() { return global; }             // M.foo
        proc foo() ref { return global; }         // M.foo#1
        proc test() {
          ref x = foo();
        }
      }
    )"""",
    {},
    {{"M.test@1", "M.foo#1"}});
}

static void test4e() {
  testMaybeRef("test4e",
    R""""(
      module M {
        var global: int;
        proc foo() ref { return global; }         // M.foo
        proc foo() { return global; }             // M.foo#1
        proc acceptsRef(ref arg: int) { }
        proc test() {
          acceptsRef(foo());
        }
      }
    )"""",
    {},
    {{"M.test@2", "M.foo"}});
}

static void test4f() {
  testMaybeRef("test4f",
    R""""(
      module M {
        var global: int;
        proc foo() ref { return global; }         // M.foo
        proc foo() { return global; }             // M.foo#1
        proc acceptsConstRef(const ref arg: int) { }
        proc test() {
          acceptsConstRef(foo());
        }
      }
    )"""",
    {},
    {{"M.test@2", "M.foo#1"}});
}

static void test4g() {
  testMaybeRef("test4g",
    R""""(
      module M {
        var global: int;
        proc foo() ref { return global; }         // M.foo
        proc foo() { return global; }             // M.foo#1
        proc acceptsIn(in arg: int) { }
        proc test() {
          acceptsIn(foo());
        }
      }
    )"""",
    {},
    {{"M.test@2", "M.foo#1"}});
}

static void test4h() {
  testMaybeRef("test4h",
    R""""(
      module M {
        var global: int;
        proc foo() ref { return global; }         // M.foo
        proc foo() { return global; }             // M.foo#1
        proc acceptsOut(out arg: int) { }
        proc test() {
          acceptsOut(foo());
        }
      }
    )"""",
    {},
    {{"M.test@2", "M.foo"}});
}

static void test4i() {
  testMaybeRef("test4i",
    R""""(
      module M {
        var global: int;
        proc foo() ref { return global; }         // M.foo
        proc foo() { return global; }             // M.foo#1
        proc acceptsInout(inout arg: int) { }
        proc test() {
          acceptsInout(foo());
        }
      }
    )"""",
    {},
    {{"M.test@2", "M.foo"}});
}

// test const ref/value return intent overload
static void test5a() {
  testMaybeRef("test5a",
    R""""(
      module M {
        var global: int;
        proc foo() const ref { return global; }   // M.foo
        proc foo() { return global; }             // M.foo#1
        proc test() {
          var x = foo();
        }
      }
    )"""",
    {},
    {{"M.test@1", "M.foo#1"}});
}

static void test5b() {
  testMaybeRef("test5b",
    R""""(
      module M {
        var global: int;
        proc foo() { return global; }               // M.foo
        proc foo() const ref { return global; }     // M.foo#1
        proc test() {
          var x = foo();
        }
      }
    )"""",
    {},
    {{"M.test@1", "M.foo"}});
}

static void test5c() {
  testMaybeRef("test5c",
    R""""(
      module M {
        var global: int;
        proc foo() { return global; }               // M.foo
        proc foo() const ref { return global; }     // M.foo#1
        proc test() {
          const ref x = foo();
        }
      }
    )"""",
    {},
    {{"M.test@1", "M.foo#1"}});
}

static void test5d() {
  testMaybeRef("test5d",
    R""""(
      module M {
        var global: int;
        proc foo() { return global; }               // M.foo
        proc foo() const ref { return global; }     // M.foo#1
        proc test() {
          ref x = foo();
        }
      }
    )"""",
    {},
    {{"M.test@1", "M.foo#1"}},
    /* expectErrors */ true);
}

static void test5e() {
  testMaybeRef("test5e",
    R""""(
      module M {
        var global: int;
        proc foo() const ref { return global; }    // M.foo
        proc foo() { return global; }              // M.foo#1
        proc acceptsRef(ref arg: int) { }
        proc test() {
          acceptsRef(foo());
        }
      }
    )"""",
    {},
    {{"M.test@2", "M.foo"}},
    /* expectErrors */ true);
}

static void test5f() {
  testMaybeRef("test5f",
    R""""(
      module M {
        var global: int;
        proc foo() const ref { return global; }    // M.foo
        proc foo() { return global; }              // M.foo#1
        proc acceptsConstRef(const ref arg: int) { }
        proc test() {
          acceptsConstRef(foo());
        }
      }
    )"""",
    {},
    {{"M.test@2", "M.foo"}});
}

static void test5g() {
  testMaybeRef("test5g",
    R""""(
      module M {
        var global: int;
        proc foo() const ref { return global; }   // M.foo
        proc foo() { return global; }             // M.foo#1
        proc acceptsIn(in arg: int) { }
        proc test() {
          acceptsIn(foo());
        }
      }
    )"""",
    {},
    {{"M.test@2", "M.foo#1"}});
}

static void test5h() {
  testMaybeRef("test5h",
    R""""(
      module M {
        var global: int;
        proc foo() const ref { return global; }     // M.foo
        proc foo() { return global; }               // M.foo#1
        proc acceptsOut(out arg: int) { }
        proc test() {
          acceptsOut(foo());
        }
      }
    )"""",
    {},
    {{"M.test@2", "M.foo"}},
    /* expectErrors */ true);
}

static void test5i() {
  testMaybeRef("test5i",
    R""""(
      module M {
        var global: int;
        proc foo() const ref { return global; }  // M.foo
        proc foo() { return global; }            // M.foo#1
        proc acceptsInout(inout arg: int) { }
        proc test() {
          acceptsInout(foo());
        }
      }
    )"""",
    {},
    {{"M.test@2", "M.foo"}},
    /* expectErrors */ true);
}

// test ref/const ref/value return intent overload
static void test6a() {
  testMaybeRef("test6a",
    R""""(
      module M {
        var global: int;
        proc foo() const ref { return global; }   // M.foo
        proc foo() { return global; }             // M.foo#1
        proc foo() ref { return global; }         // M.foo#2
        proc test() {
          var x = foo();
        }
      }
    )"""",
    {},
    {{"M.test@1", "M.foo#1"}});
}

static void test6b() {
  testMaybeRef("test6b",
    R""""(
      module M {
        var global: int;
        proc foo() ref { return global; }           // M.foo
        proc foo() { return global; }               // M.foo#1
        proc foo() const ref { return global; }     // M.foo#2
        proc test() {
          var x = foo();
        }
      }
    )"""",
    {},
    {{"M.test@1", "M.foo#1"}});
}

static void test6c() {
  testMaybeRef("test6c",
    R""""(
      module M {
        var global: int;
        proc foo() { return global; }               // M.foo
        proc foo() const ref { return global; }     // M.foo#1
        proc foo() ref { return global; }           // M.foo#2
        proc test() {
          const ref x = foo();
        }
      }
    )"""",
    {},
    {{"M.test@1", "M.foo#1"}});
}

static void test6d() {
  testMaybeRef("test6d",
    R""""(
      module M {
        var global: int;
        proc foo() ref { return global; }           // M.foo
        proc foo() { return global; }               // M.foo#1
        proc foo() const ref { return global; }     // M.foo#2
        proc test() {
          ref x = foo();
        }
      }
    )"""",
    {},
    {{"M.test@1", "M.foo"}});
}

static void test6e() {
  testMaybeRef("test6e",
    R""""(
      module M {
        var global: int;
        proc foo() const ref { return global; }    // M.foo
        proc foo() ref { return global; }          // M.foo#1
        proc foo() { return global; }              // M.foo#2
        proc acceptsRef(ref arg: int) { }
        proc test() {
          acceptsRef(foo());
        }
      }
    )"""",
    {},
    {{"M.test@2", "M.foo#1"}});
}

static void test6f() {
  testMaybeRef("test6f",
    R""""(
      module M {
        var global: int;
        proc foo() const ref { return global; }    // M.foo
        proc foo() { return global; }              // M.foo#1
        proc foo() ref { return global; }          // M.foo#2
        proc acceptsConstRef(const ref arg: int) { }
        proc test() {
          acceptsConstRef(foo());
        }
      }
    )"""",
    {},
    {{"M.test@2", "M.foo"}});
}

static void test6g() {
  testMaybeRef("test6g",
    R""""(
      module M {
        var global: int;
        proc foo() const ref { return global; }   // M.foo
        proc foo() ref { return global; }         // M.foo#1
        proc foo() { return global; }             // M.foo#2
        proc acceptsIn(in arg: int) { }
        proc test() {
          acceptsIn(foo());
        }
      }
    )"""",
    {},
    {{"M.test@2", "M.foo#2"}});
}

static void test6h() {
  testMaybeRef("test6h",
    R""""(
      module M {
        var global: int;
        proc foo() const ref { return global; }     // M.foo
        proc foo() { return global; }               // M.foo#1
        proc foo() ref { return global; }           // M.foo#2
        proc acceptsOut(out arg: int) { }
        proc test() {
          acceptsOut(foo());
        }
      }
    )"""",
    {},
    {{"M.test@2", "M.foo#2"}});
}

static void test6i() {
  testMaybeRef("test6i",
    R""""(
      module M {
        var global: int;
        proc foo() ref { return global; }        // M.foo
        proc foo() const ref { return global; }  // M.foo#1
        proc foo() { return global; }            // M.foo#2
        proc acceptsInout(inout arg: int) { }
        proc test() {
          acceptsInout(foo());
        }
      }
    )"""",
    {},
    {{"M.test@2", "M.foo"}});
}

static void test7a() {
  testMaybeRef("test7a",
    R""""(
      module M {
        config const cond = false;
        proc foo(pragma "intent ref maybe const formal" arg: int) {
          if cond then foo(arg);
        }
      }
    )"""",
    {{"M.foo@2", false}},
    {},
    /* expectErrors */ true);
}
static void test7b() {
  testMaybeRef("test7b",
    R""""(
      module M {
        config const cond = false;
        proc acceptsRef(ref arg: int) { }
        proc foo(pragma "intent ref maybe const formal" arg: int) {
          if cond then foo(arg);
          acceptsRef(arg);
        }
      }
    )"""",
    {{"M.foo@2", true}},
    {},
    /* expectErrors */ true);
}
static void test7c() {
  testMaybeRef("test7c",
    R""""(
      module M {
        config const cond = false;
        proc acceptsRef(ref arg: int) { }
        proc foo(pragma "intent ref maybe const formal" arg: int) {
          acceptsRef(arg);
          if cond then foo(arg);
        }
      }
    )"""",
    {{"M.foo@2", true}},
    {},
    /* expectErrors */ true);
}
static void test7d() {
  testMaybeRef("test7d",
    R""""(
      module M {
        config const cond = false;
        var global: int;
        proc acceptsRef(ref arg: int) { }
        proc foo(pragma "intent ref maybe const formal" arg: int) {
          acceptsRef(arg);
          if cond then foo(global);
        }
      }
    )"""",
    {{"M.foo@2", true}},
    {},
    /* expectErrors */ false);
}


// TODO: setting a maybe-const formal with =
// TODO: check param loops
// TODO: do we need ref-maybe-const tuples?

int main() {
  test1();
  test2();

  test3a();
  test3b();
  test3c();
  test3d();
  test3e();
  test3f();
  test3g();
  test3h();
  test3i();

  test4a();
  test4b();
  test4c();
  test4d();
  test4e();
  test4f();
  test4g();
  test4h();
  test4i();

  test5a();
  test5b();
  test5c();
  test5d();
  test5e();
  test5f();
  test5g();
  test5h();
  test5i();

  test6a();
  test6b();
  test6c();
  test6d();
  test6e();
  test6f();
  test6g();
  test6h();
  test6i();

  test7a();
  test7b();
  test7c();
  test7d();

  return 0;
}
