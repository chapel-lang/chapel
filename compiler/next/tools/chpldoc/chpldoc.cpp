/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include <algorithm>
#include <iostream>

#include "chpl/parsing/Parser.h"
#include "chpl/queries/Context.h"
#include "chpl/uast/ASTTypes.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"

using namespace chpl;
using namespace uast;
using namespace parsing;

// Remove the leading+trailing // or /* (*/)
std::string stripComment(const std::string &s) {
  if (s.substr(0, 2) == "//")
    return s.substr(2);
  if (s.substr(0, 2) == "/*") {
    size_t l = s.length();
    assert(s.substr(l - 2 - 1, 2) == "*/");
    return s.substr(2, l - 4);
  }
  assert(false);
  return "";
}

const char *testString = "/* comment 1 */\n"
                         "module M {\n"
                         "  // comment 2 \n"
                         "  x;\n"
                         "  /* comment 3 */\n"
                         "}\n"
                         "/* comment 4 */";

int main(int argc, char **argv) {
  Context context;
  Context *ctx = &context;

  auto parser = Parser::build(ctx);
  Parser *p = parser.get();

  auto parseResult = argc == 2 ? parser->parseFile(argv[1])
                               : parser->parseString("test1a.chpl", testString);

  if (parseResult.numErrors()) {
    // for (auto &err : parseResult.errors)
    for (size_t i = 0; i < parseResult.numErrors(); i++) {
      // std::cerr << parseResult.error(i) << '\n';
      auto &err = parseResult.error(i);
      std::cerr << err.path().c_str() << ":" << err.line() << ":"
                << err.firstColumn() << ":" << err.message() << "\n";
    }
    return 1;
  }

  std::cerr << "got " << parseResult.numTopLevelExpressions()
            << " top level expressions\n";

  // TODO why can't I do this
  // const auto &topLevelExpressions = parseResult.topLevelExpressions();
  // auto it = std::find_if(topLevelExpressions.begin(),
  // topLevelExpressions.end(), [](const auto &x){ return false; });

  int module_i = -1;
  for (ssize_t i = 0; i < parseResult.numTopLevelExpressions(); i++) {
    if (parseResult.topLevelExpression(i)->isModule()) {
      module_i = i;
      break;
    }
  }

  if (module_i == -1) {
    std::cerr << "Did not find any top level module expressions\n";
    return 1;
  }

  // Print module title
  const Module *mod = parseResult.topLevelExpression(module_i)->toModule();
  std::cout << mod->name().c_str() << "\n";
  std::cout << std::string(mod->name().length(), '=') << "\n\n";

  // Print preceding comment to module
  if (module_i >= 1) {
    if (const Comment *comment =
            parseResult.topLevelExpression(module_i - 1)->toComment()) {
      std::cout << stripComment(comment->str()) << "\n";
    }
  }

  for (size_t i = 0; i < mod->numChildren(); i++) {
    // TODO things like proc, record, class etc get pulled out into eg
    // .. {record,function,class,method,itermethod}:: name and signature for
    // functions
    //    followed by their preceding doc comment
    //    followed by their attributes and procs
    if (const Comment *comment = mod->child(i)->toComment()) {
      std::cout << stripComment(comment->str()) << "\n";
    } else {
      // TODO this should print the original text I think and not an AST dump
      ASTNode::dump(mod->child(i));
    }
  }

  // Print trailing comments to module TODO do we actually care about these
  // comments
  for (size_t i = module_i + 1; i < parseResult.numTopLevelExpressions(); i++) {
    if (const Comment *comment =
            parseResult.topLevelExpression(i)->toComment()) {
      std::cout << stripComment(comment->str()) << "\n";
    }
  }

  return 0;
}
