/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

/**
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
#include <unordered_map>
#include <unordered_set>
#include <filesystem>

#include "chpl/parsing/Parser.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/queries/Context.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/queries/query-impl.h"
#include "chpl/queries/stringify-functions.h"
#include "chpl/queries/update-functions.h"
#include "chpl/uast/AstTag.h"
#include "chpl/uast/ASTTypes.h"
#include "chpl/uast/TypeDecl.h"
#include "chpl/uast/all-uast.h"
#include "chpl/util/string-escapes.h"

using namespace chpl;
using namespace uast;
using namespace parsing;

using CommentMap = std::unordered_map<ID, const Comment*>;

std::unordered_set<asttags::AstTag> gUnhandled;
static const int indentPerDepth = 3;

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

static std::string indentLines(const std::string& s, int count) {
  std::string replacement = "\n" + std::string(count, ' ');
  std::string head = std::string(count, ' ');
  return head + std::regex_replace(s, std::regex("\n"), replacement);
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
// Return is a list of strings which have no newline chars
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

/**
 Converts an AstNode into an RST format that is suitable for the RHS of
something like:

Function:
.. function:: proc foo(arg1: int): bool
              ^^^^^^^^^^^^^^^^^^^^^^^^^
Enum:
.. enum:: enum Foo { a = 0, b = 2 }
          ^^^^^^^^^^^^^^^^^^^^^^^^^
etc.
 */
struct RstSignatureVisitor {
  std::ostream& os_;

  /** traverse each elt of begin..end, outputting `separator` between each.
   * `surroundBegin` and `surroundEnd` are output before and after respectively
   * if not null */
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
      os_ << ": ";
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
    int numThisFormal = f->thisFormal() ? 1 : 0;
    int nFormals = f->numFormals() - numThisFormal;
    if (nFormals == 0 && f->isParenless()) {
      // pass
    } else if (nFormals == 0) {
      os_ << "()";
    } else {
      auto it = f->formals();
      interpose(it.begin() + numThisFormal, it.end(), ", ", "(", ")");
    }

    // Return type
    if (const Expression* e = f->returnType()) {
      os_ << ": ";
      e->traverse(*this);
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

  bool enter(const AstNode* a) {
    printf("unhandled enter on PrettyPrintVisitor of %s\n", asttags::tagToString(a->tag()));
    a->stringify(std::cerr, StringifyKind::DEBUG_DETAIL);
    gUnhandled.insert(a->tag());
    return false;
  }
  void exit(const AstNode* a) {}
};

/**
 Stores the rst docs result of an AstNode and its children.
 `doc` looks like: (using Python's triple quote syntax)
'''.. function:: proc procName(arg1: int): bool

I am a doc comment'''

 `children` is a vector of non-owning, non-null pointers to its children.

 Another option would be to store the full std::string in rstResult, including
your children. This ends up with a quadratic time and space. Neither is
particularly concerning for this application. Between the two, space is more of
a concern because some doc comments might be quite large-ish. Remember that the
n here is the depth of nesting, which won't be very large and if you concatenate
with a stringstream, that would be plenty fast in time.
 */
struct RstResult {
  std::string doc;
  // The child pointers are non-owning. The query framework owns them.
  std::vector<RstResult*> children;

  bool indentChildren;

  RstResult(std::string doc, std::vector<RstResult*> children,
            bool indentChildren)
      : doc(doc), children(children), indentChildren(indentChildren) {}

  // TODO which one of these do I want?
  static bool update(owned<RstResult>& keep, owned<RstResult>& addin) {
    // return update(*keep, *addin);
    return chpl::defaultUpdateOwned(keep, addin);
  }
  // static bool update(rstResult& keep, rstResult& addin) {
  //   bool changed = false;
  //   changed |= chpl::update<std::string>{}(keep.doc, addin.doc);
  //   changed |= chpl::update<decltype(children)>{}(keep.children, addin.children);
  //   changed |= chpl::update<bool>{}(keep.indentChildren, addin.indentChildren);
  //   return changed;
  // }
  bool operator==(const RstResult& other) {
    return doc == other.doc && children == other.children &&
           indentChildren == other.indentChildren;
  }

  void mark(const Context *c) const {}

  void output(std::ostream& os, int indentPerDepth) {
    output(os, indentPerDepth, 0);
  }

  private: void output(std::ostream& os, int indentPerDepth, int depth) {
    os << indentLines(doc, indentPerDepth * depth);
    if (indentChildren) depth += 1;
    for (auto& child : children) {
      child->output(os, indentPerDepth, depth);
    }
  }
};


static const owned<RstResult>& rstDoc(Context * context, ID id);
static const Comment* const& previousComment(Context* context, ID id);

struct RstResultBuilder {
  Context* context_;
  std::stringstream os_;
  std::vector<RstResult*> children_;

  static const int commentIndent = 3;

  void showComment(const Comment* comment, bool indent=true) {
    if (!comment) {
      os_ << '\n';
      return;
    }

    int indentChars = indent ? commentIndent : 0;
    auto lines = prettifyComment(comment->str());
    os_ << '\n';
    for (const auto& line : lines) {
      if (line.empty()) {
        os_ << '\n';
      } else {
        indentStream(os_, indentChars) << line << '\n';
      }
    }
    os_ << '\n';
  }
  void showComment(const AstNode* node, bool indent=true) {
    showComment(previousComment(context_, node->id()), indent);
  }

  template<typename T>
  void show(const std::string& kind, const T* node, bool indentComment=true) {
    if (isNoDoc(node)) return;

    os_ << ".. " << kind << ":: ";
    RstSignatureVisitor ppv{os_};
    node->traverse(ppv);
    os_ << "\n";

    showComment(node, indentComment);

    if (auto attrs = node->attributes()) {
      if (attrs->isDeprecated()) {
        indentStream(os_, 1 * indentPerDepth) << ".. warning::\n";
        indentStream(os_, 2 * indentPerDepth) << attrs->deprecationMessage().c_str();
        os_ << "\n\n";
      }
    }
  }

