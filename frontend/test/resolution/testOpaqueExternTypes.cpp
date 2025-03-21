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
#include "chpl/resolution/can-pass.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

// Just test that an extern type declaration works.
static void test1() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  const char* program =
    R"""(
    extern type mytype;
    extern type anotherType;

    param reflexive = mytype == mytype;
    param differentTypes = mytype == anotherType;
    )""";


  auto types = resolveTypesOfVariables(context, program, { "mytype", "anotherType", "reflexive", "differentTypes" });

  auto mytype = types.at("mytype");
  assert(mytype.isType());
  assert(mytype.type());
  assert(mytype.type()->isExternType());
  assert(mytype.type()->toExternType()->linkageName() == "mytype");


  auto anotherType = types.at("anotherType");
  assert(anotherType.isType());
  assert(anotherType.type());
  assert(anotherType.type()->isExternType());
  assert(anotherType.type()->toExternType()->linkageName() == "anotherType");

  assert(types.at("reflexive").isParamTrue());
  assert(types.at("differentTypes").isParamFalse());
}

// test explicitly specifying extern linkage name.
static void test2() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  const char* program =
    R"""(
    extern type mytype;
    extern "mytype" type anotherType;

    param differentTypes = mytype == anotherType;
    )""";


  auto types = resolveTypesOfVariables(context, program, { "mytype", "anotherType", "differentTypes" });

  auto mytype = types.at("mytype");
  assert(mytype.isType());
  assert(mytype.type());
  assert(mytype.type()->isExternType());
  assert(mytype.type()->toExternType()->linkageName() == "mytype");


  auto anotherType = types.at("anotherType");
  assert(anotherType.isType());
  assert(anotherType.type());
  assert(anotherType.type()->isExternType());
  assert(anotherType.type()->toExternType()->linkageName() == "mytype");

  assert(types.at("differentTypes").isParamFalse());
}

// Make sure name similarity doesn't affect linkage-based equality.
static void test3() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  const char* program =
    R"""(
    proc f1() type {
      extern type mytype;
      return mytype;
    }
    proc f2() type {
      extern "mytype" type anotherType;
      return anotherType;
    }
    proc f3() type {
      extern type anotherType;
      return anotherType;
    }

    param shouldBeFalse1 = f1() == f2();
    param shouldBeFalse2 = f3() == f2();
    )""";


  auto types = resolveTypesOfVariables(context, program, { "shouldBeFalse1", "shouldBeFalse2" });

  assert(types.at("shouldBeFalse1").isParamFalse());
  assert(types.at("shouldBeFalse2").isParamFalse());
}

// Thorough test of externT from Atomics.chpl, but using if-then instead
// of select while we don't have select param folding.
static void test4() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  const char* program =
    R"""(
    private proc externT(type T) type {
      extern type atomic_bool;

      extern type atomic_int_least8_t;
      extern type atomic_int_least16_t;
      extern type atomic_int_least32_t;
      extern type atomic_int_least64_t;

      extern type atomic_uint_least8_t;
      extern type atomic_uint_least16_t;
      extern type atomic_uint_least32_t;
      extern type atomic_uint_least64_t;

      extern type atomic__real64;
      extern type atomic__real32;

      if T == bool     then return atomic_bool;

      if T == int(8)   then return atomic_int_least8_t;
      if T == int(16)  then return atomic_int_least16_t;
      if T == int(32)  then return atomic_int_least32_t;
      if T == int(64)  then return atomic_int_least64_t;

      if T == uint(8)  then return atomic_uint_least8_t;
      if T == uint(16) then return atomic_uint_least16_t;
      if T == uint(32) then return atomic_uint_least32_t;
      if T == uint(64) then return atomic_uint_least64_t;

      if T == real(32) then return atomic__real32;
      if T == real(64) then return atomic__real64;

      return T;
    }

    type t1 = externT(bool);
    type t2 = externT(int(8));
    type t3 = externT(int(16));
    type t4 = externT(int(32));
    type t5 = externT(int(64));
    type t6 = externT(uint(8));
    type t7 = externT(uint(16));
    type t8 = externT(uint(32));
    type t9 = externT(uint(64));
    type t10 = externT(string);
    )""";


  auto types = resolveTypesOfVariables(context, program, { "t1", "t2", "t3", "t4", "t5", "t6", "t7", "t8", "t9", "t10" });

  auto checkType = [&](const char* name, const char* linkageName, const Type* fallbackType = nullptr) {
    auto qt = types.at(name);
    assert(qt.kind() == QualifiedType::TYPE);
    assert(qt.type());
    if (linkageName) {
      assert(qt.type()->isExternType());
      assert(qt.type()->toExternType()->linkageName() == linkageName);
    } else {
      assert(qt.type() == fallbackType);
    }
  };
  checkType("t1", "atomic_bool");
  checkType("t2", "atomic_int_least8_t");
  checkType("t3", "atomic_int_least16_t");
  checkType("t4", "atomic_int_least32_t");
  checkType("t5", "atomic_int_least64_t");
  checkType("t6", "atomic_uint_least8_t");
  checkType("t7", "atomic_uint_least16_t");
  checkType("t8", "atomic_uint_least32_t");
  checkType("t9", "atomic_uint_least64_t");
  checkType("t10", nullptr, RecordType::getStringType(context));
}

// Ensure that extern variables that aren't types don't get assigned opaque
// types.
static void test5() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  const char* program =
    R"""(
    extern var notAType;
    )""";

  auto types = resolveTypesOfVariables(context, program, { "notAType" });
  auto notAType = types.at("notAType");

  assert(notAType.isUnknown());
}


int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
}
