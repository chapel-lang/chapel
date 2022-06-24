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
#include "chpl/uast/Block.h"
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
using namespace uast;

static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "test1.chpl");
  std::string contents = R""""(
      module M {
        var y: int;
      }

      module N {
        {
          import M;
          var x = M.y;
        }
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 2);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 1);
  const Variable* y = m->stmt(0)->toVariable();
  assert(y);
  const Module* n = vec[1]->toModule();
  assert(n);
  assert(n->numStmts() == 1);
  const Block* nBlock = n->stmt(0)->toBlock();
  assert(nBlock);
  assert(nBlock->numStmts() == 2);
  const Variable* x = nBlock->stmt(1)->toVariable();
  assert(x);
  const AstNode* xInit = x->initExpression();
  assert(xInit);

  // if scopeResolveModule ever goes away, this can safely
  // be replaced by resolveModule.
  const ResolutionResultByPostorderID& rr =
    scopeResolveModule(context, n->id());
  const ResolvedExpression& re = rr.byAst(xInit);

  assert(re.toId() == y->id());
}


int main() {
  test1();

  return 0;
}
