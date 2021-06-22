/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

#include "chpl/resolution/resolution-queries.h"
#include "chpl/uast/Comment.h"
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
using namespace types;
using namespace uast;

static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  {
    context->advanceToNextRevision(true);
    auto path = UniqueString::build(context, "input.chpl");
    std::string contents = "var x;\n"
                           "x;";
    setFileText(context, path, contents);

    auto& vec = resolveFile(context, path);
    assert(vec.size() == 1);
    const Module* module = vec[0].module;
    const auto* byId = vec[0].resolution; 
    const Variable* varDecl = module->child(0)->toVariable();
    const Identifier* identifier = module->child(1)->toIdentifier();
    assert(varDecl);
    assert(identifier);
    assert(0 == varDecl->name().compare("x"));
    assert(0 == identifier->name().compare("x"));
    assert(byId);
    const ResolutionResult& rr = (*byId)[identifier->id().postOrderId()];
    assert(rr.exp != nullptr);
    assert(rr.decl != nullptr);
    assert(rr.decl == varDecl);
    context->collectGarbage();
  }
}


static void test2() {
  printf("test2\n");
  Context ctx;
  Context* context = &ctx;

  {
    printf("part 1\n");
    context->advanceToNextRevision(true);
    auto path = UniqueString::build(context, "input.chpl");
    std::string contents = "";
    setFileText(context, path, contents);
   
    auto& vec = resolveFile(context, path);
    assert(vec.size() == 1);
    const Module* module = vec[0].module;
    ASTNode::dump(module, 2);
    context->collectGarbage();
  }

  {
    printf("part 2\n");
    context->advanceToNextRevision(true);
    auto path = UniqueString::build(context, "input.chpl");
    std::string contents = "var x;";
    setFileText(context, path, contents);

    auto& vec = resolveFile(context, path);
    assert(vec.size() == 1);
    const Module* module = vec[0].module;
    ASTNode::dump(module, 2);
    const Variable* varDecl = module->child(0)->toVariable();
    assert(varDecl);
    assert(0 == varDecl->name().compare("x"));
    context->collectGarbage();
  }

  for (int i = 0; i < 3; i++) {
    printf("part %i\n", 3+i);
    context->advanceToNextRevision(true);
    auto path = UniqueString::build(context, "input.chpl");
    std::string contents = "var x;\n"
                           "x;";
    setFileText(context, path, contents);

    auto& vec = resolveFile(context, path);
    assert(vec.size() == 1);
    const Module* module = vec[0].module;
    ASTNode::dump(module, 2);
    const auto* byId = vec[0].resolution; 
    const Variable* varDecl = module->child(0)->toVariable();
    const Identifier* identifier = module->child(1)->toIdentifier();
    assert(varDecl);
    assert(identifier);
    assert(0 == varDecl->name().compare("x"));
    assert(0 == identifier->name().compare("x"));
    assert(byId);
    const ResolutionResult& rr = (*byId)[identifier->id().postOrderId()];
    assert(rr.exp != nullptr);
    assert(rr.decl != nullptr);
    assert(rr.decl == varDecl);
    context->collectGarbage();
  }
}


int main() {
  test1();
  test2();

  return 0;
}
