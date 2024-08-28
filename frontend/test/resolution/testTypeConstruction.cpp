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
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  auto m = parseModule(context, "var i: int;\n"
                                "var i8: int(8);\n"
                                "var i16: int(16);\n"
                                "var i32: int(32);\n"
                                "var i64: int(64);\n"
                                "var iq: int(?);\n"
                                "var u: uint;\n"
                                "var u8: uint(8);\n"
                                "var u16: uint(16);\n"
                                "var u32: uint(32);\n"
                                "var u64: uint(64);\n"
                                "var uq: uint(?);\n");
  assert(m->numStmts() == 12);
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
  const Variable* iq = m->stmt(5)->toVariable();
  assert(iq);
  const Variable* u = m->stmt(6)->toVariable();
  assert(u);
  const Variable* u8 = m->stmt(7)->toVariable();
  assert(u8);
  const Variable* u16 = m->stmt(8)->toVariable();
  assert(u16);
  const Variable* u32 = m->stmt(9)->toVariable();
  assert(u32);
  const Variable* u64 = m->stmt(10)->toVariable();
  assert(u64);
  const Variable* uq = m->stmt(11)->toVariable();
  assert(uq);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  assert(rr.byAst(i).type().type()   == IntType::get(context, 0));
  assert(rr.byAst(i8).type().type()  == IntType::get(context, 8));
  assert(rr.byAst(i16).type().type() == IntType::get(context, 16));
  assert(rr.byAst(i32).type().type() == IntType::get(context, 32));
  assert(rr.byAst(i64).type().type() == IntType::get(context, 64));
  assert(rr.byAst(iq).type().type()  == AnyIntType::get(context));

  assert(rr.byAst(u).type().type()   == UintType::get(context, 0));
  assert(rr.byAst(u8).type().type()  == UintType::get(context, 8));
  assert(rr.byAst(u16).type().type() == UintType::get(context, 16));
  assert(rr.byAst(u32).type().type() == UintType::get(context, 32));
  assert(rr.byAst(u64).type().type() == UintType::get(context, 64));
  assert(rr.byAst(uq).type().type()  == AnyUintType::get(context));
}

static void test2() {
  printf("test2\n");
  Context ctx;
  Context* context = &ctx;

  auto m = parseModule(context, "var b: bool;\n");

  assert(m->numStmts() == 1);
  const Variable* b = m->stmt(0)->toVariable();
  assert(b);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  assert(rr.byAst(b).type().type()   == BoolType::get(context));
}

static void test3() {
  printf("test3\n");
  Context ctx;
  Context* context = &ctx;

  auto m = parseModule(context, "var r: real;\n"
                                "var r32: real(32);\n"
                                "var r64: real(64);\n"
                                "var rq: real(?);\n"
                                "var i: imag;\n"
                                "var i32: imag(32);\n"
                                "var i64: imag(64);\n"
                                "var iq: imag(?);\n"
                                "var c: complex;\n"
                                "var c64: complex(64);\n"
                                "var c128: complex(128);\n"
                                "var cq: complex(?);\n");
  assert(m->numStmts() == 12);
  const Variable* r = m->stmt(0)->toVariable();
  assert(r);
  const Variable* r32 = m->stmt(1)->toVariable();
  assert(r32);
  const Variable* r64 = m->stmt(2)->toVariable();
  assert(r64);
  const Variable* rq = m->stmt(3)->toVariable();
  assert(rq);
  const Variable* i = m->stmt(4)->toVariable();
  assert(i);
  const Variable* i32 = m->stmt(5)->toVariable();
  assert(i32);
  const Variable* i64 = m->stmt(6)->toVariable();
  assert(i64);
  const Variable* iq = m->stmt(7)->toVariable();
  assert(iq);
  const Variable* c = m->stmt(8)->toVariable();
  assert(c);
  const Variable* c64 = m->stmt(9)->toVariable();
  assert(c64);
  const Variable* c128 = m->stmt(10)->toVariable();
  assert(c128);
  const Variable* cq = m->stmt(11)->toVariable();
  assert(cq);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  assert(rr.byAst(r).type().type()    == RealType::get(context, 0));
  assert(rr.byAst(r32).type().type()  == RealType::get(context, 32));
  assert(rr.byAst(r64).type().type()  == RealType::get(context, 64));
  assert(rr.byAst(rq).type().type()   == AnyRealType::get(context));
  assert(rr.byAst(i).type().type()    == ImagType::get(context, 0));
  assert(rr.byAst(i32).type().type()  == ImagType::get(context, 32));
  assert(rr.byAst(i64).type().type()  == ImagType::get(context, 64));
  assert(rr.byAst(iq).type().type()   == AnyImagType::get(context));
  assert(rr.byAst(c).type().type()    == ComplexType::get(context, 0));
  assert(rr.byAst(c64).type().type()  == ComplexType::get(context, 64));
  assert(rr.byAst(c128).type().type() == ComplexType::get(context, 128));
  assert(rr.byAst(cq).type().type()   == AnyComplexType::get(context));
}

