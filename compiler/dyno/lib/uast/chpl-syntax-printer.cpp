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


#include "chpl/uast/chpl-syntax-printer.h"
#include "chpl/queries/global-strings.h"


using namespace chpl;
using namespace uast;

static const char* kindToString(Decl::Linkage kind) {
  switch (kind) {
    case Decl::Linkage::EXTERN: return "extern";
    case Decl::Linkage::EXPORT: return "export";
    case Decl::Linkage::DEFAULT_LINKAGE: assert(false);
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

static const char* kindToString(Function::Visibility kind) {
  switch (kind) {
    case Function::Visibility::PRIVATE: return "private";
    case Function::Visibility::PUBLIC: return "public";
    case Function::Visibility::DEFAULT_VISIBILITY: assert(false);
  }
  assert(false);
  return "";
}

static const char* kindToString(IntentList kind) {
  switch (kind) {
    case IntentList::CONST_INTENT: return "const";
    case IntentList::VAR: return "var";
    case IntentList::CONST_VAR: return "const var";
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

static const char* kindToString(Module::Kind kind) {
  switch (kind) {
    case Module::Kind::IMPLICIT: return "";
    case Module::Kind::PROTOTYPE: return "prototype";
    case Module::Kind::DEFAULT_MODULE_KIND: assert(false);
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

static const char* kindToString(VisibilityClause::LimitationKind kind) {
  switch (kind) {
    case VisibilityClause::LimitationKind::ONLY: return "only";
    case VisibilityClause::LimitationKind::EXCEPT: return "except";
    case VisibilityClause::LimitationKind::BRACES: assert(false);
    case VisibilityClause::LimitationKind::NONE: assert(false);
    default: return "";
  }
}

// TODO: Attributes
// TODO: Nesting
// TODO: Semicolons
// TODO: Newlines
// TODO: Parentheses based on operator precedence


struct ChplSyntaxVisitor {
  std::stringstream ss_;
  ChplSyntaxVisitor(std::ostringstream ss_ = std::ostringstream()) {}

  /**
   * visit each elt of begin..end, outputting `separator` between each.
   * if not null, `surroundBegin` and `surroundEnd` are output before
   * and after respectively
  */
  template<typename It>
  void interpose(It begin, It end, const char* separator,
                const char* surroundBegin=nullptr,
                const char* surroundEnd=nullptr) {
    bool first = true;
    if (surroundBegin) ss_ << surroundBegin;
    for (auto it = begin; it != end; it++) {
      if (!first) ss_ << separator;
      printChapelSyntax(ss_,*it);
      first = false;
    }
    if (surroundEnd) ss_ << surroundEnd;
  }

  template<typename T>
  void interpose(T xs, const char* separator,
                const char* surroundBegin=nullptr,
                const char* surroundEnd=nullptr) {
    interpose(xs.begin(), xs.end(), separator, surroundBegin, surroundEnd);
  }

  /*
   * helper for printing descendants of simpleBlockLike to handle when to print
   * the optional opening keyword and if braces should follow that keyword
  */
  template<typename It>
  void printBlockWithStyle(BlockStyle style,  It begin, It end,
                          const char* implicitOpeningKeyword=nullptr) {
    if (implicitOpeningKeyword && (style == BlockStyle::IMPLICIT
        || style == uast::BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK)) {
      ss_ << implicitOpeningKeyword;
    }
    if (style != BlockStyle::IMPLICIT) {
      interpose(begin, end, "\n", "{","}");
    } else {
      interpose(begin, end, "\n");
    }
  }

  template<typename T>
  void printBlockWithStyle(BlockStyle style,  T xs,
                          const char* implicitOpeningKeyword=nullptr) {
    printBlockWithStyle(style, xs.begin(), xs.end(), implicitOpeningKeyword);
  }

  template<typename T>
  void visitLiteral(const T* node) {
    /*
     * TODO: can text() be placed in `Literal.h` so this can be generalized
     * for all literals instead of creating specializations of visit for each
     * numeric literal type?
    */
    ss_ << node->text();
  }

  /*
   * helper to check if the called expression is actually a
   * management kind. Helps FnCalls not to print () in this case
  */
  bool isCalleeManagementKind(const AstNode* callee) {
    if (callee->isIdentifier() &&
      (callee->toIdentifier()->name() == "borrowed"
       || callee->toIdentifier()->name() == "owned"
       || callee->toIdentifier()->name() == "unmanaged"
       || callee->toIdentifier()->name() == "shared"))
        return true;
      return false;
  }

  void printFunctionHelper(const Function* node) {
     // storage kind
    if (node->thisFormal() != nullptr
        && node->thisFormal()->storageKind() != IntentList::DEFAULT_INTENT) {
      ss_ << kindToString(node->thisFormal()->storageKind()) <<" ";
    }

    // secondary methods
    if (node->isMethod() && !node->isPrimaryMethod()
        && node->thisFormal()->child(0)->isIdentifier()) {
      ss_ << node->thisFormal()->child(0)->toIdentifier()->name().str() << ".";
    }

    // Function Name
    ss_ << node->name().str();

    // Formals
    int numThisFormal = node->thisFormal() ? 1 : 0;
    int nFormals = node->numFormals() - numThisFormal;
    if (nFormals == 0 && node->isParenless()) {
      // pass
    } else if (nFormals == 0) {
      ss_ << "()";
    } else {
      auto it = node->formals();
      interpose(it.begin() + numThisFormal, it.end(), ", ", "(", ")");
    }

  }

  /*
   * Customized method to print just the function signature as required by
   * the old parser's `userSignature` field.
  */
  void printFunctionSignature(const Function* node) {
    // TODO: Determine how the function signature should be formatted
    // e.g print return type and intent? what about where clause?
    // github issue: https://github.com/chapel-lang/chapel/issues/19411
    if (node->visibility() != Function::Visibility::DEFAULT_VISIBILITY) {
      ss_ << kindToString(node->visibility()) << " ";
    }
    printFunctionHelper(node);
  }

  void printLinkage(const Decl* node) {
    if (node->linkage() != Decl::Linkage::DEFAULT_LINKAGE) {
      ss_ << kindToString(node->linkage()) << " ";
      if (node->linkageName() != nullptr) {
        printChapelSyntax(ss_, node->linkageName());
        ss_ << " ";
      }
    }
  }

  void printTupleContents(const TupleDecl* node) {
    ss_ << "(";
    // TODO: Can this be generalized between TupleDecl and MultiDecl?
    std::string delimiter = "";
    for (auto decl : node->decls()) {
      ss_ << delimiter;
      if (decl->isTupleDecl()) {
        printTupleContents(decl->toTupleDecl());
      } else {
        ss_ << decl->toVarLikeDecl()->name();
        if (const AstNode *te = decl->toVarLikeDecl()->typeExpression()) {
          ss_ << ": ";
          printChapelSyntax(ss_, te);
        }
        if (const AstNode *ie = decl->toVarLikeDecl()->initExpression()) {
          ss_ << " = ";
          printChapelSyntax(ss_, ie);
        }
      }
      delimiter = ", ";
    }
    ss_ << ")";
  }

  void visit(const uast::AstNode* node) {
    assert(false && "Unhandled uAST node");
  }

  void visit(const Array* node) {
    interpose(node->children(), ", ", "[", "]");
  }

  void visit(const As* node) {
    printChapelSyntax(ss_, node->symbol());
    ss_ << " as ";
    printChapelSyntax(ss_, node->rename());
  }

  //Attributes

  void visit(const Begin* node) {
    ss_ << "begin ";
    if (node->withClause()) {
      printChapelSyntax(ss_, node->withClause());
      ss_ << " ";
    }
    printBlockWithStyle(node->blockStyle(), node->stmts());
  }

  void visit(const Block* node) {
    printBlockWithStyle(node->blockStyle(), node->stmts());
  }

  void visit(const BoolLiteral* node) {
    ss_ << (node->value() ? "true" : "false");
  }

  void visit(const BracketLoop* node) {
    ss_ << "[";
    if (node->index()) {
      printChapelSyntax(ss_, node->index());
      ss_ << " in ";
    }
    printChapelSyntax(ss_, node->iterand());
    if (node->withClause()) {
      ss_<< " ";
      printChapelSyntax(ss_, node->withClause());
    }
    ss_ << "]";
    if (node->numStmts() > 0) {
      ss_ << " ";
      interpose(node->stmts(), "");
    }
  }

  void visit(const Break* node) {
    ss_ << "break";
    if (node->target()) {
      ss_<< " ";
      printChapelSyntax(ss_, node->target());
    }
  }

  void visit(const BytesLiteral* node) {
    ss_ << "b\"" << quoteStringForC(std::string(node->str().c_str())) << '"';
  }

  void visit(const Catch* node) {
    ss_ << "catch ";
    if (node->error()) {
      if (node->hasParensAroundError()) ss_ << "(";
      ss_ << node->error()->name();
      if (const AstNode* te = node->error()->typeExpression()) {
        ss_ << " : ";
        printChapelSyntax(ss_, te);
      }
      if (node->hasParensAroundError()) ss_ << ")";
      ss_ << " ";
    }
    interpose(node->stmts(), "\n","{", "}");
  }

  void visit(const Class* node) {
    ss_ << "class ";
    ss_ << node->name() << " ";
    if (node->parentClass() != nullptr) {
      ss_ << ": ";
      printChapelSyntax(ss_, node->parentClass());
      ss_ << " ";
    }
    interpose(node->decls(), "\n", "{", "}");
  }

  void visit(const Cobegin* node) {
    ss_ << "cobegin ";
    if (node->withClause()) {
      printChapelSyntax(ss_, node->withClause());
      ss_<< " ";
    }
    interpose(node->taskBodies(), "\n", "{","}");
  }

  void visit(const Coforall* node) {
    ss_ << "coforall ";

    if (node->index()) {
      printChapelSyntax(ss_, node->index());
      ss_ << " in ";
    }
    printChapelSyntax(ss_, node->iterand());
    if (node->withClause()) {
      ss_<< " ";
      printChapelSyntax(ss_, node->withClause());
    }
    ss_ << " ";
    printBlockWithStyle(node->blockStyle(), node->stmts(), "do ");
  }

  void visit(const Comment* node) {
    // TODO: create a way to filter comments using an adapted AstListIterator
    // TODO: how to control when we want comments on/off

    //    do nothing for now, can be enabled with code below
    //    ss_ << node->c_str();
  }

  void visit(const Conditional* node) {
    ss_ << "if ";
    printChapelSyntax(ss_, node->condition());
    ss_ << " ";
    printBlockWithStyle(node->thenBlockStyle(), node->thenStmts(), "then ");
    if (node->hasElseBlock()) {
      ss_ << " else ";
      printBlockWithStyle(node->elseBlockStyle(), node->elseStmts());
    }
  }

  void visit(const CStringLiteral* node) {
    ss_ << "c\"" << quoteStringForC(std::string(node->str().c_str())) << '"';
  }

  void visit(const Defer* node) {
    ss_ << "defer ";
    printBlockWithStyle(node->blockStyle(), node->stmts());
  }

  void visit(const Delete* node) {
    ss_ << "delete ";
    interpose(node->exprs(), ", ");
  }

  void visit(const Domain* node) {
    if (node->numExprs() == 1 && node->expr(0)->isTypeQuery()) {
      printChapelSyntax(ss_, node->expr(0)->toTypeQuery());
    } else if (node->numExprs() == 0) {
      // do nothing
    } else {
      interpose(node->exprs(), ", ", "{", "}");
    }
  }

  void visit(const Dot* node) {
    printChapelSyntax(ss_, node->receiver());
    ss_ << "." << node->field();
  }

  void visit(const DoWhile* node) {
    ss_ << "do ";
    printBlockWithStyle(node->blockStyle(), node->stmts());
    ss_ << " while ";
    printChapelSyntax(ss_, node->condition());
  }

  void visit(const Enum* node) {
    ss_ << "enum " << node->name() << " ";
    interpose(node->enumElements(), ", ", "{ ", " }");
  }

  void visit(const EnumElement* node) {
    ss_ << node->name();
    if (const AstNode* ie = node->initExpression()) {
      ss_ <<  " = ";
      printChapelSyntax(ss_, ie);
    }
  }

  void visit(const ErroneousExpression* node) {
    ss_ << "<ERROR: Erroneous Expression>";
  }

  void visit(const ExternBlock* node) {
    ss_ << "extern {\n";
    ss_ << node->code();
    ss_ << "}";
  }

  void visit(const FnCall* node) {
    const AstNode* callee = node->calledExpression();
    assert(callee);
    printChapelSyntax(ss_, callee);
    if (isCalleeManagementKind(callee)) {
      ss_ << " ";
      printChapelSyntax(ss_, node->actual(0));
    } else {
      if (node->callUsedSquareBrackets()) {
        interpose(node->actuals(), ", ", "[", "]");
      } else {
        interpose(node->actuals(), ", ", "(", ")");
      }
    }
  }

  void visit(const For* node) {
    ss_ << "for ";
    if (node->isParam()) {
      ss_ << "param ";
    }
    if (node->index()) {
      printChapelSyntax(ss_, node->index());
      ss_ << " in ";
    }
    printChapelSyntax(ss_, node->iterand());
    ss_ << " ";
    printBlockWithStyle(node->blockStyle(), node->stmts(), "do ");
  }

  void visit(const Forall* node) {
    ss_ << "forall ";

    if (node->index()) {
      printChapelSyntax(ss_, node->index());
      ss_ << " in ";
    }
    printChapelSyntax(ss_, node->iterand());
    if (node->withClause()) {
      ss_<< " ";
      printChapelSyntax(ss_, node->withClause());
    }
    ss_ << " ";
    printBlockWithStyle(node->blockStyle(), node->stmts(), "do ");
  }

  void visit(const Foreach* node) {
    ss_ << "foreach ";

    if (node->index()) {
      printChapelSyntax(ss_, node->index());
      ss_ << " in ";
    }
    printChapelSyntax(ss_, node->iterand());
    ss_ << " ";
    printBlockWithStyle(node->blockStyle(), node->stmts(), "do ");
  }

  void visit(const Formal* node) {
    if (node->intent() != Formal::DEFAULT_INTENT) {
      ss_ << kindToString((IntentList) node->intent()) << " ";
    }
    ss_ << node->name().str();
    if (const AstNode* te = node->typeExpression()) {
      ss_ << ": ";
      printChapelSyntax(ss_, te);
    }
    if (const AstNode* ie = node->initExpression()) {
      ss_ << " = ";
      printChapelSyntax(ss_, ie);
    }
  }

  void visit(const ForwardingDecl* node) {
    ss_ << "forwarding ";
    if (node->expr()) {
      printChapelSyntax(ss_, node->expr());
    }
  }

  void visit(const Function* node) {

    printLinkage(node);

    if (node->visibility() != Function::Visibility::DEFAULT_VISIBILITY) {
      ss_ << kindToString(node->visibility()) << " ";
    }

    if (node->isOverride()) {
      assert(node->linkage() == Decl::Linkage::DEFAULT_LINKAGE);
      ss_ << "override ";
    } else if (node->isInline()) {
      assert(node->linkage() == Decl::Linkage::DEFAULT_LINKAGE);
      ss_ << "inline ";
    }

    // Function Kind (proc, iter, ...)
    ss_ << kindToString(node->kind());
    ss_ << " ";

    printFunctionHelper(node);

    // Return Intent
    if (node->returnIntent() != Function::ReturnIntent::DEFAULT_RETURN_INTENT) {
      ss_ << " " << kindToString((IntentList) node->returnIntent());
    }

    // Return type
    if (const AstNode* e = node->returnType()) {
      ss_ << ": ";
      printChapelSyntax(ss_, e);
    }
    ss_ << " ";

    // where clause
    if (node->whereClause()) {
      ss_ << "where ";
      printChapelSyntax(ss_, node->whereClause());
      ss_ << " ";
    }

    // throws
    if (node->throws()) ss_ << "throws ";

    // function body
    interpose(node->stmts(), "\n", "{", "}");

  }

  void visit(const chpl::uast::Identifier* node) {
    ss_ << node->name().str();
  }

  void visit(const ImagLiteral* node) { visitLiteral(node); }

  void visit(const Import* node) {
    if (node->visibility() != Decl::Visibility::DEFAULT_VISIBILITY) {
      ss_ << kindToString(node->visibility());
    }
    ss_ << "import ";
    interpose(node->visibilityClauses(), ", ");
  }

  void visit(const Include* node) {
    ss_ << "include ";
    if (node->visibility() != Decl::DEFAULT_VISIBILITY) {
      ss_ << kindToString(node->visibility()) << " ";
    }
    if (node->isPrototype()) {
      ss_ << "prototype ";
    }
    ss_ << "module ";
    ss_ << node->name();
  }

  void visit(const IntLiteral* node)  { visitLiteral(node); }

  void visit(const Label* node) {
    ss_ << "label ";
    ss_ << node->name() << " ";
    printChapelSyntax(ss_, node->loop());
  }

  void visit(const Let* node) {
    ss_ << "let ";
    // custom handling to avoid printing storage kind as variables would
    // normally do
    // TODO: Can we eliminate or generalize this in a better way?
    for (auto decl : node->decls()) {
      const Variable* var = decl->toVariable();
      ss_ << var->name();
      if (const AstNode *te = var->typeExpression()) {
        ss_ << ": ";
        printChapelSyntax(ss_, te);
      }
      if (const AstNode *ie = var->initExpression()) {
        ss_ << " = ";
        printChapelSyntax(ss_, ie);
      }
    }
    ss_ << " in ";
    printChapelSyntax(ss_, node->expression());
  }

  void visit(const Local* node) {
    ss_ << "local ";
    if (node->condition()) {
      printChapelSyntax(ss_, node->condition());
      ss_ << " ";
    }
    printBlockWithStyle(node->blockStyle(), node->stmts(), "do ");
  }

  void visit(const Manage* node) {
    ss_ << "manage ";
    interpose(node->managers(), ", ");
    ss_ << " ";
    printBlockWithStyle(node->blockStyle(), node->stmts(), "do ");
  }

  void visit(const Module* node) {
    if (node->visibility() != Decl::Visibility::DEFAULT_VISIBILITY) {
      ss_ << kindToString(node->visibility()) << " ";
    }
    if (node->kind() != Module::Kind::DEFAULT_MODULE_KIND ) {
      ss_ << kindToString(node->kind()) << " ";
    }
    ss_ << "module ";
    ss_ << node->name() << " ";
    interpose(node->stmts(), "\n", "{", "}");
  }

  void visit(const MultiDecl* node) {
    ss_ << "var ";

    // TODO: Can this be generalized between TupleDecl and MultiDecl?
    std::string delimiter = "";
    for (auto decl : node->decls()) {
      ss_ << delimiter;
      ss_ << decl->toVariable()->name();
      if (const AstNode* te = decl->toVariable()->typeExpression()) {
        ss_ << ": ";
        printChapelSyntax(ss_, te);
      }
      if (const AstNode* ie = decl->toVariable()->initExpression()) {
        ss_ << " = ";
        printChapelSyntax(ss_, ie);
      }
      delimiter = ", ";
    }

  }

  void visit(const New* node) {
    ss_ << "new ";
    if (node->management() != New::Management::DEFAULT_MANAGEMENT) {
      ss_ << kindToString(node->management()) << " ";
    }
    printChapelSyntax(ss_, node->typeExpression());
  }

  void visit(const On* node) {
    ss_ << "on ";
    printChapelSyntax(ss_, node->destination());
    ss_ << " ";
    printBlockWithStyle(node->blockStyle(), node->stmts(), "do ");
  }

  void visit(const OpCall* node) {
    // TODO: Where do parens come in?
    // ex: !(this && that)
    // is different than !this && that
    if (node->isUnaryOp()) {
      bool isPostFixBang = false;
      if (node->op() == USTR("postfix!")) {
        isPostFixBang = true;
      } else {
        ss_ << node->op();
      }
      assert(node->numActuals() == 1);
      printChapelSyntax(ss_, node->actual(0));
      if (isPostFixBang) {
        ss_ << "!";
      }
    } else if (node->isBinaryOp()) {
      assert(node->numActuals() == 2);
      printChapelSyntax(ss_, node->actual(0));
      ss_ << node->op();
      printChapelSyntax(ss_, node->actual(1));
    }
  }

  void visit(const PrimCall* node) {
    ss_ << "__primitive";
    ss_ << "(";
    ss_ << '"' << quoteStringForC(primTagToName(node->prim())) << '"' << ", ";
    interpose(node->actuals(), ", ");
    ss_ << ")";
  }

  void visit(const Range* node) {
    if (auto lb = node->lowerBound()) {
      printChapelSyntax(ss_, lb);
    }
    ss_ << "..";
    if (auto ub = node->upperBound()) {
      printChapelSyntax(ss_, ub);
    }
  }

  void visit(const RealLiteral* node) { visitLiteral(node); }

  void visit(const Record* node) {
    printLinkage(node);
    ss_ << "record ";
    ss_ << node->name() << " ";
    interpose(node->decls(), "\n", "{", "}");
  }

  void visit(const Reduce* node) {
    ss_ << node->op() << " ";
    ss_ << "reduce ";
    interpose(node->actuals(), ", ");
  }

  void visit(const Require* node) {
    ss_ << "require ";
    interpose(node->exprs(), ", ");
  }

  void visit(const Return* node) {
    ss_ << "return";
    if (node->value() != nullptr) {
      ss_ << " ";
      printChapelSyntax(ss_, node->value());
    }
  }

  void visit(const Scan* node) {
    ss_ << node->op() << " ";
    ss_ << "scan ";
    interpose(node->actuals(), ", ");
  }

  void visit(const Select* node) {
    ss_ << "select ";
    printChapelSyntax(ss_, node->expr());
    ss_ << " ";
    interpose(node->whenStmts(), "\n", "{", "}");
  }

  void visit(const Serial* node) {
    ss_ << "serial ";
    if (node->condition()) {
      printChapelSyntax(ss_, node->condition());
      ss_ << " ";
    }
    printBlockWithStyle(node->blockStyle(), node->stmts(), "do ");
  }

  void visit(const StringLiteral* node) {
    ss_ << '"' << quoteStringForC(std::string(node->str().c_str())) << '"';
  }

  void visit(const Sync* node) {
    ss_ << "sync ";
    printBlockWithStyle(node->blockStyle(), node->stmts());
  }

  void visit(const TaskVar* node) {
    ss_ << kindToString((IntentList) node->intent());
    ss_ << " " << node->name();
  }

  void visit(const Throw* node) {
    ss_ << "throw ";
    printChapelSyntax(ss_, node->errorExpression());
  }

  void visit(const Try* node) {
    ss_ << "try";
    if (node->isTryBang()) {
      ss_ << "! ";
    } else {
      ss_ << " ";
    }
    interpose(node->stmts(), "\n", "{","}");
    // if try block has catch blocks
    if (!node->isTryBang()) {
      ss_ << " ";
      interpose(node->handlers(), "\n");
    }
  }

  void visit(const Tuple* node) {
    interpose(node->children(), ", ", "(", ")");
  }

  void visit(const TupleDecl* node) {
    if (node->intentOrKind() != TupleDecl::IntentOrKind::DEFAULT_INTENT &&
        node->intentOrKind() != TupleDecl::IntentOrKind::INDEX) {
      ss_ << kindToString((IntentList) node->intentOrKind()) << " ";
    }

    printTupleContents(node);

    if (const AstNode* te = node->typeExpression()) {
      ss_ << ": ";
      printChapelSyntax(ss_, te);
    }
    if (const AstNode* ie = node->initExpression()) {
      ss_ << " = ";
      printChapelSyntax(ss_, ie);
    }
  }

  void visit(const TypeDecl* node) {
    printLinkage(node);
    ss_ << "type ";
    ss_ << node->name();
  }

  void visit(const TypeQuery* node) {
    ss_ << "?";
    ss_ << node->name();
  }

  void visit(const UintLiteral* node) { visitLiteral(node); }

  void visit(const Union* node) {
    printLinkage(node);
    ss_ << "union ";
    ss_ << node->name() << " ";
    interpose(node->children(), "\n", "{", "}");
  }

  void visit(const Use* node) {
    if (node->visibility() != Decl::DEFAULT_VISIBILITY) {
      ss_ << kindToString(node->visibility());
    }
    ss_ << "use ";
    interpose(node->visibilityClauses(), ", ");
  }

  void visit(const VarArgFormal* node) {
    if (node->intent() != Formal::Intent::DEFAULT_INTENT) {
      ss_ << kindToString((IntentList) node->intent()) << " ";
    }

    ss_ << node->name();
    if (const AstNode* te = node->typeExpression()) {
      ss_ << ": ";
      printChapelSyntax(ss_, te);
    }
    if (const AstNode* ie = node->initExpression()) {
      ss_ << " = ";
      printChapelSyntax(ss_, ie);
    }

    if (const AstNode* ce = node->count()) {
      ss_ << " ...";
      if (const TypeQuery* tq = ce->toTypeQuery()) {
        printChapelSyntax(ss_, tq);
      } else {
        printChapelSyntax(ss_, ce);
      }
    }
  }

  void visit(const Variable* node) {
    if (node->isConfig()) {
      ss_ << "config ";
    } else {
      printLinkage(node);
    }

    if (node->kind() != Variable::Kind::INDEX) {
      ss_ << kindToString((IntentList) node->kind()) << " ";
    }
    ss_ << node->name();
    if (const AstNode* te = node->typeExpression()) {
      ss_ << ": ";
      printChapelSyntax(ss_, te);
    }
    if (const AstNode* ie = node->initExpression()) {
      ss_ << " = ";
      printChapelSyntax(ss_, ie);
    }
  }

  void visit(const VisibilityClause* node) {
    VisibilityClause::LimitationKind limit = node->limitationKind();
    printChapelSyntax(ss_, node->symbol());
    if (limit == VisibilityClause::LimitationKind::BRACES) {
      ss_ << ".";
      interpose(node->limitations(), ", ", "{","}");
    } else {
      ss_ << " ";
      if (limit == VisibilityClause::LimitationKind::ONLY ||
          limit == VisibilityClause::LimitationKind::EXCEPT) {
        ss_ << kindToString(limit);
        ss_ << " ";
      }
      interpose(node->limitations(), ", ");
    }
  }

  void visit(const When* node) {
    if (node->isOtherwise()) {
      ss_ << "otherwise ";
    } else {
      ss_ << "when ";
      interpose(node->caseExprs(), ", ");
      ss_ << " ";
    }
    printBlockWithStyle(node->blockStyle(), node->stmts(), "do ");
  }

  void visit(const While* node) {
    ss_ << "while ";
    printChapelSyntax(ss_, node->condition());
    ss_ << " ";
    printBlockWithStyle(node->blockStyle(), node->stmts(), "do ");
  }

  void visit(const WithClause* node) {
    ss_ << "with ";
    interpose(node->exprs(), ", ", "(", ")");
  }

  void visit(const Yield* node) {
    ss_ << "yield ";
    printChapelSyntax(ss_, node->value());
  }

  void visit(const Zip* node) {
    ss_ << "zip";
    interpose(node->actuals(),", ","(",")");
  }

};

namespace chpl {
  /* Generic printer calling the above functions */
  void printChapelSyntax(std::ostream& os, const AstNode* node) {
    auto visitor = ChplSyntaxVisitor{};
    node->dispatch<void>(visitor);
    // when using << with ss_.rdbuf(), if nothing gets added to os, then
    // os goes into fail state -> see: https://en.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt
    // check for failbit and reset
    os << visitor.ss_.rdbuf();
    if (os.fail()) {
      os.clear();
    }
    os.flush();
  }

  /***************************************************************
   * Specialization for generating userString when converting
   * from uAST to old AST
  */
  void printFunctionSignature(std::ostream& os, const Function* node) {
    auto visitor = ChplSyntaxVisitor{};
    visitor.printFunctionSignature(node);
    // when using << with ss_.rdbuf(), if nothing gets added to os, then
    // os goes into fail state -> see: https://en.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt
    // check for failbit and reset
    os << visitor.ss_.rdbuf();
    if (os.fail()) {
      os.clear();
    }
    os.flush();
  }

} // end chpl namespace
