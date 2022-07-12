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
#include <queue>

#include "chpl/parsing/Parser.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/framework/Context.h"
#include "chpl/framework/UniqueString.h"
#include "chpl/framework/query-impl.h"
#include "chpl/framework/stringify-functions.h"
#include "chpl/framework/update-functions.h"
#include "chpl/uast/AstTag.h"
#include "chpl/uast/ASTTypes.h"
#include "chpl/uast/TypeDecl.h"
#include "chpl/uast/all-uast.h"
#include "chpl/util/string-escapes.h"
#include "chpl/uast/chpl-syntax-printer.h"
#include "chpl/framework/global-strings.h"


using namespace chpl;
using namespace uast;
using namespace parsing;

using CommentMap = std::unordered_map<ID, const Comment*>;

static const int indentPerDepth = 3;
std::string commentStyle_;
bool writeStdOut_ = false;
std::string outputDir_;
bool textOnly_ = false;
const std::string templateUsage = R"RAW(**Usage**

.. code-block:: chapel

   use $MODULE;


or

.. code-block:: chapel

   import $MODULE;)RAW";

const std::string textOnlyTemplateUsage = R"RAW(

Usage:
   use $MODULE;

or

   import $MODULE;)RAW";

static bool isNoDoc(const Decl* e) {
  if (auto attrs = e->attributes()) {
    return attrs->hasPragma(pragmatags::PRAGMA_NO_DOC);
  }
  return false;
}

static std::vector<std::string> splitLines(const std::string& s) {
  std::stringstream ss(s);
  std::string line;
  std::vector<std::string> ret;
  while (std::getline(ss, line)) {
    ret.push_back(line);
  }
  return ret;
}

static bool hasSubmodule(const Module* mod) {
  for (const AstNode* child : mod->stmts()) {
    if (auto mod = child->toModule())
      if (mod->visibility() != chpl::uast::Decl::PRIVATE && !isNoDoc(mod)) {
        return true;
    }
  }
  return false;
}

static std::string strip(const std::string& s,
                         std::string pattern = "^\\s+|\\s+$") {
  auto re = std::regex(pattern);
  return std::regex_replace(s, re, "");
}

static std::string templateReplace(const std::string& templ,
                                   const std::string& key,
                                   const std::string& value) {
  return std::regex_replace(templ, std::regex(std::string("\\$") + key), value);
}

static UniqueString getNodeName(AstNode* node) {
  if (node->isNamedDecl()) {
    return node->toNamedDecl()->name();
  } else if (node->isIdentifier()) {
    return node->toIdentifier()->name();
  } else {
    assert(false && "no name defined for node");
  }
}

static char* checkProjectVersion(char* projectVersion) {
  int length = strlen(projectVersion);
  int i = 0;
  int dot = 0;
  bool check = true;
  int tagIndex = 0;

  // Supported version tags
  const char * tags[] = {"alpha", "beta", "rc"};
  const char * error = "";
  for(i=0; i<length; i++) {
    if(i>0 && projectVersion[i] == '.') {
      if(projectVersion[i-1] != '.') {
        dot++;
        if(dot>2) {
          error = "Required only two dots which separates three numbers";
          check = false;
          break;
        }
        if(i == length-1) {
          error = "Cannot end with dot, can end with either number or tag";
          check = false;
          break;
        }
      } else {
        error = "Missing number between dots";
        check = false;
        break;
      }
    } else if(projectVersion[i] == '-' && dot == 2) {
      if(projectVersion[i-1] != '.') {
        tagIndex = i+1;
        break;
      } else {
        error = "Missing number before tag";
        check = false;
        break;
      }
    } else if(projectVersion[i] == '-' && dot != 2) {
      error = "Required only two dots which separates three numbers";
      check = false;
      break;
    } else if((int)projectVersion[i] > (int)'9' ||
              (int)projectVersion[i] < (int)'0') {
      error = "Invalid Characters, only digits and dots permitted before a hyphen";
      check = false;
      break;
    }
  }
  if(dot != 2 && i == length) {
    error = "Required two dots which separates three numbers";
    check = false;
  }
  if(check && tagIndex>0) {
    int count = sizeof(tags)/sizeof(*tags);
    for(int i=0; i<count; i++) {
      if(strcmp(projectVersion+tagIndex,tags[i]) == 0) {
        check = true;
        break;
      } else {
        error = "Tag not supported, supported tags are alpha/beta/rc";
        check = false;
      }
    }
  }
  if(check) {
    return projectVersion;
  } else {
    std::cerr << "error: Invalid version format: "
              << projectVersion << " due to: " << error << std::endl;
    exit(1);
  }
  return NULL;
}