// assumes the last statement is a variable declaration for x.
// returns the type of that.
static std::pair<const Type*, const ResolvedFields*>
parseTypeAndFieldsOfX(Context* context, const char* program) {
  auto m = parseModule(context, program);
  assert(m->numStmts() > 0);
  const Variable* x = m->stmt(m->numStmts()-1)->toVariable();
  assert(x);
  assert(x->name() == "x");

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  auto qt = rr.byAst(x).type();
  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type());

  const Type* t = qt.type();
  auto ct = t->toCompositeType();
  if (auto classType = t->toClassType())
    ct = classType->basicClassType();

  assert(ct != nullptr);

  auto defaultsPolicy = DefaultsPolicy::IGNORE_DEFAULTS;
  const ResolvedFields& f = fieldsForTypeDecl(context, ct,
                                              defaultsPolicy);

  return std::make_pair(t, &f);
}

static void test4a() {
  printf("test4a\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "record R { var field: int; }\n"
                                 "var x: R;\n");
  auto rt = p.first->toRecordType();
  assert(rt);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "field");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::VAR);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}

static void test4b() {
  printf("test4b\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "record R { var field; }\n"
                                 "var x: R(int);\n");
  auto rt = p.first->toRecordType();
  assert(rt);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "field");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::VAR);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}

static void test5() {
  printf("test5\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "record R { type t; }\n"
                                 "var x: R(int);\n");
  auto rt = p.first->toRecordType();
  assert(rt);
  assert(rt->instantiatedFrom());

  auto& initialFields = fieldsForTypeDecl(context,
                                          rt->instantiatedFrom(),
                                          DefaultsPolicy::IGNORE_DEFAULTS);
  assert(rt->instantiatedFrom()->instantiatedFrom() == nullptr);
  assert(initialFields.numFields() == 1);
  assert(initialFields.fieldName(0) == "t");
  assert(initialFields.fieldHasDefaultValue(0) == false);
  assert(initialFields.fieldType(0).kind() == QualifiedType::TYPE);
  assert(initialFields.fieldType(0).type()->isAnyType());

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "t");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::TYPE);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));

}

static void test6() {
  printf("test6\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "record R { param p; }\n"
                                 "var x: R(1);\n");
  auto rt = p.first->toRecordType();
  assert(rt);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "p");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::PARAM);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
  assert(fields->fieldType(0).param() == IntParam::get(context, 1));
}

static void test7() {
  printf("test7\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "record R { type t = int; }\n"
                                 "var x: R(real);\n");
  auto rt = p.first->toRecordType();
  assert(rt);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "t");
  assert(fields->fieldHasDefaultValue(0) == true);
  assert(fields->fieldType(0).kind() == QualifiedType::TYPE);
  assert(fields->fieldType(0).type() == RealType::get(context, 0));
}

static void test8() {
  printf("test8\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "record R { type t = int; }\n"
                                 "var x: R;\n");
  auto rt = p.first->toRecordType();
  assert(rt);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "t");
  assert(fields->fieldHasDefaultValue(0) == true);
  assert(fields->fieldType(0).kind() == QualifiedType::TYPE);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}

static void test9() {
  printf("test9\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "record R { param p = 1; }\n"
                                 "var x: R(2);\n");
  auto rt = p.first->toRecordType();
  assert(rt);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "p");
  assert(fields->fieldHasDefaultValue(0) == true);
  assert(fields->fieldType(0).kind() == QualifiedType::PARAM);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
  assert(fields->fieldType(0).param() == IntParam::get(context, 2));
}

static void test10() {
  printf("test10\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "record R { param p = 1; }\n"
                                 "var x: R;\n");
  auto rt = p.first->toRecordType();
  assert(rt);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "p");
  assert(fields->fieldHasDefaultValue(0) == true);
  assert(fields->fieldType(0).kind() == QualifiedType::PARAM);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
  assert(fields->fieldType(0).param() == IntParam::get(context, 1));
}

