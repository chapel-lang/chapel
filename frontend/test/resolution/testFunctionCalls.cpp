/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

// test that we can handle non-param string for string_length_bytes
static void test4() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                           var s:string;
                           var x = __primitive("string_length_bytes", s);
                         )"""");
  assert(qt.kind() == QualifiedType::CONST_VAR);
  auto typePtr = qt.type();
  assert(typePtr);
  assert(typePtr->isIntType());
  assert(typePtr->toIntType()->bitwidth() == 64);
}

// test that we can handle non-param bytes for string_length_bytes
static void test5() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                           var b:bytes;
                           var x = __primitive("string_length_bytes", b);
                         )"""");
  assert(qt.kind() == QualifiedType::CONST_VAR);
  auto typePtr = qt.type();
  assert(typePtr);
  assert(typePtr->isIntType());
  assert(typePtr->toIntType()->bitwidth() == 64);
}

static void test6() {
  // test for primitive return type for get real/imag
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                           var a: complex(128);
                           var x = __primitive("complex_get_real", a);
                         )"""");
  assert(qt.kind() == QualifiedType::REF);
  auto typePtr = qt.type();
  assert(typePtr);
  auto realPtr = typePtr->toRealType();
  assert(realPtr->bitwidth()==64);
}

static void test7() {
  // test for primitive return type for get real/imag
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                           var a: complex(64);
                           var x = __primitive("complex_get_real", a);
                         )"""");
  assert(qt.kind() == QualifiedType::REF);
  auto typePtr = qt.type();
  assert(typePtr);
  auto realPtr = typePtr->toRealType();
  assert(realPtr->bitwidth()==32);
}

static void test8() {
  // test for primitive return type for get real/imag
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                           var a: complex(128);
                           var x = __primitive("complex_get_imag", a);
                         )"""");
  assert(qt.kind() == QualifiedType::REF);
  auto typePtr = qt.type();
  assert(typePtr);
  auto realPtr = typePtr->toRealType();
  assert(realPtr->bitwidth()==64);
}

static void test9() {
  // test for primitive return type for get real/imag
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                           var a: complex(64);
                           var x = __primitive("complex_get_imag", a);
                         )"""");
  assert(qt.kind() == QualifiedType::REF);
  auto typePtr = qt.type();
  assert(typePtr);
  auto realPtr = typePtr->toRealType();
  assert(realPtr->bitwidth()==32);
}

static void test10() {
  // check that primitive "gather tests" returns an int
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                           var x = __primitive("gather tests");
                         )"""");
  assert(qt.kind() == QualifiedType::CONST_VAR);
  auto typePtr = qt.type();
  assert(typePtr);
  assert(typePtr->isIntType());
}

static void test11() {
  // test for primitive "is wide pointer", which should return a bool
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                           var x = __primitive("is wide pointer");
                         )"""");
  assert(qt.kind() == QualifiedType::CONST_VAR);
  auto typePtr = qt.type();
  assert(typePtr);
  assert(typePtr->isBoolType());
}

// test for primitive "_wide_get_addr", which should return a void ptr
static void test12() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                           var x = __primitive("_wide_get_addr");
                         )"""");
  assert(qt.kind() == QualifiedType::CONST_VAR);
  auto typePtr = qt.type();
  assert(typePtr);
  auto cPtrType = typePtr->toCPtrType();
  assert(cPtrType);
  assert(cPtrType->isVoidPtr());
}

// test for primitive "steal", which should return the type of the argument
static void test13() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                           var a = 1;
                           var x = __primitive("steal", a);
                         )"""");
  assert(qt.kind() == QualifiedType::CONST_VAR);
  auto typePtr = qt.type();
  assert(typePtr);
  assert(typePtr->isIntType());
}


// test for primitive "type has default value", which should return a bool
static void test14() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                           var x = __primitive("type has default value");
                         )"""");
  assert(qt.kind() == QualifiedType::CONST_VAR);
  auto typePtr = qt.type();
  assert(typePtr);
  assert(typePtr->isBoolType());
}

// test for primitive "needs auto destroy", which should return a bool
static void test15() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                           var x = __primitive("needs auto destroy");
                         )"""");
  assert(qt.kind() == QualifiedType::CONST_VAR);
  auto typePtr = qt.type();
  assert(typePtr);
  assert(typePtr->isBoolType());
}

// test for primitive "getcid", which should return an int32
static void test16() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                           var x = __primitive("getcid");
                         )"""");
  assert(qt.kind() == QualifiedType::CONST_VAR);
  auto typePtr = qt.type();
  assert(typePtr);
  assert(typePtr->isIntType());
  assert(typePtr->toIntType()->bitwidth() == 32);
}

// test for primitive "get_union_id", which should return a default int
static void test17() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                           var x = __primitive("get_union_id");
                         )"""");
  assert(qt.kind() == QualifiedType::CONST_VAR);
  auto typePtr = qt.type();
  assert(typePtr);
  assert(typePtr->isIntType());
  assert(typePtr->toIntType()->bitwidth() == 64);
}

