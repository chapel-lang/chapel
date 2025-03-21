/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#include "test-parsing.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/uast/Include.h"
#include "chpl/uast/Module.h"

static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  auto pathMM = UniqueString::get(context, "MM.chpl");
  std::string contentsMM = R""""(
      module MM {
        include module Sub;
        import this.Sub.y;
        var x = y;
      }
   )"""";
  setFileText(context, pathMM, contentsMM);

  auto pathMMSub = UniqueString::get(context, "MM/Sub.chpl");
  std::string contentsMMSub = R""""(
      module Sub {
        var y: int;
      }
   )"""";
  setFileText(context, pathMMSub, contentsMMSub);


  const ModuleVec& vec = parseToplevel(context, pathMM);
  assert(vec.size() == 1);
  const Module* mm = vec[0];
  assert(mm->name() == "MM");
  assert(mm->id().symbolPath() == "MM");
  assert(mm->numStmts() == 3);
  const Include* inc = mm->stmt(0)->toInclude();
  assert(inc);
  assert(inc->visibility() == Decl::DEFAULT_VISIBILITY);
  assert(inc->isPrototype() == false);
  assert(inc->name() == "Sub");

  const Module* sub = getIncludedSubmodule(context, inc->id());
  assert(sub);
  assert(sub->name() == "Sub");
  assert(sub->id().symbolPath() == "MM.Sub");
}

int main() {
  test1();

  return 0;
}