static void test11() {
  printf("test11\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "record R { type t = int; }\n"
                                 "var x: R();\n");
  auto rt = p.first->toRecordType();
  assert(rt);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "t");
  assert(fields->fieldHasDefaultValue(0) == true);
  assert(fields->fieldType(0).kind() == QualifiedType::TYPE);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}

static void test12() {
  printf("test12\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "record R { type t = int; }\n"
                                 "var x: R(?);\n");
  auto rt = p.first->toRecordType();
  assert(rt);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "t");
  assert(fields->fieldHasDefaultValue(0) == true);
  assert(fields->fieldType(0).kind() == QualifiedType::TYPE);
  assert(fields->fieldType(0).type() == AnyType::get(context));
}

static void test13() {
  printf("test13\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "record R { param p = 1; }\n"
                                 "var x: R();\n");
  auto rt = p.first->toRecordType();
  assert(rt);

  auto fields = p.second;
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "p");
  assert(fields->fieldHasDefaultValue(0) == true);
  assert(fields->fieldType(0).kind() == QualifiedType::PARAM);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
  assert(fields->fieldType(0).param() == IntParam::get(context, 1));
}

static void test14() {
  printf("test14\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "record R { param p = 1; }\n"
                                 "var x: R(?);\n");
  auto rt = p.first->toRecordType();
  assert(rt);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "p");
  assert(fields->fieldHasDefaultValue(0) == true);
  assert(fields->fieldType(0).kind() == QualifiedType::PARAM);
  assert(fields->fieldType(0).type() == AnyType::get(context));
  assert(fields->fieldType(0).param() == nullptr);
}

static void test15() {
  printf("test15\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "class C { var field: int; }\n"
                                 "var x: C;\n");

  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->decorator().val() == ClassTypeDecorator::GENERIC_NONNIL);
  assert(ct->manager() == nullptr);

  auto bct = ct->basicClassType();
  assert(bct);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "field");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::VAR);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}

static void test16() {
  printf("test16\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "class C { var field: int; }\n"
                                 "var x: C?;\n");

  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->decorator().val() == ClassTypeDecorator::GENERIC_NILABLE);
  assert(ct->manager() == nullptr);

  auto bct = ct->basicClassType();
  assert(bct);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "field");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::VAR);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}


static void test17() {
  printf("test17\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "class C { var field: int; }\n"
                                 "var x: borrowed C;\n");
  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->decorator().val() == ClassTypeDecorator::BORROWED_NONNIL);
  assert(ct->manager() == nullptr);

  auto bct = ct->basicClassType();
  assert(bct);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "field");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::VAR);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}

static void test18() {
  printf("test18\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "class C { var field: int; }\n"
                                 "var x: borrowed C?;\n");
  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->decorator().val() == ClassTypeDecorator::BORROWED_NILABLE);
  assert(ct->manager() == nullptr);

  auto bct = ct->basicClassType();
  assert(bct);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "field");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::VAR);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}

static void test19() {
  printf("test19\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "class C { var field: int; }\n"
                                 "var x: unmanaged C;\n");
  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->decorator().val() == ClassTypeDecorator::UNMANAGED_NONNIL);
  assert(ct->manager() == nullptr);

  auto bct = ct->basicClassType();
  assert(bct);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "field");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::VAR);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}

static void test20() {
  printf("test20\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "class C { var field: int; }\n"
                                 "var x: unmanaged C?;\n");
  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->decorator().val() == ClassTypeDecorator::UNMANAGED_NILABLE);
  assert(ct->manager() == nullptr);

  auto bct = ct->basicClassType();
  assert(bct);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "field");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::VAR);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}

static void test21() {
  printf("test21\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "class C { var field: int; }\n"
                                 "var x: owned C;\n");
  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->decorator().val() == ClassTypeDecorator::MANAGED_NONNIL);
  assert(ct->manager() == AnyOwnedType::get(context));

  auto bct = ct->basicClassType();
  assert(bct);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "field");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::VAR);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}

static void test22() {
  printf("test22\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "class C { var field: int; }\n"
                                 "var x: owned C?;\n");
  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->decorator().val() == ClassTypeDecorator::MANAGED_NILABLE);
  assert(ct->manager() == AnyOwnedType::get(context));

  auto bct = ct->basicClassType();
  assert(bct);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "field");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::VAR);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}

