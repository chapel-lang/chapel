#include <typeinfo>
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "verifyASTType.h"

void VerifyASTType::preProcessStmt(Stmt* stmt) {
  switch (stmt->astType) {
  case STMT:
    if (typeid(*stmt) != typeid(Stmt)) {
      INT_FATAL(stmt, "Unexpected AST statement type: ");
    }
    break;
  case STMT_NOOP:
    if (typeid(*stmt) != typeid(NoOpStmt)) {
      INT_FATAL(stmt, "Unexpected AST statement type: ");
    }
    break;
  case STMT_VARDEF:
    if (typeid(*stmt) != typeid(VarDefStmt)) {
      INT_FATAL(stmt, "Unexpected AST statement type: ");
    }
    break;
  case STMT_TYPEDEF:
    if (typeid(*stmt) != typeid(TypeDefStmt)) {
      INT_FATAL(stmt, "Unexpected AST statement type: ");
    }
    break;
  case STMT_FNDEF:
    if (typeid(*stmt) != typeid(FnDefStmt)) {
      INT_FATAL(stmt, "Unexpected AST statement type: ");
    }
    break;
  case STMT_EXPR:
    if (typeid(*stmt) != typeid(ExprStmt)) {
      INT_FATAL(stmt, "Unexpected AST statement type: ");
    }
    break;
  case STMT_RETURN:
    if (typeid(*stmt) != typeid(ReturnStmt)) {
      INT_FATAL(stmt, "Unexpected AST statement type: ");
    }
    break;
  case STMT_BLOCK:
    if (typeid(*stmt) != typeid(BlockStmt)) {
      INT_FATAL(stmt, "Unexpected AST statement type: ");
    }
    break;
  case STMT_WHILELOOP:
    if (typeid(*stmt) != typeid(WhileLoopStmt)) {
      INT_FATAL(stmt, "Unexpected AST statement type: ");
    }
    break;
  case STMT_FORLOOP:
    if (typeid(*stmt) != typeid(ForLoopStmt)) {
      INT_FATAL(stmt, "Unexpected AST statement type: ");
    }
    break;
  case STMT_COND:
    if (typeid(*stmt) != typeid(CondStmt)) {
      INT_FATAL(stmt, "Unexpected AST statement type: ");
    }
    break;
  default:
    INT_FATAL(stmt, "Unexpected AST statement type: %d", stmt->astType);
  }
}


void VerifyASTType::preProcessExpr(Expr* expr) {
  switch (expr->astType) {
  case EXPR:
    if (typeid(*expr) != typeid(Expr)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR");
    }
    break;
  case EXPR_LITERAL:
    if (typeid(*expr) != typeid(Literal)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_LITERAL");
    }
    break;
  case EXPR_INTLITERAL:
    if (typeid(*expr) != typeid(IntLiteral)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_INTLITERAL");
    }
    break;
  case EXPR_FLOATLITERAL:
    if (typeid(*expr) != typeid(FloatLiteral)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_FLOATLITERAL");
    }
    break;
  case EXPR_STRINGLITERAL:
    if (typeid(*expr) != typeid(StringLiteral)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_STRINGLITERAL");
    }
    break;
  case EXPR_VARIABLE:
    if (typeid(*expr) != typeid(Variable)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_VARIABLE");
    }
    break;
  case EXPR_UNOP:
    if (typeid(*expr) != typeid(UnOp)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_UNOP");
    }
    break;
  case EXPR_BINOP:
    if (typeid(*expr) != typeid(BinOp)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_BINOP");
    }
    break;
  case EXPR_SPECIALBINOP:
    if (typeid(*expr) != typeid(SpecialBinOp)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_SPECIALBINOP");
    }
    break;
  case EXPR_ASSIGNOP:
    if (typeid(*expr) != typeid(AssignOp)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_ASSIGNOP");
    }
    break;
  case EXPR_SIMPLESEQ:
    if (typeid(*expr) != typeid(SimpleSeqExpr)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_SIMPLESEQ");
    }
    break;
  case EXPR_FLOOD:
    if (typeid(*expr) != typeid(FloodExpr)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_FLOOD");
    }
    break;
  case EXPR_COMPLETEDIM:
    if (typeid(*expr) != typeid(CompleteDimExpr)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_COMPLETEDIM");
    }
    break;
  case EXPR_DOMAIN:
    if (typeid(*expr) != typeid(DomainExpr)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_DOMAIN");
    }
    break;
  case EXPR_PARENOP:
    if (typeid(*expr) != typeid(ParenOpExpr)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_PARENOP");
    }
    break;
  case EXPR_SIZEOF:
    if (typeid(*expr) != typeid(SizeofExpr)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_SIZEOF");
    }
  case EXPR_CAST:
    if (typeid(*expr) != typeid(CastExpr)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_CAST");
    }
    break;
  case EXPR_FNCALL:
    if (typeid(*expr) != typeid(FnCall)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_FNCALL");
    }
    break;
  case EXPR_WRITECALL:
    if (typeid(*expr) != typeid(WriteCall)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_WRITECALL");
    }
    break;
  case EXPR_ARRAYREF:
    if (typeid(*expr) != typeid(ArrayRef)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_ARRAYREF");
    }
    break;
  case EXPR_MEMBERACCESS:
    if (typeid(*expr) != typeid(MemberAccess)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_MEMBERACCESS");
    }
  case EXPR_REDUCE:
    if (typeid(*expr) != typeid(ReduceExpr)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_REDUCE");
    }
    break;
  case EXPR_TUPLE:
    if (typeid(*expr) != typeid(Tuple)) {
      INT_FATAL(expr, "Unexpected AST expression type: EXPR_TUPLE");
    }
    break;
  default:
    INT_FATAL(expr, "Unexpected AST expression type: %d\n", expr->astType);
  }
}


