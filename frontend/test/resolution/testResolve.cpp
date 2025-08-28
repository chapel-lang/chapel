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
#include "chpl/types/IntType.h"
#include "chpl/types/QualifiedType.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/FnCall.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Variable.h"
#include "chpl/util/version-info.h"

// test resolving a very simple module
static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  {
    context->advanceToNextRevision(true);
    auto path = UniqueString::get(context, "input.chpl");
    std::string contents = "var x: int;\n"
                           "x;";
    setFileText(context, path, contents);

    const ModuleVec& vec = parseToplevel(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    assert(m->numStmts() == 2);
    const Variable* x = m->stmt(0)->toVariable();
    assert(x);
    const Identifier* xIdent = m->stmt(1)->toIdentifier();
    assert(xIdent);

    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

    assert(rr.byAst(x).type().type()->isIntType());
    assert(rr.byAst(xIdent).type().type()->isIntType());
    assert(rr.byAst(xIdent).toId() == x->id());

    context->collectGarbage();
  }
}

// test resolving a module in an incremental manner
static void test2() {
  printf("test2\n");
  Context ctx;
  Context* context = &ctx;

  {
    printf("part 1\n");
    context->advanceToNextRevision(true);
    auto path = UniqueString::get(context, "input.chpl");
    std::string contents = "";
    setFileText(context, path, contents);

    const ModuleVec& vec = parseToplevel(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    resolveModule(context, m->id());

    context->collectGarbage();
  }

  {
    printf("part 2\n");
    context->advanceToNextRevision(true);
    auto path = UniqueString::get(context, "input.chpl");
    std::string contents = "var x;";
    setFileText(context, path, contents);

    const ModuleVec& vec = parseToplevel(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    resolveModule(context, m->id());

    context->collectGarbage();
  }

  {
    printf("part 3\n");
    context->advanceToNextRevision(true);
    auto path = UniqueString::get(context, "input.chpl");
    std::string contents = "var x: int;";
    setFileText(context, path, contents);

    const ModuleVec& vec = parseToplevel(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);

    const Variable* x = m->stmt(0)->toVariable();
    assert(x);

    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    assert(rr.byAst(x).type().type()->isIntType());

    context->collectGarbage();
  }


  // Run it a few times to make sure there aren't errors related to
  // collectGarbage being run across multiple revisions.
  for (int i = 0; i < 3; i++) {
    printf("part %i\n", 3+i);
    context->advanceToNextRevision(true);
    auto path = UniqueString::get(context, "input.chpl");
    std::string contents = "var x: int;\n"
                           "x;";
    setFileText(context, path, contents);

    const ModuleVec& vec = parseToplevel(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    assert(m->numStmts() == 2);
    const Variable* x = m->stmt(0)->toVariable();
    assert(x);
    const Identifier* xIdent = m->stmt(1)->toIdentifier();
    assert(xIdent);

    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

    assert(rr.byAst(x).type().type()->isIntType());
    assert(rr.byAst(xIdent).type().type()->isIntType());
    assert(rr.byAst(xIdent).toId() == x->id());

    context->collectGarbage();
  }
}

static void test3() {
  printf("test3\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");

  {
    printf("part 1\n");
    context->advanceToNextRevision(true);
    std::string contents = "proc foo(arg: int) {\n"
                           "  return arg;\n"
                           "}\n"
                           "var y = foo(1);";
    setFileText(context, path, contents);
    const ModuleVec& vec = parseToplevel(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    const Function* procfoo = m->stmt(0)->toFunction();
    assert(procfoo && procfoo->name() == "foo");
    const Variable* y = m->stmt(1)->toVariable();
    assert(y);
    const AstNode* rhs = y->initExpression();
    assert(rhs);
    const FnCall* fnc = rhs->toFnCall();
    assert(fnc);
    const Identifier* foo = fnc->calledExpression()->toIdentifier();
    assert(foo && foo->name() == "foo");

    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    auto rrfoo = rr.byAst(foo);
    assert(rrfoo.toId() == procfoo->id());

    auto yt = rr.byAst(y).type().type();
    assert(yt->isIntType());

    context->collectGarbage();
  }

  {
    printf("part 2\n");
    context->advanceToNextRevision(true);
    std::string contents = "var y = foo(1);";
    setFileText(context, path, contents);
    const ModuleVec& vec = parseToplevel(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    const Variable* y = m->stmt(0)->toVariable();
    assert(y);
    const AstNode* rhs = y->initExpression();
    assert(rhs);
    const FnCall* fnc = rhs->toFnCall();
    assert(fnc);
    const Identifier* foo = fnc->calledExpression()->toIdentifier();
    assert(foo && foo->name() == "foo");

    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    auto rrfoo = rr.byAst(foo);
    assert(rrfoo.toId().isEmpty());

    auto rry = rr.byAst(y);
    auto yt = rry.type().type();
    assert(yt);
    assert(yt->isErroneousType());

    context->collectGarbage();
  }
}

// this test combines several ideas and is a more challenging
// case for instantiation, conversions, and type construction
static void test4() {
  printf("test4\n");
  auto context = buildStdContext();

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
                           module M {
                             class Parent { }
                             class C : Parent { type t; var x: t; }

                             proc f(in arg: Parent) { }
                             var x: owned C(int);
                             f(x);
                          }
                        )"""";

  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 5);
  const Call* call = m->stmt(4)->toCall();
  assert(call);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  const ResolvedExpression& re = rr.byAst(call);

  assert(re.type().type()->isVoidType());

  auto c = re.mostSpecific().only();
  assert(c);
  assert(c.fn()->untyped()->name() == "f");
}

// this test checks a simple instantiation situation
static void test5() {
  printf("test5\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
                           module M {
                             var x:int(64);
                             var y = x;
                             proc f(arg) { }
                             f(y);
                           }
                        )"""";

  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 4);
  const Call* call = m->stmt(3)->toCall();
  assert(call);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  const ResolvedExpression& re = rr.byAst(call);

  assert(re.type().type()->isVoidType());

  auto c = re.mostSpecific().only();
  assert(c);
  assert(c.fn()->untyped()->name() == "f");

  assert(c.fn()->numFormals() == 1);
  assert(c.fn()->formalName(0) == "arg");
  assert(c.fn()->formalType(0).kind() == QualifiedType::CONST_IN);
  assert(c.fn()->formalType(0).type() == IntType::get(context, 64));
}

// this test checks a particular incremental pattern
// that crashed earlier versions
static void test6() {
  printf("test6\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");

  {
    printf("part 1\n");
    context->advanceToNextRevision(true);
    std::string contents = R""""(
                              module M {
                                var x = 1;
                                proc f() { return x; }
                              }
                           )"""";

    setFileText(context, path, contents);
    const ModuleVec& vec = parseToplevel(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    resolveModule(context, m->id());

    context->collectGarbage();
  }

  {
    printf("part 2\n");
    context->advanceToNextRevision(true);
    std::string contents = R""""(
                              module M {
                                var x = 1;
                                proc f() { return x; }
                                f();
                              }
                           )"""";

    setFileText(context, path, contents);
    const ModuleVec& vec = parseToplevel(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);

    resolveModule(context, m->id());

    context->collectGarbage();
  }
}

// check a parenless function call
static void test7() {
  printf("test7\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
                           module M {
                             proc parenless { return 1; }
                             parenless;
                           }
                        )"""";

  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 2);
  const Identifier* ident = m->stmt(1)->toIdentifier();
  assert(ident);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  const ResolvedExpression& re = rr.byAst(ident);

  assert(re.type().type());
  assert(re.type().type()->isIntType());

  auto c = re.mostSpecific().only();
  assert(c);
  assert(c.fn()->untyped()->name() == "parenless");
  assert(c.fn()->numFormals() == 0);
}

// check a simple recursive function
static void test8() {
  printf("test8\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
                           module M {
                             proc f(arg: int) {
                               f(arg);
                             }
                             var y: int;
                             f(y);
                           }
                        )"""";

  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 3);
  const Call* call = m->stmt(2)->toCall();
  assert(call);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  const ResolvedExpression& re = rr.byAst(call);

  assert(re.type().type()->isVoidType());

  auto c = re.mostSpecific().only();
  assert(c);
  assert(c.fn()->untyped()->name() == "f");

  assert(c.fn()->numFormals() == 1);
  assert(c.fn()->formalName(0) == "arg");
  assert(c.fn()->formalType(0).kind() == QualifiedType::CONST_IN);
  assert(c.fn()->formalType(0).type() == IntType::get(context, 64));
}

// check a generic recursive function
static void test9() {
  printf("test9\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
                           module M {
                             proc f(arg) {
                               f(arg);
                             }
                             var y: int;
                             f(y);
                           }
                        )"""";

  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 3);
  const Call* call = m->stmt(2)->toCall();
  assert(call);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  const ResolvedExpression& re = rr.byAst(call);

  assert(re.type().type()->isVoidType());

  auto c = re.mostSpecific().only();
  assert(c);
  assert(c.fn()->untyped()->name() == "f");

  assert(c.fn()->numFormals() == 1);
  assert(c.fn()->formalName(0) == "arg");
  assert(c.fn()->formalType(0).kind() == QualifiedType::CONST_IN);
  assert(c.fn()->formalType(0).type() == IntType::get(context, 64));
}

// Tests 'const ref' formals disallowing coercion, and that this
// error happens after disambiguation.
static void test10() {
  printf("test10\n");
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
                           module M {
                             class Parent { }
                             class Child : Parent { }

                             /* Both functions should be considered, one
                                should be picked (numeric, since we prefer
                                instantiating), and this function should be
                                rejected. */
                             proc f(const ref arg: Parent, x: int(8)) { }
                             proc f(const ref arg: Parent, x: numeric) { }

                             var x = new Child();
                             var sixtyFourBits: int = 0;
                             f(x, sixtyFourBits);
                          }
                        )"""";

  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 8);
  const Call* call = m->stmt(7)->toCall();
  assert(call);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  const ResolvedExpression& re = rr.byAst(call);

  assert(re.type().type()->isErroneousType());
  assert(guard.numErrors() == 1);
  assert(guard.error(0)->type() == chpl::ConstRefCoercion);
  guard.realizeErrors();
}

// Test transmutation primitives (for params, currently only real(64) -> uint(64)
// is possible since there's no way to get other params of these types.

static void test11() {
  printf("test11\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  std::string contents = R""""(
                          module M {
                            var real32v: real(32);
                            var real64v: real(64);
                            var uint32v: uint(32);
                            var uint64v: uint(64);

                            param x =
                              __primitive("real32 as uint32", real32v).type == uint(32) &&
                              __primitive("real64 as uint64", real64v).type == uint(64) &&
                              __primitive("uint32 as real32", uint32v).type == real(32) &&
                              __primitive("uint64 as real64", uint64v).type == real(64);
                          }
                        )"""";

  auto type = resolveTypeOfXInit(context, contents, /* requireKnown */ true);
  assert(guard.realizeErrors() == 0);
  assert(type.isParamTrue());
}

static void test12() {
  printf("test11\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  std::string contents = R""""(
                          module M {
                            param real64p = 1.0;
                            param x = __primitive("real64 as uint64", real64p);
                          }
                        )"""";

  auto type = resolveTypeOfXInit(context, contents, /* requireKnown */ true);
  assert(guard.realizeErrors() == 0);
  assert(type.isParam() && type.type()->isUintType());
  assert(type.param() && type.param()->isUintParam());
  assert(type.param()->toUintParam()->value() == 4607182418800017408);
}

static void test13() {
  Context context;
  // Make sure no errors make it to the user, even though we will get errors.
  ErrorGuard guard(&context);
  auto variables = resolveTypesOfVariables(&context,
      R"""(
      param r1 = __primitive("version major");
      param r2 = __primitive("version minor");
      param r3 = __primitive("version update");
      param r4 = __primitive("version sha");
      )""", { "r1", "r2", "r3", "r4" });
  ensureParamInt(variables.at("r1"), getMajorVersion());
  ensureParamInt(variables.at("r2"), getMinorVersion());
  ensureParamInt(variables.at("r3"), getUpdateVersion());
  ensureParamString(variables.at("r4"), getIsOfficialRelease() ? "" : getCommitHash());
}

static void test14() {
  auto context = buildStdContext();
  // Make sure no errors make it to the user, even though we will get errors.
  ErrorGuard guard(context);
  auto variables = resolveTypesOfVariablesInit(context,
      R"""(
      param xp = 42;
      var xv = 42;
      const xcv = 42;
      param yp = "hello";
      var yv = "hello";
      const ycv = "hello";

      var r1 = __primitive("addr of", xp);
      var r2 = __primitive("addr of", xv);
      var r3 = __primitive("addr of", xcv);
      var r4 = __primitive("addr of", yp);
      var r5 = __primitive("addr of", yv);
      var r6 = __primitive("addr of", ycv);
      var r7 = __primitive("addr of", int);
      )""", { "r1", "r2", "r3", "r4", "r5", "r6", "r7" });

  auto refInt = QualifiedType(QualifiedType::REF, IntType::get(context, 0));
  auto constRefInt = QualifiedType(QualifiedType::CONST_REF, IntType::get(context, 0));
  auto refStr = QualifiedType(QualifiedType::REF, RecordType::getStringType(context));
  auto constRefStr = QualifiedType(QualifiedType::CONST_REF, RecordType::getStringType(context));

  assert(variables.at("r1") == constRefInt);
  assert(variables.at("r2") == refInt);
  assert(variables.at("r3") == constRefInt);
  assert(variables.at("r4") == constRefStr);
  assert(variables.at("r5") == refStr);
  assert(variables.at("r6") == constRefStr);
  assert(variables.at("r7").isErroneousType());

  // One error for the invalid call of "addr of" with type.
  assert(guard.realizeErrors() == 1);
}

static void test15() {
  Context context;
  // Make sure no errors make it to the user, even though we will get errors.
  ErrorGuard guard(&context);
  auto variables = resolveTypesOfVariablesInit(&context,
      R"""(
      record R {}

      var r: R;
      var x = 42;

      type t0 = __primitive("typeof", int);
      type t1 = __primitive("typeof", r);
      type t2 = __primitive("typeof", x);
      type t3 = __primitive("typeof", 42);
      type t4 = __primitive("typeof", (r, r));

      type t5 = __primitive("static typeof", int);
      type t6 = __primitive("static typeof", r);
      type t7 = __primitive("static typeof", x);
      type t8 = __primitive("static typeof", 42);
      type t9 = __primitive("static typeof", (r, r));
      )""", { "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "t8", "t9"});

  for (auto& pair : variables) {
    if (!pair.second.isErroneousType()) {
      assert(pair.second.isType());
      assert(pair.second.type() != nullptr);
    }
  }

  assert(variables.at("t0").isErroneousType());
  assert(variables.at("t1").type()->isRecordType());
  assert(variables.at("t2").type()->isIntType());
  assert(variables.at("t3").type()->isIntType());
  auto tupQt1 = variables.at("t4");
  auto tupType1 = tupQt1.type()->toTupleType();
  assert(tupType1);
  assert(tupType1->numElements() == 2);
  assert(tupType1->elementType(0).kind() == QualifiedType::VAR);
  assert(tupType1->elementType(0).type()->isRecordType());
  assert(tupType1->elementType(1).kind() == QualifiedType::VAR);
  assert(tupType1->elementType(1).type()->isRecordType());

  assert(variables.at("t5").type()->isIntType());
  assert(variables.at("t6").type()->isRecordType());
  assert(variables.at("t7").type()->isIntType());
  assert(variables.at("t8").type()->isIntType());
  auto tupQt2 = variables.at("t9");
  auto tupType2 = tupQt2.type()->toTupleType();
  assert(tupType2);
  assert(tupType2->numElements() == 2);
  assert(tupType2->elementType(0).kind() == QualifiedType::VAR);
  assert(tupType2->elementType(0).type()->isRecordType());
  assert(tupType2->elementType(1).kind() == QualifiedType::VAR);
  assert(tupType2->elementType(1).type()->isRecordType());

  // One error for the invalid call of "typeof" with type.
  assert(guard.realizeErrors() == 1);
}

static void test16() {
  auto context = buildStdContext();
  // Make sure no errors make it to the user, even though we will get errors.
  ErrorGuard guard(context);
  auto variables = resolveTypesOfVariablesInit(context,
      R"""(
      record Concrete {
          var x: int;
          var y: string;
          var z: (int, string);
      };

      record Generic {
          var x;
          var y;
          var z;
      }

      var conc: Concrete;
      var inst = new Generic(1, "hello", (1, "hello"));

      param r1 = __primitive("static field type", conc, "x") == int;
      param r2 = __primitive("static field type", conc, "y") == string;
      param r3 = __primitive("static field type", conc, "z") == (int, string);
      param r4 = __primitive("static field type", inst, "x") == int;
      param r5 = __primitive("static field type", inst, "y") == string;
      param r6 = __primitive("static field type", inst, "z") == (int, string);
      )""", { "r1", "r2", "r3", "r4", "r5", "r6"});

  for (auto& pair : variables) {
    pair.second.isParamTrue();
  }
}

static void test16b() {
  auto context = buildStdContext();
  // Make sure no errors make it to the user, even though we will get errors.
  ErrorGuard guard(context);
  auto variables = resolveTypesOfVariablesInit(context,
      R"""(
      class Parent {
          var x: int;
          var y: string;
      }

      class Child : Parent {
          var z: (int, string);
      }

      var child = new Child();

      param r1 = __primitive("static field type", child, "x") == int;
      param r2 = __primitive("static field type", child, "y") == string;
      param r3 = __primitive("static field type", child, "z") == (int, string);
      )""", { "r1", "r2", "r3" });

  for (auto& pair : variables) {
    pair.second.isParamTrue();
  }
}

// module-level split-init variables
static void test17() {
  auto context = buildStdContext();

  auto variables = resolveTypesOfVariables(context,
      R"""(
      var foo;
      foo = 5;

      proc setArgToStr(out arg: string) {
        arg = "str";
      }
      var bar;
      setArgToStr(bar);

      param foo_param;
      foo_param = 5;
      param bar_param:string;
      bar_param = "bar_param";

      )""", { "foo", "bar", "foo_param", "bar_param"});

  auto foo = variables.at("foo");
  assert(foo.kind() == QualifiedType::VAR);
  assert(foo.type());
  assert(foo.type()->isIntType());

  auto bar = variables.at("bar");
  assert(bar.kind() == QualifiedType::VAR);
  assert(bar.type());
  assert(bar.type()->isStringType());

  ensureParamInt(variables.at("foo_param"), 5);
  ensureParamString(variables.at("bar_param"), "bar_param");
}

// invalid module-level split-init
static void test18() {
  Context context;
  // Make sure no errors make it to the user, even though we will get errors.
  ErrorGuard guard(&context);

  auto variables = resolveTypesOfVariables(&context,
      R"""(
      var flag = true;
      var foo;
      if (flag) {
        foo = 5;
      } else {
        foo = "asdf";
      }
      )""", {"foo"});

  assert(guard.numErrors() == 1);
  assert(guard.error(0)->type() ==
         ErrorType::SplitInitMismatchedConditionalTypes);

  guard.realizeErrors();
}

// split-init in mutually recursive modules
static void test19() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  std::string contents =
  R""""(
  module M {
    use N;
    var x;
    x = y;
    var z: int;
  }

  module N {
    use M;
    var y;
    y = z;
  }
  )"""";

  // parse modules and get M
  auto path = UniqueString::get(context, "input.chpl");
  setFileText(context, path, std::move(contents));
  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 2);
  const Module* m = vec[0];

  // extract x
  assert(m->numStmts() > 0);
  // hardcoded stmt number
  const Variable* x = m->stmt(1)->toVariable();
  assert(x);
  assert(x->name() == "x");
  // no init expr here, but should get type set from module resolution
  /* auto initExpr = x->initExpression(); */
  /* assert(initExpr); */
  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  // check type
  auto qt = rr.byAst(x).type();
  assert(!qt.isUnknown());
  assert(qt.type()->isIntType());

  guard.realizeErrors();
}

// Accessing the param value of a split-init'd symbol in another module
static void test20() {
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  std::string contents =
      R""""(
      module M {
        module N {
          param CHPL_SOMETHING:string;
          CHPL_SOMETHING = "foo";
        }

        use N;
        param x = CHPL_SOMETHING;
      }
      )"""";

  // parse modules and get M
  auto path = UniqueString::get(context, "input.chpl");
  setFileText(context, path, std::move(contents));
  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0];

  // extract x
  assert(m->numStmts() == 3);
  // hardcoded stmt number
  const Variable* x = m->stmt(2)->toVariable();
  assert(x);
  assert(x->name() == "x");

  // check type
  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  auto qt = rr.byAst(x).type();
  ensureParamString(qt, "foo");

  guard.realizeErrors();
}

// Test resolving functions with pragma "last resort".
static void test21() {
  printf("test21\n");
  Context ctx;
  Context* context = &ctx;

  // Test no ambiguity between nearly-identical functions where just one is last
  // resort.
  {
    printf("part 1\n");
    context->advanceToNextRevision(true);

    auto path = UniqueString::get(context, "input.chpl");
    std::string contents = R""""(
        module M {
          proc foo(x: int) {
            return x;
          }
          pragma "last resort"
          proc foo(y: int) {
            return y;
          }
          var x = foo(4);
        }
                          )"""";

    setFileText(context, path, contents);

    const ModuleVec& vec = parseToplevel(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    assert(m->numStmts() == 3);

    // foo overload with x arg
    const Function* procFooX = m->stmt(0)->toFunction();
    assert(procFooX && procFooX->name() == "foo" && procFooX->numFormals() == 1);
    const NamedDecl* procFooXArg = procFooX->formal(0)->toNamedDecl();
    assert(procFooXArg && procFooXArg->name() == "x");

    // last resort foo overload with y arg
    const Function* procFooY = m->stmt(1)->toFunction();
    assert(procFooY && procFooY->name() == "foo" && procFooY->numFormals() == 1);
    const NamedDecl* procFooYArg = procFooY->formal(0)->toNamedDecl();
    assert(procFooYArg && procFooYArg->name() == "y");

    // variable initialized with foo call
    const Variable* x = m->stmt(2)->toVariable();
    assert(x);
    const AstNode* rhs = x->initExpression();
    assert(rhs);
    const FnCall* fnCall = rhs->toFnCall();
    assert(fnCall);

    // Get called foo
    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    const ResolvedExpression& re = rr.byAst(fnCall);
    auto c = re.mostSpecific().only();
    assert(c);

    // Check we called the correct foo.
    assert(c.fn()->untyped()->name() == "foo");
    assert(c.fn()->numFormals() == 1);
    assert(c.fn()->formalName(0) == "x");

    context->collectGarbage();
  }

  // Test ambiguity between two nearly-identical last resort functions.
  {
    printf("part 2\n");
    context->advanceToNextRevision(true);

    ErrorGuard guard(context);

    auto path = UniqueString::get(context, "input.chpl");
    std::string contents = R""""(
        module M {
          pragma "last resort"
          proc foo(x: int) {
            return x;
          }
          pragma "last resort"
          proc foo(y: int) {
            return y;
          }
          var x = foo(4);
        }
                          )"""";

    setFileText(context, path, contents);

    const ModuleVec& vec = parseToplevel(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    assert(m->numStmts() == 3);

    // variable initialized with foo call
    const Variable* x = m->stmt(2)->toVariable();
    assert(x);
    const AstNode* rhs = x->initExpression();
    assert(rhs);
    const FnCall* fnCall = rhs->toFnCall();
    assert(fnCall);

    // Get called foo
    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    const ResolvedExpression& re = rr.byAst(fnCall);

    // Check foo call is ambiguous
    assert(re.mostSpecific().isAmbiguous());

    // Check variable couldn't resolve
    auto rrx = rr.byAst(x);
    auto xt = rrx.type().type();
    assert(xt);
    assert(xt->isErroneousType());

    assert(guard.numErrors() == 1);
    assert(guard.error(0)->message() ==
           "Cannot resolve call to 'foo': ambiguity");
    guard.realizeErrors();

    context->collectGarbage();
  }

  // Test resolving a last resort that's the only actual option.
  {
    printf("part 3\n");
    context->advanceToNextRevision(true);

    auto path = UniqueString::get(context, "input.chpl");
    std::string contents = R""""(
        module M {
          proc foo(x: string) {
            return x;
          }
          pragma "last resort"
          proc foo(y: int) {
            return y;
          }
          var x = foo(4);
        }
                          )"""";

    setFileText(context, path, contents);

    const ModuleVec& vec = parseToplevel(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    assert(m->numStmts() == 3);

    // variable initialized with foo call
    const Variable* x = m->stmt(2)->toVariable();
    assert(x);
    const AstNode* rhs = x->initExpression();
    assert(rhs);
    const FnCall* fnCall = rhs->toFnCall();
    assert(fnCall);

    // Get called foo
    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    const ResolvedExpression& re = rr.byAst(fnCall);
    auto c = re.mostSpecific().only();
    assert(c);

    // Check we called the correct foo.
    assert(c.fn()->untyped()->name() == "foo");
    assert(c.fn()->numFormals() == 1);
    assert(c.fn()->formalName(0) == "y");
    assert(c.fn()->formalType(0).type());
    assert(c.fn()->formalType(0).type()->isIntType());

    context->collectGarbage();
  }

  // Test resolving between forwarded-to methods where just one is last resort.
  {
    printf("part 4\n");
    context->advanceToNextRevision(true);

    auto path = UniqueString::get(context, "input.chpl");
    std::string contents = R""""(
        module M {
          record MyImpl {
            proc foo(x: int) {
              return x;
            }
            pragma "last resort"
            proc foo(y: int) {
              return y;
            }
          }
          record MyForward {
            forwarding var impl: MyImpl;
          }
          var mf: MyForward;
          var x = mf.foo(4);
        }
                          )"""";

    setFileText(context, path, contents);

    const ModuleVec& vec = parseToplevel(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    assert(m->numStmts() == 4);

    // variable initialized with foo call
    const Variable* x = m->stmt(3)->toVariable();
    assert(x);
    const AstNode* rhs = x->initExpression();
    assert(rhs);
    const FnCall* fnCall = rhs->toFnCall();
    assert(fnCall);

    // Get called foo
    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    const ResolvedExpression& re = rr.byAst(fnCall);
    auto c = re.mostSpecific().only();
    assert(c);

    // Check we called the correct foo.
    assert(c.fn()->untyped()->name() == "foo");
    assert(c.fn()->numFormals() == 2);
    assert(c.fn()->formalName(0) == "this");
    assert(c.fn()->formalName(1) == "x");
    assert(c.fn()->formalType(1).type());
    assert(c.fn()->formalType(1).type()->isIntType());

    context->collectGarbage();
  }

  // Test resolving to a forwarded-to method over a last resort non-forwarded
  // one.
  {
    printf("part 5\n");
    context->advanceToNextRevision(true);

    auto path = UniqueString::get(context, "input.chpl");
    std::string contents = R""""(
        module M {
          record MyImpl {
            proc foo(x: int) {
              return x;
            }
          }
          record MyForward {
            forwarding var impl: MyImpl;

            pragma "last resort"
            proc foo(y: int) {
              return y;
            }
          }
          var mf: MyForward;
          var x = mf.foo(4);
        }
                          )"""";

    setFileText(context, path, contents);

    const ModuleVec& vec = parseToplevel(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    assert(m->numStmts() == 4);

    // variable initialized with foo call
    const Variable* x = m->stmt(3)->toVariable();
    assert(x);
    const AstNode* rhs = x->initExpression();
    assert(rhs);
    const FnCall* fnCall = rhs->toFnCall();
    assert(fnCall);

    // Get called foo
    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    const ResolvedExpression& re = rr.byAst(fnCall);
    auto c = re.mostSpecific().only();
    assert(c);

    // Check we called the correct foo.
    assert(c.fn()->untyped()->name() == "foo");
    assert(c.fn()->numFormals() == 2);
    assert(c.fn()->formalName(0) == "this");
    assert(c.fn()->formalName(1) == "x");
    assert(c.fn()->formalType(1).type());
    assert(c.fn()->formalType(1).type()->isIntType());

    context->collectGarbage();
  }
}

// Resolve calls to 'this' on a field.
static void test22() {
  printf("test22\n");
  Context ctx;
  Context* context = &ctx;

  // Resolve unambiguous call to 'this'.
  {
    printf("part 1\n");
    auto context = buildStdContext();
    ErrorGuard guard(context);

    std::string contents =
        R""""(
        module M {
          record Inner {
            proc this(arg: int) {
              return arg;
            }
          }
          record Outer {
            var inner : Inner;
          }
          var outer : Outer;
          var x = outer.inner(3);
        }
        )"""";

    auto type = resolveTypeOfXInit(context, contents, /* requireKnown */ true);
    assert(type.type());
    assert(!type.isUnknown());
    assert(type.type()->isIntType());

    guard.realizeErrors();
    context->collectGarbage();
  }

  // Above but as param
  {
    printf("part 2\n");
    context->advanceToNextRevision(true);
    ErrorGuard guard(context);

    std::string contents =
        R""""(
        module M {
          record Inner {
            proc this(param arg: int) param {
              return arg;
            }
          }
          record Outer {
            var inner : Inner;
          }
          var outer : Outer;
          param x = outer.inner(3);
        }
        )"""";

    auto type = resolveTypeOfXInit(context, contents, /* requireKnown */ true);
    ensureParamInt(type, 3);

    guard.realizeErrors();
    context->collectGarbage();
  }

  // Non-applicable 'this' candidate
  {
    printf("part 3\n");
    context->advanceToNextRevision(true);
    ErrorGuard guard(context);

    std::string contents =
        R""""(
        module M {
          record Inner {
            proc this() {
              return 3;
            }
          }
          record Outer {
            var inner : Inner;
          }
          var outer : Outer;
          param x = outer.inner(3);
        }
        )"""";

    auto type = resolveTypeOfXInit(context, contents, /* requireKnown */ true);
    assert(type.isErroneousType());
    assert(guard.numErrors() == 1);
    assert(guard.error(0)->type() == chpl::NoMatchingCandidates);

    guard.realizeErrors();

    context->collectGarbage();
  }

  // Normal method and 'this' both applicable.
  // We prefer the 'this' call, but likely want to make this a multiple
  // definition error eventually anyways.
  {
    printf("part 4\n");
    context->advanceToNextRevision(true);
    ErrorGuard guard(context);

    std::string contents =
        R""""(
        module M {
          record Inner {
            proc this(param arg: int) param {
              return 3;
            }
          }
          record Outer {
            var inner : Inner;
            proc inner(param arg: int) param {
              return 5;
            }
          }
          var outer : Outer;
          param x = outer.inner(1);
        }
        )"""";

    auto type = resolveTypeOfXInit(context, contents, /* requireKnown */ true);
    ensureParamInt(type, 3);

    guard.realizeErrors();

    context->collectGarbage();
  }

  // Normal method applicable, 'this' not applicable.
  // This doesn't resolve as we only try to resolve the field's 'this' and not
  // methods with the same name.
  // Like the above, the method and field with the same name should be made a
  // multiple definition error.
  {
    printf("part 5\n");
    context->advanceToNextRevision(true);
    ErrorGuard guard(context);

    std::string contents =
        R""""(
        module M {
          record Inner {
            proc this() param {
              return 3;
            }
          }
          record Outer {
            var inner : Inner;
            proc inner(param arg: int) param {
              return 5;
            }
          }
          operator =(ref lhs: int, const rhs: int) {}
          var outer : Outer;
          param x = outer.inner(1);
        }
        )"""";

    auto type = resolveTypeOfXInit(context, contents, /* requireKnown */ true);
    assert(type.isErroneousType());
    assert(guard.numErrors() == 1);
    assert(guard.error(0)->type() == chpl::NoMatchingCandidates);

    guard.realizeErrors();

    context->collectGarbage();
  }
}

static void test23() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  {
    std::string prog =
      R"""(
      operator =(ref lhs: int, const rhs: int) {}
      record R {
        var x : int;

        proc foo() {
          return 5;
        }
      }

      proc helper() {
        var r : R;
        return r;
      }

      proc foo() {
        return "hello";
      }

      // should be an int, not a string.
      var x = helper().foo();
      )""";

    auto t = resolveTypeOfXInit(context, prog);
    assert(t.type());
    assert(t.type()->isIntType());
    assert(t.type()->toIntType()->isDefaultWidth());
  }

  {
    context->advanceToNextRevision(false);
    std::string prog =
      R"""(
      operator =(ref lhs: int, const rhs: int) {}
      record Inner {
        var x : int;

        proc innerFoo() {
          return x;
        }
      }

      record Outer {
        var inner : Inner;

        proc helper() const ref {
          return inner;
        }
      }

      var o : Outer;
      var x = o.helper().innerFoo();
      )""";

    auto t = resolveTypeOfXInit(context, prog);
    assert(t.type());
    assert(t.type()->isIntType());
    assert(t.type()->toIntType()->isDefaultWidth());
  }
}

static void test24() {
  {
    // straightforward case for qualified module
    Context* context = buildStdContext();
    ErrorGuard guard(context);

    std::string prog =
      R"""(
      module M {
          proc fn() do return 42;
      }
      var x = M.fn();
      )""";

    auto t = resolveTypeOfXInit(context, prog);
    assert(t.type());
    assert(t.type()->isIntType());
    assert(t.type()->toIntType()->isDefaultWidth());

    // Ignore warnings for 'implicit module'.
    assert(guard.realizeErrors(/* countWarnings */ false) == 0);
  }

  {
    // qualified call when POI is involved. Make sure that although the
    // current scope isn't searched for the function, it is still searched
    // for POI functions when resolving the generic function's body.
    Context* context = buildStdContext();
    ErrorGuard guard(context);

    std::string prog =
      R"""(
      module M {
        proc genericFn(x) {
          return foo(x);
        }
      }

      operator =(ref lhs: int, const rhs: int) {}

      record myRecord {}
      proc foo(r: myRecord) do return 42;

      var x = M.genericFn(new myRecord());
      )""";

    auto t = resolveTypeOfXInit(context, prog);
    assert(t.type());
    assert(t.type()->isIntType());
    assert(t.type()->toIntType()->isDefaultWidth());

    // Ignore warnings for 'implicit module'.
    assert(guard.realizeErrors(/* countWarnings */ false) == 0);
  }
  {
    // another POI case, to make sure that the POI-based generic function
    // we just wrote isn't defaulting to some return type.
    Context* context = buildStdContext();
    ErrorGuard guard(context);

    std::string prog =
      R"""(
      module M {
        proc genericFn(x) {
          return foo(x);
        }
      }

      record myRecord1 {}
      proc foo(r: myRecord1) do return 42;

      record myRecord2 {}
      proc foo(r: myRecord2) do return "hello";

      var x = (M.genericFn(new myRecord1()), M.genericFn(new myRecord2()));
      )""";

    auto t = resolveTypeOfXInit(context, prog);
    assert(t.type());
    auto tt = t.type()->toTupleType();
    assert(tt);
    assert(tt->sortedSubstitutions().at(0).second.type()->isIntType());
    assert(tt->sortedSubstitutions().at(1).second.type()->isStringType());

    // Ignore warnings for 'implicit module'.
    assert(guard.realizeErrors(/* countWarnings */ false) == 0);
  }
  {
    // qualified call, but we're not calling a function. Rather, we're invoking
    // an overloaded call operator on a value, which we retrieve from a module.
    Context* context = buildStdContext();
    ErrorGuard guard(context);

    std::string prog =
      R"""(
      module M {
        record hasCallOperator {
          proc this(arg: bool) do return 42;
        }

        var myKindaFn: hasCallOperator;
      }

      var x = M.myKindaFn(true);
      )""";

    auto t = resolveTypeOfXInit(context, prog);
    assert(t.type());
    assert(t.type()->isIntType());
    assert(t.type()->toIntType()->isDefaultWidth());

    // Ignore warnings for 'implicit module'.
    assert(guard.realizeErrors(/* countWarnings */ false) == 0);
  }

  {
    // nested module qualified access should work too.
    Context* context = buildStdContext();
    ErrorGuard guard(context);

    std::string prog =
      R"""(
      module M {
        module N {
          proc fn() do return 42;
        }
      }
      var x = M.N.fn();
      )""";

    auto t = resolveTypeOfXInit(context, prog);
    assert(t.type());
    assert(t.type()->isIntType());
    assert(t.type()->toIntType()->isDefaultWidth());

    // Ignore warnings for 'implicit module'.
    assert(guard.realizeErrors(/* countWarnings */ false) == 0);
  }
}

static void test25() {
  // Test that 'none' has type 'nothing'
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  {
    // straightforward case for qualified module
    std::string prog = "var x = none;";

    auto t = resolveTypeOfXInit(context, prog);
    assert(t.type());
    assert(t.type()->isNothingType());
    assert(t.isParam());
    assert(t.param());
    assert(t.param()->isNoneParam());
    assert(guard.realizeErrors() == 0);
  }
}

static void test26() {
  // Test resolving a generic type field usage in a method signature.

  {
    Context* context = buildStdContext();
    ErrorGuard guard(context);
    // 'this' qualified, type field
    std::string prog =
      R"""(
        class Foo {
          type myType = string;
          proc doSomething(x : this.myType) param do return 1;
        }

        var myFoo = new Foo(int);
        var x = myFoo.doSomething(1);
      )""";

    auto t = resolveTypeOfXInit(context, prog);
    ensureParamInt(t, 1);
    assert(guard.realizeErrors() == 0);
  }

  {
    Context* context = buildStdContext();
    ErrorGuard guard(context);
    // unqualified, type field
    std::string prog =
      R"""(
        class Foo {
          type myType = string;
          proc doSomething(x : myType) param do return 1;
        }

        var myFoo = new Foo(int);
        var x = myFoo.doSomething(1);
      )""";

    auto t = resolveTypeOfXInit(context, prog);
    ensureParamInt(t, 1);
    assert(guard.realizeErrors() == 0);
  }

  {
    Context* context = buildStdContext();
    ErrorGuard guard(context);
    // 'this' qualified, type parenless proc, concrete receiver
    std::string prog =
      R"""(
        class Foo {
          proc myType type do return int;
          proc doSomething(x : this.myType) param do return 1;
        }

        var myFoo = new Foo();
        var x = myFoo.doSomething(1);
      )""";

    auto t = resolveTypeOfXInit(context, prog);
    ensureParamInt(t, 1);
    assert(guard.realizeErrors() == 0);
  }

  {
    Context* context = buildStdContext();
    ErrorGuard guard(context);
    // unqualified, type parenless proc, concrete receiver
    std::string prog =
      R"""(
        class Foo {
          proc myType type do return int;
          proc doSomething(x : myType) param do return 1;
        }

        var myFoo = new Foo();
        var x = myFoo.doSomething(1);
      )""";

    auto t = resolveTypeOfXInit(context, prog);
    ensureParamInt(t, 1);
    assert(guard.realizeErrors() == 0);
  }

  {
    Context* context = buildStdContext();
    ErrorGuard guard(context);
    // 'this' qualified, type parenless proc, generic receiver
    std::string prog =
      R"""(
        class Foo {
          type idxType;
          proc myType type do return idxType;
          proc doSomething(x : this.myType) param do return 1;
        }

        var myFoo = new Foo(int);
        var x = myFoo.doSomething(1);
      )""";

    auto t = resolveTypeOfXInit(context, prog);
    ensureParamInt(t, 1);
    assert(guard.realizeErrors() == 0);
  }

  {
    Context* context = buildStdContext();
    ErrorGuard guard(context);
    // unqualified, type parenless proc, generic receiver
    std::string prog =
      R"""(
        class Foo {
          type idxType;
          proc myType type do return idxType;
          proc doSomething(x : myType) param do return 1;
        }

        var myFoo = new Foo(int);
        var x = myFoo.doSomething(1);
      )""";

    auto t = resolveTypeOfXInit(context, prog);
    ensureParamInt(t, 1);
    assert(guard.realizeErrors() == 0);
  }
}

// Make sure that 'extern' functions still have 'ResolvedFunction' entries.
static void test27() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  // 'this' qualified, type field
  std::string prog =
    R"""(
      extern proc foo(): int;
    )""";

  auto m = parseModule(context, prog);
  auto f = m->stmt(0)->toFunction();
  assert(f && f->linkage() == Decl::EXTERN);
  auto rf = resolveConcreteFunction(context, f->id());
  assert(rf->returnType().type()->isIntType());
  assert(guard.realizeErrors() == 0);
}

// Test resolving logical AND/OR compound assignment operators
static void test28() {
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  std::string prog =
    R"""(
    proc baz() {
      var ok = true;
      ok &&= false;
      ok ||= true;
      return ok;
    }
    var x = baz();
    )""";

  auto t = resolveTypeOfXInit(context, prog);
  CHPL_ASSERT(!t.isUnknownOrErroneous());
  CHPL_ASSERT(t.type()->isBoolType());

  assert(guard.realizeErrors() == 0);
}

// This bug is hard to replicate with queries alone, but does seem to show
// up in some cases of the query system.
static void testInfiniteCycleBug() {
  auto context = buildStdContext();

  CompilerFlags flags;
  flags.set(CompilerFlags::WARN_UNSTABLE, true);
  setCompilerFlags(context, std::move(flags));

  std::string program0 =
    R""""(
    proc foo() {
      var x = 0;
      proc bar() { return x; }
      return bar();
    }
    var x = foo();
    )"""";

  std::ignore = resolveQualifiedTypeOfX(context, program0);

  context = buildStdContext();

  std::string program1 =
    R""""(
    proc baz() {
      var x = 0;
      proc ding() { return x; }
      return bar();
    }
    var x = baz();
    )"""";

  std::ignore = resolveQualifiedTypeOfX(context, program1);
}

// a callable formal (like a tuple) is preferred to functions in outer
// scopes.
static void testFormalFunctionShadowing() {
  std::string program =
    R"""(
    record R { proc this(x: int) do return 42; }

    proc foo(x) do return 1.0;
    proc foo(x: int) do return new R();
    proc bar(foo: R) do return foo(0);

    var x = bar(new R());
    )""";

  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto t = resolveTypeOfXInit(context, program);
  CHPL_ASSERT(!t.isUnknownOrErroneous());
  CHPL_ASSERT(t.type()->isIntType());
}

// a callable formal (like a tuple) interrupts the search for functions as
// an optimization for "distance" (any functions beyond the callable formal
// are further away than any functions we've already found).
static void testFunctionFormalShadowing() {
  std::string program =
    R"""(
    record R { proc this(x: int) do return 42; }

    proc foo(x: int) do return new R();
    proc bar(foo: R) {
      proc foo(x) do return 1.0;
      return foo(0);
    }

    var x = bar(new R());
    )""";

  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto t = resolveTypeOfXInit(context, program);
  CHPL_ASSERT(!t.isUnknownOrErroneous());
  CHPL_ASSERT(t.type()->isRealType());
}

// Today, we don't perform overload selection for callable objects. Instead,
// expect an error to be issued.
static void testCallableAmbiguity() {
  std::string program =
    R"""(
    module Lib {
      record R {
        proc this() do return 42;
      }
    }
    module M1 { use Lib; var x: R; }
    module M2 { use Lib; var x: R; }
    module M3 {
      use M1;
      use M2;

      var y = x(0);
    }
    )""";

  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  std::ignore = resolveTypesOfVariables(context, program, { "y" });
  assert(guard.realizeErrors());
}

// Test use of the 'scalar promotion type' primitive.
// Implementation of getting promotion types is tested more thoroughly
// elsewhere, so this is just a very basic test the prims works as expected.
static void testPromotionPrim() {
  {
    Context* context = buildStdContext();
    ErrorGuard guard(context);

    std::string prog =
      R"""(
        var d : domain(1, real);
        type t = __primitive("scalar promotion type", d);
        param x = (t == real);
      )""";

    auto x = resolveTypeOfXInit(context, prog);
    ensureParamBool(x, true);

    assert(guard.realizeErrors() == 0);
  }

  {
    Context* context = buildStdContext();
    ErrorGuard guard(context);

    std::string prog =
      R"""(
        type t = __primitive("scalar promotion type", int);
        param x = (t == int);
      )""";

    auto x = resolveTypeOfXInit(context, prog);
    ensureParamBool(x, true);

    assert(guard.realizeErrors() == 0);
  }
}

// Test the '_wide_get_locale' primitive.
static void testGetLocalePrim() {
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  auto variables = resolveTypesOfVariables(context,
    R"""(
      var x : real;
      var locId = __primitive("_wide_get_locale", x);
      var sublocId = chpl_sublocFromLocaleID(locId);
    )""", { "locId", "sublocId" });

  auto locId = variables.at("locId");
  assert(locId.type());
  assert(locId.type() == CompositeType::getLocaleIDType(context));
  auto sublocId = variables.at("sublocId");
  assert(sublocId.type());
  assert(sublocId.type()->isIntType());

  assert(guard.realizeErrors() == 0);
}

static void testArrayGetPrim() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto variables = resolveTypesOfVariablesInit(context,
    R"""(
      use CTypes;

      var a: c_ptr(int);
      var b: c_ptrConst(bool);
      var c: _ddata(real);
      var d: c_array(int(8), 4);

      var r1 = __primitive("array_get", a, 0);
      var r2 = __primitive("array_get", b, 0);
      var r3 = __primitive("array_get", c, 0);
      var r4 = __primitive("array_get", d, 0);
    )""", { "r1", "r2", "r3", "r4" });

  auto& r1 = variables.at("r1");
  assert(r1.kind() == QualifiedType::REF);
  assert(r1.type()->isIntType() && r1.type()->toIntType()->isDefaultWidth());

  // note: in production, even const c_ptr returns non-const REF from
  // this call.
  auto& r2 = variables.at("r2");
  assert(r2.kind() == QualifiedType::REF);
  assert(r2.type()->isBoolType());

  auto& r3 = variables.at("r3");
  assert(r3.kind() == QualifiedType::REF);
  assert(r3.type()->isRealType() && r3.type()->toRealType()->isDefaultWidth());

  auto& r4 = variables.at("r4");
  assert(r4.kind() == QualifiedType::REF);
  assert(r4.type()->isIntType() && r4.type()->toIntType()->bitwidth() == 8);
}

static void testAsciiPrim() {
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  auto variables = resolveTypesOfVariables(context,
    R"""(
      param b = __primitive("ascii", "b");
      param h = __primitive("ascii", "hi", 0);
      param i = __primitive("ascii", "hi", 1);
      param c = __primitive("ascii", b"c");
      param p = __primitive("ascii", b"po", 0);
      param o = __primitive("ascii", b"po", 1);
    )""", { "b", "h", "i", "c", "p", "o" });

  auto b = variables.at("b");
  ensureParamUint(b, 98);

  auto h = variables.at("h");
  ensureParamUint(h, 104);
  auto i = variables.at("i");
  ensureParamUint(i, 105);

  auto c = variables.at("c");
  ensureParamUint(c, 99);

  auto p = variables.at("p");
  ensureParamUint(p, 112);
  auto o = variables.at("o");
  ensureParamUint(o, 111);
}

// Test the '.locale' query.
static void testDotLocale() {
  Context* context = buildStdContext();
  ErrorGuard guard(context);

  auto loc = resolveTypeOfXInit(context,
    R"""(
      var myVar : int;
      var x = myVar.locale;
    )""");

  assert(loc.kind() == QualifiedType::CONST_VAR);
  assert(loc.type());
  assert(loc.type() == CompositeType::getLocaleType(context));

  assert(guard.realizeErrors() == 0);
}

// .bytes() should be rewritten to .chpl_bytes()
static void testDotBytes() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto variables = resolveTypesOfVariables(context,
    R"""(
      var str = "hello";
      var bts = b"hello";

      var x = str.bytes();
      var y = bts.bytes();
    )""", { "x", "y" });

  for (auto& kv : variables) {
    auto& qt = kv.second;

    assert(!qt.isUnknownOrErroneous());
    assert(qt.type()->isArrayType());
    assert(qt.type()->toArrayType()->eltType().type()->isUintType());
    assert(qt.type()->toArrayType()->eltType().type()->toUintType()->bitwidth() == 8);
  }
}

// even if a formal's type has defaults, if it's explicitly made generic
// with (?) it should not be concrete.
static void testExplicitlyGenericFormal() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);
  auto qt = resolveTypeOfXInit(context,
    R"""(
      record R {
        type myType = int;
      }
      var intR: R(int);
      var realR: R(real);

      proc foo(x: R(?) = intR) do return x;
      var x = foo(realR);
    )""");

  CHPL_ASSERT(qt.type()->isRecordType());
  CHPL_ASSERT(qt.type()->toRecordType()->name() == "R");
  for (auto& sub : qt.type()->toRecordType()->substitutions()) {
    CHPL_ASSERT(sub.second.type()->isRealType());
  }
}

static void testGlobalMultiDecl() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto xQt = resolveTypeOfXInit(context,
    R"""(
      operator =(ref lhs: int, const rhs: int) {}
      var a, b: int;
      proc foo() do return a;
      var x = foo();
    )""");

  assert(xQt.type()->isIntType());
}

// Ensure that we don't attempt to resolve functions with actuals that are
// generic but would never be generic during a "real" invocation. This
// is a quirk of "initial signatures", which do not use instantiation
// info, and thus occasionally have generic formals where we don't expect.
static void testGenericTypeInInitialSignature() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  std::ignore = resolveTypeOfXInit(context,
    R"""(
      record R { type field; }

      proc compilerWarning(param msg: string...) {
        __primitive("warning");
      }

      proc foo(type t) type {
        if t == R(?) then compilerWarning("t is generic");
        return t;
      }

      proc sig(x: R, y: foo(x.type)) do return x;

      var x = sig(new R(int), new R(int));
    )""");


  // No warnings should have been emitted, because the call to 'foo()' during
  // resolving the initial signature should've been skipped.
}

