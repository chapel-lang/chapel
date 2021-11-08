/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/BoolType.h"
#include "chpl/types/ComplexType.h"
#include "chpl/types/ImagType.h"
#include "chpl/types/IntType.h"
#include "chpl/types/Param.h"
#include "chpl/types/RealType.h"
#include "chpl/types/RecordType.h"
#include "chpl/types/UintType.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Variable.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace parsing;
using namespace resolution;
using namespace types;
using namespace uast;

static const Module* parseModule(Context* context, const char* src) {
  auto path = UniqueString::build(context, "input.chpl");
  std::string contents = src;
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 1);

  return vec[0];
}

static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  auto m = parseModule(context, "var i: int;\n"
                                "var i8: int(8);\n"
                                "var i16: int(16);\n"
                                "var i32: int(32);\n"
                                "var i64: int(64);\n"
                                "var u: uint;\n"
                                "var u8: uint(8);\n"
                                "var u16: uint(16);\n"
                                "var u32: uint(32);\n"
                                "var u64: uint(64);\n");
  assert(m->numStmts() == 10);
  const Variable* i = m->stmt(0)->toVariable();
  assert(i);
  const Variable* i8 = m->stmt(1)->toVariable();
  assert(i8);
  const Variable* i16 = m->stmt(2)->toVariable();
  assert(i16);
  const Variable* i32 = m->stmt(3)->toVariable();
  assert(i32);
  const Variable* i64 = m->stmt(4)->toVariable();
  assert(i64);
  const Variable* u = m->stmt(5)->toVariable();
  assert(u);
  const Variable* u8 = m->stmt(6)->toVariable();
  assert(u8);
  const Variable* u16 = m->stmt(7)->toVariable();
  assert(u16);
  const Variable* u32 = m->stmt(8)->toVariable();
  assert(u32);
  const Variable* u64 = m->stmt(9)->toVariable();
  assert(u64);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  assert(rr.byAst(i).type.type()   == IntType::get(context, 0));
  assert(rr.byAst(i8).type.type()  == IntType::get(context, 8));
  assert(rr.byAst(i16).type.type() == IntType::get(context, 16));
  assert(rr.byAst(i32).type.type() == IntType::get(context, 32));
  assert(rr.byAst(i64).type.type() == IntType::get(context, 64));

  assert(rr.byAst(u).type.type()   == UintType::get(context, 0));
  assert(rr.byAst(u8).type.type()  == UintType::get(context, 8));
  assert(rr.byAst(u16).type.type() == UintType::get(context, 16));
  assert(rr.byAst(u32).type.type() == UintType::get(context, 32));
  assert(rr.byAst(u64).type.type() == UintType::get(context, 64));
}

static void test2() {
  printf("test2\n");
  Context ctx;
  Context* context = &ctx;

  auto m = parseModule(context, "var b: bool;\n"
                                "var b8: bool(8);\n"
                                "var b16: bool(16);\n"
                                "var b32: bool(32);\n"
                                "var b64: bool(64);\n");
  assert(m->numStmts() == 5);
  const Variable* b = m->stmt(0)->toVariable();
  assert(b);
  const Variable* b8 = m->stmt(1)->toVariable();
  assert(b8);
  const Variable* b16 = m->stmt(2)->toVariable();
  assert(b16);
  const Variable* b32 = m->stmt(3)->toVariable();
  assert(b32);
  const Variable* b64 = m->stmt(4)->toVariable();
  assert(b64);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  assert(rr.byAst(b).type.type()   == BoolType::get(context, 0));
  assert(rr.byAst(b8).type.type()  == BoolType::get(context, 8));
  assert(rr.byAst(b16).type.type() == BoolType::get(context, 16));
  assert(rr.byAst(b32).type.type() == BoolType::get(context, 32));
  assert(rr.byAst(b64).type.type() == BoolType::get(context, 64));
}

static void test3() {
  printf("test3\n");
  Context ctx;
  Context* context = &ctx;

  auto m = parseModule(context, "var r: real;\n"
                                "var r32: real(32);\n"
                                "var r64: real(64);\n"
                                "var i: imag;\n"
                                "var i32: imag(32);\n"
                                "var i64: imag(64);\n"
                                "var c: complex;\n"
                                "var c64: complex(64);\n"
                                "var c128: complex(128);\n");
  assert(m->numStmts() == 9);
  const Variable* r = m->stmt(0)->toVariable();
  assert(r);
  const Variable* r32 = m->stmt(1)->toVariable();
  assert(r32);
  const Variable* r64 = m->stmt(2)->toVariable();
  assert(r64);
  const Variable* i = m->stmt(3)->toVariable();
  assert(i);
  const Variable* i32 = m->stmt(4)->toVariable();
  assert(i32);
  const Variable* i64 = m->stmt(5)->toVariable();
  assert(i64);
  const Variable* c = m->stmt(6)->toVariable();
  assert(c);
  const Variable* c64 = m->stmt(7)->toVariable();
  assert(c64);
  const Variable* c128 = m->stmt(8)->toVariable();
  assert(c128);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  assert(rr.byAst(r).type.type()    == RealType::get(context, 0));
  assert(rr.byAst(r32).type.type()  == RealType::get(context, 32));
  assert(rr.byAst(r64).type.type()  == RealType::get(context, 64));
  assert(rr.byAst(i).type.type()    == ImagType::get(context, 0));
  assert(rr.byAst(i32).type.type()  == ImagType::get(context, 32));
  assert(rr.byAst(i64).type.type()  == ImagType::get(context, 64));
  assert(rr.byAst(c).type.type()    == ComplexType::get(context, 0));
  assert(rr.byAst(c64).type.type()  == ComplexType::get(context, 64));
  assert(rr.byAst(c128).type.type() == ComplexType::get(context, 128));
}

