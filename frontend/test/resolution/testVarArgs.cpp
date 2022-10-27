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
#include "chpl/uast/For.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/IntentList.h"
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

static void printHeader(std::string text) {
  std::string edge = "###";
  std::string topBot(text.size() + 2, '#');
  std::cout << "\n";
  std::cout << edge << topBot << edge << "\n";
  std::cout << edge << " " << text << " " << edge << "\n";
  std::cout << edge << topBot << edge << "\n";
}

//
// Collects the types of Decls and Identifiers
//
struct Collector {
  using RV = ResolvedVisitor<Collector>;

  std::multimap<std::string, QualifiedType> declTypes;
  std::multimap<std::string, QualifiedType> identTypes;

  Collector() { }

  Collector(const Collector& other) : declTypes(other.declTypes), identTypes(other.identTypes) {
  }

  bool enter(const uast::VarLikeDecl* decl, RV& rv) {
    if (rv.hasAst(decl)) {
      const ResolvedExpression& rr = rv.byAst(decl);
      declTypes.emplace(decl->name().str(), rr.type());
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

static const char* typeToVal(std::string type) {
  // supports 'int' and other types like 'int(8)'
  if (type.find("int") == 0) return "42";
  else if (type == "real") return "2.0";
  else if (type == "string") return "\"hello\"";

  return "";
}

static bool isParamEq(QualifiedType qt, int n) {
  if (qt.isParam() && qt.param() != nullptr) {
    if (auto ip = qt.param()->toIntParam()) {
      return ip->value() == n;
    }
  }

  return false;
}


struct ArgInfo {
  using Pair = std::pair<IntentList,std::string>;
  std::vector<Pair> args;
  bool isValuesOnly = false;

  static ArgInfo IRS(IntentList argKind = IntentList::VAR) {
    std::vector<std::string> types = {"int", "real", "string"};
    return ArgInfo(argKind, types);
  }

  static ArgInfo Values(std::vector<std::string> vals) {
    ArgInfo ret(vals);
    ret.isValuesOnly = true;
    return ret;
  }

  ArgInfo() {}

  ArgInfo(std::vector<Pair> args) : args(args) {}

  ArgInfo(IntentList argKind, int count, std::string type) {
    for (int i = 0; i < count; i++) {
      args.push_back(Pair(argKind, type));
    }
  }
  ArgInfo(int count,
          std::string type) : ArgInfo(IntentList::VAR, count, type) {}

  ArgInfo(IntentList argKind, std::vector<std::string> types) {
    for (auto s : types) {
      args.push_back(Pair(argKind, s));
    }
  }
  ArgInfo(std::vector<std::string> types) : ArgInfo(IntentList::VAR, types) {}

  int minFormals() {
    if (args.size() == 0) {
      return 1;
    } else {
      return args.size();
    }
  }

  std::pair<std::string,std::string> buildArgStrings() {
    std::string decls;
    int i = 0;
    std::string argList;
    for (auto p : args) {
      std::string argName = "arg" + std::to_string(i);
      if (i > 0) argList += ",";
      argList += isValuesOnly ? p.second : argName;
      i += 1;

      if (!isValuesOnly) {
        bool isTypeActual = p.first == IntentList::TYPE;
        std::string initExpr = isTypeActual ? p.second : typeToVal(p.second);
        //stream << kindToString(p.first) << " " << argName;
        std::string kind(kindToString(p.first));
        decls += kind + " " + argName;
        if (!isTypeActual) {
          decls += " : " + p.second;
        }
        if (initExpr == "") {
          decls += ";\n";
        } else {
          decls += " = " + initExpr + ";\n";
        }
      }
    }

    return std::pair<std::string,std::string>(decls,argList);
  }
};

static void printBuiltProgram(IntentList formalIntent,
                              std::string formalType,
                              std::string count,
                              ArgInfo info,
                              bool fail,
                              std::string program) {
  printf("\n");
  printf("################################################\n");
  printf("Resolving program: ");
  if (fail) {
    printf("(note: resolution failure expected.)");
  }
  printf("\n");

  if (verbose) {
    printf("========================\n");
    printf("%s\n", program.c_str());
    printf("========================\n");
  } else {
    printf("\nPassing: ");
    int i = 0;
    std::string actuals;
    for (auto p : info.args) {
      if (i > 0) actuals += ", ";
      i += 1;
      actuals += kindToString(p.first);
      actuals += ":";
      actuals += p.second;
    }
    printf("``%s``\n", actuals.c_str());


    printf("To VarArgs: ");
    printf("``%s args", kindToString(formalIntent));
    if (formalType != "") printf(": %s", formalType.c_str());
    printf("...%s``\n", count.c_str());
    printf("\n");
  }
}

static std::string buildProgram(IntentList formalIntent,
                                std::string formalType,
                                std::string count,
                                ArgInfo info,
                                bool fail) {
  std::ostringstream stream;

  // Build the VarArg function
  stream << "proc varArgFn(";
  stream << kindToString(formalIntent);
  stream << " args";
  if (formalType != "") {
    stream << ": " << formalType;
  }
  stream << "..." << count;
  stream << ") {\n";
  stream << "  var varArgRet : args.type;\n";
  stream << "  return varArgRet;\n";
  stream << "}\n\n";

  // Build the matching function with normal formals
  stream << "proc normalFn(";
  for (int i = 0; i < info.minFormals(); i++) {
    if (i > 0) stream << ", ";
    stream << kindToString(formalIntent) << " formal_" << std::to_string(i);
    if (formalType != "") {
      stream << ": " << formalType;
      // TODO: should replace type-queries with an identifier for subsequent
      // formals (i.e. "?T" --> "T")
      //
      // TODO: This doesn't account for nested type queries.
      if (formalType[0] == '?') {
        stream << "_" << std::to_string(i);
      }
    }
  }
  stream << ") {\n";
  stream << "  var ret : (";
  for (int i = 0; i < info.minFormals(); i++) {
    if (i > 0) stream << ", ";
    stream << "formal_" << std::to_string(i);
  }
  stream << ").type;\n";
  stream << "  return ret;\n";
  stream << "}\n\n";

  // Build the declarations for the actuals and the list of actuals
  auto declArgList = info.buildArgStrings();
  stream << declArgList.first;
  std::string argList = declArgList.second;

  stream << "var x = varArgFn(" << argList << ");\n";
  stream << "var y = normalFn(" << argList << ");\n";

  std::string program = stream.str();

  if (debug) {
    printBuiltProgram(formalIntent, formalType, count, info, fail, program);
  }

  return program;
}

static void printErrors(const ErrorGuard& guard) {
  if (!verbose) {
    printf("Found %lu errors.\n\n", guard.errors().size());
  } else {
    printf("======== Errors ========\n");
    ErrorWriter ew(guard.context(), std::cout, ErrorWriter::DETAILED, false);
    for (auto err : guard.errors()) {
      err->write(ew);
    }
    printf("========================\n\n");
  }
}

static void validate(std::string formalType,
                     std::string count,
                     ArgInfo info,
                     Collector& col) {
  std::ostringstream oss;
  auto kind = chpl::StringifyKind::CHPL_SYNTAX;
  oss << "--------------------\n";

  auto varArgType = col.onlyDecl("args");
  oss << "varargs : ";
  varArgType.stringify(oss, kind);
  oss << "\n";

  oss << "=== comparing formals ===\n";
  for (size_t i = 0; i < info.args.size(); i++) {
    QualifiedType elt = varArgType.type()->toTupleType()->elementType(i);
    std::string name = "formal_" + std::to_string(i);
    auto formal = col.onlyDecl(name);
    oss << "  expecting: ";
    formal.stringify(oss, kind);
    oss << "\n";
    oss << "    computed: ";
    elt.stringify(oss, kind);
    oss << "\n";
    assert(elt == formal);
  }

  if (formalType != "" && formalType[0] == '?') {
    oss << "\n=== comparing type queries ===\n";
    auto name = std::string(1, formalType[1]);
    auto varArgQuery = col.onlyDecl(name);
    for (size_t i = 0; i < info.args.size(); i++) {
      std::string q = name + "_" + std::to_string(i);
      auto normalQuery = col.onlyDecl(q);
      oss << "  expecting: ";
      normalQuery.stringify(oss, kind);
      oss << "\n";
      oss << "    computed: ";
      varArgQuery.stringify(oss, kind);
      oss << "\n";
      assert(varArgQuery == normalQuery);
    }
    oss << "\n";
  }

  if (count != "") {
    oss << "\n=== comparing count query ===\n";
    const int varArgSize = varArgType.type()->toTupleType()->numElements();
    int64_t computed = -1;

    if (count[0] == '?') {
      // 'args...?' is valid, but not something we need to validate
      if (count.size() > 1) {
        // chop off leading '?'
        std::string name = count.substr(1);
        auto countQuery = col.onlyDecl(name);
        computed = countQuery.param()->toIntParam()->value();
      }
    } else {
      bool isNum = std::all_of(count.begin(), count.end(), ::isdigit);
      if (isNum) {
        computed = std::stoi(count);
      } else {
        auto countQuery = col.onlyDecl(count);
        computed = countQuery.param()->toIntParam()->value();
      }
    }

    if (computed != -1) {
      oss << "  expecting: ";
      oss << varArgSize << "\n";
      oss << "    computed: ";
      oss << computed << "\n";
      assert(varArgSize == computed);
    } else {
      oss << "  unnamed query '?'\n";
    }
  }

  auto varArgRet = col.onlyDecl("varArgRet");
  auto normalRet = col.onlyDecl("ret");
  assert(normalRet == varArgRet);
  oss << "\n=== comparing return type ===\n";
  oss << "  expecting: ";
  normalRet.stringify(oss, kind);
  oss << "\n";
  oss << "    computed: ";
  varArgRet.stringify(oss, kind);
  oss << "\n\n";
  oss << "Success.\n";

  if (debug && verbose) {
    std::cout << oss.str();
  }
}

static void testMatcher(IntentList formalIntent,
                       std::string formalType,
                       std::string count,
                       ArgInfo info,
                       bool fail = false) {
  if (formalType != "") {
    if (formalType[0] == '?') {
      assert(formalType.size() == 2);
    }
  }

  std::string program = buildProgram(formalIntent, formalType, count,
                                     info, fail);

  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  const Module* m = parseModule(context, program.c_str());

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  Collector col;
  ResolvedVisitor<Collector> rv(context, m, col, rr);
  m->traverse(rv);

  if (guard.errors().size()) {
    if (debug) {
      printErrors(guard);
    }
    assert(fail);
    assert(guard.realizeErrors());
    return;
  } else {
    assert(!fail);
    assert(!guard.realizeErrors());
  }

  validate(formalType, count, info, col);
}

static Collector
customHelper(std::string program, bool fail = false,
             std::vector<const ErrorBase*>* errorsOut=nullptr) {
  Context* context = getNewContext();
  ErrorGuard guard(context);

  const Module* m = parseModule(context, program.c_str());

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
  Collector pc;
  ResolvedVisitor<Collector> rv(context, m, pc, rr);
  m->traverse(rv);

  if (guard.errors().size() && errorsOut) {
    for (auto err : guard.errors()) errorsOut->push_back(err);
  }

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

  // Consume the errors so the destructor does not abort.
  assert(fail == guard.realizeErrors());

  return pc;
}

static void testGenericType() {
  printHeader("fully-generic varargs");

  auto irsVar = ArgInfo::IRS(IntentList::VAR);
  testMatcher(IntentList::DEFAULT_INTENT , "" , "" , irsVar);
  testMatcher(IntentList::IN             , "" , "" , irsVar);
  testMatcher(IntentList::REF            , "" , "" , irsVar);
  testMatcher(IntentList::CONST_REF      , "" , "" , irsVar);
  testMatcher(IntentList::CONST_INTENT   , "" , "" , irsVar);
  testMatcher(IntentList::CONST_IN       , "" , "" , irsVar);

  testMatcher(IntentList::PARAM, "", "", ArgInfo::IRS(IntentList::PARAM));
  testMatcher(IntentList::TYPE, "", "", ArgInfo::IRS(IntentList::TYPE));

  testMatcher(IntentList::DEFAULT_INTENT, "int(?)", "",
              ArgInfo({"int(8)", "int", "int(32)"}));

  // TODO: Expressions like 'param x : int(8) = 42' are not currently working.
  //testMatcher(IntentList::PARAM, "int(?)", "",
  //           ArgInfo(IntentList::PARAM, {"int(8)", "int", "int(32)"}));

  // Errors

  testMatcher(IntentList::DEFAULT_INTENT, "int(?)", "",
              ArgInfo(3, "string"), true);
}

static void intentActualMismatch() {
  printHeader("passing args to wrong intent");

  testMatcher(IntentList::PARAM, "", "",
              ArgInfo::IRS(), true);
  testMatcher(IntentList::TYPE, "", "",
              ArgInfo::IRS(), true);
}

static void typeQuery() {
  printHeader("varargs with type query");

  testMatcher(IntentList::DEFAULT_INTENT, "?i", "",
              ArgInfo(3, "int"));
  testMatcher(IntentList::DEFAULT_INTENT, "?s", "",
              ArgInfo(3, "string"));
  testMatcher(IntentList::PARAM, "?i", "",
              ArgInfo(IntentList::PARAM, 3, "int"));

  // Passing in params to default-intent should not result in the formals
  // being treated as params
  testMatcher(IntentList::DEFAULT_INTENT, "?t", "",
              ArgInfo::Values({"true","false","true"}));

  // Errors

  // pass different types to type query
  testMatcher(IntentList::DEFAULT_INTENT, "?t", "",
              ArgInfo::IRS(), true);

  // TODO: Consider the following:
  //   proc fn(args:int(?w)...)
  //   fn(1, 2:int(8), 3:int(32));
  //
  // This *should* fail to resolve, but at the time of this comment does not.
  // Also at this time, code like this resolves successfully:
  //   proc fn(A : ?T, B : T)
  //   fn(5, "hi")
  //
  // We currently issue a somewhat helpful error recognizing that we can't make
  // a type query from this argument, but this doesn't prevent resolving the
  // call.
  testMatcher(IntentList::DEFAULT_INTENT, "int(?t)", "",
              ArgInfo({"int(8)", "int", "int(32)"}), true);
}

static void testConcrete() {
  printHeader("varargs with type exprs");

  // simple 1:1 cases
  testMatcher(IntentList::DEFAULT_INTENT, "int", "",
              ArgInfo(3, "int"));

  // int coercion
  testMatcher(IntentList::DEFAULT_INTENT, "int", "",
              ArgInfo({"int", "int(8)", "int(32)"}));

  // varargs specifiying a tuple as the formal type
  testMatcher(IntentList::DEFAULT_INTENT, "(int,int,int)", "",
              ArgInfo(3, "(int,int,int)"));

  // Errors

  // pass int(64) to int(8)
  testMatcher(IntentList::DEFAULT_INTENT, "int(8)", "",
              ArgInfo(3, "int"), true);


  testMatcher(IntentList::DEFAULT_INTENT, "(int(8),int,int)", "",
              ArgInfo(3, "(int,int,int)"), true);
}

static void testParamCount() {
  auto paramFn = std::string(
R"""(
proc fn(param n : int, args...n) {
  param ret = n;
  return n;
}
)""");

  std::vector<const ErrorBase*> errors;
  auto errMsg = "Cannot resolve call to fn: no matching candidates";

  auto good = std::string(R"""(var x = fn(3, 1, 2.0, "hello");)""");
  auto gc = customHelper(paramFn + good);
  assert(isParamEq(gc.onlyDecl("n"), 3));

  auto less = std::string(R"""(var x = fn(3, 1, 2.0);)""");
  customHelper(paramFn + less, true, &errors);
  assert(errors.size() == 1 && errors[0]->message() == errMsg);
  errors.clear();

  auto more = std::string(R"""(var x = fn(3, 1, 2.0, "hello", "oops");)""");
  customHelper(paramFn + more, true, &errors);
  assert(errors.size() == 1 && errors[0]->message() == errMsg);
  errors.clear();

  // non-integrals should not be valid in count-expressions
  auto paramBoolFn = std::string(
R"""(
proc fn(param n : bool, args...n) {
  var ret : args.type;
  return ret;
}
fn(true, true);
)""");
  customHelper(paramBoolFn, true, &errors);
  assert(errors.size() == 1 && errors[0]->message() == errMsg);
  errors.clear();

  // TODO: Make sure uints resolve (cannot cast to param uints yet)
}

static void testCount() {
  printHeader("testing count expressions");
  testMatcher(IntentList::DEFAULT_INTENT, "int", "3",
              ArgInfo(3, "int"));

  testMatcher(IntentList::DEFAULT_INTENT, "int", "?",
              ArgInfo(5, "int"));
  testMatcher(IntentList::DEFAULT_INTENT, "int", "?n",
              ArgInfo(5, "int"));

  // With type queries
  testMatcher(IntentList::DEFAULT_INTENT, "?t", "?n",
              ArgInfo(4, "int"));
  testMatcher(IntentList::DEFAULT_INTENT, "?t", "3",
              ArgInfo(3, "int"));

  testParamCount();

  // Errors

  testMatcher(IntentList::DEFAULT_INTENT, "int", "3",
              ArgInfo(5, "int"), true);
  testMatcher(IntentList::DEFAULT_INTENT, "int", "3",
              ArgInfo(2, "int"), true);
  testMatcher(IntentList::DEFAULT_INTENT, "int", "1",
              ArgInfo(2, "int"), true);

  // TODO: Better error message
  testMatcher(IntentList::DEFAULT_INTENT, "int", "0",
              ArgInfo(2, "int"), true);

  // zero actuals, varargs require at least one
  testMatcher(IntentList::DEFAULT_INTENT, "int", "",
              ArgInfo(), true);

  auto countTypeFn = std::string(
R"""(
proc fn(args:?t...?n) {
  type tq = t;
  param cq = n;
}
fn(1, 2, 3);
)""");
  auto results = customHelper(countTypeFn);
  assert(isParamEq(results.onlyDecl("cq"), 3));
  assert(results.onlyDecl("tq").type()->isIntType());
}

static void testAlignment() {
  printHeader("testing alignment");

{
  // Alignment tests where args is of fixed-size depending on a param int

  auto paramFn = std::string(
R"""(
proc fn(param n : int, args...n, y : int, z : real) {
  param ret = n;
  return n;
}
)""");

  auto errMsg = "Cannot resolve call to fn: no matching candidates";
  std::vector<const ErrorBase*> errors;

  auto good = std::string(R"""(var x = fn(3, 1, 2, 3, 4, 5.0);)""");
  customHelper(paramFn + good);

  auto less = std::string(R"""(var x = fn(3, 1, 2, 2.0);)""");
  customHelper(paramFn + less, true, &errors);
  assert(errors.size() == 1 && errors[0]->message() == errMsg);
  errors.clear();

  auto more = std::string(R"""(var x = fn(3, 1, 2, 3, 4, 5, 6, 7.0);)""");
  customHelper(paramFn + more, true, &errors);
  assert(errors.size() == 1 && errors[0]->message() == errMsg);
  errors.clear();

  auto named = std::string(R"""(var x = fn(z=5.0, 3, 1, 2, 3, 4);)""");
  customHelper(paramFn + named);
}

{
  // Issue an error for functions with multiple VarArg formals

  auto multiVarArg = std::string(
R"""(
proc fn(left..., right...) {
  var ret = ((...left), (...right));
  return ret;
}
)""");

  auto multiCall = std::string(R"""(var x = fn(1,2,3,4,5);)""");
  customHelper(multiVarArg + multiCall, true);
}

{
  // Testing named arguments and alignment

  auto program = std::string(
R"""(
proc reference(args...) {
  var ret : args.type;
  return ret;
}

proc fn(x:string, args..., y:real) {
  var ret : args.type;
  return ret;
}

var R = reference(1, "test", 3.0);
var a = fn(x="hello", y=5.0, 1, "test", 3.0);
var b = fn(x="hello", 1, "test", 3.0, 5.0);
var c = fn(y=5.0, "hello", 1, "test", 3.0);
)""");

  auto pc = customHelper(program);
  auto ref = pc.onlyDecl("R");
  assert(ref == pc.onlyDecl("a"));
  assert(ref == pc.onlyDecl("b"));
  assert(ref == pc.onlyDecl("c"));

}

}

//
// TODO: test where-clauses:
//
//   count-query:
//     proc fn(args:int...?n) where n > 1 { }
//     proc fn(arg:int) { }
//
//   type-query:
//     proc fn(args:?T...) where isRange(T) {}
//
// TODO: verbose output (current mode), and 'simple' output
// showing arguments and formals, and success/failure
//


int main(int argc, char** argv) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--debug") == 0) {
      debug = true;
    } else if (strcmp(argv[i], "--verbose") == 0) {
      verbose = true;
    }
  }

  testGenericType();
  intentActualMismatch();
  typeQuery();
  testConcrete();
  testCount();
  testAlignment();

  printf("\nAll tests passed successfully.\n");

  // Cleanup
  for (auto* con : globalContexts) {
    delete con;
  }

  return 0;
}
