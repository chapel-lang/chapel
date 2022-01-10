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

/*
  To test this today, you can force sub_test to use this version by doing:
  > CHPL_CHPLDOC_NEXT=$(find $(pwd) -name chpldoc -type f) ./util/test/start_test.py test/chpldoc/enum.doc.chpl
  You'll want to make sure that the find command finds a single chpldoc executable
 */

#include <algorithm>
#include <cctype>
#include <cstring>
#include <ios>
#include <iostream>
#include <fstream>
#include <limits>
#include <regex>
#include <sstream>
#include <unordered_set>
#include <filesystem>

#include "chpl/parsing/Parser.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/queries/Context.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/queries/stringify-functions.h"
#include "chpl/uast/all-uast.h"
#include "chpl/util/string-escapes.h"

using namespace chpl;
using namespace uast;
using namespace parsing;

std::unordered_set<asttags::ASTTag> gUnhandled;

const std::string templateUsage = R"RAW(**Usage**

.. code-block:: chapel

   use $MODULE;


or

.. code-block:: chapel

   import $MODULE;)RAW";

static std::vector<std::string> splitLines(const std::string& s) {
  std::stringstream ss(s);
  std::string line;
  std::vector<std::string> ret;
  while (std::getline(ss, line)) {
    ret.push_back(line);
  }
  return ret;
}

static std::string strip(const std::string& s, std::string pattern = "^\\s+|\\s+$") {
  auto re = std::regex(pattern);
  return std::regex_replace(s, re, "");
}

static std::string templateReplace(const std::string& templ,
                                   const std::string& key,
                                   const std::string& value) {
  return std::regex_replace(templ, std::regex(std::string("\\$") + key), value);
}

static size_t countLeadingSpaces(const std::string& s) {
  size_t i = 0;
  while (std::isspace(s[i])) {i += 1;}
  return i;
}

static std::ostream& indentStream(std::ostream& os, size_t num) {
  const char* spaces = "        ";
  constexpr size_t N = sizeof(spaces);
  while (num > N) {
    os << spaces;
    num -= N;
  }
  auto leftover = N - num;
  os << (spaces + leftover);

  return os;
}


// Remove the leading+trailing // or /* (*/)
// Dedent by the least amount of leading whitespace
// Replace leading whitespace with `indent` spaces
static std::vector<std::string> prettifyComment(const std::string& s) {
  std::string ret = s;

  if (ret.substr(0, 2) == "//") {
    ret.erase(0, 2);
  } else if (ret.substr(0, 2) == "/*") {
    size_t l = ret.length();
    assert(ret.substr(l - 2, 2) == "*/" && "comment didn't end with */");
    ret.erase(l - 2, 2);
    ret.erase(0, 2);
  } else {
    assert(false && "comment didn't start with // or /*");
  }

  ret = strip(ret, "^\n+|\n+$");

  auto lines = splitLines(ret);
  if (lines.empty()) return lines;

  size_t toTrim = std::numeric_limits<size_t>::max();
  for (const std::string& line : lines) {
    if (line.empty()) continue;
    toTrim = std::min(toTrim, countLeadingSpaces(line));
  }

  assert(toTrim < std::numeric_limits<size_t>::max() && "probably an empty comment");

  for (std::string& line : lines) {
    line.erase(0, toTrim);
  }

  return lines;
}

static std::string commentSynopsis(const Comment* c) {
  if (!c) return "";
  auto lines = prettifyComment(c->str());
  if (lines.empty()) return "";
  return lines[0];
}

static const char* kindToRstString(bool isMethod, Function::Kind kind) {
  switch (kind) {
  case Function::Kind::PROC: return isMethod ? "method" : "function";
  case Function::Kind::ITER: return isMethod ? "itermethod" : "iter";
  case Function::Kind::OPERATOR: return "operator";
  }
  assert(false);
  return "";
}

static const char* kindToString(Function::Visibility kind) {
  switch (kind) {
  case Function::Visibility::PRIVATE: return "private";
  case Function::Visibility::PUBLIC: return "public";
  case Function::Visibility::DEFAULT_VISIBILITY: assert(false);
  }
  assert(false);
  return "";
}

