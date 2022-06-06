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

// test resolving a very simple module
// Test resolving a simple primary and secondary method call on a record.
static void test1() {
  Context ctx;
  Context* context = &ctx;

  context->advanceToNextRevision(true);

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents =
    " record r {\n"
    "   proc doPrimary() {}\n"
    " }\n"
    " proc r.doSecondary() {}\n"
    " var obj: r;\n"
    " obj.doPrimary();\n"
    " obj.doSecondary();\n";

  setFileText(context, path, contents);

  // Get the module.
  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);

  // Unpack all the uAST we need for the test.
  assert(m->numStmts() == 5);
  auto r = m->stmt(0)->toRecord();
  assert(r);
  assert(r->numDeclOrComments() == 1);
  auto fnPrimary = r->declOrComment(0)->toFunction();
  assert(fnPrimary);
  auto fnSecondary = m->stmt(1)->toFunction();
  assert(fnSecondary);
  auto obj = m->stmt(2)->toVariable();
  assert(obj);
  auto callPrimary = m->stmt(3)->toFnCall();
  assert(callPrimary);
  auto callSecondary = m->stmt(4)->toFnCall();
  assert(callSecondary);

  // Resolve the module.
  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  // Get the type of 'r'.
  auto& qtR = typeForModuleLevelSymbol(context, r->id());

  // Assert some things about the primary call.
  auto& reCallPrimary = rr.byAst(callPrimary);
  auto& qtCallPrimary = reCallPrimary.type();
  assert(qtCallPrimary.type()->isVoidType());
  auto tfsCallPrimary = reCallPrimary.mostSpecific().only();
  assert(tfsCallPrimary);

  // Check the primary call receiver.
  assert(tfsCallPrimary->id() == fnPrimary->id());
  assert(tfsCallPrimary->numFormals() == 1);
  assert(tfsCallPrimary->formalName(0) == "this");
  assert(tfsCallPrimary->formalType(0).type() == qtR.type());

  // Assert some things about the secondary call.
  auto& reCallSecondary = rr.byAst(callSecondary);
  auto& qtCallSecondary = reCallSecondary.type();
  assert(qtCallSecondary.type()->isVoidType());
  auto tfsCallSecondary = reCallSecondary.mostSpecific().only();
  assert(tfsCallSecondary);

  // Check the secondary call receiver.
  assert(tfsCallSecondary->id() == fnSecondary->id());
  assert(tfsCallSecondary->numFormals() == 1);
  assert(tfsCallSecondary->formalName(0) == "this");
  assert(tfsCallSecondary->formalType(0).type() == qtR.type());

  context->collectGarbage();
}

static void test2() {
  Context ctx;
  Context* context = &ctx;

  context->advanceToNextRevision(true);

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents =
    R""""(
      record r {
        proc primary { }
      }
      proc r.secondary { }
      var obj: r;
      obj.primary;
      obj.secondary;
    )"""";
  setFileText(context, path, contents);

  // Get the module.
  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);

  // Unpack all the uAST we need for the test.
  assert(m->numStmts() == 5);
  auto r = m->stmt(0)->toRecord();
  assert(r);
  assert(r->numDeclOrComments() == 1);
  auto fnPrimary = r->declOrComment(0)->toFunction();
  assert(fnPrimary);
  auto fnSecondary = m->stmt(1)->toFunction();
  assert(fnSecondary);
  auto obj = m->stmt(2)->toVariable();
  assert(obj);
  auto callPrimary = m->stmt(3)->toDot();
  assert(callPrimary);
  auto callSecondary = m->stmt(4)->toDot();
  assert(callSecondary);

  // Resolve the module.
  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  // Get the type of 'r'.
  auto& qtR = typeForModuleLevelSymbol(context, r->id());

  // Assert some things about the primary call.
  auto& reCallPrimary = rr.byAst(callPrimary);
  auto& qtCallPrimary = reCallPrimary.type();
  assert(qtCallPrimary.type()->isVoidType());
  auto tfsCallPrimary = reCallPrimary.mostSpecific().only();
  assert(tfsCallPrimary);

  // Check the primary call receiver.
  assert(tfsCallPrimary->id() == fnPrimary->id());
  assert(tfsCallPrimary->numFormals() == 1);
  assert(tfsCallPrimary->formalName(0) == "this");
  assert(tfsCallPrimary->formalType(0).type() == qtR.type());

  // Assert some things about the secondary call.
  auto& reCallSecondary = rr.byAst(callSecondary);
  auto& qtCallSecondary = reCallSecondary.type();
  assert(qtCallSecondary.type()->isVoidType());
  auto tfsCallSecondary = reCallSecondary.mostSpecific().only();
  assert(tfsCallSecondary);

  // Check the secondary call receiver.
  assert(tfsCallSecondary->id() == fnSecondary->id());
  assert(tfsCallSecondary->numFormals() == 1);
  assert(tfsCallSecondary->formalName(0) == "this");
  assert(tfsCallSecondary->formalType(0).type() == qtR.type());

  context->collectGarbage();
}


int main() {
  test1();
  test2();

  return 0;
}

