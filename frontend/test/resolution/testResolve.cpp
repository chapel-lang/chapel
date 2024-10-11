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
  Context ctx;
  Context* context = &ctx;

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
  Context ctx;
  Context* context = &ctx;
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

                             var x: owned Child;
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
  Context context;
  // Make sure no errors make it to the user, even though we will get errors.
  ErrorGuard guard(&context);
  auto variables = resolveTypesOfVariablesInit(&context,
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

  auto refInt = QualifiedType(QualifiedType::REF, IntType::get(&context, 0));
  auto constRefInt = QualifiedType(QualifiedType::CONST_REF, IntType::get(&context, 0));
  auto refStr = QualifiedType(QualifiedType::REF, RecordType::getStringType(&context));
  auto constRefStr = QualifiedType(QualifiedType::CONST_REF, RecordType::getStringType(&context));

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
  Context context;
  // Make sure no errors make it to the user, even though we will get errors.
  ErrorGuard guard(&context);
  auto variables = resolveTypesOfVariablesInit(&context,
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
      var inst: Generic(int, string, (int, string));

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

// module-level split-init variables
static void test17() {
  Context context;

  auto variables = resolveTypesOfVariables(&context,
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
  Context ctx;
  Context* context = &ctx;
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
    context->advanceToNextRevision(true);
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
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  {
    // straightforward case for qualified module
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
    context->advanceToNextRevision(false);
    std::string prog =
      R"""(
      module M {
        proc genericFn(x) {
          return foo(x);
        }
      }

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
    context->advanceToNextRevision(false);
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
    context->advanceToNextRevision(false);
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

  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  {
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

    context->advanceToNextRevision(false);
  }

  {
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

    context->advanceToNextRevision(false);
  }

  {
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

    context->advanceToNextRevision(false);
  }

  {
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

    context->advanceToNextRevision(false);
  }

  {
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

    context->advanceToNextRevision(false);
  }

  {
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

    context->advanceToNextRevision(false);
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

static void test28(Context* context) {
  // Test that if we fail to resolve a call to an iterator, but there's
  // a tagged overload (e.g., a leader or a follower), that we find the tagged
  // overload instead.
  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  ErrorGuard guard(context);

  std::string prog =
    R"""(
      iter foo(param tag) where tag == iterKind.standalone {
        yield 1;
        yield 2;
        yield 3;
      }
      var loop = foo();
      forall i in loop {}
      forall j in foo() {}
    )""";

  auto vars = resolveTypesOfVariables(context, prog, {"loop", "i", "j"});

  CHPL_ASSERT(vars.at("loop").type());
  CHPL_ASSERT(vars.at("loop").type()->isIteratorType());
  CHPL_ASSERT(vars.at("i").type());
  CHPL_ASSERT(vars.at("i").type()->isIntType());
  CHPL_ASSERT(vars.at("i").type()->toIntType()->isDefaultWidth());
  CHPL_ASSERT(vars.at("i") == vars.at("j"));
}

static void test29(Context* context) {
  // Test that if we fail to resolve a call to a non-iterator, but there's
  // a tagged overload (e.g., a leader or a follower), that we _don't_ find
  // the tagged overload, since procs can't be tagged.
  ADVANCE_PRESERVING_STANDARD_MODULES_(context);

  std::string prog =
    R"""(
      proc foo(param tag) where tag == iterKind.standalone {
        yield 1;
        yield 2;
        yield 3;
      }
      var loop = foo();
      forall i in loop {}
      forall j in foo() {}
    )""";

  auto vars = resolveTypesOfVariables(context, prog, {"loop", "i", "j"});
  for (auto var : vars) {
    CHPL_ASSERT(var.second.isUnknownOrErroneous());
  }
}

// This bug is hard to replicate with queries alone, but does seem to show
// up in some cases of the query system.
static void testInfiniteCycleBug() {
  auto context = buildStdContext();
  auto ctx = context.get();

  ctx->advanceToNextRevision(false);
  setupModuleSearchPaths(ctx, false, false, {}, {});

  CompilerFlags flags;
  flags.set(CompilerFlags::WARN_UNSTABLE, true);
  setCompilerFlags(ctx, std::move(flags));

  std::string program0 =
    R""""(
    proc foo() {
      var x = 0;
      proc bar() { return x; }
      return bar();
    }
    var x = foo();
    )"""";

  std::ignore = resolveQualifiedTypeOfX(ctx, program0);

  ctx->advanceToNextRevision(false);
  setupModuleSearchPaths(ctx, false, false, {}, {});

  std::string program1 =
    R""""(
    proc baz() {
      var x = 0;
      proc ding() { return x; }
      return bar();
    }
    var x = baz();
    )"""";

  std::ignore = resolveQualifiedTypeOfX(ctx, program1);
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

  auto ctx = buildStdContext();
  test28(ctx.get());
  test29(ctx.get());

  testInfiniteCycleBug();

  return 0;
}