static const char* kindToString(Function::Kind kind) {
  switch (kind) {
  case Function::Kind::PROC: return "proc";
  case Function::Kind::ITER: return "iter";
  case Function::Kind::OPERATOR: return "operator";
  }
  assert(false);
  return "";
}

static const char* kindToString(Function::ReturnIntent kind) {
  switch (kind) {
  case Function::ReturnIntent::CONST: return "const";
  case Function::ReturnIntent::REF: return "ref";
  case Function::ReturnIntent::CONST_REF: return "const ref";
  case Function::ReturnIntent::PARAM: return "param";
  case Function::ReturnIntent::TYPE: return "type";
  case Function::ReturnIntent::DEFAULT_RETURN_INTENT: assert(false);
  }
  assert(false);
  return "";
}

static const char* kindToString(Formal::Intent kind) {
  switch (kind) {
  case Formal::Intent::CONST: return "const";
  case Formal::Intent::CONST_REF: return "const ref";
  case Formal::Intent::REF: return "ref";
  case Formal::Intent::IN: return "in";
  case Formal::Intent::CONST_IN: return "const in";
  case Formal::Intent::OUT: return "out";
  case Formal::Intent::INOUT: return "inout";
  case Formal::Intent::PARAM: return "param";
  case Formal::Intent::TYPE: return "type";
  case Formal::Intent::DEFAULT_INTENT: assert(false);
  }
  assert(false);
  return "";
}

static const char* kindToString(Variable::Kind kind) {
  switch (kind) {
  case Variable::Kind::VAR: return "var";
  case Variable::Kind::CONST: return "const";
  case Variable::Kind::PARAM: return "param";
  case Variable::Kind::TYPE: return "type";
  case Variable::Kind::REF: return "ref";
  case Variable::Kind::CONST_REF: return "const ref";
  case Variable::Kind::INDEX: assert(false);
  }
  assert(false);
  return "";
}

static const char* kindToString(New::Management kind) {
  switch (kind) {
  case New::Management::BORROWED: return "borrowed";
  case New::Management::OWNED: return "owned";
  case New::Management::SHARED: return "shared";
  case New::Management::UNMANAGED: return "unmanaged";
  case New::Management::DEFAULT_MANAGEMENT: assert(false);
  default:
    assert(false);
  }
  assert(false);
  return "";
}

static bool isNoDoc(const Decl* e) {
  if (auto attrs = e->attributes()) {
    return attrs->hasPragma(pragmatags::PRAGMA_NO_DOC);
  }
  return false;
}

// This actually works okay and the formatting we have to do is pretty specific to RST anyways so I'm happy with this solution
struct PrettyPrintVisitor {
  std::ostream& os_;

  template<typename It>
  void interpose(It begin, It end, const char* separator, const char* surroundBegin=nullptr, const char* surroundEnd=nullptr) {
    bool first = true;
    if (surroundBegin) os_ << surroundBegin;
    for (auto it = begin; it != end; it++) {
      if (!first) os_ << separator;
      (*it)->traverse(*this);
      first = false;
    }
    if (surroundEnd) os_ << surroundEnd;
  }

  template<typename T>
  void interpose(T xs, const char* separator, const char* surroundBegin=nullptr, const char* surroundEnd=nullptr) {
    interpose(xs.begin(), xs.end(), separator, surroundBegin, surroundEnd);
  }

  bool enter(const Identifier* r) {
    os_ << r->name().c_str();
    return false;
  }

  bool enter(const Record* r) {
    os_ << r->name().c_str();
    return false;
  }

  bool enter(const BoolLiteral* b) {
    os_ << (b->value() ? "true" : "false");
    return false;
  }

  template<typename T>
  bool enterLiteral(const T* l) {
    os_ << l->text().c_str();
    return false;
  }

  bool enter(const IntLiteral* l)  { return enterLiteral(l); }
  bool enter(const UintLiteral* l) { return enterLiteral(l); }
  bool enter(const RealLiteral* l) { return enterLiteral(l); }
  bool enter(const ImagLiteral* l) { return enterLiteral(l); }

  bool enter(const StringLiteral* l) {
    os_ << '"' << quoteStringForC(std::string(l->str().c_str())) << '"';
    return false;
  }

  bool enter(const CStringLiteral* l) {
    os_ << "c\"" << quoteStringForC(std::string(l->str().c_str())) << '"';
    return false;
  }