static void test23() {
  printf("test23\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "class C { var field: int; }\n"
                                 "var x: shared C;\n");
  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->decorator().val() == ClassTypeDecorator::MANAGED_NONNIL);
  assert(ct->manager() == AnySharedType::get(context));

  auto bct = ct->basicClassType();
  assert(bct);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "field");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::VAR);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}

static void test23q() {
  printf("test23q\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "class C { var field: int; }\n"
                                 "var x: shared C?;\n");
  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->decorator().val() == ClassTypeDecorator::MANAGED_NILABLE);
  assert(ct->manager() == AnySharedType::get(context));

  auto bct = ct->basicClassType();
  assert(bct);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "field");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::VAR);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}

static void test24() {
  printf("test24\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "class C { var field; }\n"
                                 "var x: C(int);\n");
  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->decorator().val() == ClassTypeDecorator::GENERIC_NONNIL);
  assert(ct->manager() == nullptr);

  auto bct = ct->basicClassType();
  assert(bct);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "field");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::VAR);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}

static void test25() {
  printf("test25\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "class C { var field; }\n"
                                 "var x: C(int)?;\n");
  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->decorator().val() == ClassTypeDecorator::GENERIC_NILABLE);
  assert(ct->manager() == nullptr);

  auto bct = ct->basicClassType();
  assert(bct);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "field");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::VAR);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}

static void test26() {
  printf("test26\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "class C { var field; }\n"
                                 "var x: borrowed C(int)?;\n");
  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->decorator().val() == ClassTypeDecorator::BORROWED_NILABLE);
  assert(ct->manager() == nullptr);

  auto bct = ct->basicClassType();
  assert(bct);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "field");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::VAR);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}

static void test27() {
  printf("test27\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "class C { type t; }\n"
                                 "var x: owned C(int)?;\n");
  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->decorator().val() == ClassTypeDecorator::MANAGED_NILABLE);
  assert(ct->manager() == AnyOwnedType::get(context));

  auto bct = ct->basicClassType();
  assert(bct);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "t");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::TYPE);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}

static void test28() {
  printf("test28\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "class C { type t = int; }\n"
                                 "var x: C;\n");
  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->decorator().val() == ClassTypeDecorator::GENERIC_NONNIL);
  assert(ct->manager() == nullptr);

  auto bct = ct->basicClassType();
  assert(bct);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "t");
  assert(fields->fieldHasDefaultValue(0) == true);
  assert(fields->fieldType(0).kind() == QualifiedType::TYPE);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}

static void test29() {
  printf("test29\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "class C { type t = int; }\n"
                                 "var x: C();\n");
  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->decorator().val() == ClassTypeDecorator::GENERIC_NONNIL);
  assert(ct->manager() == nullptr);

  auto bct = ct->basicClassType();
  assert(bct);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "t");
  assert(fields->fieldHasDefaultValue(0) == true);
  assert(fields->fieldType(0).kind() == QualifiedType::TYPE);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}

static void test30() {
  printf("test30\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "class C { type t = int; }\n"
                                 "var x: borrowed C?;\n");
  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->decorator().val() == ClassTypeDecorator::BORROWED_NILABLE);
  assert(ct->manager() == nullptr);

  auto bct = ct->basicClassType();
  assert(bct);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "t");
  assert(fields->fieldHasDefaultValue(0) == true);
  assert(fields->fieldType(0).kind() == QualifiedType::TYPE);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}

static void test31() {
  printf("test31\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "class C { type t = int; }\n"
                                 "var x: borrowed C()?;\n");
  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->decorator().val() == ClassTypeDecorator::BORROWED_NILABLE);
  assert(ct->manager() == nullptr);

  auto bct = ct->basicClassType();
  assert(bct);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "t");
  assert(fields->fieldHasDefaultValue(0) == true);
  assert(fields->fieldType(0).kind() == QualifiedType::TYPE);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
}

static void test32() {
  printf("test32\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "class C { type t = int; }\n"
                                 "var x: shared C(real)?;\n");
  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->decorator().val() == ClassTypeDecorator::MANAGED_NILABLE);
  assert(ct->manager() == AnySharedType::get(context));

  auto bct = ct->basicClassType();
  assert(bct);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "t");
  assert(fields->fieldHasDefaultValue(0) == true);
  assert(fields->fieldType(0).kind() == QualifiedType::TYPE);
  assert(fields->fieldType(0).type() == RealType::get(context, 0));
}