static void test4() {
  printf("test4\n");
  Context ctx;
  Context* context = &ctx;

  auto m = parseModule(context, "record R { var field; }\n"
                                "var x: R(int);\n");
  assert(m->numStmts() == 2);
  const Record* r = m->stmt(0)->toRecord();
  assert(r);
  const Variable* x = m->stmt(1)->toVariable();
  assert(x);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  auto qt = rr.byAst(x).type;
  assert(qt.type());

  auto rt = qt.type()->toRecordType();
  assert(rt);

  assert(rt->numFields() == 1);
  assert(rt->fieldName(0) == "field");
  assert(rt->fieldHasDefaultValue(0) == false);
  assert(rt->fieldType(0).kind() == QualifiedType::VALUE);
  assert(rt->fieldType(0).type() == IntType::get(context, 0));
}

static void test5() {
  printf("test5\n");
  Context ctx;
  Context* context = &ctx;

  auto m = parseModule(context, "record R { type t; }\n"
                                "var x: R(int);\n");
  assert(m->numStmts() == 2);
  const Record* r = m->stmt(0)->toRecord();
  assert(r);
  const Variable* x = m->stmt(1)->toVariable();
  assert(x);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  auto qt = rr.byAst(x).type;
  assert(qt.type());

  auto rt = qt.type()->toRecordType();
  assert(rt);

  assert(rt->numFields() == 1);
  assert(rt->fieldName(0) == "t");
  assert(rt->fieldHasDefaultValue(0) == false);
  assert(rt->fieldType(0).kind() == QualifiedType::TYPE);
  assert(rt->fieldType(0).type() == IntType::get(context, 0));
}

static void test6() {
  printf("test6\n");
  Context ctx;
  Context* context = &ctx;

  auto m = parseModule(context, "record R { param p; }\n"
                                "var x: R(1);\n");
  assert(m->numStmts() == 2);
  const Record* r = m->stmt(0)->toRecord();
  assert(r);
  const Variable* x = m->stmt(1)->toVariable();
  assert(x);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  auto qt = rr.byAst(x).type;
  assert(qt.type());

  auto rt = qt.type()->toRecordType();
  assert(rt);

  assert(rt->numFields() == 1);
  assert(rt->fieldName(0) == "p");
  assert(rt->fieldHasDefaultValue(0) == false);
  assert(rt->fieldType(0).kind() == QualifiedType::PARAM);
  assert(rt->fieldType(0).type() == IntType::get(context, 0));
  assert(rt->fieldType(0).param() == IntParam::get(context, 1));
}

static void test7() {
  printf("test7\n");
  Context ctx;
  Context* context = &ctx;

  auto m = parseModule(context, "record R { type t = int; }\n"
                                "var x: R;\n"
                                "var y: R(real);\n");
  assert(m->numStmts() == 3);
  const Record* r = m->stmt(0)->toRecord();
  assert(r);
  const Variable* x = m->stmt(1)->toVariable();
  assert(x);
  const Variable* y = m->stmt(2)->toVariable();
  assert(y);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  {
    auto qt = rr.byAst(x).type;
    assert(qt.type());

    auto rt = qt.type()->toRecordType();
    assert(rt);

    assert(rt->numFields() == 1);
    assert(rt->fieldName(0) == "t");
    assert(rt->fieldHasDefaultValue(0) == true);
    assert(rt->fieldType(0).kind() == QualifiedType::TYPE);
    assert(rt->fieldType(0).type() == IntType::get(context, 0));
  }

  {
    auto qt = rr.byAst(y).type;
    assert(qt.type());

    auto rt = qt.type()->toRecordType();
    assert(rt);

    assert(rt->numFields() == 1);
    assert(rt->fieldName(0) == "t");
    assert(rt->fieldHasDefaultValue(0) == true);
    assert(rt->fieldType(0).kind() == QualifiedType::TYPE);
    assert(rt->fieldType(0).type() == RealType::get(context, 0));
  }
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();

  return 0;
}
