/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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
#include "chpl/types/all-types.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/For.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"
#include "chpl/uast/While.h"

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

static auto myiter = std::string(R""""(
                     iter myiter() {
                       yield 1;
                     }

                     )"""");

static auto recIter = std::string(R""""(
                      record R {
                        iter these() {
                          yield 1;
                        }
                      }

                      )"""");

std::vector<ErrorMessage> errors;
static void collectErrors(const ErrorMessage& err) { errors.push_back(err); }

static const Module* parseModule(Context* context, const char* src) {
  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = src;
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 1);

  return vec[0];
}

static const Module* parseModule(Context* context, std::string& str) {
  return parseModule(context, str.c_str());
}

//
// Testing resolution of loop index variables
// TODO:
// - param loops
// - zippered iteration
// - forall loops
// - coforall loops
// - error messages
//

static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  //
  // Test iteration over an iterator call
  //

  auto iterText = myiter +
                  R""""(
                  for i in myiter() {
                  }
                  )"""";

  const Module* m = parseModule(context, iterText);
  auto loop = m->stmt(1)->toFor();

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  auto idx = rr.byAst(loop->index());
  assert(idx.type().type() == IntType::get(context, 0));
}

static void test2() {
  printf("test2\n");
  Context ctx;
  Context* context = &ctx;

  errors.clear();
  ctx.setErrorHandler(collectErrors);

  //
  // Make sure nothing explodes when encountering an ambiguity
  //

  auto iterText = myiter +
                  R""""(
                  iter myiter() {
                    yield "str";
                  }
                  for i in myiter() {
                  }
                  )"""";

  const Module* m = parseModule(context, iterText);
  auto loop = m->stmt(2)->toFor();

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  auto idx = rr.byAst(loop->index());
  assert(idx.type().isErroneousType());
  assert(errors.size() == 1);
}

static void test3() {
  printf("test3\n");
  Context ctx;
  Context* context = &ctx;

  //
  // Test iteration over expression requiring ".these()"
  //

  auto theseText = recIter +
                   R""""(
                   var r : R;
                   for i in r {
                   }
                   )"""";

  const Module* m = parseModule(context, theseText);
  auto loop = m->stmt(2)->toFor();

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  auto idx = rr.byAst(loop->index());
  assert(idx.type().type() == IntType::get(context, 0));
}

static void test4() {
  printf("test4\n");
  Context ctx;
  Context* context = &ctx;

  //
  // Should call .these on the returned value
  //
  auto theseText = recIter +
                   R""""(
                   proc retR() {
                     var r : R;
                     return r;
                   }
                   for i in retR() {
                   }
                   )"""";

  const Module* m = parseModule(context, theseText);
  auto loop = m->stmt(2)->toFor();

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  auto idx = rr.byAst(loop->index());
  assert(idx.type().type() == IntType::get(context, 0));
}

static void test5() {
  printf("test5\n");
  Context ctx;
  Context* context = &ctx;

  errors.clear();
  ctx.setErrorHandler(collectErrors);

  //
  // Should issue some kind of error
  //
  auto theseText = R""""(
                   record R {
                   }
                   var r : R;
                   for i in r {
                   }
                   )"""";

  errors.clear();
  const Module* m = parseModule(context, theseText);
  auto loop = m->stmt(2)->toFor();

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  auto idx = rr.byAst(loop->index());
  assert(idx.type().isErroneousType());
  assert(errors.size() == 1);
}

static void test6() {
  printf("test6\n");
  Context ctx;
  Context* context = &ctx;

  errors.clear();
  ctx.setErrorHandler(collectErrors);

  //
  // Should issue some kind of ambiguity error
  //
  auto theseText = R""""(
                   record R {
                     iter these() {
                       yield 1;
                     }
                     iter these() {
                       yield "str";
                     }
                   }
                   var r : R;
                   for i in r {
                   }
                   )"""";

  const Module* m = parseModule(context, theseText);
  auto loop = m->stmt(2)->toFor();

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  auto idx = rr.byAst(loop->index());
  assert(idx.type().isErroneousType());
  assert(errors.size() == 1);
}

static void test7() {
  printf("test7\n");
  Context ctx;
  Context* context = &ctx;

  errors.clear();
  ctx.setErrorHandler(collectErrors);

  auto iterText = myiter +
                  R""""(
                  for myiter() {
                  }
                  )"""";
  const Module* m = parseModule(context, iterText);

  resolveModule(context, m->id());

  assert(errors.size() == 0);
}

static void test8() {
  printf("test8\n");
  Context ctx;
  Context* context = &ctx;

  errors.clear();
  ctx.setErrorHandler(collectErrors);

  auto theseText = recIter +
                   R""""(
                   var r : R;
                   for r {
                   }
                   )"""";
  const Module* m = parseModule(context, theseText);
  auto loop = m->stmt(2)->toFor();

  //
  // Confirm that a '.these' iterator was still resolved and stored
  //
  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  auto& iterandRE = rr.byAst(loop->iterand());
  auto& associatedFns = iterandRE.associatedFns();
  assert(associatedFns.size() == 1);

  auto theseFn = associatedFns[0];
  auto recR = m->stmt(0)->toRecord();
  auto recThese = recR->declOrComment(0)->toFunction();
  assert(theseFn->id() == recThese->id());

  assert(errors.size() == 0);
}

static void test9() {
  printf("test9\n");
  Context ctx;
  Context* context = &ctx;

  errors.clear();
  ctx.setErrorHandler(collectErrors);

  auto iterText = R""""(
                   iter myIter() {
                     var i : int;
                     while __primitive("C for loop",
                             __primitive("=", i, 0),
                             __primitive("<=", i, 9),
                             __primitive("+=", i, 1)) {
                       yield i;
                     }
                   }

                   var x = 0;
                   for i in myIter() {
                     __primitive("+=", x, i);
                   }
                   )"""";
  const Module* m = parseModule(context, iterText);
  auto loop = m->stmt(2)->toFor();

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  auto idx = rr.byAst(loop->index());
  assert(idx.type().type() == IntType::get(context, 0));

  const TypedFnSignature* sig = rr.byAst(loop->iterand()).mostSpecific().only();
  auto fn = resolveFunction(context, sig, nullptr);
  auto rf = fn->resolutionById();
  auto whileLoop = m->stmt(0)->toFunction()->stmt(1)->toWhile();
  auto cond = rf.byAst(whileLoop->condition());
  assert(cond.type().type() == BoolType::get(context, 0));

  assert(errors.size() == 0);
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

  return 0;
}
