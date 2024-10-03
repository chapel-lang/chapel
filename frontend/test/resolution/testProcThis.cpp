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

#include "chpl/framework/ErrorWriter.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"

static void test1() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  QualifiedType qt = resolveTypeOfXInit(context,
                R""""(
                  module M {
                    record R { }
                    proc R.this(arg: int) { return 1.0; }
                    var a: R;
                    var x = a(0);
                  }
                )"""");

  assert(!guard.realizeErrors());
  assert(qt.type());
  assert(qt.type() == RealType::get(context, 0));
}

static void test2() {
  // Test resolution of 'this' on a taskVar in begin statement
  printf("test2\n");
  auto config = getConfigWithHome();
  Context ctx(config);
  Context* context = &ctx;
  ErrorGuard guard(context);
  setupModuleSearchPaths(context, false, false, {}, {});

  auto program = R""""(
                  module M {
                    record R { var y : int; }
                    proc R.this(i: int) { return this.y + i; }

                    var myVar = new R(42);                      

                    begin with (in myVar) {                        
                      var x = myVar[5];                        
                    } 
                  }
                )"""";
  auto m = parseModule(context, std::move(program));
  assert(m->numStmts() > 0);
  auto begin = m->stmt(m->numStmts()-1)->toBegin();
  assert(begin);
  const Variable* x = begin->stmt(0)->toVariable();
  assert(x);
  assert(x->name() == "x");

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  auto qt = rr.byAst(x).type();
  assert(qt.type()->isIntType());
  assert(!guard.realizeErrors()); 
}

//test resolution of 'this' on a taskVar in a coforall statement
static void test3() {
  printf("test3\n");
  auto config = getConfigWithHome();
  Context ctx(config);
  Context* context = &ctx;
  ErrorGuard guard(context);
  setupModuleSearchPaths(context, false, false, {}, {});

  auto program = R""""(
                  module M {
                    record R { var y : int; }
                    proc R.this(i: int) { return this.y + i; }

                    var myVar = new R(42);                      

                    coforall i in 1..10 with (in myVar) {                        
                      var x = myVar[5];                        
                    } 
                  }
                )"""";
  auto m = parseModule(context, std::move(program));
  assert(m->numStmts() > 0);
  auto coforall = m->stmt(m->numStmts()-1)->toCoforall();
  assert(coforall);
  const Variable* x = coforall->stmt(0)->toVariable();
  assert(x);
  assert(x->name() == "x");

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  auto qt = rr.byAst(x).type();
  assert(qt.type()->isIntType());

  assert(!guard.realizeErrors()); 
}

int main() {
  test1();
  test2();
  test3();

  return 0;
}