static void test33() {
  printf("test33\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "class C { type t = int; }\n"
                                 "var x: C(real)?;\n");
  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->decorator().val() == ClassTypeDecorator::GENERIC_NILABLE);
  assert(ct->manager() == nullptr);

  auto bct = ct->basicClassType();
  assert(bct);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "t");
  assert(fields->fieldHasDefaultValue(0) == true);
  assert(fields->fieldType(0).kind() == QualifiedType::TYPE);
  assert(fields->fieldType(0).type() == RealType::get(context, 0));
}

static void test34() {
  printf("test34\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context, "class C { type t = int; }\n"
                                 "var x: unmanaged C(real);\n");
  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->decorator().val() == ClassTypeDecorator::UNMANAGED_NONNIL);
  assert(ct->manager() == nullptr);

  auto bct = ct->basicClassType();
  assert(bct);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "t");
  assert(fields->fieldHasDefaultValue(0) == true);
  assert(fields->fieldType(0).kind() == QualifiedType::TYPE);
  assert(fields->fieldType(0).type() == RealType::get(context, 0));
}

// test a challenging case
// TODO: remove this test and instead check it generates a
// multiply defined symbol error.
// see issue #18817.
static void testTypeAndFnSameName() {
  printf("testTypeAndFnSameName\n");
  Context ctx;
  Context* context = &ctx;

  auto m = parseModule(context, R""""(
                                  record R {
                                    type t;
                                  }

                                  {
                                    proc R(arg: int) {
                                      return arg;
                                    }

                                    var x = R(arg=1);
                                    var y:R(t=real);
                                  }
                                )"""");

  assert(m->numStmts() == 2);
  const Record* r = m->stmt(0)->toRecord();
  assert(r);
  const Block* b = m->stmt(1)->toBlock();
  assert(b);
  assert(b->numStmts() == 3);
  const Function* f = b->stmt(0)->toFunction();
  assert(f);
  const Variable* x = b->stmt(1)->toVariable();
  assert(x);
  const Variable* y = b->stmt(2)->toVariable();
  assert(y);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  const Type* xt = rr.byAst(x).type().type();
  assert(xt);
  assert(xt == IntType::get(context, 0));
  const Type* yt = rr.byAst(y).type().type();
  assert(yt);
  auto yrt = yt->toRecordType();
  assert(yrt);

  assert(yrt->id() == r->id());
}

static void test35() {
  printf("test35\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context,
                        R""""(
                          record R {
                            var a = 1;
                            var b: int;
                            type t;
                            const c: t;
                            var d = c;
                          }
                          var x: R(real);
                        )"""");
  auto rt = p.first->toRecordType();
  assert(rt);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 5);

  assert(fields->fieldName(0) == "a");
  assert(fields->fieldHasDefaultValue(0) == true);
  assert(fields->fieldType(0).kind() == QualifiedType::VAR);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));

  assert(fields->fieldName(1) == "b");
  assert(fields->fieldHasDefaultValue(1) == false);
  assert(fields->fieldType(1).kind() == QualifiedType::VAR);
  assert(fields->fieldType(1).type() == IntType::get(context, 0));

  assert(fields->fieldName(2) == "t");
  assert(fields->fieldHasDefaultValue(2) == false);
  assert(fields->fieldType(2).kind() == QualifiedType::TYPE);
  assert(fields->fieldType(2).type() == RealType::get(context, 0));

  assert(fields->fieldName(3) == "c");
  assert(fields->fieldHasDefaultValue(3) == false);
  assert(fields->fieldType(3).kind() == QualifiedType::CONST_VAR);
  assert(fields->fieldType(3).type() == RealType::get(context, 0));

  assert(fields->fieldName(4) == "d");
  assert(fields->fieldHasDefaultValue(4) == true);
  assert(fields->fieldType(4).kind() == QualifiedType::VAR);
  assert(fields->fieldType(4).type() == RealType::get(context, 0));
}

static void test36() {
  printf("test36\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context,
                        R""""(
                          class ClassA {
                            var field: RootClass;
                          }
                          var x: owned ClassA;
                        )"""");
  auto ct = p.first->toClassType();
  assert(ct);

  auto bct = ct->basicClassType();
  assert(bct);
  assert(bct->parentClassType()->isObjectType());

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "field");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::VAR);
  auto fct = fields->fieldType(0).type()->toClassType();
  assert(fct);
  assert(fct->decorator().isUnknownManagement());
  assert(fct->decorator().isNonNilable());
  assert(fct->basicClassType()->name() == "RootClass");
  assert(fct->basicClassType() == BasicClassType::getRootClassType(context));
}

