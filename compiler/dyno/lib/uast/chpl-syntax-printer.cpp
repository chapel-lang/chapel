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



namespace chpl {

  using namespace uast;

  static const char* kindToString(TaskVar::Intent kind) {
    switch (kind) {
      case TaskVar::Intent::VAR: return "var";
      case TaskVar::Intent::CONST: return "const";
      case TaskVar::Intent::CONST_REF: return "const ref";
      case TaskVar::Intent::REF: return "ref";
      case TaskVar::Intent::IN: return "in";
      case TaskVar::Intent::CONST_IN: return "const in";
    }
    assert(false);
  }

  static const char* kindToString(VisibilityClause::LimitationKind kind) {
    switch (kind) {
      case VisibilityClause::LimitationKind::NONE: assert(false);
      case VisibilityClause::LimitationKind::ONLY: return "only";
      case VisibilityClause::LimitationKind::EXCEPT: return "except";
      case VisibilityClause::LimitationKind::BRACES: assert(false);
      default: return "";
    }
  }

  static const char* kindToString(IntentList kind) {
    switch (kind) {
      case IntentList::DEFAULT_INTENT: assert(false);
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
      default: return "";
    }
    return "";
  }

  static const char* kindToString(Decl::Linkage kind) {
    switch (kind) {
      case Decl::Linkage::DEFAULT_LINKAGE: assert(false);
      case Decl::Linkage::EXTERN: return "extern";
      case Decl::Linkage::EXPORT: return "export";
    }
    assert(false);
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

  static const char* kindToString(Module::Kind kind) {
    switch (kind) {
      case Module::Kind::DEFAULT_MODULE_KIND: assert(false);
      case Module::Kind::IMPLICIT: return "";
      case Module::Kind::PROTOTYPE: return "prototype";
    }
    assert(false);
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

  static const char* kindToString(TupleDecl::IntentOrKind kind) {
    switch(kind) {
      case TupleDecl::IntentOrKind::DEFAULT_INTENT: assert(false);
      case TupleDecl::IntentOrKind::CONST_INTENT: return "const";
      case TupleDecl::IntentOrKind::VAR: return "var";
      case TupleDecl::IntentOrKind::CONST_VAR: return "const var";
      case TupleDecl::IntentOrKind::CONST_REF: return "const ref";
      case TupleDecl::IntentOrKind::REF: return "ref";
      case TupleDecl::IntentOrKind::IN: return "in";
      case TupleDecl::IntentOrKind::CONST_IN: return "const in";
      case TupleDecl::IntentOrKind::OUT: return "out";
      case TupleDecl::IntentOrKind::INOUT: return "inout";
      case TupleDecl::IntentOrKind::INDEX: assert(false);
      case TupleDecl::IntentOrKind::PARAM: return "param";
      case TupleDecl::IntentOrKind::TYPE: return "type";
    }
    assert(false);
  }


  /** visit each elt of begin..end, outputting `separator` between each.
   * `surroundBegin` and `surroundEnd` are output before and after respectively
   * if not null */
  template<typename It>
  void interpose(std::ostream& ss_, It begin, It end, const char* separator, const char* surroundBegin=nullptr, const char* surroundEnd=nullptr) {
    bool first = true;
    if (surroundBegin) ss_ << surroundBegin;
    for (auto it = begin; it != end; it++) {
      if (!first) ss_ << separator;
      printAst(ss_,*it);
      first = false;
    }
    if (surroundEnd) ss_ << surroundEnd;
  }

  template<typename T>
  void interpose(std::ostream& ss_, T xs, const char* separator, const char* surroundBegin=nullptr, const char* surroundEnd=nullptr) {
    interpose(ss_, xs.begin(), xs.end(), separator, surroundBegin, surroundEnd);
  }

  template<typename T>
  void visitLiteral(std::ostream& ss_, const T* node) {
    ss_ << node->text().c_str();
  }

  // TODO: Nesting
  // TODO: Semicolons
  // TODO: Newlines


  struct ChplSyntaxVisitor {
    std::stringstream ss_;


    void printLinkages(const Decl* node) {
      if (node->linkage() != Decl::Linkage::DEFAULT_LINKAGE) {
        ss_ << kindToString(node->linkage()) << " ";
        if (node->linkageName() != nullptr) {
          printAst(ss_, node->linkageName());
          ss_ << " ";
        }
      }
    }

    void visit(const uast::ASTNode* node) {
      ss_ << tagToString(node->tag());
    }

    void visit(const Array* node) {
      interpose(ss_, node->children(), ", ", "[", "]");
    }

    void visit(const As* node) {
      printAst(ss_, node->symbol());
      ss_ << " as ";
      printAst(ss_, node->rename());
    }

    //Attributes

    void visit(const Begin* node) {
      ss_ << "begin ";
      if (node->withClause()) {
        printAst(ss_, node->withClause());
        ss_ << " ";
      }
      if (node->blockStyle() == BlockStyle::IMPLICIT) {
        interpose(ss_, node->stmts(), "\n");
      } else {
        interpose(ss_, node->stmts(), "\n", "{", "}");
      }
    }

    void visit(const Block* node) {
      interpose(ss_, node->stmts(), " ", "{","}");
    }

    void visit(const BoolLiteral* node) {
      ss_ << (node->value() ? "true" : "false");
    }

    void visit(const BracketLoop* node) {
      ss_ << "[";
      if (node->index()) {
        printAst(ss_, node->index());
        ss_ << " in ";
      }
      printAst(ss_, node->iterand());
      if (node->withClause()) {
        ss_<< " ";
        printAst(ss_, node->withClause());
      }
      ss_ << "]";
      if (node->numStmts() > 0) {
        ss_ << " ";
        interpose(ss_, node->stmts(), "");
      }
    }

    void visit(const Break* node) {
      ss_ << "break";
      if (node->target()) {
        ss_<< " ";
        printAst(ss_, node->target());
      }
      ss_ << ";";
    }

    //builder
    //builderResult

    void visit(const BytesLiteral* node) {
      ss_ << "b\"" << quoteStringForC(std::string(node->str().c_str())) << '"';
    }

    void visit(const Catch* node) {
      ss_ << "catch ";
      if (node->error()){
        if (node->hasParensAroundError()) {
          ss_ << "(";
          ss_ << node->error()->name().c_str();
          if (const Expression* te = node->error()->toVariable()->typeExpression()) {
            ss_ << " : ";
            printAst(ss_, te);
          }
          ss_ << ") ";
        } else {
          ss_ << node->error()->name().c_str();
          if (const Expression* te = node->error()->toVariable()->typeExpression()) {
            ss_ << " : ";
            printAst(ss_, te);
          }
          ss_ << " ";
        }
      }
      interpose(ss_, node->stmts(), "\n","{", "}");
    }

    void visit(const Class* node) {
      ss_ << "class ";
      ss_ << node->name().c_str() << " ";
      if (node->parentClass() != nullptr) {
        ss_ << ": ";
        printAst(ss_, node->parentClass());
        ss_ << " ";
      }
      interpose(ss_, node->decls(), "\n", "{", "}");
    }

    void visit(const Cobegin* node) {
      ss_ << "cobegin ";
      if (node->withClause()) {
        printAst(ss_, node->withClause());
        ss_<< " ";
      }
      interpose(ss_, node->taskBodies(), "\n", "{","}");
    }

    void visit(const Coforall* node) {
      ss_ << "coforall ";

      if (node->index()) {
        printAst(ss_, node->index());
        ss_ << " in ";
      }
      printAst(ss_, node->iterand());
      if (node->withClause()) {
        ss_<< " ";
        printAst(ss_, node->withClause());
      }
      ss_ << " ";
      if (node->blockStyle() == BlockStyle::IMPLICIT || node->blockStyle() == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK) {
        ss_ << "do ";
      }
      if (node->blockStyle() == BlockStyle::EXPLICIT || node->blockStyle() == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK) {
        interpose(ss_, node->stmts(), "\n", "{","}");
      } else {
        interpose(ss_, node->stmts(), "\n");
      }
    }

    void visit(const Comment* node){
      // TODO: Discuss if we want to provide comments back
      ss_ << node->c_str();
    }

    void visit(const Conditional* node) {
      ss_ << "if ";
      printAst(ss_, node->condition());
      ss_ << " ";
      if (node->thenBlockStyle() == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK ||
          node->thenBlockStyle() == BlockStyle::IMPLICIT) {
        ss_ << "then ";
      }
      printAst(ss_, node->thenBlock());
      if (node->hasElseBlock()) {
        ss_ << " else ";
        printAst(ss_, node->elseBlock());
      }
    }

    void visit(const CStringLiteral* node) {
      ss_ << "c\"" << quoteStringForC(std::string(node->str().c_str())) << '"';
    }

    void visit(const Defer* node) {
      ss_ << "defer ";
      const BlockStyle style = node->blockStyle();
      if (style == BlockStyle::EXPLICIT || style == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK) {
        interpose(ss_, node->stmts(), "\n", "{","}");
      } else {
        interpose(ss_, node->stmts(), "\n");
      }
    }

    void visit(const Delete* node) {
      ss_ << "delete ";
      interpose(ss_, node->exprs(), ", ");
    }

    void visit(const Domain* node) {
      if (node->numExprs() == 1 && node->expr(0)->isTypeQuery()) {
        ss_ << "?";
        printAst(ss_, node->expr(0));
      } else if (node->numExprs() == 0) {
        // do nothing
      } else {
        interpose(ss_, node->exprs(), ", ", "{", "}");
      }
    }

    void visit(const Dot* node) {
      printAst(ss_, node->receiver());
      ss_ << "." << node->field().c_str();
    }

    void visit(const DoWhile* node) {
      ss_ << "do ";
      BlockStyle style = node->blockStyle();
      if (style == BlockStyle::EXPLICIT || style == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK) {
        interpose(ss_, node->stmts(), "\n", "{", "}");
      } else {
        interpose(ss_, node->stmts(),"\n");
      }
      ss_ << " while ";
      printAst(ss_, node->condition());
    }

    void visit(const Enum* node) {
      ss_ << "enum " << node->name().c_str() << " ";
      interpose(ss_, node->enumElements(), ", ", "{ ", " }");
    }

    void visit(const EnumElement* node) {
      ss_ << node->name().c_str();
      if (const Expression* ie = node->initExpression()) {
        ss_ <<  " = ";
        printAst(ss_, ie);
      }
    }

    //erroneousExpression

    void visit(const ExternBlock* node) {
      ss_ << "extern {\n";
      ss_ << node->code();
      ss_ << "}";
    }

    void visit(const FnCall* node) {
      const Expression* callee = node->calledExpression();
      if (!callee) {
        printf("ERROR %s\n", asttags::tagToString(node->tag()));
        node->stringify(std::cerr, StringifyKind::CHPL_SYNTAX);
      }
      assert(callee);  // This should be true because OpCall is handled

      printAst(ss_, callee);
      if (callee->isIdentifier() && (callee->toIdentifier()->name() == "borrowed" || callee->toIdentifier()->name() == "owned"
                                     || callee->toIdentifier()->name() == "unmanaged" || callee->toIdentifier()->name() == "shared")) {
        ss_ << " ";
        printAst(ss_, node->actual(0));
      } else {
        if (node->callUsedSquareBrackets()) {
          interpose(ss_, node->actuals(), ", ", "[", "]");
        } else {
          interpose(ss_, node->actuals(), ", ", "(", ")");
        }
      }

    }

    void visit(const For* node) {
      ss_ << "for ";
      if (node->isParam()) {
        ss_ << "param ";
      }
      if (node->index()) {
        printAst(ss_, node->index());
        ss_ << " in ";
      }
      printAst(ss_, node->iterand());
      ss_ << " ";
      if (node->blockStyle() == BlockStyle::IMPLICIT || node->blockStyle() == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK) {
        ss_ << "do ";
      }
      if (node->blockStyle() == BlockStyle::EXPLICIT || node->blockStyle() == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK) {
        interpose(ss_, node->stmts(), "\n", "{","}");
      } else {
        interpose(ss_, node->stmts(), "\n");
      }
    }

    void visit(const Forall* node) {
      ss_ << "forall ";

      if (node->index()) {
        printAst(ss_, node->index());
        ss_ << " in ";
      }
      printAst(ss_, node->iterand());
      if (node->withClause()) {
        ss_<< " ";
        printAst(ss_, node->withClause());
      }
      ss_ << " ";
      if (node->blockStyle() == BlockStyle::IMPLICIT || node->blockStyle() == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK) {
        ss_ << "do ";
      }
      if (node->blockStyle() == BlockStyle::EXPLICIT || node->blockStyle() == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK) {
        interpose(ss_, node->stmts(), "\n", "{","}");
      } else {
        interpose(ss_, node->stmts(), "\n");
      }
    }

    void visit(const Foreach* node) {
      ss_ << "foreach ";

      if (node->index()) {
        printAst(ss_, node->index());
        ss_ << " in ";
      }
      printAst(ss_, node->iterand());
      ss_ << " ";
      const BlockStyle style = node->blockStyle();
      if (style == BlockStyle::IMPLICIT || style == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK) {
        ss_ << "do ";
      }
      if (style == BlockStyle::EXPLICIT || style == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK) {
        interpose(ss_, node->stmts(), "\n", "{","}");
      } else {
        interpose(ss_, node->stmts(), "\n");
      }
    }

    void visit(const Formal* node) {
      if (node->intent() != Formal::DEFAULT_INTENT) {
        ss_ << kindToString(node->intent()) << " ";
      }
      ss_ << node->name().str();
      if (const Expression* te = node->typeExpression()) {
        ss_ << ": ";
        printAst(ss_, te);
      }
      if (const Expression* ie = node->initExpression()) {
        ss_ << " = ";
        printAst(ss_, ie);
      }
    }

    void visit(const ForwardingDecl* node) {
      ss_ << "forwarding ";
      if (node->expr()) {
        printAst(ss_, node->expr());
      }
    }

    void visit(const Function* node) {

      printLinkages(node);

      if (node->visibility() != Function::Visibility::DEFAULT_VISIBILITY) {
        ss_ << kindToString(node->visibility()) << " ";
      }

      if (node->isOverride()) {
        ss_ << "override ";
      } else if (node->isInline()) {
        ss_ << "inline ";
      }

      // Function Name
      ss_ << kindToString(node->kind()) << " " << node->name().str();

      // Formals
      int numThisFormal = node->thisFormal() ? 1 : 0;
      int nFormals = node->numFormals() - numThisFormal;
      if (nFormals == 0 && node->isParenless()) {
        // pass
      } else if (nFormals == 0) {
        ss_ << "()";
      } else {
        auto it = node->formals();
        interpose(ss_, it.begin() + numThisFormal, it.end(), ", ", "(", ")");
      }

      // Return type
      if (const Expression* e = node->returnType()) {
        ss_ << " : ";
        printAst(ss_, e);
      }

      // Return Intent
      if (node->returnIntent() != Function::ReturnIntent::DEFAULT_RETURN_INTENT) {
        ss_ << " " << kindToString(node->returnIntent());
      }

      interpose(ss_, node->stmts(), "\n", " {", "}");

    }

    void visit(const Identifier* node) {
      ss_ << node->name().str();
    }

    void visit(const ImagLiteral* node) { return visitLiteral(ss_, node); }

    void visit(const Import* node) {
      if (node->visibility() != Decl::Visibility::DEFAULT_VISIBILITY) {
        ss_ << kindToString(node->visibility());
      }
      ss_ << "import ";
      interpose(ss_, node->visibilityClauses(), ", ");
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
      ss_ << node->name().c_str();
    }

    void visit(const IntLiteral* node)  { return visitLiteral(ss_, node); }

    void visit(const Label* node) {
      ss_ << "label ";
      ss_ << node->name().c_str() << " ";
      printAst(ss_, node->loop());
    }

    //let

    void visit(const Local* node) {
      ss_ << "local ";
      if (node->condition()) {
        printAst(ss_, node->condition());
        ss_ << " ";
      }
      BlockStyle style = node->blockStyle();
      if (style == BlockStyle::IMPLICIT || style == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK) {
        ss_ << "do ";
      }
      if (style == BlockStyle::EXPLICIT || style == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK) {
        interpose(ss_, node->stmts(), "\n", "{","}");
      } else {
        interpose(ss_, node->stmts(), "\n");
      }
    }

    //manage

    void visit(const Module* node) {
      if (node->visibility() != Decl::Visibility::DEFAULT_VISIBILITY) {
        ss_ << kindToString(node->visibility()) << " ";
      }
      if (node->kind() != Module::Kind::DEFAULT_MODULE_KIND ) {
        ss_ << kindToString(node->kind()) << " ";
      }
      ss_ << "module ";
      ss_ << node->name().c_str() << " ";
      interpose(ss_, node->stmts(), "\n", "{", "}");
    }

    void visit(const MultiDecl* node) {
      ss_ << "var ";

      // TODO: Can this be generalized between TupleDecl and MultiDecl?
      std::string delimiter = "";
      for (auto decl : node->decls()) {
        ss_ << delimiter;
        ss_ << decl->toVariable()->name().c_str();
        if (const Expression* te = decl->toVariable()->typeExpression()) {
          ss_ << ": ";
          printAst(ss_, te);
        }
        if (const Expression* ie = decl->toVariable()->initExpression()) {
          ss_ << " = ";
          printAst(ss_, ie);
        }
        delimiter = ", ";
      }

    }

    void visit(const New* node) {
      ss_ << "new ";
      if (node->management() != New::Management::DEFAULT_MANAGEMENT) {
        ss_ << kindToString(node->management()) << " ";
      }
      printAst(ss_, node->typeExpression());
    }

    void visit(const On* node) {
      ss_ << "on ";
      printAst(ss_, node->destination());
      ss_ << " ";
      BlockStyle style = node->blockStyle();
      if (style == BlockStyle::IMPLICIT || style == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK) {
        ss_ << "do ";
      }
      if (style == BlockStyle::EXPLICIT || style == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK) {
        interpose(ss_, node->stmts(), "\n", "{", "}");
      } else {
        interpose(ss_, node->stmts(),"\n");
      }
    }

    void visit(const OpCall* node) {
      // TODO: Where do parens come in?
      // ex: !(this && that)
      // is different than !this && that
      if (node->isUnaryOp()) {
        ss_ << node->op().c_str();
        assert(node->numActuals() == 1);
        printAst(ss_, node->actual(0));

      } else if (node->isBinaryOp()) {
        assert(node->numActuals() == 2);
        printAst(ss_, node->actual(0));
        ss_ << " " << node->op().c_str() << " ";
        printAst(ss_, node->actual(1));
      }
    }

    //primCall

    void visit(const Range* node) {
      if (auto lb = node->lowerBound()) {
        printAst(ss_, lb);
      }
      ss_ << "..";
      if (auto ub = node->upperBound()) {
        printAst(ss_, ub);
      }
    }

    void visit(const RealLiteral* node) { return visitLiteral(ss_, node); }

    void visit(const Record* node) {
      printLinkages(node);
      ss_ << "record ";
      ss_ << node->name().c_str() << " ";
      interpose(ss_, node->decls(), "\n", "{", "}");
    }

    //reduce

    void visit(const Require* node) {
      ss_ << "require ";
      interpose(ss_, node->exprs(), ", ");
    }

    void visit(const Return* node) {
      ss_ << "return";
      if (node->value() != nullptr) {
        ss_ << " ";
        printAst(ss_, node->value());
      }
      ss_ << ";";
    }

    //scan

    void visit(const Select* node) {
      ss_ << "select ";
      printAst(ss_, node->expr());
      ss_ << " ";
      interpose(ss_, node->whenStmts(), "\n", "{", "}");
    }

    void visit(const Serial* node) {
      ss_ << "serial ";
      if (node->condition()) {
        printAst(ss_, node->condition());
        ss_ << " ";
      }
      BlockStyle style = node->blockStyle();
      if (style == BlockStyle::IMPLICIT || style == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK) {
        ss_ << "do ";
      }
      if (style == BlockStyle::EXPLICIT || style == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK) {
        interpose(ss_, node->stmts(), "\n", "{", "}");
      } else {
        interpose(ss_, node->stmts(),"\n");
      }
    }

    void visit(const StringLiteral* node) {
      ss_ << '"' << quoteStringForC(std::string(node->str().c_str())) << '"';
    }

    void visit(const Sync* node) {
      ss_ << "sync ";
      BlockStyle style = node->blockStyle();
      if (style == BlockStyle::EXPLICIT || style == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK) {
        interpose(ss_, node->stmts(), "\n", "{", "}");
      } else {
        interpose(ss_, node->stmts(),"");
      }
    }

    void visit(const TaskVar* node) {
      ss_ << kindToString(node->intent());
      ss_ << " " << node->name().c_str();
    }

    void visit(const Throw* node) {
      ss_ << "throw ";
      printAst(ss_, node->errorExpression());
    }

    void visit(const Try* node) {
      ss_ << "try";
      if (node->isTryBang()) {
        ss_ << "! ";
      } else {
        ss_ << " ";
      }
      interpose(ss_, node->stmts(), "\n", "{","}");
      // if try block has catch blocks
      if (!node->isTryBang()) {
        ss_ << " ";
        interpose(ss_, node->handlers(), "\n");
      }
    }

    void visit(const Tuple* node) {
      interpose(ss_, node->children(), ", ", "(", ")");
    }

    void visit(const TupleDecl* node) {
      if (node->intentOrKind() != TupleDecl::IntentOrKind::DEFAULT_INTENT &&
         node->intentOrKind() != TupleDecl::IntentOrKind::INDEX) {
        ss_ << kindToString(node->intentOrKind()) << " ";
      }
      ss_ << "(";
      // TODO: Can this be generalized between TupleDecl and MultiDecl?
      std::string delimiter = "";
      for (auto decl : node->decls()) {
        ss_ << delimiter;
          ss_ << decl->toVarLikeDecl()->name().c_str();
          if (const Expression* te = decl->toVarLikeDecl()->typeExpression()) {
            ss_ << ": ";
            printAst(ss_, te);
          }
          if (const Expression* ie = decl->toVarLikeDecl()->initExpression()) {
            ss_ << " = ";
            printAst(ss_, ie);
          }
          delimiter = ", ";
        }
      ss_ << ")";


      if (const Expression* te = node->typeExpression()) {
        ss_ << ": ";
        printAst(ss_, te);
      }
      if (const Expression* ie = node->initExpression()) {
        ss_ << " = ";
        printAst(ss_, ie);
      }
    }

    void visit(const TypeDecl* node) {
      printLinkages(node);
      ss_ << "type ";
      ss_ << node->name().c_str();
    }

    void visit(const TypeQuery* node) {
      ss_ << node->name().c_str();
    }

    void visit(const UintLiteral* node) { return visitLiteral(ss_, node); }

    void visit(const Union* node) {
      printLinkages(node);
      ss_ << "union ";
      ss_ << node->name().c_str() << " ";
      interpose(ss_, node->children(), "\n", "{", "}");
    }

    void visit(const Use* node) {
      if (node->visibility() != Decl::DEFAULT_VISIBILITY) {
        ss_ << kindToString(node->visibility());
      }
      ss_ << "use ";
      interpose(ss_, node->visibilityClauses(), ", ");
    }

    //varargformal

    void visit(const Variable* node) {
      if (node->isConfig()) {
        ss_ << "config ";
      } else {
        printLinkages(node);
      }

      if (node->kind() != Variable::Kind::INDEX) {
        ss_ << kindToString(node->kind()) << " ";
      }
      ss_ << node->name().c_str();
      if (const Expression* te = node->typeExpression()) {
        ss_ << ": ";
        printAst(ss_, te);
      }
      if (const Expression* ie = node->initExpression()) {
        ss_ << " = ";
        printAst(ss_, ie);
      }
    }

    void visit(const VisibilityClause* node) {
      if (node->limitationKind() == VisibilityClause::LimitationKind::ONLY &&
          node->limitationKind() == VisibilityClause::LimitationKind::EXCEPT) {
        ss_ << kindToString(node->limitationKind());
        printAst(ss_, node->symbol());
        interpose(ss_, node->limitations(), ", ");
      } else if (node->limitationKind() == VisibilityClause::LimitationKind::BRACES) {
        printAst(ss_, node->symbol());
        interpose(ss_, node->limitations(), ", ", "{","}");
      } else { //NONE
        printAst(ss_, node->symbol());
        interpose(ss_, node->limitations(), ", ");
      }
    }

    void visit(const When* node) {
      if (node->isOtherwise()) {
        ss_ << "otherwise ";
      } else {
        ss_ << "when ";
        interpose(ss_, node->caseExprs(), ", ");
        ss_ << " ";
      }
      BlockStyle style = node->blockStyle();
      if (style == BlockStyle::IMPLICIT || style == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK) {
        ss_ << "do ";
      }
      if (style == BlockStyle::EXPLICIT || style == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK) {
        interpose(ss_, node->stmts(), "\n", "{", "}");
      } else {
        interpose(ss_, node->stmts(),"\n");
      }

    }

    void visit(const While* node) {
      ss_ << "while ";
      printAst(ss_, node->condition());
      ss_ << " ";
      BlockStyle style = node->blockStyle();
      if (style == BlockStyle::IMPLICIT || style == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK) {
        ss_ << "do ";
      }
      if (style == BlockStyle::EXPLICIT || style == BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK) {
        interpose(ss_, node->stmts(), "\n", "{", "}");
      } else {
        interpose(ss_, node->stmts(),"\n");
      }
    }

    void visit(const WithClause* node) {
      ss_ << "with ";
      interpose(ss_, node->exprs(), ", ", "(", ")");
    }

    void visit(const Yield* node) {
      ss_ << "yield ";
      printAst(ss_, node->value());
    }

    void visit(const Zip* node) {
      ss_ << "zip";
      interpose(ss_, node->actuals(),", ","(",")");
    }

  };

  /***************************************************************
   * Specializations for generating userString when converting
   * from uAST to old AST
   */

  struct UserStringVisitor {
    std::ostringstream ss_;

    void visit(const Function* node) {
      std::stringstream ss;
      if (node->visibility() != Function::Visibility::DEFAULT_VISIBILITY) {
        ss_ << kindToString(node->visibility()) << " ";
      }

      // storage kind
      if (node->thisFormal() != nullptr && node->thisFormal()->storageKind() != IntentList::DEFAULT_INTENT) {
        ss_ << kindToString(node->thisFormal()->storageKind()) <<" ";
      }
      // secondary methods
      if (node->isMethod() && !node->isPrimaryMethod() && node->thisFormal()->child(0)->isIdentifier()) {
//        assert(node->thisFormal()->numChildren() >= 1);
//        assert(node->thisFormal()->child(0)->isIdentifier());
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
        interpose(ss_, it.begin() + numThisFormal, it.end(), ", ", "(", ")");
      }

      // Return type
      if (const Expression* e = node->returnType()) {
        ss_ << ": ";
        printAst(ss_, e);
      }

      // Return Intent
      if (node->returnIntent() != Function::ReturnIntent::DEFAULT_RETURN_INTENT) {
        ss_ << " " << kindToString(node->returnIntent());
      }

    }

    void visit(const ASTNode* node) {
      ss_ << tagToString(node->tag());
    }

  };

  /* Generic printer calling the above functions */
  void printAst(std::ostream& os, const ASTNode* node) {
    auto visitor = ChplSyntaxVisitor{};
    node->dispatch<void>(visitor);
    // TODO: Avoid making copy of string here
    // challenge: when using rdbuf(), if nothing gets added to ss_, then
    // os goes into fail state -> see: https://en.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt
    os << visitor.ss_.str();
  }

  /* Generic printer calling the above functions */
  void printUserString(std::ostream& os, const Function* node) {
    auto visitor = UserStringVisitor{};
    node->dispatch<void>(visitor);
    // TODO: Avoid making copy of string here
    // challenge: when using rdbuf() as so:, if nothing gets added to ss_, then
    // os << visitor.ss_.rdbuf()
    // os goes into fail state -> see: https://en.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt
    os << visitor.ss_.str();
  }

} // end chpl namespace

