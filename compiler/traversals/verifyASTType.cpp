#include <typeinfo>
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "verifyASTType.h"

static void verifyASTType(BaseAST* ast) {
  switch (ast->astType) {
  case STMT:
    if (typeid(*ast) != typeid(Stmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: ");
    }
    break;
  case STMT_NOOP:
    if (typeid(*ast) != typeid(NoOpStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: ");
    }
    break;
  case STMT_VARDEF:
    if (typeid(*ast) != typeid(VarDefStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: ");
    }
    break;
  case STMT_TYPEDEF:
    if (typeid(*ast) != typeid(TypeDefStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: ");
    }
    break;
  case STMT_FNDEF:
    if (typeid(*ast) != typeid(FnDefStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: ");
    }
    break;
  case STMT_EXPR:
    if (typeid(*ast) != typeid(ExprStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: ");
    }
    break;
  case STMT_RETURN:
    if (typeid(*ast) != typeid(ReturnStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: ");
    }
    break;
  case STMT_BLOCK:
    if (typeid(*ast) != typeid(BlockStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: ");
    }
    break;
  case STMT_WHILELOOP:
    if (typeid(*ast) != typeid(WhileLoopStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: ");
    }
    break;
  case STMT_FORLOOP:
    if (typeid(*ast) != typeid(ForLoopStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: ");
    }
    break;
  case STMT_COND:
    if (typeid(*ast) != typeid(CondStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: ");
    }
    break;
  case EXPR:
    if (typeid(*ast) != typeid(Expr)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR");
    }
    break;
  case EXPR_LITERAL:
    if (typeid(*ast) != typeid(Literal)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_LITERAL");
    }
    break;
  case EXPR_BOOLLITERAL:
    if (typeid(*ast) != typeid(BoolLiteral)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_BOOLLITERAL");
    }
  case EXPR_INTLITERAL:
    if (typeid(*ast) != typeid(IntLiteral)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_INTLITERAL");
    }
    break;
  case EXPR_FLOATLITERAL:
    if (typeid(*ast) != typeid(FloatLiteral)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_FLOATLITERAL");
    }
    break;
  case EXPR_COMPLEXLITERAL:
    if (typeid(*ast) != typeid(ComplexLiteral)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_COMPLEXLITERAL");
    }
  case EXPR_STRINGLITERAL:
    if (typeid(*ast) != typeid(StringLiteral)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_STRINGLITERAL");
    }
    break;
  case EXPR_VARIABLE:
    if (typeid(*ast) != typeid(Variable)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_VARIABLE");
    }
    break;
  case EXPR_UNOP:
    if (typeid(*ast) != typeid(UnOp)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_UNOP");
    }
    break;
  case EXPR_BINOP:
    if (typeid(*ast) != typeid(BinOp)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_BINOP");
    }
    break;
  case EXPR_SPECIALBINOP:
    if (typeid(*ast) != typeid(SpecialBinOp)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_SPECIALBINOP");
    }
    break;
  case EXPR_ASSIGNOP:
    if (typeid(*ast) != typeid(AssignOp)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_ASSIGNOP");
    }
    break;
  case EXPR_SIMPLESEQ:
    if (typeid(*ast) != typeid(SimpleSeqExpr)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_SIMPLESEQ");
    }
    break;
  case EXPR_FLOOD:
    if (typeid(*ast) != typeid(FloodExpr)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_FLOOD");
    }
    break;
  case EXPR_COMPLETEDIM:
    if (typeid(*ast) != typeid(CompleteDimExpr)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_COMPLETEDIM");
    }
    break;
  case EXPR_DOMAIN:
    if (typeid(*ast) != typeid(DomainExpr)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_DOMAIN");
    }
    break;
  case EXPR_PARENOP:
    if (typeid(*ast) != typeid(ParenOpExpr)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_PARENOP");
    }
    break;
  case EXPR_SIZEOF:
    if (typeid(*ast) != typeid(SizeofExpr)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_SIZEOF");
    }
  case EXPR_CAST:
    if (typeid(*ast) != typeid(CastExpr)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_CAST");
    }
    break;
  case EXPR_FNCALL:
    if (typeid(*ast) != typeid(FnCall)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_FNCALL");
    }
    break;
  case EXPR_IOCALL:
    if (typeid(*ast) != typeid(IOCall)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_IOCALL");
    }
    break;
  case EXPR_ARRAYREF:
    if (typeid(*ast) != typeid(ArrayRef)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_ARRAYREF");
    }
    break;
  case EXPR_MEMBERACCESS:
    if (typeid(*ast) != typeid(MemberAccess)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_MEMBERACCESS");
    }
  case EXPR_REDUCE:
    if (typeid(*ast) != typeid(ReduceExpr)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_REDUCE");
    }
    break;
  case EXPR_TUPLE:
    if (typeid(*ast) != typeid(Tuple)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_TUPLE");
    }
    break;
  case SYMBOL:
    if (typeid(*ast) != typeid(Symbol)) {
      INT_FATAL(ast, "Unexpected AST symbol type: SYMBOL");
    }
    break;
  case SYMBOL_UNRESOLVED:
    if (typeid(*ast) != typeid(UnresolvedSymbol)) {
      INT_FATAL(ast, "Unexpected AST symbol type: SYMBOL_USEBEFOREDEF");
    }
    break;
  case SYMBOL_MODULE:
    if (typeid(*ast) != typeid(ModuleSymbol)) {
      INT_FATAL(ast, "Unexpected AST symbol type: SYMBOL_MODULE");
    }
    break;
  case SYMBOL_VAR:
    if (typeid(*ast) != typeid(VarSymbol)) {
      INT_FATAL(ast, "Unexpected AST symbol type: SYMBOL_VAR");
    }
    break;
  case SYMBOL_PARAM:
    if (typeid(*ast) != typeid(ParamSymbol)) {
      INT_FATAL(ast, "Unexpected AST symbol type: SYMBOL_PARAM");
    }
    break;
  case SYMBOL_TYPE:
    if (typeid(*ast) != typeid(TypeSymbol)) {
      INT_FATAL(ast, "Unexpected AST symbol type: SYMBOL_TYPE");
    }
    break;
  case SYMBOL_CLASS:
    if (typeid(*ast) != typeid(ClassSymbol)) {
      INT_FATAL(ast, "Unexpected AST symbol type: SYMBOL_CLASS");
    }
    break;
  case SYMBOL_REDUCE:
    if (typeid(*ast) != typeid(ReduceSymbol)) {
      INT_FATAL(ast, "Unexpected AST symbol type: ");
    }
    break;
  case SYMBOL_FN:
    if (typeid(*ast) != typeid(FnSymbol)) {
      INT_FATAL(ast, "Unexpected AST symbol type: ");
    }
    break;
  case SYMBOL_ENUM:
    if (typeid(*ast) != typeid(EnumSymbol)) {
      INT_FATAL(ast, "Unexpected AST symbol type: ");
    }
    break;
  case TYPE:
    if (typeid(*ast) != typeid(Type)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE");
    }
    break;
  case TYPE_BUILTIN:
    if (typeid(*ast) != typeid(Type)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE_BUILTIN");
    }
    break;
  case TYPE_ENUM:
    if (typeid(*ast) != typeid(EnumType)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE_ENUM");
    }
    break;
  case TYPE_DOMAIN:
    if (typeid(*ast) != typeid(DomainType)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE_DOMAIN");
    }
    break;
  case TYPE_INDEX:
    if (typeid(*ast) != typeid(IndexType)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE_INDEX");
    }
    break;
  case TYPE_ARRAY:
    if (typeid(*ast) != typeid(ArrayType)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE_ARRAY");
    }
    break;
  case TYPE_USER:
    if (typeid(*ast) != typeid(UserType)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE_USER");
    }
    break;
  case TYPE_CLASS:
    if (typeid(*ast) != typeid(ClassType)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE_CLASS");
    }
    break;
  case TYPE_TUPLE:
    if (typeid(*ast) != typeid(TupleType)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE_TUPLE");
    }
    break;
  case AST_TYPE_END:
    INT_FATAL(ast, "Unexpected AST type: AST_TYPE_END");
    break;
  }
}


void VerifyASTType::preProcessStmt(Stmt* stmt) {
  verifyASTType(stmt);
}


void VerifyASTType::preProcessExpr(Expr* expr) {
  verifyASTType(expr);
}


void VerifyASTType::preProcessSymbol(Symbol* sym) {
  verifyASTType(sym);
}


void VerifyASTType::preProcessType(Type* type) {
  verifyASTType(type);
}