static void test37() {
  printf("test37\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context,
                        R""""(
                          class ClassA {
                            var x: int;
                          }
                          class ClassB {
                            var field: ClassA;
                          }
                          var x: owned ClassB;
                        )"""");
  auto ct = p.first->toClassType();
  assert(ct);

  auto bct = ct->basicClassType();
  assert(bct);
  assert(bct->parentClassType()->isObjectType());

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "field");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::VAR);
  auto fct = fields->fieldType(0).type()->toClassType();
  assert(fct);
  assert(fct->decorator().isUnknownManagement());
  assert(fct->decorator().isNonNilable());
  assert(fct->basicClassType()->name() == "ClassA");
}


static void test38() {
  printf("test38\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context,
                        R""""(
                          class Parent {
                            var parentField:int;
                          }
                          class Child : Parent {
                            var childObject: RootClass;
                          }
                          var x: owned Child;
                        )"""");
  auto ct = p.first->toClassType();
  assert(ct);

  auto bct = ct->basicClassType();
  assert(bct);
  assert(!bct->parentClassType()->isObjectType());

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "childObject");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::VAR);
  auto fct = fields->fieldType(0).type()->toClassType();
  assert(fct);
  assert(fct->decorator().isUnknownManagement());
  assert(fct->decorator().isNonNilable());
  assert(fct->basicClassType()->name() == "RootClass");
  assert(fct->basicClassType() == BasicClassType::getRootClassType(context));

  auto pct = bct->parentClassType()->toBasicClassType();
  assert(pct);
  assert(pct->parentClassType()->isObjectType());
  assert(pct->parentClassType() == BasicClassType::getRootClassType(context));

  auto& parentFields = fieldsForTypeDecl(context, pct, DefaultsPolicy::IGNORE_DEFAULTS);
  assert(parentFields.numFields() == 1);
  assert(parentFields.fieldName(0) == "parentField");
  assert(parentFields.fieldHasDefaultValue(0) == false);
  assert(parentFields.fieldType(0).kind() == QualifiedType::VAR);
  assert(parentFields.fieldType(0).type() == IntType::get(context, 0));
}

static void test39() {
  printf("test39\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context,
                        R""""(
                          class C {
                            var next: owned C;
                          }
                          var x: owned C;
                        )"""");
  auto ct = p.first->toClassType();
  assert(ct);

  auto bct = ct->basicClassType();
  assert(bct);
  auto pct = bct->parentClassType()->toBasicClassType();
  assert(pct);
  assert(pct->isObjectType());

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "next");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::VAR);
  assert(fields->fieldType(0).type() == ct);
}

static void test40() {
  printf("test40\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context,
                        R""""(
                          proc useType(type t) {
                            var ret: t;
                            return ret;
                          }
                          record R { }
                          proc R.init() { }
                          proc R.deinit() { }
                          var x = useType(R);
                        )"""");

  auto rt = p.first->toRecordType();
  assert(rt);
  assert(rt->instantiatedFrom() == nullptr);

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 0);
}

static void test41() {
  printf("test41\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context,
                        R""""(
                          proc useType(type t) {
                            var ret: t;
                            return ret;
                          }
                          record R {
                            param p;
                            proc init(param p) { this.p = p; }
                            proc deinit() { }
                          }
                          var x = useType(R(1));
                        )"""");

  auto rt = p.first->toRecordType();
  assert(rt);
  assert(rt->instantiatedFrom());

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "p");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::PARAM);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
  assert(fields->fieldType(0).param() == IntParam::get(context, 1));
}


static void test42() {
  printf("test42\n");
  Context ctx;
  Context* context = &ctx;

  auto p = parseTypeAndFieldsOfX(context,
                        R""""(
                          proc useType(type t) {
                            var ret: t(1);
                            return ret;
                          }
                          record R {
                            param p;
                            proc init(param p) { this.p = p; }
                            proc deinit() { }
                          }
                          var x = useType(R);
                        )"""");

  auto rt = p.first->toRecordType();
  assert(rt);
  assert(rt->instantiatedFrom());

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 1);
  assert(fields->fieldName(0) == "p");
  assert(fields->fieldHasDefaultValue(0) == false);
  assert(fields->fieldType(0).kind() == QualifiedType::PARAM);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
  assert(fields->fieldType(0).param() == IntParam::get(context, 1));
}

