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

static std::string pragmaFlagsToString(const Decl* node) {
  std::string ret;
  if (!node->attributes()) return ret;

  // TODO: Add spaces after parsers are merged.
  for (auto pragma : node->attributes()->pragmas()) {
    ret += "pragma";
    ret += "\"";
    ret += pragmaTagToName(pragma);
    ret += "\"";
  }
  return ret;
}

// stolen from convert-uast.cpp to prevent printing { } inside arrays
// probably a better way to do this here
static bool isBracketLoopMaybeArrayType(const uast::BracketLoop* node) {
  if (!node->isExpressionLevel()) return false;
  if (node->iterand()->isZip()) return false;
  if (node->numStmts() != 1) return false;
  if (node->index() && node->stmt(0)->isConditional()) return false;
  return true;
}

// TODO: Attributes

struct ChplSyntaxVisitor {
  std::stringstream ss_;
  int indentDepth = 0;
  const int spacesPerIndentDepth = 2;


  void indentStream() {
    int i = indentDepth;
    while (i > 0) {
      for (int j=0; j<spacesPerIndentDepth; j++){
        ss_ << " ";
      }
      i--;
    }
  }

  void printAst(const AstNode* node) {
    node->dispatch<void>(*this);
  }

  ChplSyntaxVisitor(std::ostringstream ss_ = std::ostringstream()) {}

  /**
   * visit each elt of begin..end, outputting `separator` between each.
   * if not null, `surroundBegin` and `surroundEnd` are output before
   * and after respectively
  */
  template<typename It>
  void interpose(It begin, It end, const char* separator,
                const char* surroundBegin=nullptr,
                const char* surroundEnd=nullptr,
                const char* terminator=nullptr) {
    bool first = true;
    if (surroundBegin) ss_ << surroundBegin;
    for (auto it = begin; it != end; it++) {
      if (!first && !it->isComment()) ss_ << separator;
      if (separator[0]=='\n' ) indentStream();
      printAst(*it);
      if (terminator && isExpressionRequiringSemi(*it))
        ss_ << terminator;
      first = false;
    }
    if (surroundEnd) {
      if (surroundEnd[0]=='\n') {
        ss_ << surroundEnd[0];
        indentDepth--;
        indentStream();
        indentDepth++;
        ss_ << ++surroundEnd;
      }
      else
        ss_ << surroundEnd;
    }
  }

  template<typename T>
  void interpose(T xs, const char* separator,
                const char* surroundBegin=nullptr,
                const char* surroundEnd=nullptr,
                const char* terminator=nullptr) {
    interpose(xs.begin(), xs.end(), separator, surroundBegin, surroundEnd, terminator);
  }

  /*
   * helper for printing descendants of simpleBlockLike to handle when to print
   * the optional opening keyword and if braces should follow that keyword
  */
  template<typename It>
  void printBlockWithStyle(BlockStyle style,  It begin, It end,
                          const char* implicitOpeningKeyword=nullptr,
                          const char* terminator=nullptr) {
    if (implicitOpeningKeyword) {
      ss_ << implicitOpeningKeyword;
    }
    if (style != BlockStyle::IMPLICIT) {
      interpose(begin, end, "\n", "{\n","\n}", terminator);
    } else {
      if (implicitOpeningKeyword) {
        ss_ << "\n";
        interpose(begin, end, "\n", nullptr, nullptr, terminator);
      } else {
        interpose(begin, end, "", nullptr, nullptr, terminator);
      }


    }
  }