  bool enter(const Dot *d) {
    d->receiver()->traverse(*this);
    os_ << "." << d->field().c_str();
    return false;
  }

  bool enter(const New *d) {
    os_ << "new ";
    if (d->management() != New::Management::DEFAULT_MANAGEMENT) {
      os_ << kindToString(d->management()) << " ";
    }
    d->typeExpression()->traverse(*this);
    return false;
  }

  bool enter(const Enum* e) {
    os_ << "enum " << e->name().c_str() << " ";
    interpose(e->enumElements(), ", ", "{ ", " }");
    return false;
  }

  bool enter(const EnumElement* e) {
    os_ << e->name().c_str();
    if (const Expression* ie = e->initExpression()) {
      os_ <<  " = ";
      ie->traverse(*this);
    }
    return false;
  }

  bool enter(const Variable* v) {
    if (v->isConfig()) {
      os_ << "config ";
    }
    if (v->kind() != Variable::Kind::INDEX) {
      os_ << kindToString(v->kind()) << " ";
    }
    os_ << v->name().c_str();
    if (const Expression* te = v->typeExpression()) {
      os_ << ": ";
      te->traverse(*this);
    }
    if (const Expression* ie = v->initExpression()) {
      os_ << " = ";
      ie->traverse(*this);
    }
    return false;
  }

  bool enter(const Formal* f) {
    if (f->intent() != Formal::DEFAULT_INTENT) {
      os_ << kindToString(f->intent()) << " ";
    }
    os_ << f->name().c_str();
    if (const Expression* te = f->typeExpression()) {
      os_ << ":";
      te->traverse(*this);
    }
    if (const Expression* ie = f->initExpression()) {
      os_ << "=";
      ie->traverse(*this);
    }
    return false;
  }

  bool enter(const Function* f) {
    // Visibility
    if (f->visibility() != Function::Visibility::DEFAULT_VISIBILITY) {
      os_ << kindToString(f->visibility()) << " ";
    }

    // Function Name
    os_ << kindToString(f->kind()) << " " << f->name().c_str();

    // Formals
    // TODO this prints all 0-arity as paren-less
    int numThisFormal = f->thisFormal() ? 1 : 0;
    if (f->numFormals() - numThisFormal) {
      auto it = f->formals();
      interpose(it.begin() + numThisFormal, it.end(), ", ", "(", ")");
    }

    // Return Intent
    if (f->returnIntent() != Function::ReturnIntent::DEFAULT_RETURN_INTENT) {
      os_ << " " << kindToString(f->returnIntent());
    }
    return false;
  }

  bool enter(const OpCall* call) {
    if (call->isUnaryOp()) {
      os_ << call->op().c_str();
      assert(call->numActuals() == 1);
      call->actual(0)->traverse(*this);

    } else if (call->isBinaryOp()) {
      assert(call->numActuals() == 2);
      call->actual(0)->traverse(*this);
      os_ << call->op().c_str();
      call->actual(1)->traverse(*this);
    }
    return false;
  }

  bool enter(const Call* call) {
    const Expression* callee = call->calledExpression();
    if (!callee) {
      printf("ERROR %s\n", asttags::tagToString(call->tag()));
      call->stringify(std::cerr, StringifyKind::DEBUG_DETAIL);
    }
    assert(callee);  // This should be true because OpCall is handled
    callee->traverse(*this);
    interpose(call->actuals(), ", ", "(", ")");

    return false;
  }

  bool enter(const Domain* dom) {
    interpose(dom->children(), ", ", "{", "}");
    return false;
  }

  bool enter(const Tuple* tup) {
    interpose(tup->children(), ", ", "(", ")");
    return false;
  }

  bool enter(const Array* arr) {
    interpose(arr->children(), ", ", "[", "]");
    return false;
  }

  bool enter(const Range* range) {
    if (auto lb = range->lowerBound()) {
      lb->traverse(*this);
    }
    os_ << "..";
    if (auto ub = range->upperBound()) {
      ub->traverse(*this);
    }
    return false;
  }

  // TODO this isn't quite right currently, the dump for var x: [{1..3}] int = ...
  // has the extra int. Maybe its not supposed to be a BracketLoop though?
  // that would be more like [i in 0..15] writeln(i);
  // M@26BracketLoop
  // M@23  Domain
  // M@22    Range
  // M@20      IntLiteral
  // M@21      IntLiteral
  // M@25  Block
  // M@24    Identifier int
  bool enter(const BracketLoop* bl) {
    interpose(bl->children(), "", "[", "]");
    return false;
  }