static void testRecursiveTypeConstructor() {
  printf("testRecursiveTypeConstructor\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto p = parseTypeAndFieldsOfX(context,
      R"""(
      class Node {
        type eltType = int;
        var data: eltType;
        var next: unmanaged Node(eltType)?;
      }
      var x = new unmanaged Node(real, 3.14);
      )""");

  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->basicClassType());
  assert(ct->basicClassType()->instantiatedFrom());

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 3);
  assert(fields->fieldName(0) == "eltType");
  assert(fields->fieldHasDefaultValue(0) == true);
  assert(fields->fieldType(0).kind() == QualifiedType::TYPE);
  assert(fields->fieldType(0).type() == RealType::get(context, 0));
  assert(fields->fieldName(1) == "data");
  assert(fields->fieldHasDefaultValue(1) == false);
  assert(fields->fieldType(1).kind() == QualifiedType::VAR);
  assert(fields->fieldType(1).type() == RealType::get(context, 0));
  assert(fields->fieldName(2) == "next");
  assert(fields->fieldHasDefaultValue(2) == false);
  assert(fields->fieldType(2).kind() == QualifiedType::VAR);
  assert(fields->fieldType(2).type()->toClassType());
  assert(fields->fieldType(2).type()->toClassType()->basicClassType() == ct->basicClassType());
}

static void testRecursiveTypeConstructorAlias() {
  printf("testRecursiveTypeConstructorAlias\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto p = parseTypeAndFieldsOfX(context,
      R"""(
      class Node {
          type eltType = int;
          var data: eltType;
          var next: UnmanagedIntNode;
      }
      proc UnmanagedIntNode type do return unmanaged Node(int)?;
      var x = new unmanaged Node(int, 314);
      )""");

  auto ct = p.first->toClassType();
  assert(ct);
  assert(ct->basicClassType());
  assert(ct->basicClassType()->instantiatedFrom());

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 3);
  assert(fields->fieldName(0) == "eltType");
  assert(fields->fieldHasDefaultValue(0) == true);
  assert(fields->fieldType(0).kind() == QualifiedType::TYPE);
  assert(fields->fieldType(0).type() == IntType::get(context, 0));
  assert(fields->fieldName(1) == "data");
  assert(fields->fieldHasDefaultValue(1) == false);
  assert(fields->fieldType(1).kind() == QualifiedType::VAR);
  assert(fields->fieldType(1).type() == IntType::get(context, 0));
  assert(fields->fieldName(2) == "next");
  assert(fields->fieldHasDefaultValue(2) == false);
  assert(fields->fieldType(2).kind() == QualifiedType::VAR);
  assert(fields->fieldType(2).type()->toClassType());
  assert(fields->fieldType(2).type()->toClassType()->basicClassType() == ct->basicClassType());
}

static void testRecursiveTypeConstructorGeneric() {
  printf("testRecursiveTypeConstructorGeneric\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  std::ignore = resolveTypeOfXInit(context,
      R"""(
      class Node {
          type eltType = int;
          var data: eltType;
          var next: Node(eltType)?;
      }
      var x = new unmanaged Node(int, 314);
      )""", /* requireTypeKnown */ false);

  bool foundError = false;
  for (auto& err : guard.errors()) {
    if (err->type() == ErrorType::MissingFormalInstantiation) {
      foundError = true;
      break;
    }
  }
  assert(foundError);
  assert(guard.realizeErrors() > 0);
}