// Ensure that conditinal early returns in 'param' code are respected.
static void testEarlyReturn() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto qt = resolveTypeOfXInit(context,
    R"""(
      record R { type field; }

      proc compilerError(param msg: string...) {
        __primitive("error");
      }

      proc foo(type t) type {
        if t == int then return uint;
        compilerError("case not handled");
      }

      type x = foo(int);
    )""");
  CHPL_ASSERT(qt.type()->isUintType());

  // guard should have no errors
}

// Ensure that if execution is guaranteed to return in all paths, even if
// the exact path is not known at compile time, subsequent
// code like compilerError is not resolved.
static void testRuntimeEarlyReturn() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto qt = resolveTypeOfXInit(context,
    R"""(
      record R { type field; }

      proc compilerError(param msg: string...) {
        __primitive("error");
      }

      proc foo(type t) {
        var cond: bool;
        if cond {
          return 1;
        } else {
          return 2;
        }
        compilerError("case not handled");
      }

      var x = foo(int);
    )""");
  CHPL_ASSERT(qt.type()->isIntType());

  // guard should have no errors
}

// Ensure code after continue / break is not resolved in param loops.
static void testEarlyContinue() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::ignore = resolveTypeOfXInit(context,
    R"""(
      for param i in 1..3 {
        if i == 1 then continue;
        if i == 2 then break;
        if i == 3 {
          var cond: bool;
          if cond then {
            continue;
          } else {
            continue;
          }
        }
        compilerError("noooo", 0);
      }
      var x = 42;
    )""");

  // guard should have no errors
}

