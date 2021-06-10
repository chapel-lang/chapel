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

/* This file supports conversion of uAST to the older AST used by
   the rest of the compiler. It uses recursive functions to perform
   the conversion.
 */

#include "convert-uast.h"

#include "DoWhileStmt.h"
#include "WhileDoStmt.h"
#include "build.h"
#include "docsDriver.h"

#include "chpl/frontend/frontend-queries.h"
#include "chpl/uast/all-uast.h"
#include "chpl/util/string-escapes.h"

using namespace chpl;

namespace {

struct Converter {
  chpl::Context* context;

  Converter(chpl::Context* context) : context(context) { }

  Expr* convertAST(const uast::ASTNode* node);

  Expr* convertComment(const uast::Comment* node) {
    // old ast does not represent comments
    return nullptr;
  }

  Expr* convertErroneousExpression(const uast::ErroneousExpression* node) {
    return new CallExpr(PRIM_ERROR);
  }

  UnresolvedSymExpr* convertIdentifier(const uast::Identifier* node) {
    return new UnresolvedSymExpr(node->name().c_str());
  }

  BlockStmt* createBlockWithStmts(
      uast::ASTListIteratorPair<uast::Expression> stmts) {
    BlockStmt* block = new BlockStmt();
    for (auto stmt: stmts) {
      auto ast = convertAST(stmt);
      if (ast) {
        Expr* e = toExpr(ast);
        INT_ASSERT(e);
        block->insertAtTail(e);
      }
    }
    return block;
  }

  BlockStmt* convertBlock(const uast::Block* node) {
    return createBlockWithStmts(node->stmts());
  }

  BlockStmt* convertLocal(const uast::Local* node) {
    BlockStmt* body = createBlockWithStmts(node->stmts()); 
    if (node->condition()) {
      Expr* e = toExpr(convertAST(node->condition()));
      INT_ASSERT(e);
      return buildLocalStmt(e, body);
    } else {
      return buildLocalStmt(body);
    }
  }

  CallExpr* convertNew(const uast::New* node) {
    switch (node->management()) {
      case uast::New::DEFAULT_MANAGEMENT:
        return new CallExpr(PRIM_NEW);
      case uast::New::OWNED:
        return new CallExpr(PRIM_NEW,
                            new NamedExpr(astr_chpl_manager,
                                          new SymExpr(dtOwned->symbol)));
      case uast::New::SHARED:
        return new CallExpr(PRIM_NEW,
                            new NamedExpr(astr_chpl_manager,
                                          new SymExpr(dtShared->symbol)));
      case uast::New::UNMANAGED:
        return new CallExpr(PRIM_NEW,
                            new NamedExpr(astr_chpl_manager,
                                          new SymExpr(dtUnmanaged->symbol)));
      case uast::New::BORROWED:
        return new CallExpr(PRIM_NEW,
                            new NamedExpr(astr_chpl_manager,
                                          new SymExpr(dtBorrowed->symbol)));
    }
    INT_FATAL("case not handled");
    return nullptr;
  }

  BlockStmt* convertSerial(const uast::Serial* node) {
    BlockStmt* body = createBlockWithStmts(node->stmts()); 
    if (node->condition()) {
      Expr* e = toExpr(convertAST(node->condition()));
      INT_ASSERT(e);
      return buildSerialStmt(e, body);
    } else {
      return buildSerialStmt(new SymExpr(gTrue), body);
    }

  }

