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
#include "test-minimal-modules.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

#include <sstream>

template <typename F>
void testCPtrArg(const char* formalType, const char* actualType, F&& test) {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  std::stringstream ss;

  ss << "record rec { type someType; }" << std::endl;
  ss << "proc f(x: " << formalType << ") {}" << std::endl;
  ss << "var arg: " << actualType << ";" << std::endl;
  ss << "var x = f(arg);" << std::endl;

  auto program = ss.str();

  auto filePath = UniqueString::get(context, "testFile.chpl");
  setFileText(context, filePath, std::move(program));
  auto modules = parseToplevel(context, filePath);

  assert(modules.size() == 1);
  auto mainMod = modules[0];
  assert(mainMod->numStmts() == 4);

  auto fChild = mainMod->child(1);
  assert(fChild->isFunction());
  auto fFn = fChild->toFunction();
  assert(fFn->name() == "f");

  auto fCallVar = mainMod->child(3);
  assert(fCallVar->isVariable());

  auto& modResResult = resolveModule(context, mainMod->id());
  auto& rr = modResResult.byAst(fCallVar->toVariable()->initExpression());

  debuggerBreakHere();
  auto fn = rr.mostSpecific().only().fn();

  const types::CPtrType* formalTypePtr = nullptr;
  if (fn != nullptr) {
    auto formalQt = fn->formalType(0);
    if (auto tt = formalQt.type()) {
      formalTypePtr = tt->toCPtrType();
    }
  }

  test(fn, formalTypePtr, guard);
}

static void test1() {
  testCPtrArg("c_ptr", "c_ptr(int)", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(fn);
    assert(t);
    auto eltT = t->eltType();
    assert(eltT && eltT->isIntType());
    assert(eltT->toIntType()->isDefaultWidth());
  });
}

static void test2() {
  testCPtrArg("c_ptr", "c_ptr(real)", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(fn);
    assert(t);
    auto eltT = t->eltType();
    assert(eltT && eltT->isRealType());
    assert(eltT->toRealType()->isDefaultWidth());
  });
}

static void test3() {
  testCPtrArg("c_ptr(int(?w))", "c_ptr(int(32))", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(fn);
    assert(t);
    auto eltT = t->eltType();
    assert(eltT && eltT->isIntType());
    assert(eltT == IntType::get(eg.context(), 32));
  });
}

static void test4() {
  testCPtrArg("c_ptr(rec(?t))", "c_ptr(rec(int))", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(fn);
    assert(t);
    auto eltT = t->eltType();
    assert(eltT && eltT->isRecordType());
    auto rt = eltT->toRecordType();
    assert(rt->name() == "rec");
    auto& fields = fieldsForTypeDecl(eg.context(), rt, DefaultsPolicy::IGNORE_DEFAULTS);
    assert(fields.numFields() == 1 && fields.fieldType(0).type()->isIntType());
  });
}

static void test5() {
  testCPtrArg("c_ptr(int(?w))", "c_ptr(uint(32))", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(!fn);
    assert(eg.realizeErrors() == 1);
  });
}

static void test6() {
  testCPtrArg("c_ptr(int(64))", "c_ptr(int(32))", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(!fn);
    assert(eg.realizeErrors() == 1);
  });
}

static void test7() {
  testCPtrArg("c_ptr(int)", "c_ptr(int)", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(fn);
    assert(t);
    auto eltT = t->eltType();
    assert(eltT && eltT->isIntType());
    assert(eltT->toIntType()->isDefaultWidth());
  });
}

static void test8() {
  testCPtrArg("c_ptr(void)", "c_ptr(int)", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(t);
    assert(t->isVoidPtr());
    // expect no errors; this should be valid.
  });
}

static void test9() {
  testCPtrArg("c_ptrConst", "c_ptrConst(int)", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(fn);
    assert(t);
    assert(t->isConst());
    auto eltT = t->eltType();
    assert(eltT && eltT->isIntType());
    assert(eltT->toIntType()->isDefaultWidth());
  });
}