  bool enter(const Block* b) {
    return true;
  }

  bool enter(const ASTNode* a) {
    printf("unhandled enter on PrettyPrintVisitor of %s\n", asttags::tagToString(a->tag()));
    a->stringify(std::cerr, StringifyKind::DEBUG_DETAIL);
    gUnhandled.insert(a->tag());
    return false;
  }
  void exit(const ASTNode* a) {}
};

struct ChpldocVisitor {
  const BuilderResult& br_;
  std::ostream& os_;
  const Comment* lastComment_ = nullptr;
  std::vector<const Module*> moduleStack_;
  int depth = 0;

  static const int indentPerDepth = 3; // 3????

  void showComment(bool indent) {
    if (!lastComment_) return;

    int indentChars = indent ? ((depth + 1) * indentPerDepth) : 0;
    auto lines = prettifyComment(lastComment_->str());
    for (const auto& line : lines) {
      if (line.empty()) {
        os_ << '\n';
      } else {
        indentStream(os_, indentChars) << line << '\n';
      }
    }
  }

  template<typename T>
  void show(const std::string& kind, const T* node) {
    if (isNoDoc(node)) return;

    indentStream(os_, depth * indentPerDepth) << ".. " << kind << ":: ";
    PrettyPrintVisitor ppv{os_};
    node->traverse(ppv);
    os_ << "\n\n";

    showComment(true);
    os_ << "\n";

    if (auto attrs = node->attributes()) {
      if (attrs->isDeprecated()) {
        indentStream(os_, (depth + 1) * indentPerDepth) << ".. warning::\n";
        indentStream(os_, (depth + 2) * indentPerDepth) << attrs->deprecationMessage().c_str();
        os_ << "\n\n";
      }
    }
  }

  bool enter(const Comment* comment) {
    lastComment_ = comment;
    return false;
  }
  void exit(const Comment* ast) {}
  void exit(const ASTNode* ast) { lastComment_ = nullptr; }

  bool enter(const Module* m) {
    if (isNoDoc(m)) return false;

    moduleStack_.push_back(m);
    // header
    os_ << ".. default-domain:: chpl\n\n";
    os_ << ".. module:: " << m->name().c_str() << '\n';
    if (lastComment_) {
      os_ << "    :synopsis: " << commentSynopsis(lastComment_) << '\n';
    }
    os_ << '\n';

    // module title
    os_ << m->name().c_str() << "\n";
    os_ << std::string(m->name().length(), '=') << "\n";

    // usage
    // TODO branch on whether FLAG_MODULE_INCLUDED_BY_DEFAULT or equivalent
    os_ << templateReplace(templateUsage, "MODULE", m->name().c_str()) << '\n';

    showComment(false);
    os_ << '\n';
    return true;
  }
  void exit(const Module* m) { moduleStack_.pop_back();}

  bool enter(const Function* f) {
    // TODO this doesn't fire on privateProc either
    if (f->visibility() == Decl::Visibility::PRIVATE)
      return false;
    show(kindToRstString(f->isMethod(), f->kind()), f);
    return false;
  }
  bool enter(const Variable* v) {
    show("data", v);
    return false;
  }
  bool enter(const Record* r) {
    show("record", r);
    depth += 1;
    return true;
  }
  bool enter(const Enum* e) {
    show("enum", e);
    return false;
  }
  void exit(const Record* r) { depth -= 1; }

  bool enter(const ASTNode* ast) {
    // printf("Skipping %s\n", asttags::tagToString(ast->tag()));
    // std::cerr << "\n===";
    // ast->stringify(std::cerr, StringifyKind::DEBUG_DETAIL);
    // std::cerr << "===\n";
    return false;
  }
};

const std::string testString = R"RAW(
/*
  comment 1
  This thing is super long

*/
module M {
  // comment 2
  var x = 10;

  //
  const y = doSomething(10, x);

  // comment 2.5
  proc foo(x : int) { return 10; }


  // random comment

  // comment for an enum
  enum myEnum {a=1, b, c=32};

  // comment for a param
  param paramX = 10;

