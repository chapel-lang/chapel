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
  struct ChplSyntaxVisitor {
    std::stringstream ss_;

    void visit(const uast::ASTNode* node) {
      ss_ << tagToString(node->tag());
    }

    void visit(const uast::Variable* node) {
      if (node->isConfig()) {
        ss_ << "config ";
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

    void visit(const uast::Formal* node) {
      if (node->intent() != Formal::DEFAULT_INTENT) {
        ss_ << kindToString(node->intent()) << " ";
      }
      ss_ << node->name().str();
      if (const Expression* te = node->typeExpression()) {
        ss_ << ": ";
        printAst(ss_, te);
      }
      if (const Expression* ie = node->initExpression()) {
        ss_ << "=";
        printAst(ss_, ie);
      }
    }

    void visit(const uast::Function* node) {
      std::stringstream ss;
      if (node->visibility() != Function::Visibility::DEFAULT_VISIBILITY) {
        ss_ << kindToString(node->visibility()) << " ";
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
        ss_ << ": ";
        printAst(ss_, e);
      }

      // Return Intent
      if (node->returnIntent() != Function::ReturnIntent::DEFAULT_RETURN_INTENT) {
        ss_ << " " << kindToString(node->returnIntent());
      }

    }

    void visit(const BracketLoop* node) {
      interpose(ss_, node->children(), "", "[", "]");
    }


    void visit(const uast::Identifier* node) {
      ss_ << node->name().str();
    }

    void visit(const Record* node) {
      ss_ << node->name().c_str();
    }

    void visit(const OpCall* node) {
      if (node->isUnaryOp()) {
        ss_ << node->op().c_str();
        assert(node->numActuals() == 1);
        printAst(ss_, node->actual(0));

      } else if (node->isBinaryOp()) {
        assert(node->numActuals() == 2);
        printAst(ss_, node->actual(0));
        ss_ << node->op().c_str();
        printAst(ss_, node->actual(1));
      }
    }

    void visit(const Call* node) {
      const Expression* callee = node->calledExpression();
      if (!callee) {
        printf("ERROR %s\n", asttags::tagToString(node->tag()));
        node->stringify(std::cerr, StringifyKind::CHPL_SYNTAX);
      }
      assert(callee);  // This should be true because OpCall is handled
      printAst(ss_, callee);
      interpose(ss_, node->actuals(), ", ", "(", ")");
    }

    void visit(const Domain* node) {
      interpose(ss_, node->children(), ", ", "{", "}");
    }

    void visit(const Tuple* node) {
      interpose(ss_, node->children(), ", ", "(", ")");
    }

    void visit(const Array* node) {
      interpose(ss_, node->children(), ", ", "[", "]");
    }

    void visit(const Range* node) {
      if (auto lb = node->lowerBound()) {
        printAst(ss_, lb);
      }
      ss_ << "..";
      if (auto ub = node->upperBound()) {
        printAst(ss_, ub);
      }
    }

    void visit(const BoolLiteral* node) {
        ss_ << (node->value() ? "true" : "false");
    }

    template<typename T>
    void visitLiteral(const T* node) {
      ss_ << node->text().c_str();
    }

    void visit(const IntLiteral* node)  { return visitLiteral(node); }
    void visit(const UintLiteral* node) { return visitLiteral(node); }
    void visit(const RealLiteral* node) { return visitLiteral(node); }
    void visit(const ImagLiteral* node) { return visitLiteral(node); }

    void visit(const StringLiteral* node) {
      ss_ << '"' << quoteStringForC(std::string(node->str().c_str())) << '"';
    }

    void visit(const CStringLiteral* node) {
      ss_ << "c\"" << quoteStringForC(std::string(node->str().c_str())) << '"';
    }

    void visit(const Dot* node) {
      printAst(ss_, node->receiver());
      ss_ << "." << node->field().c_str();
    }

    void visit(const New* node) {
      ss_ << "new ";
      if (node->management() != New::Management::DEFAULT_MANAGEMENT) {
        ss_ << kindToString(node->management()) << " ";
      }
      printAst(ss_, node->typeExpression());
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

  };


  struct UserStringVisitor {
    std::stringstream ss_;

    void visit(const uast::Function* node) {
      std::stringstream ss;
      if (node->visibility() != Function::Visibility::DEFAULT_VISIBILITY) {
        ss_ << kindToString(node->visibility()) << " ";
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

    void visit(const uast::ASTNode* node) {
      ss_ << tagToString(node->tag());
    }

  };

  /* Generic printer calling the above functions */
  void printAst(std::ostream& os, const uast::ASTNode* node) {
    auto visitor = ChplSyntaxVisitor{};
    node->dispatch<void>(visitor);
    os << visitor.ss_.rdbuf();
    os << std::flush;
  }

  /* Generic printer calling the above functions */
  void printUserString(std::ostream& os, const uast::Function* node) {
    auto visitor = UserStringVisitor{};
    node->dispatch<void>(visitor);
    os << visitor.ss_.rdbuf();
    os << std::flush;
  }

} // end chpl namespace

