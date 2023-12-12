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

#include "chpl/types/all-types.h"

// test that we can handle non-param string for string_length_bytes
static void test1() {
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
static void test2() {
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

static void test3() {
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

static void test4() {
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

static void test5() {
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

static void test6() {
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

static void test7() {
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
static void test8() {
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
static void test9() {
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

// test for primitive "getcid", which should return an int32
static void test10() {
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
static void test11() {
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
static void test12() {
  voidTypeTestHelper(R"""(var x = __primitive("chpl_init_record");)""");
}

// test for primitive "chpl_comm_get", which should return void
static void test13() {
  voidTypeTestHelper(R"""(var x = __primitive("chpl_comm_get");)""");
}

// test for primitive "chpl_comm_put", which should return void
static void test14() {
  voidTypeTestHelper(R"""(var x = __primitive("chpl_comm_put");)""");
}

// test for primitive "chpl_comm_array_get", which should return void
static void test15() {
  voidTypeTestHelper(R"""(var x = __primitive("chpl_comm_array_get");)""");
}

// test for primitive "chpl_comm_array_put", which should return void
static void test16() {
  voidTypeTestHelper(R"""(var x = __primitive("chpl_comm_array_put");)""");
}

// test for primitive "chpl_comm_remote_prefetch", which should return void
static void test17() {
  voidTypeTestHelper(R"""(var x = __primitive("chpl_comm_remote_prefetch");)""");
}

// test for primitive "chpl_comm_get_strd", which should return void
static void test18() {
  voidTypeTestHelper(R"""(var x = __primitive("chpl_comm_get_strd");)""");
}


// test for primitive "chpl_comm_put_strd", which should return void
static void test19() {
  voidTypeTestHelper(R"""(var x = __primitive("chpl_comm_put_strd");)""");
}

// test for primitive "shift_base_pointer", which should return void
static void test20() {
  voidTypeTestHelper(R"""(var x = __primitive("shift_base_pointer");)""");
}

// test for primitive "array_set", which should return void
static void test21() {
  voidTypeTestHelper(R"""(var x = __primitive("array_set");)""");
}

// test for primitive "array_set_first", which should return void
static void test22() {
  voidTypeTestHelper(R"""(var x = __primitive("array_set_first");)""");
}

// test for primitive "auto destroy runtime type", which should return void
static void test23() {
  voidTypeTestHelper(R"""(var x = __primitive("auto destroy runtime type");)""");
}

// test for primitive "create fn type", which should return void
static void test24() {
  voidTypeTestHelper(R"""(var x = __primitive("create fn type");)""");
}

// "chpl_task_getRequestedSubloc", which should return int 64
static void test25() {
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

// "class name by id", which should return a cString
static void test26() {
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
static void test27() {
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
static void test28() {
  Context ctx;
  auto context = &ctx;
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R"""(var x = __primitive("chpl_lookupFilename");)""");
  assert(qt.kind() == QualifiedType::CONST_VAR);
  auto typePtr = qt.type();
  assert(typePtr);
  assert(typePtr->isCStringType());
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
  test28();

  return 0;
}
