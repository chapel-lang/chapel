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
#include "chpl/types/all-types.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

static void test1() {
  // make sure that function return type computation does not throw
  // away the param.
  auto context = buildStdContext();
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         proc p(param x: int(64), param y: int(64)) param do return __primitive("+", x, y);

                         param x = p(1,2);
                         )"""");
  assert(qt.kind() == QualifiedType::PARAM);
  auto typePtr = qt.type();
  auto paramPtr = qt.param();
  assert(typePtr && typePtr->isIntType());
  assert(paramPtr && paramPtr->isIntParam() && paramPtr->toIntParam()->value() == 3);
}

static void test2() {
  // make sure unknown types don't slip into candidate search
  auto context = buildStdContext();
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         proc test(z) {}
                         var x = test(y);
                         )"""", false);
  assert(qt.isUnknown());
}

static void helpTest3(const std::string& theFunction) {
  // Make sure that types depending on earlier actual types are properly
  // enforced
  {
    auto context = buildStdContext();
    auto qt = resolveTypeOfXInit(context, theFunction +
                                 R""""(
                                 var x = f(true, "hello", "world");
                                 )"""");
    assert(qt.type() != nullptr);
    assert(qt.type()->isStringType());
  }
  {
    auto context = buildStdContext();
    auto qt = resolveTypeOfXInit(context, theFunction +
                                 R""""(
                                 var x = f(false, 0, 1);
                                 )"""");
    assert(qt.type() != nullptr);
    assert(qt.type()->isIntType());
  }
  {
    auto context = buildStdContext();
    auto qt = resolveTypeOfXInit(context, theFunction +
                                 R""""(
                                 var x = f(true, 0, 1);
                                 )"""");
    assert(qt.isUnknown());
  }
  {
    auto context = buildStdContext();
    auto qt = resolveTypeOfXInit(context, theFunction +
                                 R""""(
                                 var x = f(false, "hello", "world");
                                 )"""");
    assert(qt.isUnknown());
  }
  {
    auto context = buildStdContext();
    auto qt = resolveTypeOfXInit(context, theFunction +
                                 R""""(
                                 var x = f(true, "hello", 0);
                                 )"""");
    assert(qt.isUnknown());
  }
}

static void test3a() {
  const std::string theFunction =
    R""""(
    proc f(param a: bool,
           const ref b: if a then string else int,
           const ref c: b.type) const ref {
      return c;
    })"""";
  helpTest3(theFunction);
}

static void test3b() {
  const std::string theFunction =
    R""""(
    proc f(param a: bool,
           b: if a then string else int,
           c: b.type) {
      return c;
    })"""";
  helpTest3(theFunction);
}

// Test calling dependently typed type constructor, for type with param field
static void test4() {
  // With param field type declared
  {
    printf("part 1\n");
    auto context = buildStdContext();
    ErrorGuard guard(context);

    const std::string program =
      R""""(
      record Foo {
        param fooStrides : int;
      }
      proc bar(param strides = 1, other: Foo(strides)) {
        return other.fooStrides;
      }
      var x = bar(1, new Foo(1));
      )"""";

    auto qt = resolveTypeOfXInit(context, program);
    for (auto& e : guard.errors()) std::cout << e->message() << std::endl;
    assert(qt.type() != nullptr);
    assert(qt.type()->isIntType());

    context->collectGarbage();
  }

  // Param field of generic type
  {
    printf("part 2\n");
    auto context = buildStdContext();

    const std::string program =
      R""""(
      record Foo {
        param fooStrides;
      }
      proc bar(param strides = 1, other: Foo(strides)) {
        return other.fooStrides;
      }
      var x = bar(1, new Foo(1));
      )"""";

    auto qt = resolveTypeOfXInit(context, program);
    assert(qt.type() != nullptr);
    assert(qt.type()->isIntType());

    context->collectGarbage();
  }

  // With param formal type declared
  {
    printf("part 3\n");
    auto context = buildStdContext();

    const std::string program =
      R""""(
      record Foo {
        param fooStrides : int;
      }
      proc bar(param strides : int, other: Foo(strides)) {
        return other.fooStrides;
      }
      var x = bar(1, new Foo(1));
      )"""";

    auto qt = resolveTypeOfXInit(context, program);
    assert(qt.type() != nullptr);
    assert(qt.type()->isIntType());

    context->collectGarbage();
  }
}

// Test calling dependently typed type constructor, in a type formal
static void test5() {
  // With param field type declared
  {
    printf("part 1\n");
    auto context = buildStdContext();
    ErrorGuard guard(context);

    const std::string program =
      R""""(
      record Foo {
        param fooField : int;
      }
      proc bar(param strides = 1, type other: Foo(strides)) param {
        return other.fooField;
      }
      var x = bar(1, Foo(1));
      )"""";

    auto qt = resolveTypeOfXInit(context, program);
    for (auto& e : guard.errors()) std::cout << e->message() << std::endl;
    assert(qt.type() != nullptr);
    ensureParamInt(qt, 1);

    context->collectGarbage();
  }

  // Param field of generic type
  {
    printf("part 2\n");
    auto context = buildStdContext();
    ErrorGuard guard(context);

    const std::string program =
      R""""(
      record Foo {
        param fooField;
      }
      proc bar(param strides = 1, type other: Foo(strides)) param {
        return other.fooField;
      }
      var x = bar(1, Foo(1));
      )"""";

    auto qt = resolveTypeOfXInit(context, program);
    for (auto& e : guard.errors()) std::cout << e->message() << std::endl;
    assert(qt.type() != nullptr);
    ensureParamInt(qt, 1);

    context->collectGarbage();
  }

  // type field
  {
    printf("part 3\n");
    auto context = buildStdContext();
    ErrorGuard guard(context);

    const std::string program =
      R""""(
      record Foo {
        type fooField;
      }
      proc bar(x, type other: Foo(x.type)) type {
        return other.fooField;
      }
      type x = bar(1, Foo(int));
      )"""";

    auto qt = resolveTypeOfXInit(context, program);
    for (auto& e : guard.errors()) std::cout << e->message() << std::endl;
    assert(qt.type() != nullptr);
    assert(qt.type()->isIntType());
    assert(qt.kind() == QualifiedType::TYPE);

    context->collectGarbage();
  }
}

// Test that formals with default values but no type expression
// get proper (non-DEFAULT) intent and allow implicit conversions.
// This is a regression test for a bug where formals with inferred types
// from default values disallowed implicit conversions.
static void test6() {
  printf("test6\n");

  // Basic test: int to real conversion with default value
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);
    const std::string program =
      R""""(
      proc foo(type idk, x=0.5) {}
      foo(int, 1);
      )"""";

    auto path = UniqueString::get(context, "input.chpl");
    setFileText(context, path, program);
    const ModuleVec& vec = parseToplevel(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    const Call* call = m->stmt(1)->toCall();
    assert(call);

    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    const ResolvedExpression& re = rr.byAst(call);

    // Should resolve successfully
    assert(!re.type().isUnknown());
    auto c = re.mostSpecific().only();
    assert(c);
    assert(c.fn()->untyped()->name() == "foo");

    // Check that the formal 'x' has a non-DEFAULT intent (should be CONST_IN for real)
    assert(c.fn()->numFormals() == 2);
    assert(c.fn()->formalName(1) == "x");
    auto formalKind = c.fn()->formalType(1).kind();
    assert(formalKind == QualifiedType::CONST_IN);
    assert(c.fn()->formalType(1).type()->isRealType());
  }

  // Test with record type
  {
    auto context = buildStdContext();
    ErrorGuard guard(context);
    const std::string program =
      R""""(
      record R {
        var x: int;
      }
      proc baz(r = new R(42)) {}
      baz(new R(10));
      )"""";

    auto path = UniqueString::get(context, "input3.chpl");
    setFileText(context, path, program);
    const ModuleVec& vec = parseToplevel(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    const Call* call = m->stmt(2)->toCall();
    assert(call);

    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    const ResolvedExpression& re = rr.byAst(call);

    // Should resolve successfully
    assert(!re.type().isUnknown());
    auto c = re.mostSpecific().only();
    assert(c);
    assert(c.fn()->untyped()->name() == "baz");

    // Check that the formal has a non-DEFAULT intent
    assert(c.fn()->numFormals() == 1);
    assert(c.fn()->formalName(0) == "r");
    auto formalKind = c.fn()->formalType(0).kind();
    assert(formalKind == QualifiedType::CONST_REF);
    assert(c.fn()->formalType(0).type()->isRecordType());
  }
}

int main() {
  test1();
  test2();
  test3a();
  test3b();
  test4();
  test5();
  test6();

  return 0;
}