  template<typename T>
  void printBlockWithStyle(BlockStyle style,  T xs,
                          const char* implicitOpeningKeyword=nullptr,
                          const char* terminator=nullptr) {
    printBlockWithStyle(style, xs.begin(), xs.end(), implicitOpeningKeyword, terminator);
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
   * reserved word. Helps FnCalls not to print () in this case
  */
  bool isCalleeReservedWord(const AstNode* callee) {
    if (callee->isIdentifier() &&
      (callee->toIdentifier()->name() == USTR("borrowed")
       || callee->toIdentifier()->name() == USTR("owned")
       || callee->toIdentifier()->name() == USTR("unmanaged")
       || callee->toIdentifier()->name() == USTR("shared")
       || callee->toIdentifier()->name() == USTR("sync")
       || callee->toIdentifier()->name() == USTR("single")))
        return true;
      return false;
  }

  bool isExpressionRequiringSemi(const AstNode* node) {
    return    !node->isCatch()
           && !node->isClass()
           && !node->isCobegin()
           && !node->isComment()
           && !node->isConditional()
           && !node->isFunction()
           && !node->isIndexableLoop()
           && !node->isLabel()
           && !node->isLoop()
           && !node->isModule()
           && !node->isRecord()
           && !node->isSelect()
           && !node->isSimpleBlockLike()
           && !node->isSync()
           && !node->isTry()
           && !node->isUnion()
           && !node->isTry();
  }

  bool isPostfix(const OpCall* op) {
    return (op->isUnaryOp() &&
            (op->op() == USTR("postfix!") || op->op() == USTR("?")));
  }

  void printFunctionHelper(const Function* node) {
     // storage kind
    if (node->thisFormal() != nullptr
        && node->thisFormal()->storageKind() != IntentList::DEFAULT_INTENT) {
      ss_ << kindToString(node->thisFormal()->storageKind()) <<" ";
    }

    // print out the receiver type for secondary methods
    if (node->isMethod() && !node->isPrimaryMethod()) {
      auto typeExpr = node->thisFormal()->typeExpression();
      assert(typeExpr);

      if (auto ident = typeExpr->toIdentifier()) {
        ss_ << ident->name().str();
      } else {
        ss_ << "(";
        typeExpr->dispatch<void>(*this);
        ss_ << ")";
      }

      ss_ << ".";
    }

    if (node->kind() == Function::Kind::OPERATOR && node->name() == USTR("=")) {
      // TODO: remove this once the old parser is out of the question
      // TODO: this is only here to support tests from the old parser
      // printing extra spaces around an assignment operator
      ss_ << " ";
      // Function Name
      ss_ << node->name().str();
      ss_ << " ";
    } else {
      // Function Name
      ss_ << node->name().str();
    }

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
    indentDepth++;
    printBlockWithStyle(node->blockStyle(), node->stmts(), nullptr, ";");
    indentDepth--;
  }

  void visit(const Block* node) {
    indentDepth++;
    printBlockWithStyle(node->blockStyle(), node->stmts(), nullptr,";");
    indentDepth--;
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
    if (isBracketLoopMaybeArrayType(node) &&
        node->iterand()->isDomain() &&
        node->iterand()->toDomain()->numExprs() == 1) {
        printChapelSyntax(ss_, node->iterand()->toDomain()->expr(0));
    } else {
      printChapelSyntax(ss_, node->iterand());
    }

    if (node->withClause()) {
      ss_<< " ";
      printChapelSyntax(ss_, node->withClause());
    }
    ss_ << "]";
    if (node->numStmts() > 0) {
      ss_ << " ";
      if (node->blockStyle()==BlockStyle::EXPLICIT) {
        indentDepth++;
        printBlockWithStyle(node->blockStyle(), node->stmts(), nullptr,";");
        indentDepth--;
      } else {
        interpose(node->stmts(), "");
        if (node->stmt(0)->isOpCall() && !node->isExpressionLevel())
          ss_ << ";";
      }
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
    interpose(node->stmts(), "\n","{\n", "\n}", ";");
  }

  void visit(const Class* node) {
    ss_ << "class ";
    ss_ << node->name() << " ";
    if (node->parentClass() != nullptr) {
      ss_ << ": ";
      printAst(node->parentClass());
      ss_ << " ";
    }
    indentDepth++;
    interpose(node->decls(), "\n", "{\n", "\n}", ";");
    indentDepth--;
  }

  void visit(const Cobegin* node) {
    ss_ << "cobegin ";
    if (node->withClause()) {
      printChapelSyntax(ss_, node->withClause());
      ss_<< " ";
    }
    indentDepth++;
    interpose(node->taskBodies(), "\n", "{\n","\n}", ";");
    indentDepth--;
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
    indentDepth++;
    printBlockWithStyle(node->blockStyle(), node->stmts(), "do ", ";");
    indentDepth--;
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
    if (node->isExpressionLevel()) {
      ss_ << "then ";
      printAst(node->thenStmt(0));
    } else {
      if (node->thenBlockStyle() == BlockStyle::IMPLICIT && !node->hasElseBlock()) {
        ss_ << "then ";
        printAst(node->thenStmt(0));
        ss_ << ";";
      } else {
        indentDepth++;
        printBlockWithStyle(node->thenBlockStyle(), node->thenStmts(), "then ", ";");
        indentDepth--;
      }

    }
    if (node->hasElseBlock()) {
      ss_ << " ";
      if (node->isExpressionLevel()) {
        ss_ << "else ";
        printAst(node->elseStmt(0));
      } else {
        ss_ << "\n";
        indentStream();
        indentDepth++;
        printBlockWithStyle(node->elseBlockStyle(), node->elseStmts(), "else ", ";");
        indentDepth--;
      }
    }
  }

  void visit(const Continue* node) {
    ss_ << "continue";
    if (auto ident = node->target()) {
      printChapelSyntax(ss_, ident);
    }
  }

  void visit(const CStringLiteral* node) {
    ss_ << "c\"" << quoteStringForC(std::string(node->str().c_str())) << '"';
  }

  void visit(const Defer* node) {
    ss_ << "defer ";
    printBlockWithStyle(node->blockStyle(), node->stmts(), nullptr, ";");
  }

  void visit(const Delete* node) {
    ss_ << "delete ";
    interpose(node->exprs(), ", ");
  }

  void visit(const Domain* node) {
    if (node->numExprs() == 1 && !node->expr(0)->isRange()) {
      if (auto op = node->expr(0)->toOpCall()) {
        ss_ << "(";
        printChapelSyntax(ss_, op);
        ss_ << ")";
      } else {
        printChapelSyntax(ss_, node->expr(0));
      }
    } else if (node->numExprs() == 0) {
      // do nothing
    } else {
      if (node->expr(0)->isOpCall()) {
        interpose(node->exprs(), ", ", "[", "]");
      } else {
        interpose(node->exprs(), ", ", "{", "}");
      }
    }
  }

  void visit(const Dot* node) {
    printChapelSyntax(ss_, node->receiver());
    ss_ << "." << node->field();
  }

  void visit(const DoWhile* node) {
    ss_ << "do ";
    indentDepth++;
    printBlockWithStyle(node->blockStyle(), node->stmts(), nullptr, ";");
    indentDepth--;
    ss_ << " while ";
    printChapelSyntax(ss_, node->condition());
    ss_ << ";";
  }

  void visit(const EmptyStmt* node) {
    //ss_ << ";";
    // do nothing, let terminator handle?
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
    if (isCalleeReservedWord(callee)) {
      ss_ << " ";
      printChapelSyntax(ss_, node->actual(0));
    } else {
      if (node->callUsedSquareBrackets()) {
        ss_ << "[";
      } else {
        ss_ << "(";
      }
      std::string sep;
      for (int i = 0; i < node->numActuals(); i++) {
        ss_ << sep;
        if (node->isNamedActual(i)) {
          ss_ << node->actualName(i);
          // The spaces around = are just to satisfy old tests
          // TODO: Remove spaces around `=` when removing old parser
          ss_ << " = ";
        }

        printChapelSyntax(ss_, node->actual(i));

        sep = ", ";
      }
      if (node->callUsedSquareBrackets()) {
        ss_ << "]";
      } else {
        ss_ << ")";
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
    if (node->isExpressionLevel()) {
      ss_ << "do ";
      printAst(node->stmt(0));
    } else {
      indentDepth++;
      if (node->blockStyle() == BlockStyle::EXPLICIT) {
        printBlockWithStyle(node->blockStyle(), node->stmts(), nullptr, ";");
      } else {
        printBlockWithStyle(node->blockStyle(), node->stmts(), "do ", ";");
      }
      indentDepth--;
    }
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
    if (node->isExpressionLevel()) {
     ss_ << "do ";
     printAst(node->stmt(0));
    } else {
      indentDepth++;
      printBlockWithStyle(node->blockStyle(), node->stmts(), "do ", ";");
      indentDepth--;
    }
  }

  void visit(const Foreach* node) {
    ss_ << "foreach ";

    if (node->index()) {
      printChapelSyntax(ss_, node->index());
      ss_ << " in ";
    }
    printChapelSyntax(ss_, node->iterand());
    ss_ << " ";
    indentDepth++;
    printBlockWithStyle(node->blockStyle(), node->stmts(), "do ", ";");
    indentDepth--;
  }

  void visit(const Formal* node) {
    if (node->attributes()) ss_ << pragmaFlagsToString(node);

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
    if (node->linkage() != chpl::uast::Decl::EXTERN || node->numStmts() > 0 ) {
      indentDepth++;
      interpose(node->stmts(), "\n", "{\n", "\n}", ";");
      indentDepth--;
    }
    else
      ss_ << ";";
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
    printAst(node->loop());
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
    indentDepth++;
    printBlockWithStyle(node->blockStyle(), node->stmts(), "do ", ";");
    indentDepth--;
  }

  void visit(const Manage* node) {
    ss_ << "manage ";
    interpose(node->managers(), ", ");
    ss_ << " ";
    indentDepth++;
    printBlockWithStyle(node->blockStyle(), node->stmts(), "do ", ";");
    indentDepth--;
  }

  void visit(const Module* node) {
    if (node->visibility() != Decl::Visibility::DEFAULT_VISIBILITY) {
      ss_ << kindToString(node->visibility()) << " ";
    }
    if (node->kind() != Module::Kind::DEFAULT_MODULE_KIND && node->kind() != Module::Kind::IMPLICIT) {
      ss_ << kindToString(node->kind()) << " ";
    }
    ss_ << "module ";
    ss_ << node->name() << " ";
    indentDepth++;
    interpose(node->stmts(), "\n", "{\n", "\n}\n",";");
    indentDepth--;
  }

  void visit(const MultiDecl* node) {
    bool isConfig = false;
    std::string kind;
    for (auto decl : node->decls()) {
      if (!decl->isComment()) {
        if (decl->isVariable() ) {
          if (decl->toVariable()->isConfig())
            isConfig = true;
          kind = kindToString((IntentList)decl->toVariable()->kind());
        }
        else if (decl->isTupleDecl()) {

        }
      }
    }
    if (isConfig) {
      ss_ << "config ";
    } else {
      printLinkage(node);
    }

    ss_ << kind << " ";

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
    indentDepth++;
    printBlockWithStyle(node->blockStyle(), node->stmts(), "do ", ";");
    indentDepth--;
  }

  void visit(const OpCall* node) {
    bool needsParens = false;
    std::string outerOp, innerOp;
    if (node->isUnaryOp()) {
      bool isPostFixBang = false;
      bool isNilable = false;
      outerOp = node->op().str();
      if (node->op() == USTR("postfix!")) {
        isPostFixBang = true;
        outerOp = "!";
      } else if (node->op() == USTR("?")) {
        isNilable = true;
        outerOp = "?";
      } else {
        ss_ << node->op();
      }
      assert(node->numActuals() == 1);
      if (node->actual(0)->isOpCall()) {
        innerOp = node->actual(0)->toOpCall()->op().str();
        needsParens = needParens(outerOp.c_str(), innerOp.c_str(), true,
                                 (isPostFixBang || isNilable),
                                 node->actual(0)->toOpCall()->isUnaryOp(),
                                 isPostfix(node->actual(0)->toOpCall()) , false);
      }
      if (needsParens) ss_ << "(";
      printChapelSyntax(ss_, node->actual(0));
      if (needsParens) ss_ << ")";
      if (isPostFixBang) {
        ss_ << "!";
      } else if (isNilable) {
        ss_ << "?";
      }
    } else if (node->isBinaryOp()) {
      assert(node->numActuals() == 2);
      outerOp = node->op().str();
      if (node->actual(0)->isOpCall()) {
        innerOp = node->actual(0)->toOpCall()->op().str();
        needsParens = needParens(outerOp.c_str(), innerOp.c_str(), false, false,
                                 node->actual(0)->toOpCall()->isUnaryOp(),
                                 isPostfix(node->actual(0)->toOpCall()), false);
      }
      if (needsParens) ss_ << "(";
      printChapelSyntax(ss_, node->actual(0));
      if (needsParens) ss_ << ")";
      needsParens = false;
      if (node->op() == USTR("by"))
        ss_ << " ";
      ss_ << node->op();
      if (node->op() == USTR("by"))
        ss_ << " ";
      if (node->actual(1)->isOpCall()) {
        innerOp = node->actual(1)->toOpCall()->op().str();
        needsParens = needParens(outerOp.c_str(), innerOp.c_str(), false,
                                 false, node->actual(1)->toOpCall()->isUnaryOp(),
                                 isPostfix(node->actual(1)->toOpCall()) , true);
      }
      if (needsParens) ss_ << "(";
      printChapelSyntax(ss_, node->actual(1));
      if (needsParens) ss_ << ")";
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
    indentDepth++;
    interpose(node->decls(), "\n", "{\n", "\n}",";");
    indentDepth--;
  }

  void visit(const Reduce* node) {
    if (node->opExpr()) {
      printChapelSyntax(ss_, node->opExpr());
    } else {
      ss_ << node->op();
    }
    ss_ << " ";
    ss_ << "reduce ";
    if (node->actual(0)->isOpCall()) ss_<<"(";
    printChapelSyntax(ss_, node->actual(0));
    if (node->actual(0)->isOpCall()) ss_<<")";
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
    printChapelSyntax(ss_, node->op());
    ss_ << " scan ";
    printChapelSyntax(ss_, node->iterand());
  }

  void visit(const Select* node) {
    ss_ << "select ";
    printChapelSyntax(ss_, node->expr());
    ss_ << " ";
    indentDepth++;
    interpose(node->whenStmts(), "\n", "{\n", "\n}", ";");
    indentDepth--;
  }

  void visit(const Serial* node) {
    ss_ << "serial ";
    if (node->condition()) {
      printChapelSyntax(ss_, node->condition());
      ss_ << " ";
    }
    indentDepth++;
    printBlockWithStyle(node->blockStyle(), node->stmts(), "do ", ";");
    indentDepth--;
  }

  void visit(const StringLiteral* node) {
    ss_ << '"' << quoteStringForC(std::string(node->str().c_str())) << '"';
  }

  void visit(const Sync* node) {
    ss_ << "sync ";
    indentDepth++;
    printBlockWithStyle(node->blockStyle(), node->stmts(), nullptr, ";");
    indentDepth--;
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
    if (node->isExpressionLevel()) {
      printAst(node->stmt(0));
    } else {
      indentDepth++;
      interpose(node->stmts(), "\n", "{\n","\n}", ";");
      indentDepth--;
    }
    // if try block has catch blocks
    if (!node->isTryBang()) {
      ss_ << " ";
      indentDepth++;
      interpose(node->handlers(), " ");
      indentDepth--;
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
    indentDepth++;
    interpose(node->decls(), "\n", "{\n", "\n}", ";");
    indentDepth--;
  }

  void visit(const Use* node) {
    if (node->visibility() != Decl::DEFAULT_VISIBILITY) {
      ss_ << kindToString(node->visibility());
      ss_ << " ";
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

    ss_ << " ...";
    if (const AstNode* ce = node->count()) {
      printChapelSyntax(ss_, ce);
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
    } else if (limit == VisibilityClause::LimitationKind::NONE &&
               node->numLimitations() == 1) {
      assert(node->limitation(0)->isIdentifier());
      ss_ << ".";
      printChapelSyntax(ss_, node->limitation(0));
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
    indentDepth++;
    printBlockWithStyle(node->blockStyle(), node->stmts(), "do ", ";");
    indentDepth--;
  }

  void visit(const While* node) {
    ss_ << "while ";
    printChapelSyntax(ss_, node->condition());
    ss_ << " ";
    indentDepth++;
    printBlockWithStyle(node->blockStyle(), node->stmts(), "do ", ";");
    indentDepth--;
  }

  void visit(const WithClause* node) {
    ss_ << "with ";
    interpose(node->exprs(), ", ", "(", ")");
  }

  void visit(const Yield* node) {
    ss_ << "yield ";
    printAst(node->value());
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
    // os goes into fail state
    // see: https://en.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt
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
    // os goes into fail state
    // see: https://en.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt
    // check for failbit and reset
    os << visitor.ss_.rdbuf();
    if (os.fail()) {
      os.clear();
    }
    os.flush();
  }

  /*
   * Operator precedence according to the table in the spec,
   * expressions.rst
   *
   * unary flag is needed because unary - (and +) have higher precedence
   * than binary - (and +).
   *
   * postfix flag is needed because postfix ! has higher precedence than
   * prefix !.
   *
   * Returns precedence: higher is tighter-binding.
   * Returns -1 for unhandled operator -- caller should respond conservatively.
   *
   * author - Paul Cassella (@cassella)
   */
   int opToPrecedence(UniqueString op, bool unary, bool postfix) {
    // new is precedence 19, but doesn't come through this path.
    if (postfix && (USTR("?") == op || USTR("!") == op))
      return 18;
    else if (USTR(":") == op)
      return 17;
    else if (USTR("**") == op)
      return 16;
    // reduce/scan/dmapped are precedence 15, but don't come through this path.
    else if (USTR("!") == op || USTR("~") == op)
      return 14;
    else if (USTR("*") == op ||
             USTR("/") == op || USTR("%") == op)
      return 13;
    else if (unary &&
             (USTR("+") == op || USTR("-") == op))
      return 12;
    else if (USTR("<<") == op || USTR(">>") == op)
      return 11;
    else if (USTR("&") == op)
      return 10;
    else if (USTR("^") == op)
      return 9;
    else if (USTR("|") == op)
      return 8;
    else if (USTR("+") == op || USTR("-") == op)
      return 7;
    // .. and ..< are precedence 6, but don't come through this path.
    else if (USTR("<") == op || USTR("<=") == op ||
             USTR(">") == op || USTR(">=") == op ||
             USTR("<") == op)
      return 5;
    else if (USTR("==") == op || USTR("!=") == op)
      return 4;
    else if (USTR("&&") == op)
      return 3;
    else if (USTR("||") == op)
      return 2;
    // by and align are precedence 1 too, but don't come through this path.
    else if (USTR("#") == op)
      return 1;

    return -1;
  }

  /*
   * needParens(outer, inner, ...) is called to evaluate whether we need
   * to add parens around the inner op.  We're here when
   * appendExpr(outer) is calling appendExpr(expr->get(1)) or
   * expr->get(2), which has become appendExpr(inner).
   *
   * Given an AST node outer, with one child inner, this function
   * returns true if we need to emit parens around the child expression.
   * That is, if emitting the expression without parenthesis would
   * change the semantics from what the AST represents.
   *
   * If the inner (child) operator has higher precedence than the outer
   * (parent), then we don't need parens, as emitting
   * "a outer_op b inner_op c" is equivalent to
   * "a outer_op (b inner_op c)".
   *
   * If the child operator has equal precedence to the outer, then we
   * generally don't need parenthesis, except for a few exceptions.  If
   * the desired expression is a-(b-c) or a-(b+c), then the parentheses
   * are needed.  Of course they're not needed for a+(b-c) or a+(b+c).
   * If the desired expression is a/(b/c) or a/(b*c) or a%(b/c), they're
   * needed, but not for a*(b*c) or a*(b/c).
   * (TODO: a*(b/c) might need the parens for overflow reasons.)
   *
   * Unary - (and +) have higher precedence than binary - (and +), so we
   * need the unary flag to know which case we're in.
   *
   * The postfix flag is needed because postfix ! has higher precedence
   * than prefix !.
   *
   * The innerIsRHS flag is needed because we need parens to express
   * a-(b-c) where inner- is the RHS of the outer-.  But we can emit
   * a-b-c instead of (a-b)-c -- when inner- is the LHS of outer-.
   * Also, to distinguish (-1)**2 (parens needed) from 1**(-2) (not needed).
   *
   * author - Paul Cassella (@cassella)
   */
  bool needParens(UniqueString outer, UniqueString inner,
                        bool outerUnary, bool outerPostfix,
                        bool innerUnary, bool innerPostfix,
                        bool innerIsRHS) {
    bool ret = false;
    int outerprec, innerprec;

    if (outer.isEmpty())
      return false;

    outerprec = opToPrecedence(outer, outerUnary, outerPostfix);
    innerprec = opToPrecedence(inner, innerUnary, innerPostfix);

    // -1 means opToPrecedence wasn't expecting one of these operators.
    // Conservatively wrap parentheses around the representation of this
    // AST node.
    if (outerprec == -1 || innerprec == -1)
      return true;

    // We never need parens around the unary expression on the RHS:
    // 1**-2 vs 1**(-2)
    if (innerUnary && innerIsRHS)
      return false;

    if (outerprec > innerprec)
      ret = true;

    // If inner and outer have the same precedence, and inner is the
    // rhs, it needs parens if a op1 (b op2 c) isn't equivalent to
    // a op1 b op2 c.  (Note op1 and op2 may be the same op, a - (b - c))
    if (innerIsRHS &&
        (USTR("-") == outer ||
         USTR("/") == outer ||  USTR("%") == outer ||
         USTR("<<") == outer ||  USTR(">>") == outer ||
         // (a==b)==true vs. a==(b==true)
          USTR("==") == outer ||  USTR("!=") == outer)
        && outerprec == innerprec)
      ret = true;

    // ** is right-associative, and a**(b**c) != (a**b)**c.
    if (!innerIsRHS &&  USTR("**") == outer && outerprec == innerprec)
      ret = true;

    return ret;
  }

  /*
   * Do we want to print spaces around this binary operator?
   */
   bool wantSpaces(UniqueString op, bool printingType)
  {
    if (USTR("**") == op)
      return false;
    if (printingType)
      return false;
    return true;
  }


} // end chpl namespace
