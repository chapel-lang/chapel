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

  auto methodT = typedSignatureInitial(context, methodU);
  auto functionT = typedSignatureInitial(context, functionU);

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

int main() {
  test1();

  return 0;
}