static void voidTypeTestHelper(std::string program) {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context, program);
  assert(qt.kind() == QualifiedType::CONST_VAR);
  auto typePtr = qt.type();
  assert(typePtr);
  assert(typePtr->isVoidType());
}

// test for primitive "chpl_init_record", which should return void
static void test18() {
  voidTypeTestHelper(R"""(var x = __primitive("chpl_init_record");)""");
}

// test for primitive "chpl_comm_get", which should return void
static void test19() {
  voidTypeTestHelper(R"""(var x = __primitive("chpl_comm_get");)""");
}

// test for primitive "chpl_comm_put", which should return void
static void test20() {
  voidTypeTestHelper(R"""(var x = __primitive("chpl_comm_put");)""");
}

// test for primitive "chpl_comm_array_get", which should return void
static void test21() {
  voidTypeTestHelper(R"""(var x = __primitive("chpl_comm_array_get");)""");
}

// test for primitive "chpl_comm_array_put", which should return void
static void test22() {
  voidTypeTestHelper(R"""(var x = __primitive("chpl_comm_array_put");)""");
}

// test for primitive "chpl_comm_remote_prefetch", which should return void
static void test23() {
  voidTypeTestHelper(R"""(var x = __primitive("chpl_comm_remote_prefetch");)""");
}

// test for primitive "chpl_comm_get_strd", which should return void
static void test24() {
  voidTypeTestHelper(R"""(var x = __primitive("chpl_comm_get_strd");)""");
}


// test for primitive "chpl_comm_put_strd", which should return void
static void test25() {
  voidTypeTestHelper(R"""(var x = __primitive("chpl_comm_put_strd");)""");
}

// test for primitive "shift_base_pointer", which should return void
static void test26() {
  voidTypeTestHelper(R"""(var x = __primitive("shift_base_pointer");)""");
}

// test for primitive "array_set", which should return void
static void test27() {
  voidTypeTestHelper(R"""(var x = __primitive("array_set");)""");
}

// test for primitive "array_set_first", which should return void
static void test28() {
  voidTypeTestHelper(R"""(var x = __primitive("array_set_first");)""");
}

// test for primitive "auto destroy runtime type", which should return void
static void test29() {
  voidTypeTestHelper(R"""(var x = __primitive("auto destroy runtime type");)""");
}

// test for primitive "create fn type", which should return void
static void test30() {
  voidTypeTestHelper(R"""(var x = __primitive("create fn type");)""");
}

// "chpl_task_getRequestedSubloc", which should return int 64
static void test31() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R"""(var x = __primitive("chpl_task_getRequestedSubloc");)""");
  assert(qt.kind() == QualifiedType::CONST_VAR);
  auto typePtr = qt.type();
  assert(typePtr);
  assert(typePtr->isIntType());
  assert(typePtr->toIntType()->bitwidth() == 64);
}

// "sizeof_ddata_element", which should return int 64 (actually sizeType)
static void test32() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R"""(var x = __primitive("sizeof_ddata_element");)""");
  assert(qt.kind() == QualifiedType::CONST_VAR);
  auto typePtr = qt.type();
  assert(typePtr);
  assert(typePtr->isIntType());
  assert(typePtr->toIntType()->bitwidth() == 64);
}

// "_wide_get_node", which should return int 32 (actually nodeIdType)
static void test33() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R"""(var x = __primitive("_wide_get_node");)""");
  assert(qt.kind() == QualifiedType::CONST_VAR);
  auto typePtr = qt.type();
  assert(typePtr);
  assert(typePtr->isIntType());
  assert(typePtr->toIntType()->bitwidth() == 32);
}

// "class name by id", which should return a cString
static void test34() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R"""(var x = __primitive("class name by id");)""");
  assert(qt.kind() == QualifiedType::CONST_VAR);
  auto typePtr = qt.type();
  assert(typePtr);
  assert(typePtr->isCStringType());
}

// "ref to string", which should return a cString
static void test35() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R"""(var x = __primitive("ref to string");)""");
  assert(qt.kind() == QualifiedType::CONST_VAR);
  auto typePtr = qt.type();
  assert(typePtr);
  assert(typePtr->isCStringType());
}

// "chpl_lookupFilename", which should return a cString
static void test36() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R"""(var x = __primitive("chpl_lookupFilename");)""");
  assert(qt.kind() == QualifiedType::CONST_VAR);
  auto typePtr = qt.type();
  assert(typePtr);
  assert(typePtr->isCStringType());
}

// "sizeof_bundle", which should return int 64 (actually sizeType)
static void test37() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R"""(var x = __primitive("sizeof_bundle");)""");
  assert(qt.kind() == QualifiedType::CONST_VAR);
  auto typePtr = qt.type();
  assert(typePtr);
  assert(typePtr->isIntType());
  assert(typePtr->toIntType()->bitwidth() == 64);
}


int main() {
  test1();
  test2();
  test3a();
  test3b();
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
  test28();
  test29();
  test30();
  test31();
  test32();
  test33();
  test34();
  test35();
  test36();
  test37();

  return 0;
}
