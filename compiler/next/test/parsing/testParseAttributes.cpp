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

#include "chpl/parsing/Parser.h"
#include "chpl/queries/Context.h"
#include "chpl/uast/all-uast.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace uast;
using namespace parsing;

static bool areAttributesEqual(const Decl* lhs, const Decl* rhs) {
  auto lhsAttr = lhs->attributes();
  auto rhsAttr = rhs->attributes();

  if (!lhsAttr && !rhsAttr) return true;
  if (!lhsAttr || !rhsAttr) return false;

  bool equalPragmas = true;
  for (PragmaTag pragma : lhsAttr->pragmas()) {
    if (!rhsAttr->hasPragma(pragma)) {
      equalPragmas = false;
      break;
    }
  }

  return lhsAttr->isDeprecated() == rhsAttr->isDeprecated() &&
    lhsAttr->deprecationMessage() == rhsAttr->deprecationMessage() &&
    equalPragmas;
}

static void test0(Parser* parser) {
  auto parseResult = parser->parseString("test0.chpl",
      "pragma \"no doc\"\n"
      "var a, b, c = 0;\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto multiVar = mod->stmt(0)->toMultiDecl();
  assert(multiVar);

  auto parentAttr = multiVar->attributes();
  assert(parentAttr);
  assert(!parentAttr->isDeprecated());
  assert(parentAttr->deprecationMessage().isEmpty());
  assert(parentAttr->hasPragma(PRAGMA_NO_DOC));

  for (auto decl : multiVar->decls()) {
    assert(decl->isVariable());
    assert(areAttributesEqual(multiVar, decl));
  }
}

static void test1(Parser* parser) {
  auto parseResult = parser->parseString("test1.chpl",
      "pragma \"no doc\"\n"
      "var (a, b, c) = foo;\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto tupleVar = mod->stmt(0)->toTupleDecl();
  assert(tupleVar);

  auto parentAttr = tupleVar->attributes();
  assert(parentAttr);
  assert(!parentAttr->isDeprecated());
  assert(parentAttr->deprecationMessage().isEmpty());
  assert(parentAttr->hasPragma(PRAGMA_NO_DOC));

  for (auto decl : tupleVar->decls()) {
    assert(decl->isVariable());
    assert(areAttributesEqual(tupleVar, decl));
  }
}

static void test2(Parser* parser) {
  auto parseResult = parser->parseString("test2.chpl",
      "pragma \"no doc\"\n"
      " class C {\n"
      "  var a = 0;\n"
      "}\n");
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);

  auto cls = mod->stmt(0)->toClass();
  assert(cls);

  auto clsAttr = cls->attributes();
  assert(clsAttr);

  assert(!clsAttr->isDeprecated());
  assert(clsAttr->deprecationMessage().isEmpty());
  assert(clsAttr->hasPragma(PRAGMA_NO_DOC));

  assert(cls->numDeclOrComments() == 1);
  auto var = cls->declOrComment(0)->toVariable();
  assert(var);
  assert(!var->attributes());
}

static void test3(Parser* parser) {
  auto parseResult = parser->parseString("test2.chpl",
      "pragma \"no doc\"\n"
      "var a, b, c = 0;\n");
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  assert(mod->numStmts() == 1);
}

int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::build(ctx);
  Parser* p = parser.get();

  test0(p);
  test1(p);
  test2(p);
  test3(p);

  return 0;
}