static void testRecursiveTypeConstructorMutual() {
  printf("testRecursiveTypeConstructor\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto p = parseTypeAndFieldsOfX(context,
      R"""(
      class MutA {
        type eltType;
        var data: eltType;
        var next: owned MutB(eltType)?;
      }

      class MutB {
        type eltType;
        var data: eltType;
        var next: owned MutA(eltType)?;
      }

      var x = new MutA(real, 3.14, new MutB(real, 2.71));
      )""");

  auto ctA = p.first->toClassType();
  assert(ctA);
  assert(ctA->basicClassType());
  assert(ctA->basicClassType()->instantiatedFrom());

  auto fieldsA = p.second;
  assert(fieldsA);
  assert(fieldsA->numFields() == 3);
  assert(fieldsA->fieldName(0) == "eltType");
  assert(fieldsA->fieldHasDefaultValue(0) == false);
  assert(fieldsA->fieldType(0).kind() == QualifiedType::TYPE);
  assert(fieldsA->fieldType(0).type() == RealType::get(context, 0));
  assert(fieldsA->fieldName(1) == "data");
  assert(fieldsA->fieldHasDefaultValue(1) == false);
  assert(fieldsA->fieldType(1).kind() == QualifiedType::VAR);
  assert(fieldsA->fieldType(1).type() == RealType::get(context, 0));
  assert(fieldsA->fieldName(2) == "next");
  assert(fieldsA->fieldHasDefaultValue(2) == false);
  assert(fieldsA->fieldType(2).kind() == QualifiedType::VAR);
  assert(fieldsA->fieldType(2).type()->toClassType());

  auto ctB = fieldsA->fieldType(2).type()->toClassType();
  assert(ctB);
  assert(ctB->basicClassType());
  assert(ctB->basicClassType()->instantiatedFrom());

  auto defaultsPolicy = DefaultsPolicy::IGNORE_DEFAULTS;
  const ResolvedFields& fieldsB = fieldsForTypeDecl(context, ctB->basicClassType(),
                                                    defaultsPolicy);

  assert(fieldsB.numFields() == 3);
  assert(fieldsB.fieldName(0) == "eltType");
  assert(fieldsB.fieldHasDefaultValue(0) == false);
  assert(fieldsB.fieldType(0).kind() == QualifiedType::TYPE);
  assert(fieldsB.fieldType(0).type() == RealType::get(context, 0));
  assert(fieldsB.fieldName(1) == "data");
  assert(fieldsB.fieldHasDefaultValue(1) == false);
  assert(fieldsB.fieldType(1).kind() == QualifiedType::VAR);
  assert(fieldsB.fieldType(1).type() == RealType::get(context, 0));
  assert(fieldsB.fieldName(2) == "next");
  assert(fieldsB.fieldHasDefaultValue(2) == false);
  assert(fieldsB.fieldType(2).kind() == QualifiedType::VAR);
  assert(fieldsB.fieldType(2).type()->toClassType());
  assert(fieldsB.fieldType(2).type()->toClassType()->basicClassType() == ctA->basicClassType());
}

static void test43() {
  printf("test43\n");
  Context ctx;
  Context* context = &ctx;
  auto p = parseTypeAndFieldsOfX(context,
                        R""""(
                        class A {
                          type TX;
                          var x : TX;
                        }

                        class B : A {
                          type TY;
                          var y : TY;
                        }

                        var x : unmanaged B(int, real)?;
                        )"""");

  auto rt = p.first->toClassType();
  assert(rt);
  assert(rt->decorator().isUnmanaged());

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 2);
  assert(fields->fieldType(0).type()->isRealType());

  auto parent = rt->basicClassType()->parentClassType();
  auto pf = parent->substitutions();
  assert(pf.size() == 1);
  assert(pf.begin()->second.type()->isIntType());
}

static void test44() {
  printf("test44\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  // Test that symbols referred to in type constructors are visible, including:
  // - types declared as a sibling to the type being constructed
  // - types visible through a 'private use'
  std::string program = R"""(
  module M {
    module Other {
      enum color {red, green, blue}

      class Parent {
        param col : color;
      }
    }

    private use Other;

    enum coords {x, y};

    class Child : Parent {
      type T;
      param p : coords;
    }

    // Type constructor should be able to see 'coords' and 'foobar'
    var x : unmanaged Child(color.red, int, coords.x)?;
    }
    )""";

  auto p = parseTypeAndFieldsOfX(context, program.c_str());

  auto xt = p.first->toClassType();
  assert(xt->decorator().isUnmanaged());
  assert(xt->decorator().isNilable());

  auto fields = p.second;
  assert(fields);
  assert(fields->numFields() == 2);
  assert(fields->fieldType(0).type()->isIntType());

  auto pt = fields->fieldType(1);
  assert(pt.type()->isEnumType());
  assert(pt.param()->isEnumParam());
  auto param = pt.param()->toEnumParam();
  // TODO: properly stringify enum params
  assert(param->value().str() == "M.coords@0");


  auto parent = xt->basicClassType()->parentClassType();
  auto pf = parent->substitutions();
  assert(pf.size() == 1);
  assert(pf.begin()->second.type()->isEnumType());
  assert(pf.begin()->second.param()->toEnumParam()->value().str() == "M.Other.color@0");
}

int main() {
  test1();
  test2();
  test3();
  test4a();
  test4b();
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
  test23q();
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
  testTypeAndFnSameName();
  test35();
  test36();
  test37();
  test38();
  test39();
  test40();
  test41();
  test42();
  testRecursiveTypeConstructor();
  testRecursiveTypeConstructorAlias();
  testRecursiveTypeConstructorGeneric();
  testRecursiveTypeConstructorMutual();
  test43();
  test44();

  return 0;
}