static std::string indentLines(const std::string& s, int count) {
  if (s.empty())
    return s;
  std::string head = std::string(count, ' ');
  std::string ret = head + s;
  return ret;
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

// Remove the leading+trailing commentStyle (/*+, *+/)
// Dedent by the least amount of leading whitespace
// Return is a list of strings which may have newline chars
static std::vector<std::string> prettifyComment(const std::string& comment,
                                                const std::string& commentStyle) {
  std::string ret = comment;
  std::string commentEnd = commentStyle;
  reverse(commentEnd.begin(), commentEnd.end());
  const int styleLen = commentStyle.size();
  if (ret.substr(0, styleLen) == commentStyle) {
    size_t l = ret.length();
    if (ret.substr(l - styleLen, styleLen) != commentEnd)
      // TODO: this is a broken comment at this point, no?
      return {};
    ret.erase(l - styleLen, styleLen);
    ret.erase(0, styleLen);
  } else {
    return {};
  }

  auto lines = splitLines(ret);
  if (lines.empty()) return lines;

  size_t toTrim = std::numeric_limits<size_t>::max();
  // exclude the first line from the minimum
  for (auto it = lines.begin(); it < lines.end(); ++it) {
    if (it->empty()) continue;
    if (it == lines.begin()) {
      *it = strip(*it, "^\\s+");
      continue;
    }
    toTrim = std::min(toTrim, countLeadingSpaces(*it));
  }

  assert((toTrim < std::numeric_limits<size_t>::max() || lines.size()==1) &&
          "probably an empty comment");


  for (auto it = lines.begin(); it < lines.end(); ++it) {
    if (it != lines.begin())
      it->erase(0, toTrim);
  }

  return lines;
}

static std::string commentSynopsis(const Comment* c,
                                   const std::string& commentStyle) {
  if (!c) return "";
  auto lines = prettifyComment(c->str(), commentStyle);
  if (lines.empty()) return "";
  return lines[0];
}

static const char* kindToRstString(bool isMethod, Function::Kind kind) {
  switch (kind) {
  case Function::Kind::PROC: return isMethod ? "method" : "function";
  case Function::Kind::ITER: return isMethod ? "itermethod" : "iterfunction";
  case Function::Kind::OPERATOR: return "operator";
  case Function::Kind::LAMBDA: return "lambda";
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
  case Function::Kind::LAMBDA: return "lambda";
  }
  assert(false);
  return "";
}

static const char* kindToString(IntentList kind) {
  switch (kind) {
    case IntentList::CONST_INTENT: return "const";
    case IntentList::VAR: return "var";
    case IntentList::CONST_VAR: return "const";
    case IntentList::CONST_REF: return "const ref";
    case IntentList::REF: return "ref";
    case IntentList::IN: return "in";
    case IntentList::CONST_IN: return "const in";
    case IntentList::OUT: return "out";
    case IntentList::INOUT: return "inout";
    case IntentList::PARAM: return "param";
    case IntentList::TYPE: return "type";
    case IntentList::DEFAULT_INTENT: assert(false);
    default: return "";
  }
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

static bool isCalleReservedWord(const AstNode* callee) {
  if (callee->isIdentifier() &&
      (callee->toIdentifier()->name() == USTR("borrowed")
       || callee->toIdentifier()->name() == USTR("owned")
       || callee->toIdentifier()->name() == USTR("unmanaged")
       || callee->toIdentifier()->name() == USTR("shared")
       || callee->toIdentifier()->name() == USTR("sync")
       || callee->toIdentifier()->name() == USTR("single")
       || callee->toIdentifier()->name() == USTR("atomic")))
      return true;
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
  bool printingType_ = false;
  /** traverse each elt of begin..end, outputting `separator` between each.
   * `surroundBegin` and `surroundEnd` are output before and after respectively
   * if not null */
  template<typename It>
  void interpose(It begin, It end, const char* separator,
                 const char* surroundBegin=nullptr,
                 const char* surroundEnd=nullptr) {
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
  void interpose(T xs, const char* separator,
                 const char* surroundBegin=nullptr,
                 const char* surroundEnd=nullptr) {
    interpose(xs.begin(), xs.end(), separator, surroundBegin, surroundEnd);
  }

  bool enter(const Identifier* r) {
    os_ << r->name().c_str();
    return false;
  }

  bool enter(const Record* r) {
    // TODO: Shouldn't this be record, not Record?
    if (textOnly_) os_ << "Record: ";
    os_ << r->name().c_str();
    return false;
  }

  bool enter(const Class* c) {
    os_ << c->name().c_str();
    if (c->parentClass()) {
      os_ << " : ";
      c->parentClass()->traverse(*this);
    }
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
    os_ << '"' << escapeStringC(l->str().c_str()) << '"';
    return false;
  }

  bool enter(const CStringLiteral* l) {
    os_ << "c\"" << escapeStringC(l->str().c_str()) << '"';
    return false;
  }

  bool enter(const Dot* d) {
    d->receiver()->traverse(*this);
    os_ << "." << d->field().c_str();
    return false;
  }

  bool enter(const New* d) {
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
    if (const AstNode* ie = e->initExpression()) {
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
      os_ << kindToString((IntentList) v->kind()) << " ";
    }
    os_ << v->name().c_str();
    if (const AstNode* te = v->typeExpression()) {
      printingType_ = true;
      os_ << ": ";
      te->traverse(*this);
      printingType_ = false;
    }
    if (const AstNode* ie = v->initExpression()) {
      os_ << " = ";
      if (v->storageKind() == chpl::uast::IntentList::TYPE)
        printingType_ = true;
      ie->traverse(*this);
      if (v->storageKind() == chpl::uast::IntentList::TYPE)
        printingType_ = false;
    }
    return false;
  }

  bool enter(const Formal* f) {
    if (f->intent() != Formal::DEFAULT_INTENT) {
      os_ << kindToString((IntentList) f->intent()) << " ";
    }
    os_ << f->name().c_str();
    if (const AstNode* te = f->typeExpression()) {
      os_ << ": ";
      printingType_ = true;
      te->traverse(*this);
      printingType_ = false;
    }
    if (const AstNode* ie = f->initExpression()) {
      os_ << " = ";
      ie->traverse(*this);
    }
    return false;
  }

  bool enter(const Function* f) {
    // Linkage
    if (f->linkage() == Decl::Linkage::EXPORT)
      os_ << "export ";

    // Visibility
    if (f->visibility() != Function::Visibility::DEFAULT_VISIBILITY) {
      os_ << kindToString(f->visibility()) << " ";
    }

    if (f->isOverride()) {
      os_ << "override ";
    }

    // Function Name
    os_ << kindToString(f->kind());
    os_ << " ";

    // storage kind
    if (f->thisFormal() != nullptr
        && f->thisFormal()->storageKind() != IntentList::DEFAULT_INTENT) {
      os_ << kindToString(f->thisFormal()->storageKind()) <<" ";
    }

    // print out the receiver type for secondary methods
    if (f->isMethod() && !f->isPrimaryMethod()) {
      auto typeExpr = f->thisFormal()->typeExpression();
      assert(typeExpr);

      if (auto ident = typeExpr->toIdentifier()) {
        os_ << ident->name().str();
      } else {
        os_ << "(";
        typeExpr->traverse(*this);
        os_ << ")";
      }

      os_ << ".";
    }

    if (f->kind() == Function::Kind::OPERATOR && f->name() == USTR("=")) {
      // TODO: remove this once the old parser is out of the question
      // TODO: this is only here to support tests from the old parser
      // printing extra spaces around an assignment operator
      os_ << " ";
      // Function Name
      os_ << f->name().str();
      os_ << " ";
    } else {
      // Function Name
      os_ << f->name().str();
    }

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
    if (const AstNode* e = f->returnType()) {
      os_ << ": ";
      printingType_ = true;
      e->traverse(*this);
      printingType_ = false;
    }

    // Return Intent
    if (f->returnIntent() != Function::ReturnIntent::DEFAULT_RETURN_INTENT &&
        f->returnIntent() != Function::ReturnIntent::CONST) {
      os_ << " " << kindToString((IntentList) f->returnIntent());
    }

    // throws
    if (f->throws()) os_ << " throws";

    return false;
  }

  bool isPostfix(const OpCall* op) {
    return (op->isUnaryOp() &&
            (op->op() == USTR("postfix!") || op->op() == USTR("?")));
  }

  void printUnaryOp(const OpCall* node) {
    assert(node->numActuals() == 1);
    UniqueString unaryOp;
    bool isPostFixBang = false;
    bool isNilable = false;
    unaryOp = node->op();
    if (unaryOp == USTR("postfix!")) {
      isPostFixBang = true;
      unaryOp = USTR("!");
    } else if (node->op() == USTR("?")) {
      if (node->actual(0)->isFnCall()) {
        isNilable = true;
      } else {
        os_ << "nilable ";
      }
    } else {
      os_ << unaryOp;
    }
    opHelper(node, 0, (isPostFixBang || isNilable));
    if (isPostFixBang || isNilable) {
      os_ << unaryOp;
    }
  }

  void opHelper(const OpCall* node, int pos, bool postfix) {
    bool needsParens = false;
    bool isRHS = pos;
    UniqueString outerOp, innerOp;
    outerOp = node->op();
    if (node->actual(pos)->isOpCall()) {
      innerOp = node->actual(pos)->toOpCall()->op();
      needsParens = needParens(outerOp, innerOp, node->isUnaryOp(), postfix,
                               node->actual(pos)->toOpCall()->isUnaryOp(),
                               isPostfix(node->actual(pos)->toOpCall()), isRHS);
    }
    // handle printing parens around tuples
    // ex: 3*(4*string) != 3*4*string
    needsParens = tupleOpNeedsParens(node, needsParens, isRHS, outerOp,
                                     innerOp);
    if (needsParens) os_ << "(";
    node->actual(pos)->traverse(*this);
    if (needsParens) os_ << ")";
  }

  // check if this is a start tuple decl, for example:
  // 3*string or 2*(3*string), etc
  bool isStarTupleDecl(UniqueString op, const OpCall* node) const {
    // TODO: need to adjust the rules as to what exactly can construct
    //  a star tuple, it's not clear how to filter FnCalls
    //  ex: 3*(real(64)) is a * op with lhs = IntLiteral(3) and
    //  rhs = FnCall(real->64)
    bool ret = false;
    if (node && op == USTR("*") && node->actual(0)->isIntLiteral() &&
        (node->actual(1)->isIdentifier() ||
         node->actual(1)->isTuple() ||
         node->actual(1)->isFnCall())) {
      ret = true;
    }
    return ret;
  }

  bool tupleOpNeedsParens(const OpCall* node, bool needsParens, bool isRHS,
                          UniqueString& outerOp, UniqueString& innerOp) const {
    if (needsParens || !isRHS) return needsParens;
    bool ret = needsParens;
    bool isOuterStarTuple = isStarTupleDecl(outerOp, node);
    bool isInnerStarTuple = isStarTupleDecl(innerOp,
                                            node->actual(1)->toOpCall());
    if ((isOuterStarTuple || isInnerStarTuple)) {
      ret = true;
    }
    return ret;
  }

  void printBinaryOp(const OpCall* node) {
    assert(node->numActuals() == 2);
    bool addSpace = wantSpaces(node->op(), printingType_) ||
                    node->op() == USTR("by") ||
                    node->op() == USTR("align") ||
                    node->op() == USTR("reduce=") ||
                    node->op() == USTR("reduce") ||
                    node->op() == USTR("scan") ||
                    node->op() == USTR("dmapped");
    opHelper(node, 0, false);
    if (addSpace && node->op() != USTR(":"))
      os_ << " ";
    os_ << node->op();
    if (addSpace)
      os_ << " ";
    opHelper(node, 1, false);
  }

  bool enter(const OpCall* node) {
    if (node->isUnaryOp()) {
      printUnaryOp(node);
    } else if (node->isBinaryOp()) {
      printBinaryOp(node);
    }
    return false;
  }

  bool enter(const FnCall* call) {
    const AstNode* callee = call->calledExpression();
    assert(callee);
    callee->traverse(*this);
    if (isCalleReservedWord(callee)) {
      os_ << " ";
      call->actual(0)->traverse(*this);
    } else {
      if (call->callUsedSquareBrackets()) {
        os_ << "[";
      } else {
        os_ << "(";
      }
      std::string sep;
      for (int i = 0; i < call->numActuals(); i++) {
        os_ << sep;
        if (call->isNamedActual(i)) {
          os_ << call->actualName(i);
          // The spaces around = are just to satisfy old tests
          // TODO: Remove spaces around `=` when removing old parser
          os_ << " = ";
        }

        call->actual(i)->traverse(*this);

        sep = ", ";
      }
      if (call->callUsedSquareBrackets()) {
        os_ << "]";
      } else {
        os_ << ")";
      }
    }
    return false;
  }

  bool enter(const Domain* dom) {
    interpose(dom->children(), ", ", "{", "}");
    return false;
  }

  bool enter(const Tuple* tup) {
    os_ << "(";
    bool first = true;
    for (auto it = tup->children().begin(); it != tup->children().end(); it++) {
      if (!first) os_ << ", ";
      (*it)->traverse(*this);
      first = false;
    }
    if (tup->numChildren() == 1) {
      os_ << ",";
    }
    os_ << ")";
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

  bool enter(const BracketLoop* bl) {
    printChapelSyntax(os_, bl);
    return false;
  }

  bool enter(const Block* b) {
    return true;
  }

  bool enter(const Use* node) {
    node->stringify(os_, StringifyKind::CHPL_SYNTAX);
    return false;
  }

  bool enter(const Import* node) {
    node->stringify(os_, StringifyKind::CHPL_SYNTAX);
    return false;
  }

  bool enter(const VarArgFormal* node) {
    node->stringify(os_, StringifyKind::CHPL_SYNTAX);
    return false;
  }

  bool enter(const TypeQuery* node) {
    node->stringify(os_, StringifyKind::CHPL_SYNTAX);
    return false;
  }

  bool enter(const Conditional* node) {
    node->stringify(os_, StringifyKind::CHPL_SYNTAX);
    return false;
  }

  bool enter(const AstNode* a) {
    printChapelSyntax(os_, a);
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

  void outputModule(std::string outDir, std::string name, int indentPerDepth) {
    std::string ext = textOnly_ ? ".txt" : ".rst";
    auto outpath = outDir + "/" + name + ext;

    std::error_code err;
    if (!std::filesystem::create_directories(outDir, err)) {
      // TODO the above seems to return false when already exists,
      // but I don't think that should be the case
      // So check we really got an error
      if (err) {
        std::cerr << "Could not create " << outDir << " because "
                  << err.message() << "\n";
        return;
      }
    }
    std::ofstream ofs = std::ofstream(outpath, std::ios::out);
    output(ofs, indentPerDepth);
  }

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
static const std::vector<UniqueString>& getModulePath(Context* context, ID id);

struct RstResultBuilder {
  Context* context_;
  std::stringstream os_;
  std::vector<RstResult*> children_;
  const std::string commentStyle = commentStyle_;
  static const int commentIndent = 3;
  int indentDepth_ = 1;

  bool showComment(const Comment* comment, bool indent=true) {
    if (!comment || comment->str().substr(0, 2) == "//") {
      os_ << '\n';
      return false;
    }
    int addDepth = textOnly_ ? 1 : 0;
    int indentChars = indent ? (addDepth + indentDepth_) * commentIndent : 0;
    auto lines = prettifyComment(comment->str(), commentStyle);
    if (!lines.empty())
      os_ << '\n';
    for (const auto& line : lines) {
      if (line.empty()) {
        // insert blank spaces here to match test output from chpldoc
        // TODO: See if spaces needed for the docs to format correctly
        indentStream(os_, indentChars) << '\n';
      } else {
        indentStream(os_, indentChars) << line << '\n';
      }
    }
    return true;
  }
  bool showComment(const AstNode* node, bool indent=true) {
    bool commentShown = showComment(previousComment(context_, node->id()), indent);
    if (commentShown && ((textOnly_ && !node->isModule()) || !textOnly_)) os_ << "\n";
    return commentShown;
  }

  template<typename T>
  bool show(const std::string& kind, const T* node, bool indentComment=true) {
    if (isNoDoc(node)) return false;

    if (!textOnly_) os_ << ".. " << kind << ":: ";
    RstSignatureVisitor ppv{os_};
    node->traverse(ppv);
    if (!textOnly_) os_ << "\n";
    bool commentShown = showComment(node, indentComment);

    // TODO: Fix all this because why are we checking for specific node types?
    if (commentShown && !textOnly_ && (node->isEnum() ||
                                       node->isClass() ||
                                       node->isRecord() ||
                                       node->isModule())) {
      os_ << "\n";
    }

    if (auto attrs = node->attributes()) {
      if (attrs->isDeprecated() && !textOnly_) {
        auto comment = previousComment(context_, node->id());
        if (comment && !comment->str().empty() &&
            comment->str().substr(0, 2) == "/*" &&
            comment->str().find("deprecat") != std::string::npos ) {
            // do nothing because deprecation was mentioned in doc comment
        } else {
          // write the deprecation warning and message
          indentStream(os_, indentDepth_ * indentPerDepth) << ".. warning::\n\n";
          indentStream(os_, (indentDepth_ + 1) * indentPerDepth);
          if (attrs->deprecationMessage().isEmpty()) {
            // write a generic message because there wasn't a specific one
            os_ << getNodeName((AstNode*) node) << " is deprecated";
          } else {
            // use the specific deprecation message
            os_ << strip(attrs->deprecationMessage().c_str());
          }
          os_ << "\n\n";
        }
      }
    }
    return commentShown;
  }

  void visitChildren(const AstNode* n) {
    std::vector<RstResult*> subModules;
    for (auto child : n->children()) {
      if (auto &r = rstDoc(context_, child->id())) {
        if (child->isModule()) {
          if (!writeStdOut_) {
            // lookup the module path
            std::vector<UniqueString> modulePath = getModulePath(context_,
                                                                 child->id());
            modulePath.pop_back();
            std::string parentPath = modulePath.back().str();
            std::string outdir = outputDir_ + "/" + parentPath;
            r->outputModule(outdir, child->toModule()->name().str(),
                            indentPerDepth);

          } else {
            subModules.push_back(r.get());
          }
        } else {
          children_.push_back(r.get());
        }
      }
    }
    // add the submodules to the end so that all the fields/functions of
    // an individual module are printed together, and not interrupted by
    // a submodule's declaration
    for (auto subModule : subModules) {
      children_.push_back(subModule);
    }
  }

  owned<RstResult> visit(const Module* m) {
    if (m->visibility() == Decl::Visibility::PRIVATE || isNoDoc(m))
      return {};
    // lookup the module path
    std::vector<UniqueString> modulePath = getModulePath(context_, m->id());
    std::string moduleName;
    // build up the module path string using `.` to separate the components
    for (auto p : modulePath) {
      moduleName += p.str();
      if (p != modulePath.back()) {
        moduleName += ".";
      }
    }
    assert(!moduleName.empty());
    const Comment* lastComment = nullptr;
    // header
    if (!textOnly_) {
      os_ << ".. default-domain:: chpl\n\n";
      os_ << ".. module:: " << m->name().c_str() << '\n';
      lastComment = previousComment(context_, m->id());
      if (lastComment) {
        indentStream(os_, 1 * indentPerDepth);
        os_ << ":synopsis: " << commentSynopsis(lastComment, commentStyle)
            << '\n';
      }
      os_ << '\n';

      // module title
      os_ << m->name().c_str() << "\n";
      os_ << std::string(m->name().length(), '=') << "\n";

      // usage
      // TODO branch on whether FLAG_MODULE_INCLUDED_BY_DEFAULT or equivalent
      os_ << templateReplace(templateUsage, "MODULE", moduleName) << "\n";
    } else {
      os_ << m->name().c_str();
      os_ << templateReplace(textOnlyTemplateUsage, "MODULE", moduleName) << "\n";
      lastComment = previousComment(context_, m->id());
    }
    if (hasSubmodule(m)) {
      if (!textOnly_) {
        os_ << "\n";
        os_ << "**Submodules**" << std::endl << std::endl;

        os_ << ".. toctree::" << std::endl;
        indentStream(os_, 1 * indentPerDepth);

        os_ << ":maxdepth: 1" << std::endl;
        indentStream(os_, 1 * indentPerDepth);

        os_ << ":glob:" << std::endl << std::endl;
        indentStream(os_, 1 * indentPerDepth);

        os_ << moduleName << "/*" << std::endl;
      } else {
        os_ << "\nSubmodules for this module are located in the ";
        os_ << moduleName << "/ directory" << std::endl;
      }
    }
    if (textOnly_) indentDepth_ --;
    showComment(m, textOnly_);
    if (textOnly_) indentDepth_ ++;

    visitChildren(m);

    return getResult(textOnly_);
  }

  owned<RstResult> visit(const Function* f) {
    if (f->visibility() == Decl::Visibility::PRIVATE || isNoDoc(f))
      return {};
    bool doIndent = f->isMethod() && f->isPrimaryMethod();
    if (doIndent) indentDepth_ ++;
    show(kindToRstString(f->isMethod(), f->kind()), f);
    if (doIndent) indentDepth_ --;
    return getResult();
  }

  owned<RstResult> visit(const Variable* v) {
    if (v->visibility() == Decl::Visibility::PRIVATE || isNoDoc(v))
      return {};

    if (v->isField()) {
      indentDepth_ ++;
      show("attribute", v);
      indentDepth_ --;
    }
    else if (v->storageKind() == IntentList::TYPE)
      show("type", v);
    else
      show("data", v);

    return getResult();
  }

  owned<RstResult> visit(const Record* r) {
    if (isNoDoc(r)) return {};
    show("record", r);
    visitChildren(r);
    return getResult(true);
  }
  owned<RstResult> visit(const Enum* e) {
    if (isNoDoc(e)) return {};
    show("enum", e);
    return getResult();
  }

  owned<RstResult> visit(const MultiDecl* md) {
    if (isNoDoc(md)) return {};

    std::string kind;
    std::queue<const AstNode*> expressions;

    for (auto decl : md->decls()) {
      if (decl->toVariable()->typeExpression() ||
          decl->toVariable()->initExpression()) {
        expressions.push(decl);
        kind = decl->toVariable()->isField() ? "attribute" : "data";
      }
    }
    std::string prevTypeExpression;
    std::string prevInitExpression;
    for (auto decl : md->decls()) {
      if (kind=="attribute" && decl != md->decls().begin()->toDecl()) {
        indentStream(os_, 1 * indentPerDepth);
      }
      // write kind
      if (!textOnly_) os_ << ".. " << kind << ":: ";
      if (decl->toVariable()->kind() != Variable::Kind::INDEX) {
        os_ << kindToString((IntentList) decl->toVariable()->kind()) << " ";
      }
      // write name
      os_ << decl->toVariable()->name();

      if (expressions.empty()) {
        assert(!prevTypeExpression.empty() || !prevInitExpression.empty());
        multiDeclHelper(decl, prevTypeExpression, prevInitExpression);
      } else {
        // our decl doesn't have a typeExpression or initExpression,
        // or it is the same one
        // is previousInit or previousType set?
        if (!prevInitExpression.empty() || !prevTypeExpression.empty()) {
          multiDeclHelper(decl, prevTypeExpression, prevInitExpression);
        } else { // use the expression in the queue
          // take the one from the front of the expressions queue
          if (auto te = expressions.front()->toVariable()->typeExpression()) {
            //write out type expression->stringify
            os_ << ": ";
            te->stringify(os_, CHPL_SYNTAX);
            //set previous type expression = ": variableName.type"
            prevTypeExpression =
                ": " + decl->toVariable()->name().str() + ".type";
          }
          if (auto exp = expressions.front()->toVariable()->initExpression()) {
            // write out initExpression->stringify
            os_ << " = ";
            exp->stringify(os_, CHPL_SYNTAX);
            // set previous init expression = "= variableName"
            prevInitExpression = " = " + decl->toVariable()->name().str();
          }
        }
        if (decl == expressions.front()) {
          // our decl is the same as the first one in the queue, so we should
          // pop it and clear the previousInit and previousType
          expressions.pop();
          prevTypeExpression.clear();
          prevInitExpression.clear();
        }
      }
      showComment(md, true);

      if (auto attrs = md->attributes()) {
        if (attrs->isDeprecated() && !textOnly_) {
          indentStream(os_, 1 * indentPerDepth) << ".. warning::\n";
          indentStream(os_, 2 * indentPerDepth) << attrs->deprecationMessage();
          os_ << "\n\n";
        }
      }
      os_ << "\n";
    }
    return getResult();
  }

  void multiDeclHelper(const Decl* decl, std::string& prevTypeExpression,
                       std::string& prevInitExpression) {
    if (!prevTypeExpression.empty()) {
      // write prevTypeExpression
      os_ << prevTypeExpression;
      // set prevTypeExpression = ": thisVariableName.type"
      prevTypeExpression = ": " + decl->toVariable()->name().str() + ".type";
    }
    if (!prevInitExpression.empty()) {
      // write prevInitExpression
      os_ << prevInitExpression;
      // set prevInitExpression  = "= thisVariableName"
      prevInitExpression = " = " + decl->toVariable()->name().str();
    }
  }

  owned<RstResult> visit(const Class* c) {
    if (isNoDoc(c) || c->visibility() == chpl::uast::Decl::PRIVATE) return {};
    show("class", c);
    visitChildren(c);
    return getResult(true);
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
      // check that we didn't already use this comment, which can happen
      // if a module has a comment and it's first function does not,
      // in that case we improperly apply the comment to both the module
      // and the function.
      for (const auto& mapEntry : map) {
        if (mapEntry.second == lastComment_)
          return;
      }
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
  UniqueString emptyParent;
  const auto& builderResult = parseFileToBuilderResult(context,
                                                       path,
                                                       emptyParent);

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
  UniqueString modPath;
  UniqueString parentPath;

  assert(context->filePathForId(id, modPath, parentPath));

  const auto& map = commentMap(context, modPath);
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

static const std::vector<UniqueString>& getModulePath(Context* context, ID id) {
  QUERY_BEGIN(getModulePath, context, id);
  std::vector<UniqueString> result;
  if (!id.isEmpty()) {
    ID parentID = idToParentModule(context, id);
    while (!parentID.isEmpty()) {
      // a submodule
      UniqueString modulePath = parentID.symbolName(context);
      // place parent names in the front
      result.insert(result.begin(), modulePath);
      // see if we have another parent
      parentID = idToParentModule(context, parentID);
    }
    // a top level module
    result.push_back(id.symbolName(context));
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
  bool textOnly = false;
  std::string outputDir;
  bool processUsedModules = false;
  std::string author;
  std::string commentStyle =  "/*";
  std::string projectVersion = "0.0.1";
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
    } else if (std::strcmp("--text-only", argv[i]) ==  0) {
      ret.textOnly = true;
    } else if (std::strcmp("--output-dir", argv[i]) == 0) {
      assert(i < (argc - 1));
      ret.outputDir = argv[i + 1];
      i += 1;
    } else if (std::strcmp("--author", argv[i]) == 0) {
      assert(i < (argc - 1));
      ret.author = argv[i + 1];
      i += 1;
    } else if (std::strcmp("--process-used-modules", argv[i]) ==  0) {
      ret.processUsedModules = true;
    } else if (std::strcmp("--comment-style", argv[i]) == 0) {
      assert(i < (argc - 1));
      ret.commentStyle = argv[i + 1];
      i += 1;
    } else if (std::strcmp("--project-version", argv[i]) == 0) {
      assert(i < (argc - 1));
      ret.projectVersion = checkProjectVersion(argv[i + 1]);
      i += 1;
    } else {
      ret.files.push_back(argv[i]);
    }
  }
  return ret;
}

// TODO what do we do if multiple modules (submodules) are in one file?
static std::string moduleName(const BuilderResult& builderResult) {
  for (const auto& ast : builderResult.topLevelExpressions()) {
    if (const Module* m = ast->toModule()) {
      /* TODO: This is wrong if the module is an implicit module and it just
          imports or uses another module. In that case the name should be that
          of the used or imported module.
      */
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
  writeStdOut_ = args.stdout;
  textOnly_ = args.textOnly;
  if (args.commentStyle.substr(0,2) != "/*") {
    std::cerr << "error: comment label should start with /*" << std::endl;
    return 1;
  }
  commentStyle_ = args.commentStyle;
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
    UniqueString emptyParent;

    // TODO: Change which query we use to parse files as suggested by @mppf
    // parseFileContainingIdToBuilderResult(Context* context, ID id);
    // and then work with the module ID to find the preceeding comment.
    const BuilderResult& builderResult = parseFileToBuilderResult(ctx,
                                                                  path,
                                                                  emptyParent);

    if (builderResult.numErrors() > 0) {
      // TODO: handle errors better, don't rely on parse query to emit them
      // per @mppf: if dyno-chpldoc wants to quit on an error, it should
      // configure Context::reportError to have a custom function that does so.

      for (auto e : builderResult.errors()) {
      // just display the error messages right now, see TODO above
        if (e.kind() == ErrorMessage::Kind::ERROR ||
            e.kind() == ErrorMessage::Kind::SYNTAX) {
              std::cerr << "Error parsing " << path << ": "
                        << builderResult.error(0).message() << "\n";
              return 1;
            }
      }
    }
    if (!args.stdout) {
      auto name = moduleName(builderResult);
      if (!textOnly_)
        outputDir_ = args.saveSphinx + "/source/modules";
      else
        outputDir_ = args.saveSphinx;
      for (const auto& ast : builderResult.topLevelExpressions()) {
        if (args.dump) {
          ast->stringify(std::cerr, StringifyKind::DEBUG_DETAIL);
        }
        if (auto& r = rstDoc(ctx, ast->id())) {
          r->outputModule(outputDir_, name, indentPerDepth);
        }
      }
    }
    else {
      for (const auto& ast : builderResult.topLevelExpressions()) {
        if (args.dump) {
          ast->stringify(std::cerr, StringifyKind::DEBUG_DETAIL);
        }
        if (auto& r = rstDoc(ctx, ast->id())) {
          r->output(std::cout, indentPerDepth);
        }
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

  return 0;
}
