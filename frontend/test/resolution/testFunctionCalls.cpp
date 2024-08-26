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
#include "chpl/types/all-types.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

static void test1() {
  // make sure that function return type computation does not throw
  // away the param.
  Context ctx;
  auto context = &ctx;
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
  Context ctx;
  auto context = &ctx;
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
    Context ctx;
    auto context = &ctx;
    auto qt = resolveTypeOfXInit(context, theFunction +
                                 R""""(
                                 var x = f(true, "hello", "world");
                                 )"""");
    assert(qt.type() != nullptr);
    assert(qt.type()->isStringType());
  }
  {
    Context ctx;
    auto context = &ctx;
    auto qt = resolveTypeOfXInit(context, theFunction +
                                 R""""(
                                 var x = f(false, 0, 1);
                                 )"""");
    assert(qt.type() != nullptr);
    assert(qt.type()->isIntType());
  }
  {
    Context ctx;
    auto context = &ctx;
    auto qt = resolveTypeOfXInit(context, theFunction +
                                 R""""(
                                 var x = f(true, 0, 1);
                                 )"""");
    assert(qt.isUnknown());
  }
  {
    Context ctx;
    auto context = &ctx;
    auto qt = resolveTypeOfXInit(context, theFunction +
                                 R""""(
                                 var x = f(false, "hello", "world");
                                 )"""");
    assert(qt.isUnknown());
  }
  {
    Context ctx;
    auto context = &ctx;
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
  Context ctx;
  auto context = &ctx;

  // With param field type declared
  {
    printf("part 1\n");
    context->advanceToNextRevision(true);

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
    assert(qt.type() != nullptr);
    assert(qt.type()->isIntType());

    context->collectGarbage();
  }

  // Param field of generic type
  {
    printf("part 2\n");
    context->advanceToNextRevision(true);

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
    context->advanceToNextRevision(true);

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

int main() {
  test1();
  test2();
  test3a();
  test3b();
  test4();

  return 0;
}
