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

  auto qt = resolveTypeOfXInit(context,
                             R""""(
                               record R {
                                 var field: int;
                               }
                               var rec: R;
                               var x = rec.field;
                             )"""");

  assert(qt.kind() == QualifiedType::REF);
  assert(qt.type() == IntType::get(context, 0));
}

static void test2() {
  printf("test2\n");
  Context ctx;
  Context* context = &ctx;

  // We want the return expression 'idxType' to find the record's field,
  // not the module-scope type declaration.
  auto path = UniqueString::get(context, "test2.chpl");
  std::string contents = R""""(
      module M {
        type idxType = int;

        record R {
          type idxType;
          var x : idxType;
        }

        proc R.helper() : idxType {
          return x;
        }
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);

  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 3);

  const AggregateDecl* ad = m->stmt(1)->toAggregateDecl();
  const VarLikeDecl* field = ad->declOrComment(0)->toVarLikeDecl();

  const Function* fn = m->stmt(2)->toFunction();
  assert(fn);
  const Identifier* ret = fn->returnType()->toIdentifier();
  assert(ret);

  auto results = scopeResolveFunction(context, fn->id());
  assert(field->id() == results->resolutionById().byAst(ret).toId());
}


int main() {
  test1();
  test2();

  return 0;
}