  Expr* convertWithClause(const uast::WithClause* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  /// ControlFlows ///

  Expr* convertConditional(const uast::Conditional* node) {
    Expr* condExpr = toExpr(convertAST(node->condition()));
    BlockStmt* thenBlock = createBlockWithStmts(node->thenStmts());
    BlockStmt* elseBlock = nullptr;

    if (node->hasElseBlock())
      elseBlock = createBlockWithStmts(node->elseStmts());

    return buildIfStmt(condExpr, thenBlock, elseBlock);
  }

  /// Loops ///
  BlockStmt* convertDoWhile(const uast::DoWhile* node) {
    Expr* condExpr = toExpr(convertAST(node->condition()));
    BlockStmt* body = createBlockWithStmts(node->stmts());
    return DoWhileStmt::build(condExpr, body);
  }

  BlockStmt* convertWhile(const uast::While* node) {
    Expr* condExpr = toExpr(convertAST(node->condition()));
    BlockStmt* body = createBlockWithStmts(node->stmts());
    return WhileDoStmt::build(condExpr, body);
  }

  /// IndexableLoops ///
  BlockStmt* convertBracketLoop(const uast::BracketLoop* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  BlockStmt* convertCoforall(const uast::Coforall* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  BlockStmt* convertFor(const uast::For* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  BlockStmt* convertForall(const uast::Forall* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  BlockStmt* convertForeach(const uast::Foreach* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  /// Literals ///

  /// NumericLiterals ///
  Expr* convertImagLiteral(const uast::ImagLiteral* node) {
    SymExpr* se = buildImagLiteral(node->text().c_str());
    VarSymbol* v = toVarSymbol(se->symbol());
    INT_ASSERT(v && v->immediate && v->immediate->const_kind == NUM_KIND_IMAG);
    INT_ASSERT(v->immediate->real_value() == node->value());
    return se;
  }

  Expr* convertIntLiteral(const uast::IntLiteral* node) {
    SymExpr* se = buildIntLiteral(node->text().c_str());
    VarSymbol* v = toVarSymbol(se->symbol());
    INT_ASSERT(v && v->immediate && v->immediate->const_kind == NUM_KIND_INT);
    INT_ASSERT(v->immediate->int_value() == node->value());
    return se;
  }

  Expr* convertRealLiteral(const uast::RealLiteral* node) {
    SymExpr* se = buildRealLiteral(node->text().c_str());
    VarSymbol* v = toVarSymbol(se->symbol());
    INT_ASSERT(v && v->immediate && v->immediate->const_kind == NUM_KIND_REAL);
    INT_ASSERT(v->immediate->int_value() == node->value());
    return se;
  }

  Expr* convertUintLiteral(const uast::UintLiteral* node) {
    SymExpr* se = buildIntLiteral(node->text().c_str());
    VarSymbol* v = toVarSymbol(se->symbol());
    INT_ASSERT(v && v->immediate && v->immediate->const_kind == NUM_KIND_UINT);
    INT_ASSERT(v->immediate->uint_value() == node->value());
    return se;
  }

  /// StringLikeLiterals ///
  Expr* convertBytesLiteral(const uast::BytesLiteral* node) {
    std::string quoted = quoteStringForC(node->str());
    SymExpr* se = buildBytesLiteral(quoted.c_str());
    VarSymbol* v = toVarSymbol(se->symbol());
    INT_ASSERT(v && v->immediate);
    INT_ASSERT(v->immediate->const_kind == CONST_KIND_STRING);
    INT_ASSERT(v->immediate->string_kind == STRING_KIND_BYTES);
    return se;
  }

  Expr* convertCStringLiteral(const uast::CStringLiteral* node) {
    std::string quoted = quoteStringForC(node->str());
    SymExpr* se = buildCStringLiteral(quoted.c_str());
    VarSymbol* v = toVarSymbol(se->symbol());
    INT_ASSERT(v && v->immediate);
    INT_ASSERT(v->immediate->const_kind == CONST_KIND_STRING);
    INT_ASSERT(v->immediate->string_kind == STRING_KIND_C_STRING);
    return se;

  }

  Expr* convertStringLiteral(const uast::StringLiteral* node) {
    std::string quoted = quoteStringForC(node->str());
    SymExpr* se = buildStringLiteral(quoted.c_str());
    VarSymbol* v = toVarSymbol(se->symbol());
    INT_ASSERT(v && v->immediate);
    INT_ASSERT(v->immediate->const_kind == CONST_KIND_STRING);
    INT_ASSERT(v->immediate->string_kind == STRING_KIND_STRING);
    return se;
  }

  /// Calls ///

  Expr* convertDot(const uast::Dot* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  Expr* convertFnCall(const uast::FnCall* node) {
    const uast::Expression* calledExpression = node->calledExpression();
    INT_ASSERT(calledExpression);
    Expr* calledExpr = toExpr(convertAST(calledExpression));
    INT_ASSERT(calledExpr);

    CallExpr* ret = new CallExpr(calledExpr);

    int nActuals = node->numActuals();
    for (int i = 0; i < nActuals; i++) {
      Expr* actual = toExpr(convertAST(node->actual(i)));
      INT_ASSERT(actual);
      if (node->isNamedActual(i)) {
        actual = buildNamedActual(node->actualName(i).c_str(), actual);
      }
      ret->insertAtTail(actual);
    }

    return ret;
  }

  Expr* convertOpCall(const uast::OpCall* node) {
    CallExpr* ret = new CallExpr(node->op().c_str());
    int nActuals = node->numActuals();
    for (int i = 0; i < nActuals; i++) {
      Expr* actual = toExpr(convertAST(node->actual(i)));
      INT_ASSERT(actual);
      ret->insertAtTail(actual);
    }

    return ret;
  }

  Expr* convertPrimCall(const uast::PrimCall* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  Expr* convertZip(const uast::Zip* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  /// Decls ///

  Expr* convertMultiDecl(const uast::MultiDecl* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  Expr* convertTupleDecl(const uast::TupleDecl* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  /// NamedDecls ///

  Expr* convertEnumElement(const uast::EnumElement* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  Expr* convertFunction(const uast::Function* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  DefExpr* convertModule(const uast::Module* node) {
    chpl::UniqueString ustr = node->name();
    const char* name = ustr.c_str();
    const char* path = context->filePathForID(node->id()).c_str();
    ModTag tag = MOD_USER; // TODO: distinguish internal/standard/etc
    bool priv = (node->visibility() == uast::Decl::PRIVATE);
    bool prototype = (node->kind() == uast::Module::PROTOTYPE ||
                      node->kind() == uast::Module::IMPLICIT);
    BlockStmt* body = createBlockWithStmts(node->stmts());

    ModuleSymbol* mod = buildModule(name,
                                    tag,
                                    body,
                                    path,
                                    priv,
                                    prototype,
                                    /* docs */ nullptr);

    return new DefExpr(mod);
  }

  /// VarLikeDecls ///

  Expr* convertFormal(const uast::Formal* node) {
    INT_FATAL("TODO");
    return nullptr;
  }
  Expr* convertTaskVar(const uast::TaskVar* node) {
    INT_FATAL("TODO");
    return nullptr;
  }
  Expr* convertVariable(const uast::Variable* node) {
    auto stmts = new BlockStmt(BLOCK_SCOPELESS);

    auto varSym = new VarSymbol(node->name().c_str());
    // Adjust the variable according to its kind
    switch (node->kind()) {
      case uast::Variable::VAR:
        varSym->qual = QUAL_VAL;
        break;
      case uast::Variable::CONST:
        varSym->addFlag(FLAG_CONST);
        varSym->qual = QUAL_CONST;
        break;
      case uast::Variable::CONST_REF:
        varSym->addFlag(FLAG_CONST);
        varSym->addFlag(FLAG_REF_VAR);
        varSym->qual = QUAL_CONST_REF;
        break;
      case uast::Variable::REF:
        varSym->addFlag(FLAG_REF_VAR);
        varSym->qual = QUAL_REF;
        break;
      case uast::Variable::PARAM:
        varSym->addFlag(FLAG_PARAM);
        varSym->qual = QUAL_PARAM;
        break;
      case uast::Variable::TYPE:
        varSym->addFlag(FLAG_TYPE_VARIABLE);
        break;
      case uast::Variable::INDEX:
        break;
    }

    Expr* typeExpr = nullptr;
    Expr* initExpr = nullptr;
    if (node->typeExpression()) {
      typeExpr = convertAST(node->typeExpression());
      INT_ASSERT(typeExpr);
    }
    if (node->initExpression()) {
      initExpr = convertAST(node->initExpression());
      INT_ASSERT(initExpr);
    }
    auto defExpr = new DefExpr(varSym, initExpr, typeExpr);
    stmts->insertAtTail(defExpr);

    // Add a PRIM_END_OF_STATEMENT.
    if (fDocs == false) {
      CallExpr* end = new CallExpr(PRIM_END_OF_STATEMENT);
      stmts->insertAtTail(end);
    }

    return stmts;
  }

  /// TypeDecls

  Expr* convertEnum(const uast::Enum* node) {
    INT_FATAL("TODO");
    return nullptr;
  }
};

/// Generic conversion calling the above functions ///
Expr* Converter::convertAST(const uast::ASTNode* node) {
  switch (node->tag()) {
    #define CONVERT(NAME) \
      case chpl::uast::asttags::NAME: \
      { \
        astlocMarker markAstLoc(node->id()); \
        return convert##NAME((const chpl::uast::NAME*) node); \
      }

    #define IGNORE(NAME) \
      case chpl::uast::asttags::NAME: \
        INT_FATAL("case not handled"); \
        return nullptr;

    #define AST_NODE(NAME) CONVERT(NAME)
    #define AST_LEAF(NAME) CONVERT(NAME)
    #define AST_BEGIN_SUBCLASSES(NAME) IGNORE(START_##NAME)
    #define AST_END_SUBCLASSES(NAME) IGNORE(END_##NAME)

    #include "chpl/uast/ASTClassesList.h"

    IGNORE(NUM_AST_TAGS)

    #undef AST_NODE
    #undef AST_LEAF
    #undef AST_BEGIN_SUBCLASSES
    #undef AST_END_SUBCLASSES
    #undef CONVERT
    #undef IGNORE
  }

  INT_FATAL("case not handled in convertAST");
  return nullptr;
}

} // end anonymous namespace

ModuleSymbol* convertToplevelModule(chpl::Context* context,
                                    const chpl::uast::Module* mod) {
  printf("Converting module named %s\n", mod->name().c_str());
  astlocMarker markAstLoc(mod->id());
  Converter c(context);
  DefExpr* def = c.convertModule(mod);
  ModuleSymbol* ret = toModuleSymbol(def->sym);
  ModuleSymbol::addTopLevelModule(ret);
  return ret;
}
