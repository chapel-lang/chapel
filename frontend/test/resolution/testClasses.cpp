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
#include "chpl/types/BasicClassType.h"
#include "chpl/types/ClassType.h"
#include "chpl/types/IntType.h"
#include "chpl/types/QualifiedType.h"
#include "chpl/uast/Class.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/FnCall.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Variable.h"

static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "test1.chpl");
  std::string contents = R""""(
                           module M {
                             class C { }

                             proc C.method() { }

                             proc function(arg: C) { }
                          }
                        )"""";

  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 3);
  auto c = m->stmt(0)->toClass();
  assert(c);
  auto method = m->stmt(1)->toFunction();
  assert(method);
  auto function = m->stmt(2)->toFunction();
  assert(function);

  auto methodU = UntypedFnSignature::get(context, method);
  auto functionU = UntypedFnSignature::get(context, function);

  ResolutionContext rcval(context);
  auto methodT = typedSignatureInitial(&rcval, methodU);
  auto functionT = typedSignatureInitial(&rcval, functionU);

  auto it = initialTypeForTypeDecl(context, c->id());
  assert(it);
  auto bct = it->getCompositeType()->toBasicClassType();
  assert(bct);

  auto borrowedNonNil = ClassType::get(context, bct, nullptr,
                                       ClassTypeDecorator(
                                         ClassTypeDecorator::BORROWED_NONNIL));
  auto anyNonNil = ClassType::get(context, bct, nullptr,
                                  ClassTypeDecorator(
                                    ClassTypeDecorator::GENERIC_NONNIL));

  assert(methodT->formalName(0) == "this");
  assert(methodT->formalType(0).kind() == QualifiedType::CONST_IN);
  assert(methodT->formalType(0).type() == borrowedNonNil);

  assert(functionT->formalName(0) == "arg");
  assert(functionT->formalType(0).kind() == QualifiedType::DEFAULT_INTENT);
  assert(functionT->formalType(0).type() == anyNonNil);
}

static void test2() {
  printf("test2\n");

  // This test exists to ensure the compiler can correctly infer the type
  // of 'myParent' without resolving the field's initialization expression,
  // which would result in recursion.
  std::string program = R"""(
    var sentinelValue = new MyRecord();

    record MyRecord {
      var inst : unmanaged MyClass?;

      proc init() { }
    }

    class MyClass {
      const myParent : MyRecord = sentinelValue;
    }

    proc main() {
      var x = sentinelValue;
    }
  )""";

  Context ctx;
  auto context = &ctx;
  ErrorGuard guard(context);
  auto m = parseModule(context, program);
  auto results = resolveModule(context, m->id());

  auto var = findVariable(m, "x");
  auto rec = m->stmt(1)->toRecord();
  auto qt = results.byAst(var).type();
  assert(!qt.isErroneousType());
  assert(qt.type()->isRecordType());
  assert(qt.type()->toRecordType()->id() == rec->id());
}

// test that we can resolve the type of multi-decl fields properly when they
// they are later used as a return type of a method
static void test3() {
  printf("test3\n");

  std::string program = R"""(
    class C {
      var a, b : int;
    }

    proc C.getA() {
      return a;
    }

    var foo = new C(40, 2);
    var x = foo.getA();
    )""";
  Context ctx;
  auto context = &ctx;
  ErrorGuard guard(context);
  auto m = parseModule(context, program);
  auto results = resolveModule(context, m->id());
  auto var = findVariable(m, "x");
  auto qt = results.byAst(var).type();
  assert(qt.type()->isIntType());
  assert(qt.type()->toIntType()->bitwidth() == 64);
}

// Test resolving dependently-typed field usage in method body
static void test4() {
  printf("test4\n");

  Context ctx;
  auto context = &ctx;
  ErrorGuard guard(context);

  std::string program = R"""(
    class Bar {
      param rank : int;
      var myTup : rank*int;

      proc getSomething() {
        return myTup;
      }
    }

    var b = new Bar(2);
    var x = b.getSomething();
    )""";

  auto m = parseModule(context, program);
  auto r = resolveModule(context, m->id());

  auto x = findVariable(m, "x");
  auto qt = r.byAst(x).type();
  assert(qt.type()->isTupleType());
  assert(qt.type()->toTupleType()->numElements() == 2);
  assert(qt.type()->toTupleType()->starType().type());
  assert(qt.type()->toTupleType()->starType().type()->isIntType());
}

int main() {
  test1();
  test2();
  test3();
  test4();

  return 0;
}