  void visitChildren(const AstNode* n) {
    for (auto child : n->children()) {
      if (auto &r = rstDoc(context_, child->id())) {
        children_.push_back(r.get());
      }
    }
  }

  owned<RstResult> visit(const Module* m) {
    // header
    os_ << ".. default-domain:: chpl\n\n";
    os_ << ".. module:: " << m->name().c_str() << '\n';
    const Comment* lastComment = previousComment(context_, m->id());
    if (lastComment) {
      os_ << "    :synopsis: " << commentSynopsis(lastComment) << '\n';
    }
    os_ << '\n';

    // module title
    os_ << m->name().c_str() << "\n";
    os_ << std::string(m->name().length(), '=') << "\n";

    // usage
    // TODO branch on whether FLAG_MODULE_INCLUDED_BY_DEFAULT or equivalent
    os_ << templateReplace(templateUsage, "MODULE", m->name().c_str()) << "\n";

    showComment(lastComment, false);

    visitChildren(m);

    return getResult();
  }

  owned<RstResult> visit(const Function* f) {
    // TODO this doesn't fire on privateProc either
    if (f->visibility() == Decl::Visibility::PRIVATE)
      return {};
    show(kindToRstString(f->isMethod(), f->kind()), f);
    return getResult();
  }
  owned<RstResult> visit(const Variable* v) {
    show("data", v);
    return getResult();
  }
  owned<RstResult> visit(const Record* r) {
    show("record", r);
    visitChildren(r);
    return getResult(true);
  }
  owned<RstResult> visit(const Enum* e) {
    show("enum", e);
    return getResult();
  }

  // TODO all these nullptr gets stored in the query map... can we avoid that?
  owned<RstResult> visit(const AstNode* n) { return {}; }

  owned<RstResult> getResult(bool indentChildren = false) {
    return std::make_unique<RstResult>(os_.str(), children_, indentChildren);
  }
};

/**
 Visitor that collects a mapping from ID -> comment
 Note that we only store this info for comments that we think we might
 need in the future (since not all nodes get doc comments)
 This is more efficient than answering a query like getPreviousComment, which
 would have to goto parent, linear search, and check if previousSibling is a
 Comment
*/
struct CommentVisitor {
  CommentMap& map;
  const Comment* lastComment_ = nullptr;

  void put(const AstNode* n) {
    if (lastComment_) {
      map.emplace(n->id(), lastComment_);
    }
  }

  bool enter(const Comment* comment) {
    lastComment_ = comment;
    return false;
  }
  void exit(const Comment* ast) {}
  void exit(const AstNode* ast) { lastComment_ = nullptr; }

#define DEF_ENTER(type, recurse)                                               \
  bool enter(const type* node) {                                               \
    put(node);                                                                 \
    return recurse;                                                            \
  }

  DEF_ENTER(Module, true)
  DEF_ENTER(TypeDecl, true)
  DEF_ENTER(Function, false)
  DEF_ENTER(Variable, false)

#undef DEF_ENTER

  bool enter(const AstNode* ast) {
    return false;
  }
};

/**
 Get a mapping from ID -> doc comment for an entire file. This mapping is only
 populated for IDs that can have a doc comment. We work at a file level here
 because the doc comment for a Module is its previous sibling
 */
static const CommentMap&
commentMap(Context* context, UniqueString path) {
  QUERY_BEGIN(commentMap, context, path);
  CommentMap result;

  const auto& builderResult = parseFile(context, path);

  CommentVisitor cv{result};
  for (const auto& ast : builderResult.topLevelExpressions()) {
    ast->traverse(cv);
  }

  return QUERY_END(result);
}

/**
 This is a "projection query" of commentMap to get the doc comment for an id.
 This insulates a specific id from irrelevant changes.
 */
static const Comment* const& previousComment(Context* context, ID id) {
  QUERY_BEGIN(previousComment, context, id);
  const Comment* result = nullptr;
  const auto& map = commentMap(context, context->filePathForId(id));
  auto it = map.find(id);
  if (it != map.end()) {
    result = it->second;
  }
  return QUERY_END(result);
}

static const owned<RstResult>& rstDoc(Context* context, ID id) {
  QUERY_BEGIN(rstDoc, context, id);
  owned<RstResult> result;
  // Comments have empty id
  if (!id.isEmpty()) {
    const AstNode* node = idToAst(context, id);
    RstResultBuilder cqv{context};
    result = node->dispatch<owned<RstResult>>(cqv);
  }

  return QUERY_END(result);
}

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
    proc bar: int { return 10; }
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
    UniqueString path = UniqueString::get(ctx, "selftest.chpl");
    setFileText(ctx, path, testString);
  }

  if (args.files.size() > 1) {
    std::cerr << "WARNING only handling one file right now\n";
  }

  for (auto cpath : args.files) {
    UniqueString path = UniqueString::get(ctx, cpath);
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

    for (const auto& ast : builderResult.topLevelExpressions()) {
      if (args.dump) {
        ast->stringify(std::cerr, StringifyKind::DEBUG_DETAIL);
      }
      if (auto& r = rstDoc(ctx, ast->id())) {
        r->output(os, indentPerDepth);
      }
    }

    if (args.dump) {
      const auto& map = commentMap(ctx, path);
      for (const auto& pair : map) {
        std::cerr << pair.first.symbolPath().c_str() << " "
                  << (pair.second ? pair.second->str() : "<nullptr>") << "\n";
      }
    }
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