static void test10() {
  testCPtrArg("c_ptrConst", "c_ptrConst(real)", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(fn);
    assert(t);
    assert(t->isConst());
    auto eltT = t->eltType();
    assert(eltT && eltT->isRealType());
    assert(eltT->toRealType()->isDefaultWidth());
  });
}

static void test11() {
  testCPtrArg("c_ptrConst(int(?w))", "c_ptrConst(int(32))", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(fn);
    assert(t);
    assert(t->isConst());
    auto eltT = t->eltType();
    assert(eltT && eltT->isIntType());
    assert(eltT == IntType::get(eg.context(), 32));
  });
}

static void test12() {
  testCPtrArg("c_ptrConst(rec(?t))", "c_ptrConst(rec(int))", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(fn);
    assert(t);
    assert(t->isConst());
    auto eltT = t->eltType();
    assert(eltT && eltT->isRecordType());
    auto rt = eltT->toRecordType();
    assert(rt->name() == "rec");
    auto& fields = fieldsForTypeDecl(eg.context(), rt, DefaultsPolicy::IGNORE_DEFAULTS);
    assert(fields.numFields() == 1 && fields.fieldType(0).type()->isIntType());
  });
}

static void test13() {
  testCPtrArg("c_ptrConst(int(?w))", "c_ptrConst(uint(32))", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(!fn);
    assert(eg.realizeErrors() == 1);
  });
}

static void test14() {
  testCPtrArg("c_ptrConst(int(64))", "c_ptrConst(int(32))", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(!fn);
    assert(eg.realizeErrors() == 1);
  });
}

static void test15() {
  testCPtrArg("c_ptrConst(int)", "c_ptrConst(int)", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(fn);
    assert(t);
    assert(t->isConst());
    auto eltT = t->eltType();
    assert(eltT && eltT->isIntType());
    assert(eltT->toIntType()->isDefaultWidth());
  });
}

static void test16() {
  testCPtrArg("c_ptrConst(void)", "c_ptrConst(int)", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(t);
    assert(t->isConst());
    assert(t->isVoidPtr());
    // expect no errors; this should be valid.
  });
}

static void test17() {
  testCPtrArg("c_ptr(void)", "c_ptrConst(int)", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(fn);
    assert(t);
    assert(!t->isConst());
    assert(t->isVoidPtr());
    // expect no errors; this is the pattern of passing a const to deallocate
  });
}

static void test18() {
  testCPtrArg("c_ptrConst(int)", "c_ptr(int)", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(fn);
    assert(t);
    assert(t->isConst());
    assert(t->eltType()->isIntType());
  });
}

static void test19() {
  testCPtrArg("c_ptrConst(int(?w))", "c_ptr(int(32))", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(fn);
    assert(t);
    assert(t->isConst());
    auto eltT = t->eltType();
    assert(eltT && eltT->isIntType());
    assert(eltT == IntType::get(eg.context(), 32));
  });
}

static void test20() {
  testCPtrArg("c_ptrConst(rec(?t))", "c_ptr(rec(int))", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(fn);
    assert(t);
    auto eltT = t->eltType();
    assert(eltT && eltT->isRecordType());
    auto rt = eltT->toRecordType();
    assert(rt->name() == "rec");
    auto& fields = fieldsForTypeDecl(eg.context(), rt, DefaultsPolicy::IGNORE_DEFAULTS);
    assert(fields.numFields() == 1 && fields.fieldType(0).type()->isIntType());
  });
}

static void test21() {
  testCPtrArg("c_ptr(int(?w))", "c_ptrConst(int(32))", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(!fn);
    assert(eg.realizeErrors() == 1);
  });
}

static void test22() {
  testCPtrArg("c_ptr(rec(?t))", "c_ptrConst(rec(int))", [](const TypedFnSignature* fn, const CPtrType* t, ErrorGuard& eg) {
    assert(!fn);
    assert(eg.realizeErrors() == 1);
  });
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

  return 0;
}