// Ensure code after continue / break is not resolved in param loops.
static void testEarlyRuntimeContinue() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  std::ignore = resolveTypeOfXInit(context,
    R"""(
      for i in 1..3 {
        var cond: bool;
        if cond then {
          continue;
        } else {
          continue;
        }
        compilerError("noooo", 0);
      }
      var x = 42;
    )""");

  // guard should have no errors
}

static void testGenericSync() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto qt = resolveTypeOfXInit(context,
    R"""(
      proc foo (x: sync) do return 3;
      var a : sync int;
      var x = foo(a);
    )""");

  assert(qt.type() && qt.type()->isIntType());

  // guard should have no errors
}

static void testUseOfUninitializedVar() {
  auto context = buildStdContext();
  ErrorGuard guard(context);
  auto qt = resolveTypeOfXInit(context,
    R"""(
       proc foo() {
        var y;
        return y;
       }
       var x = foo();
    )""");
  assert(qt.isUnknownKindOrType());
  // expect uninitialized var y, can't establish type for call expression foo()
  assert(guard.realizeErrors() == 2);
}

/* test cases copied from testTypePropertyPrimitives */
static void testAnyPod() {
  auto base =
    /* prelude */ R"""(
            pragma "ignore noinit"
            record r1 {}
            record r2 { var x: int; var y: real; }
            record r3 { var x: int; var y: real; var z: integral; }
            record r4 { var x: r2; var y: int; }
            record r5 {
              proc deinit() {}
            }
            record r6 {
              proc init=(rhs: r6) {}
            }
            record r7 {
              operator=(lhs: r7, rhs: r7) {}
            }
            record r8 {}
            operator=(lhs: r8, rhs: r8) {}
            // Should be marked POD irregardless of the generic.
            pragma "plain old data"
            record r9 { type T; var x: T; }
            class c1 { var x: int; }
            record r10 { var x: owned c1?; }
            record r11 { var x: r9(?); }
            operator =(ref lhs: int, const rhs: int) {}
            operator =(ref lhs: real, const rhs: real) {}
            )""";

  auto runTest = [base](const char* type, bool shouldWork) {
    auto ctx = buildStdContext();
    ErrorGuard guard(ctx);

    auto fullProg = std::string(base) +
      "proc foo(type arg: chpl_anyPOD) param do return true;\n" +
      "pragma \"last resort\" proc foo(type arg) param do return false;\n" +
      "param x = foo(" + std::string(type) + ");\n";

    auto qt = resolveTypeOfXInit(ctx, fullProg);
    ensureParamBool(qt, shouldWork);
  };

  const char* podTypes[] = {
    "bool", "int", "int(8)", "int(16)", "int(32)",
    "int(64)", "uint", "uint(8)", "uint(16)",
    "uint(32)", "uint(64)", "real(32)", "real(64)",
    "complex", "imag",
    "r2", "r4", "r9",
    "borrowed c1", "borrowed c1?", "unmanaged c1", "unmanaged c1?",
  };
  for (size_t i = 0; i < sizeof(podTypes) / sizeof(podTypes[0]); i++) {
    runTest(podTypes[i], true);
  }

  // skip "integral", because it's a built-in generic type and we don't
  // resolve calls with this type as an actual.
  const char* nonPodTypes[] = {
    /* "integral", */ "r1", "r3", "r5", "r6", "r7", "r8", "r10",
    // TODO: Currently marked as non-POD even though all the members are
    // marked as POD - this is because 'r9' is technically generic, which
    // causes problems.
    "r11",
    "c1", "owned c1", "owned c1?", "shared c1", "shared c1?",
  };
  for (size_t i = 0; i < sizeof(nonPodTypes) / sizeof(nonPodTypes[0]); i++) {
    runTest(nonPodTypes[i], false);
  }

  // TODO:(these are todo'd in the original test file, too)
  // { {"atomic int"}, Test::FALSE },
  // { {"single int"}, Test::FALSE },
  // { {"sync int"}, Test::FALSE },
}

