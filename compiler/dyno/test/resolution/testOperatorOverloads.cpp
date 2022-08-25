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

/* #include "chpl/parsing/parsing-queries.h" */
/* #include "chpl/resolution/resolution-queries.h" */
/* #include "chpl/types/ComplexType.h" */
/* #include "chpl/types/IntType.h" */
/* #include "chpl/types/Param.h" */
/* #include "chpl/types/RealType.h" */
/* #include "chpl/uast/Module.h" */
#include "common.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

static void test1() {
  Context ctx;
  auto context = &ctx;

  QualifiedType qt1 = resolveTypeOfXInit(context, "\
  record R {\
    var field: int;\
  }\
  var myR: R;\
  var x = myR : int;");
  assert(qt1.kind() == QualifiedType::UNKNOWN);
  assert(qt1.type() && qt1.type()->isErroneousType());
  ctx.advanceToNextRevision(false);

  /* QualifiedType qt2 = resolveTypeOfXInit(context, "\ */
  /* record R {\ */
  /*   var field: int;\ */
  /*   operator :(x: R, type t: int) { return x.field; }\ */
  /* }\ */
  /* var myR: R;\ */
  /* var x = myR : int;"); */
  /* ctx.advanceToNextRevision(false); */
}

int main() {
  test1();
  return 0;
}
