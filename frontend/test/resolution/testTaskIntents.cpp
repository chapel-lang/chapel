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

#include "test-resolution.h"

#include "chpl/framework/ErrorWriter.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/ResolvedVisitor.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/IntentList.h"
#include "chpl/uast/For.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"
#include "chpl/uast/While.h"
#include "./ErrorGuard.h"

#include <algorithm>
#include <sstream>
#include <map>

static bool debug = false;
static bool verbose = false;

static std::vector<Context*> globalContexts;
static Context* getNewContext() {
  Context* ret = new Context();
  globalContexts.push_back(ret);
  return ret;
}

std::vector<const ErrorBase*> errors;

static const Module* parseModule(Context* context, const char* src) {
  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = src;
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);

  return vec[0];
}

//
// Collects the types of Decls and Identifiers
//
struct Collector {
  using RV = ResolvedVisitor<Collector>;

  std::multimap<std::string, QualifiedType> declTypes;
  std::multimap<std::string, QualifiedType> identTypes;

  std::multimap<std::string, ID> declIds;
  std::multimap<std::string, ResolvedExpression> shadowVars;

  Collector() { }

  Collector(const Collector& other) : declTypes(other.declTypes), identTypes(other.identTypes) {
  }

  bool enter(const uast::NamedDecl* decl, RV& rv) {
    if (rv.hasAst(decl)) {
      const ResolvedExpression& rr = rv.byAst(decl);
      auto name = decl->name().str();
      declTypes.emplace(name, rr.type());

      if (decl->isTaskVar() || decl->isReduceIntent()) {
        shadowVars.emplace(name, rr);
      } else {
        declIds.emplace(name, decl->id());
      }
    }

    return true;
  }

  bool enter(const uast::Identifier* ident, RV& rv) {
    if (rv.hasAst(ident)) {
      const ResolvedExpression& rr = rv.byAst(ident);
      identTypes.emplace(ident->name().str(), rr.type());
    }

    return true;
  }

  bool enter(const uast::TypeQuery* tq, RV& rv) {
    if (rv.hasAst(tq)) {
      const ResolvedExpression& rr = rv.byAst(tq);
      declTypes.emplace(tq->name().str(), rr.type());
    }

    return true;
  }

  void exit(const uast::Call* call, RV& rv) {
    if (call->isTuple()) return;

    //
    // TODO: Eventually this logic should be moved into ResolvedVisitor as
    // a helper method.
    //
    if (rv.hasAst(call)) {
      const ResolvedExpression& result = rv.byAst(call);
      if (result.mostSpecific().isEmpty() == false) {
        const TypedFnSignature* sig = result.mostSpecific().only();
        auto fn = resolveFunction(rv.context(), sig, result.poiScope());

        ResolvedVisitor<Collector> newRV(rv.context(), nullptr, *this, fn->resolutionById());
        auto untyped = idToAst(rv.context(), sig->id());
        assert(untyped->id() == sig->id());
        untyped->traverse(newRV);
      }
    }
  }

  bool enter(const uast::AstNode* ast, RV& rv) {
    return true;
  }
  void exit(const uast::AstNode* ast, RV& rv) {
  }

  QualifiedType onlyDecl(std::string name) {
    assert(declTypes.count(name) == 1);
    return declTypes.find(name)->second;
  }

  QualifiedType onlyIdent(std::string name) {
    assert(identTypes.count(name) == 1);
    return identTypes.find(name)->second;
  }

  const ResolvedExpression& onlyShadow(std::string name) {
    assert(shadowVars.count(name) == 1);
    return shadowVars.find(name)->second;
  }

  ID onlyDeclId(std::string name) {
    assert(declIds.count(name) == 1);
    return declIds.find(name)->second;
  }

  void dump() {
    std::ostringstream stream;
    auto kind = chpl::StringifyKind::CHPL_SYNTAX;
    stream << "===== Collector =====\n";
    stream << "--- Decls ---\n";
    for (auto p : declTypes) {
      stream << p.first << ": ";
      p.second.stringify(stream, kind);
      stream << "\n";
    }
    stream << "--- Idents ---\n";
    for (auto p : identTypes) {
      stream << p.first << ": ";
      p.second.stringify(stream, kind);
      stream << "\n";
    }
    stream << "=====================\n";

    std::cout << stream.str();
  }
};