static void testTupleFormalWithDefault() {
  auto context = buildStdContext();
  ErrorGuard guard(context);
  auto qt = resolveTypeOfXInit(context,
    R"""(
       proc foo((x, y) = (1, 2)) {
         return x + y;
       }
       var x = foo((3,4));
    )""");
  assert(!qt.isUnknownOrErroneous());
  assert(qt.type()->isIntType());
}

// regression test. For generic types that used their type parameter
// as an argument to a 'new' of another type, while computing their generic
// type, we'd end up resolving 'new C(unknown)', which caused issues. Now, as
// in other cases like 'foo(unknown)', we skip resolving such calls.
static void testSkipUnknownInNew() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto qt = resolveTypeOfXInit(context,
    R"""(
    class C {
      type argT;
    }

    record R {
      type argT;
      var myC = new unmanaged C(argT);
    }

    var x = new R(int);
  )""");
  assert(!qt.isUnknownOrErroneous());
  assert(qt.type()->isRecordType());
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  test9();
  test10();
  test11();
  test12();
  test13();
  test14();
  test15();
  test16();
  test16b();
  test17();
  test18();
  test19();
  test20();
  test21();
  test22();
  test23();
  test24();
  test25();
  test26();
  test27();
  test28();

  testInfiniteCycleBug();

  testFormalFunctionShadowing();
  testFunctionFormalShadowing();
  testCallableAmbiguity();

  testPromotionPrim();
  testGetLocalePrim();
  testArrayGetPrim();
  testAsciiPrim();

  testDotLocale();
  testDotBytes();

  testExplicitlyGenericFormal();

  testGlobalMultiDecl();

  testGenericTypeInInitialSignature();

  testEarlyReturn();
  testRuntimeEarlyReturn();
  testEarlyContinue();
  testEarlyRuntimeContinue();

  testGenericSync();

  testUseOfUninitializedVar();

  testAnyPod();
  testTupleFormalWithDefault();

  testSkipUnknownInNew();

  return 0;
}
