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

#include "test-parsing.h"

#include "chpl/parsing/Parser.h"
#include "chpl/framework/Context.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Block.h"
#include "chpl/uast/Forall.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/TaskVar.h"
#include "chpl/uast/WithClause.h"
#include "chpl/uast/Zip.h"

#include <iostream>
#include <algorithm>

std::map<AstTag, std::tuple<std::string, std::string, std::string>> gLoopBits = {
  { asttags::For, {"for", "in 1..10", ""}},
  { asttags::Forall, {"forall", "in 1..10", ""}},
  { asttags::Coforall, {"coforall", "in 1..10", ""}},
  { asttags::While, {"while true", "", ""}},
  { asttags::DoWhile, {"do", "", "while true;"}},
};

int gFreshVarCounter = 0;

class Part {
 private:
  chpl::optional<chpl::uast::AstTag> expectTag;
  std::string theString;

 public:

  static Part attr(std::string name) {
    Part toReturn;
    toReturn.theString = std::move(name);
    return toReturn;
  }

  static Part loop(AstTag tag) {
    Part toReturn;
    toReturn.expectTag = tag;
    return toReturn;
  }

  void wrap(std::vector<std::string>& lines) const {
    if (expectTag) {
      // indent everyhting
      for (auto& line : lines) line = "  " + line;

      // It's some kind of loop, put it together!
      auto it = gLoopBits.find(*expectTag);
      assert(it != gLoopBits.end());

      auto beforeVar = std::get<0>(it->second);
      auto afterVar = std::get<1>(it->second);
      auto afterEndBody = std::get<2>(it->second);

      if (!afterVar.empty()) {
        auto newVar = "i" + std::to_string(gFreshVarCounter++);
        lines.insert(lines.begin(), beforeVar + " " + newVar + " " + afterVar + " {");
      } else {
        lines.insert(lines.begin(), beforeVar + " {");
      }
      lines.push_back("} " + afterEndBody);
    } else {
      // It's an attribute
      lines.insert(lines.begin(), "@" + theString);
    }
  }

  void unwrap(int& childCounter, const uast::Loop*& node) const {
    if (expectTag) {
      assert(node->tag() == *expectTag);
      childCounter = 0;
      if (node->body()->numChildren() > 0) {
        auto nodeChild = node->body()->child(0);
        assert(nodeChild && nodeChild->isLoop());
        node = nodeChild->toLoop();
      } else {
        node = nullptr;
      }
    } else {
      assert(node->attributeGroup());
      // asserts internally
      auto nthChild = node->attributeGroup()->child(childCounter);
      auto nthAttribute = nthChild->toAttribute();
      assert(nthAttribute);
      assert(nthAttribute->name() == theString);
      childCounter++;
    }
  }
};

static void runTest(const char* testName, Parser* parser, const std::vector<Part>& parts) {
  parser->context()->advanceToNextRevision(false);
  std::vector<std::string> lines;
  for (auto it = parts.rbegin(); it != parts.rend(); it++) {
    it->wrap(lines);
  }

  std::string program;
  for (auto& line : lines) {
    program += line;
    program += "\n";
  }

  std::cout << "---- Running test for the following program ----" << std::endl;
  std::cout << program << std::endl << std::endl;

  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, testName, program.c_str());
  assert(!guard.realizeErrors());
  assert(parseResult.numTopLevelExpressions() == 1);

  // Each part will "descend" into the AST, modifying childCounter
  // and currentLoop.
  int childCounter = 0;
  auto firstNode = parseResult.topLevelExpression(0)->toModule()->stmt(0);
  assert(firstNode && firstNode->isLoop());
  auto currentLoop = firstNode->toLoop();
  for (auto& part : parts) {
    assert(currentLoop);
    part.unwrap(childCounter, currentLoop);
  }
  assert(currentLoop == nullptr); // made it to the end
}

static void test1(Parser* parser) {
  for (auto pair : gLoopBits) {
    runTest("oneLoopOneAttr.chpl", parser, {
        Part::attr("llvm.assertVectorized"),
        Part::loop(pair.first),
    });
  }
}

static void test2(Parser* parser) {
  for (auto pair : gLoopBits) {
    runTest("oneLoopTwoAttr.chpl", parser, {
        Part::attr("llvm.assertVectorized"),
        Part::attr("llvm.attribute"),
        Part::loop(pair.first),
    });
  }
}

static void test3(Parser* parser) {
  for (auto pair1 : gLoopBits) {
    for (auto pair2 : gLoopBits) {
      runTest("outerLoopAttr.chpl", parser, {
          Part::attr("llvm.assertVectorized"),
          Part::loop(pair1.first),
            Part::loop(pair2.first),
      });
    }
  }
}

static void test4(Parser* parser) {
  for (auto pair1 : gLoopBits) {
    for (auto pair2 : gLoopBits) {
      runTest("innerLoopAttr.chpl", parser, {
          Part::loop(pair1.first),
            Part::attr("llvm.assertVectorized"),
            Part::loop(pair2.first),
      });
    }
  }
}

static void test5(Parser* parser) {
  for (auto pair1 : gLoopBits) {
    for (auto pair2 : gLoopBits) {
      runTest("bothLoopAttr.chpl", parser, {
          Part::attr("llvm.attribute"),
          Part::loop(pair1.first),
            Part::attr("llvm.assertVectorized"),
            Part::loop(pair2.first),
      });
    }
  }
}

static void test6(Parser* parser) {
  for (auto pair1 : gLoopBits) {
    for (auto pair2 : gLoopBits) {
      runTest("bothLoopAttr.chpl", parser, {
          Part::attr("llvm.attribute"),
          Part::attr("llvm.notARealAttributeButAllowed"),
          Part::loop(pair1.first),
            Part::attr("llvm.assertVectorized"),
            Part::attr("llvm.alsoNotARealAttributeButAllowed"),
            Part::loop(pair2.first),
      });
    }
  }
}

int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::createForTopLevelModule(ctx);
  Parser* p = &parser;

  test1(p);
  test2(p);
  test3(p);
  test4(p);
  test5(p);
  test6(p);

  return 0;
}