void VerifyASTType::preProcessSymbol(Symbol* sym) {
  switch (sym->astType) {
  case SYMBOL:
    if (typeid(*sym) != typeid(Symbol)) {
      INT_FATAL(sym, "Unexpected AST symbol type: SYMBOL");
    }
    break;
  case SYMBOL_USEBEFOREDEF:
    if (typeid(*sym) != typeid(UseBeforeDefSymbol)) {
      INT_FATAL(sym, "Unexpected AST symbol type: SYMBOL_USEBEFOREDEF");
    }
    break;
  case SYMBOL_VAR:
    if (typeid(*sym) != typeid(VarSymbol)) {
      INT_FATAL(sym, "Unexpected AST symbol type: SYMBOL_VAR");
    }
    break;
  case SYMBOL_PARAM:
    if (typeid(*sym) != typeid(ParamSymbol)) {
      INT_FATAL(sym, "Unexpected AST symbol type: SYMBOL_PARAM");
    }
    break;
  case SYMBOL_TYPE:
    if (typeid(*sym) != typeid(TypeSymbol)) {
      INT_FATAL(sym, "Unexpected AST symbol type: SYMBOL_TYPE");
    }
    break;
  case SYMBOL_CLASS:
    if (typeid(*sym) != typeid(ClassSymbol)) {
      INT_FATAL(sym, "Unexpected AST symbol type: SYMBOL_CLASS");
    }
    break;
  case SYMBOL_REDUCE:
    if (typeid(*sym) != typeid(ReduceSymbol)) {
      INT_FATAL(sym, "Unexpected AST symbol type: ");
    }
    break;
  case SYMBOL_FN:
    if (typeid(*sym) != typeid(FnSymbol)) {
      INT_FATAL(sym, "Unexpected AST symbol type: ");
    }
    break;
  case SYMBOL_ENUM:
    if (typeid(*sym) != typeid(EnumSymbol)) {
      INT_FATAL(sym, "Unexpected AST symbol type: ");
    }
    break;
  default:
    INT_FATAL(sym, "Unexpected AST symbol type: %d\n", sym->astType);
  }
}



void VerifyASTType::preProcessType(Type* type) {
  switch (type->astType) {
  case TYPE:
    if (typeid(*type) != typeid(Type)) {
      INT_FATAL(type, "Unexpected AST type type: TYPE");
    }
    break;
  case TYPE_BUILTIN:
    if (typeid(*type) != typeid(Type)) {
      INT_FATAL(type, "Unexpected AST type type: TYPE_BUILTIN");
    }
    break;
  case TYPE_ENUM:
    if (typeid(*type) != typeid(EnumType)) {
      INT_FATAL(type, "Unexpected AST type type: TYPE_ENUM");
    }
    break;
  case TYPE_DOMAIN:
    if (typeid(*type) != typeid(DomainType)) {
      INT_FATAL(type, "Unexpected AST type type: TYPE_DOMAIN");
    }
    break;
  case TYPE_INDEX:
    if (typeid(*type) != typeid(IndexType)) {
      INT_FATAL(type, "Unexpected AST type type: TYPE_INDEX");
    }
    break;
  case TYPE_ARRAY:
    if (typeid(*type) != typeid(ArrayType)) {
      INT_FATAL(type, "Unexpected AST type type: TYPE_ARRAY");
    }
    break;
  case TYPE_USER:
    if (typeid(*type) != typeid(UserType)) {
      INT_FATAL(type, "Unexpected AST type type: TYPE_USER");
    }
    break;
  case TYPE_CLASS:
    if (typeid(*type) != typeid(ClassType)) {
      INT_FATAL(type, "Unexpected AST type type: TYPE_CLASS");
    }
    break;
  case TYPE_TUPLE:
    if (typeid(*type) != typeid(TupleType)) {
      INT_FATAL(type, "Unexpected AST type type: TYPE_TUPLE");
    }
    break;
  default:
    INT_FATAL(type, "Unexpected AST type type: %d\n", type->astType);
  }
}
