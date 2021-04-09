// g++ -fPIC -g -I compiler/libchplcomp/include/ compiler/libchplcomp/lib/AST/*.cpp compiler/libchplcomp/lib/Frontend/*.cpp compiler/libchplcomp/lib/Frontend/Parser/{bison-chapel,flex-chapel}.cpp compiler/libchplcomp/test/Frontend/testParse.cpp

#include "chpl/Frontend/Parser.h"
#include "chpl/AST/Context.h"
#include "chpl/AST/Expr.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace ast;

int main() {
  Context astContext;
  auto parser = Parser(&astContext);

  {
    auto parseResult = parser.parseString("<test>", "");
    assert(parseResult.topLevelExprs.size() == 0);
    assert(parseResult.parseErrors.size() == 0);
  }

  {
    auto parseResult = parser.parseString("<test>", "x;");
    assert(parseResult.topLevelExprs.size() == 1);
    assert(parseResult.topLevelExprs[0]->isIdentifier());
    assert(parseResult.parseErrors.size() == 0);
  }

  {
    auto parseResult = parser.parseString("<test>", "x; y;");
    assert(parseResult.topLevelExprs.size() == 2);
    assert(parseResult.topLevelExprs[0]->isIdentifier());
    assert(parseResult.topLevelExprs[1]->isIdentifier());
    assert(parseResult.parseErrors.size() == 0);
  }

  {
    auto parseResult = parser.parseString("<test>", "/* hi */ y;");
    assert(parseResult.topLevelExprs.size() == 2);
    assert(parseResult.topLevelExprs[0]->isComment());
    assert(parseResult.topLevelExprs[1]->isIdentifier());
    assert(parseResult.parseErrors.size() == 0);
  }


  return 0;
}