  // comment for a param with type
  param paramY:int = 10;

  // split init const
  const splitInit;

  splitInit = 10;

  // comment for a config const
  config const configConstY = 10;

  // comment for a type
  type t = int;

  // comment for record
  record R {
    // comment for a type
    type t;

    // comment for a constant
    const myConst = 100.1;

    // this operator
    operator +(x:R) {return new R();}

    // comment for a method
    proc foo(a:real, b:int=37) { return 10; }

    // paren-less proc
    proc bar { return 10; }
  }

  proc R.secondaryMethod() const { return 42; }

  private proc privateProc { return 37; }

  pragma "no doc"
  proc procNoDoc { return 42; }

  // got a comment for ya
  deprecated "yo this thing is deprecated"
  proc oldNews { return 327; }

  var x : [1..3] int = [1, 2, 3];
}

pragma "no doc"
module N { }
/* comment 4 */;
)RAW";

  // /* nested module comment */
  // module NestedModule {
  //   // nested var comment
  //   var nestedX = 22;
  // }

// temporary CLI to get testing
struct Args {
  std::string saveSphinx = "docs";
  bool selfTest = false;
  bool stdout = false;
  bool dump = false;
  std::vector<std::string> files;
};

static Args parseArgs(int argc, char **argv) {
  Args ret;
  for (int i = 1; i < argc; i++) {
    if  (std::strcmp("--no-html", argv[i]) == 0){
      // pass
    } else if (std::strcmp("--save-sphinx", argv[i]) == 0) {
      assert(i < (argc - 1));
      ret.saveSphinx = argv[i + 1];
      i += 1;
    } else if (std::strcmp("--self-test", argv[i]) == 0) {
      ret.selfTest = true;
    } else if (std::strcmp("--stdout", argv[i]) == 0) {
      ret.stdout = true;
    } else if (std::strcmp("--dump", argv[i]) == 0) {
      ret.dump = true;
    } else {
      ret.files.push_back(argv[i]);
    }
  }
  return ret;
}

// TODO what do we do if multiple modules are in one file?
static std::string moduleName(const BuilderResult& builderResult) {
  for (const auto& ast : builderResult.topLevelExpressions()) {
    if (const Module* m = ast->toModule()) {
      return m->name().str();
    }
  }
  assert(false);
  return "";
}

int main(int argc, char** argv) {
  Context context;
  Context *ctx = &context;

  Args args = parseArgs(argc, argv);

  if (args.selfTest) {
    args.files.push_back("selftest.chpl");
    UniqueString path = UniqueString::build(ctx, "selftest.chpl");
    setFileText(ctx, path, testString);
  }

  if (args.files.size() > 1) {
    std::cerr << "WARNING only handling one file right now\n";
  }

  for (auto cpath : args.files) {
    UniqueString path = UniqueString::build(ctx, cpath);
    const BuilderResult& builderResult = parseFile(ctx, path);

    std::ofstream ofs;
    if (!args.stdout) {
      auto name = moduleName(builderResult);
      auto outdir = args.saveSphinx + "/source/modules";
      auto outpath = outdir + "/" + name + ".rst";

      std::error_code err;
      if (!std::filesystem::create_directories(outdir, err)) {
        // TODO the above seems to return false when already exists,
        // but I don't think that should be the case
        // So check we really got an error
        if (err) {
          std::cerr << "Could not create " << outdir << " because "
                    << err.message() << "\n";
          return 1;
        }
      }
      // TODO is the output name from the module or filename?

      // TODO nested modules need some context to end up in the right place
      // (so this will probably go in the visitor)
      ofs = std::ofstream(outpath, std::ios::out);
    }
    std::ostream& os = args.stdout ? std::cout : ofs;
    ChpldocVisitor v{builderResult, os};
    for (const auto& ast : builderResult.topLevelExpressions()) {
      if (args.dump) {
        ast->stringify(std::cerr, StringifyKind::DEBUG_DETAIL);
      }
      ast->traverse(v);
    }
    os << "\n";
  }

  if (!gUnhandled.empty()) {
    printf("ERROR did not pretty print %ld nodes with tags:\n", gUnhandled.size());
    for (auto tag : gUnhandled) {
      printf("  %s\n", asttags::tagToString(tag));
    }
    return 1;
  }

  return 0;
}
