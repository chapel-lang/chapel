#include "common.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"

using namespace parsing;

namespace impl {

const Module* parseModule(Context* context, const char* src) {
  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = src;
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);

  return vec[0];
}

const Module* parseModule(Context* context, std::string& str) {
  return parseModule(context, str.c_str());
}

QualifiedType
parseTypeOfXInit(Context* context, const char* program, bool requireTypeKnown) {
  auto m = parseModule(context, program);
  assert(m->numStmts() > 0);
  const Variable* x = m->stmt(m->numStmts()-1)->toVariable();
  assert(x);
  assert(x->name() == "x");
  auto initExpr = x->initExpression();
  assert(initExpr);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  auto qt = rr.byAst(initExpr).type();
  if (requireTypeKnown) assert(qt.type());

  return qt;
}

}