static const char* kindToString(IntentList kind) {
  switch (kind) {
    case IntentList::REF: return "ref";
    case IntentList::CONST_INTENT: return "const";
    case IntentList::CONST_REF: return "const ref";
    case IntentList::IN: return "in";
    case IntentList::CONST_IN: return "const in";
    case IntentList::OUT: return "out";
    case IntentList::PARAM: return "param";
    case IntentList::TYPE: return "type";
    case IntentList::VAR: return "var";
    case IntentList::CONST_VAR: return "const";
    case IntentList::DEFAULT_INTENT: return "";
    default: assert(false);
  }
  return "";
}

static void printErrors(const ErrorGuard& guard) {
  if (!verbose) {
    printf("Found %lu errors.\n\n", guard.errors().size());
  } else {
    printf("======== Errors ========\n");
    ErrorWriter ew(guard.context(), std::cout, ErrorWriter::DETAILED, false);
    for (auto& err : guard.errors()) {
      err->write(ew);
    }
    printf("========================\n\n");
  }
}

static Collector customHelper(std::string program, Context* context, bool fail = false) {
  ErrorGuard guard(context);

  const Module* m = parseModule(context, program.c_str());

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  Collector pc;
  ResolvedVisitor<Collector> rv(context, m, pc, rr);
  m->traverse(rv);

  if (debug) {
    printf("\n################################################\n");
    printf("Resolving program:");
    if (fail) printf(" (note: resolution failure expected.)");
    printf("\n");
    printf("========================\n");
    printf("%s\n", program.c_str());
    printf("========================\n");

    if (guard.errors().size()) printErrors(guard);

    if (verbose) pc.dump();
    printf("\n");
  }

  if (!fail) {
    assert(!guard.realizeErrors());
  }

  return pc;
}

static void kindHelper(IntentList kind) {
  Context* context = getNewContext();

  std::string program;
  program += "var x = 0;\n";
  program += "forall i in 1..10 with (";
  program += kindToString(kind);
  program += " x) {\n";
  program += "  var y = x;\n";
  program += "}\n";

  auto col = customHelper(program, context);

  {
    IntentList useKind = kind;
    if (useKind == IntentList::CONST_INTENT) {
      useKind = IntentList::CONST_VAR;
    }
    QualifiedType expected = QualifiedType(useKind, IntType::get(context, 0));
    QualifiedType shadowX = col.onlyIdent("x");
    assert(expected == shadowX);
  }

  {
    QualifiedType yType = col.onlyDecl("y");
    assert(yType.type() == IntType::get(context, 0));
  }

  {
    // Test that the shadow variable points to the original
    auto& rr = col.onlyShadow("x");
    assert(rr.toId() == col.onlyDeclId("x"));
  }
}

static void testKinds() {
  kindHelper(IntentList::REF);
  kindHelper(IntentList::CONST_INTENT);
  kindHelper(IntentList::CONST_REF);
  kindHelper(IntentList::IN);
  kindHelper(IntentList::CONST_IN);
}

static void testReduce() {
  Context* context = getNewContext();
  // Very simple test focusing on scope resolution
  std::string program = R"""(
var x = 0;
forall i in 1..10 with (+ reduce x) {
  x += 1;
}
)""";

  ErrorGuard guard(context);

  const Module* m = parseModule(context, program.c_str());

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  auto decl = m->stmt(0);
  auto forall = m->stmt(1)->toForall();
  auto reduce = forall->withClause()->expr(0);
  auto plusEq = forall->body()->stmt(0)->toCall();
  auto innerX = plusEq->actual(0);

  // Assert reduce points to outer decl
  assert(rr.byAst(reduce).toId() == decl->id());

  // Assert inner ident points to reduce
  assert(rr.byAst(innerX).toId() == reduce->id());
}


//
// TODO:
// - type resolution for reduce-intents
// - type resolution for `in` task-intents
// - test begins, cobegins, foralls
// - const-checking
// - implicit shadow variables (flat, nested)
//
int main(int argc, char** argv) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--debug") == 0) {
      debug = true;
    } else if (strcmp(argv[i], "--verbose") == 0) {
      verbose = true;
    }
  }

  testKinds();
  testReduce();

  printf("\nAll tests passed successfully.\n");

  // Cleanup
  for (auto* con : globalContexts) {
    delete con;
  }

  return 0;
}
