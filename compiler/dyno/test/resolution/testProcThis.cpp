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
#include "chpl/uast/all-uast.h"
#include "common.h"

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

// error handler that causes the test to fail if it runs
static void reportError(Context* context, const ErrorMessage& err) {
  printf("error encountered - %s\n", err.message().c_str());
  assert(false && "fatal error");
}

static void test1() {
  Context ctx;
  Context* context = &ctx;

  // configure context to fail test if there are any errors
  context->setErrorHandler(reportError);

  QualifiedType qt = resolveTypeOfXInit(context,
                R""""(
                  module M {
                    record R { }
                    proc R.this(arg: int) { return 1.0; }
                    var a: R;
                    var x = a(0);
                  }
                )"""");

  assert(qt.type());
  assert(qt.type() == RealType::get(context, 0));
}

int main() {
  test1();

  return 0;
}

