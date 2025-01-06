/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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
#include <sstream>

std::map<AstTag, std::tuple<std::string, std::string, std::string>> gLoopBits = {
  { asttags::For, {"for", "in 1..10", ""}},
  { asttags::Forall, {"forall", "in 1..10", ""}},
  { asttags::Coforall, {"coforall", "in 1..10", ""}},
  { asttags::While, {"while true", "", ""}},
  { asttags::DoWhile, {"do", "", "while true;"}},
};

int gFreshVarCounter = 0;

class Part;
static void validateParts(const AstNode* container,
                          const std::vector<Part>& parts);
class Part {
 private:
  chpl::optional<chpl::uast::AstTag> expectTag;
  std::string theString;
  std::vector<Part> childParts;

 public:

  static Part attr(std::string name) {
    Part toReturn;
    toReturn.theString = std::move(name);
    return toReturn;
  }

  static Part loop(AstTag tag, std::vector<Part> children = {}) {
    Part toReturn;
    toReturn.expectTag = tag;
    toReturn.childParts = std::move(children);
    return toReturn;
  }

  bool isLoop() const {
    return !!expectTag;
  }

  void generateProgram(std::ostream& lines, int indent = 0) const {
    auto doIndent = [&]() {
      for (int i = 0; i < indent; i++) lines << ' ';
    };

    if (expectTag) {
      // It's some kind of loop, put it together!
      auto it = gLoopBits.find(*expectTag);
      assert(it != gLoopBits.end());

      auto beforeVar = std::get<0>(it->second);
      auto afterVar = std::get<1>(it->second);
      auto afterEndBody = std::get<2>(it->second);

      doIndent();
      if (!afterVar.empty()) {
        auto newVar = "i" + std::to_string(gFreshVarCounter++);
        lines << beforeVar << " " << newVar << " " << afterVar << " {" << std::endl;
      } else {
        lines << beforeVar << " {" << std::endl;
      }

      for (auto& child : childParts) child.generateProgram(lines, indent+2);

      doIndent();
      lines << "} " << afterEndBody << std::endl;
    } else {
      // It's an attribute
      doIndent();
      lines << "@" << theString << std::endl;
    }
  }

  void validateParsed(int attrChildCounter, const uast::Loop* node) const {
    if (expectTag) {
      assert(node->tag() == *expectTag);
      validateParts(node->body(), childParts);
    } else {
      assert(childParts.empty());
      assert(node->attributeGroup());
      // asserts internally
      auto nthChild = node->attributeGroup()->child(attrChildCounter);
      auto nthAttribute = nthChild->toAttribute();
      assert(nthAttribute);
      assert(nthAttribute->name() == theString);
    }
  }
};

static void validateParts(const AstNode* container,
                          const std::vector<Part>& parts) {
  if (parts.size() == 0) return;

  // If there's any part, there must be at least one loop, otherwise it's
  // floating attributes.

  auto nthLoop = [&](int i) {
    auto nodeChild = container->child(i);
    assert(nodeChild && nodeChild->isLoop());
    return nodeChild->toLoop();
  };
  int childCounter = 0;
  int attrChildCounter = 0;
  auto childLoop = nthLoop(0);

  for (auto& part : parts) {
    if (childLoop == nullptr) childLoop = nthLoop(childCounter);
    part.validateParsed(attrChildCounter, childLoop);

    if (part.isLoop()) {
      if (attrChildCounter > 0) {
        assert(childLoop->attributeGroup() &&
               childLoop->attributeGroup()->numAttributes() == attrChildCounter);
      } else {
        assert(!childLoop->attributeGroup());
      }
      attrChildCounter = 0;
      childCounter++;
      childLoop = nullptr;
      // Don't immediately access loop here because we might be the last part.
    } else {
      attrChildCounter++;
    }
  }
}

static void runTest(const char* testName, Parser* parser, const std::vector<Part>& parts) {
  parser->context()->advanceToNextRevision(false);
  std::ostringstream programStream;
  for (auto& part : parts) {
    part.generateProgram(programStream);
  }
  std::string program = programStream.str();

  std::cout << "---- Running test for the following program ----" << std::endl;
  std::cout << program << std::endl << std::endl;

  ErrorGuard guard(parser->context());
  auto parseResult = parseStringAndReportErrors(parser, testName, program.c_str());
  assert(!guard.realizeErrors());
  assert(parseResult.numTopLevelExpressions() == 1);

  validateParts(parseResult.topLevelExpression(0)->toModule(), parts);
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
          Part::loop(pair1.first, {
            Part::loop(pair2.first),
          }),
      });
    }
  }
}

static void test4(Parser* parser) {
  for (auto pair1 : gLoopBits) {
    for (auto pair2 : gLoopBits) {
      runTest("innerLoopAttr.chpl", parser, {
          Part::loop(pair1.first, {
            Part::attr("llvm.assertVectorized"),
            Part::loop(pair2.first),
          }),
      });
    }
  }
}

static void test5(Parser* parser) {
  for (auto pair1 : gLoopBits) {
    for (auto pair2 : gLoopBits) {
      runTest("bothLoopAttr.chpl", parser, {
          Part::attr("llvm.attribute"),
          Part::loop(pair1.first, {
            Part::attr("llvm.assertVectorized"),
            Part::loop(pair2.first),
          }),
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
          Part::loop(pair1.first, {
            Part::attr("llvm.assertVectorized"),
            Part::attr("llvm.alsoNotARealAttributeButAllowed"),
            Part::loop(pair2.first),
          }),
      });
    }
  }
}

static void test7(Parser* parser) {
  for (auto pair1 : gLoopBits) {
    for (auto pair2 : gLoopBits) {
      runTest("siblingLoopFirst.chpl", parser, {
          Part::attr("llvm.attribute"),
          Part::loop(pair1.first),
          Part::loop(pair2.first),
      });
    }
  }
}

static void test8(Parser* parser) {
  for (auto pair1 : gLoopBits) {
    for (auto pair2 : gLoopBits) {
      runTest("siblingLoopSecond.chpl", parser, {
          Part::loop(pair1.first),
          Part::attr("llvm.attribute"),
          Part::loop(pair2.first),
      });
    }
  }
}

static void test9(Parser* parser) {
  for (auto pair1 : gLoopBits) {
    for (auto pair2 : gLoopBits) {
      runTest("siblingLoopBoth.chpl", parser, {
          Part::attr("llvm.attribute"),
          Part::loop(pair1.first),
          Part::attr("llvm.attribute"),
          Part::loop(pair2.first),
      });
    }
  }
}

static void test10(Parser* parser) {
  for (auto pair1 : gLoopBits) {
    for (auto pair2 : gLoopBits) {
      for (auto pair3 : gLoopBits) {
        runTest("nestedAndSiblings.chpl", parser, {
            Part::attr("llvm.attribute1"),
            Part::attr("llvm.attribute2"),
            Part::loop(pair1.first, {
              Part::attr("llvm.attribute3"),
              Part::attr("llvm.attribute4"),
              Part::loop(pair2.first),
            }),
            Part::attr("llvm.attribute5"),
            Part::attr("llvm.attribute6"),
            Part::loop(pair3.first),
        });
      }
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
  test7(p);
  test8(p);
  test9(p);
  test10(p);

  return 0;
}
