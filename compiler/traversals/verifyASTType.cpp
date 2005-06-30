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
      INT_FATAL(ast, "Unexpected AST statement type: STMT");
    }
    break;
  case STMT_NOOP:
    if (typeid(*ast) != typeid(NoOpStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: STMT_NOOP");
    }
    break;
  case STMT_DEF:
    if (typeid(*ast) != typeid(DefStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: STMT_DEF");
    }
    break;
  case STMT_EXPR:
    if (typeid(*ast) != typeid(ExprStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: STMT_EXPR");
    }
    break;
  case STMT_RETURN:
    if (typeid(*ast) != typeid(ReturnStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: STMT_RETURN");
    }
    break;
  case STMT_BLOCK:
    if (typeid(*ast) != typeid(BlockStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: STMT_BLOCK");
    }
    break;
  case STMT_WHILELOOP:
    if (typeid(*ast) != typeid(WhileLoopStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: STMT_WHILELOOP");
    }
    break;
  case STMT_FORLOOP:
    if (typeid(*ast) != typeid(ForLoopStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: STMT_FORLOOP");
    }
    break;
  case STMT_COND:
    if (typeid(*ast) != typeid(CondStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: STMT_COND");
    }
    break;
  case STMT_WHEN:
    if (typeid(*ast) != typeid(WhenStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: STMT_WHEN");
    }
    break;
  case STMT_SELECT:
    if (typeid(*ast) != typeid(SelectStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: STMT_SELECT");
    }
    break;
  case STMT_LABEL:
    if (typeid(*ast) != typeid(LabelStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: STMT_LABEL");
    }
    break;
  case STMT_GOTO:
    if (typeid(*ast) != typeid(GotoStmt)) {
      INT_FATAL(ast, "Unexpected AST statement type: STMT_GOTO");
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
    break;
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
    break;
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
  case EXPR_VARINIT:
    if (typeid(*ast) != typeid(VarInitExpr)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_VARINIT");
    }
    break;
  case EXPR_USERINIT:
    if (typeid(*ast) != typeid(UserInitExpr)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_USERINIT");
    }
    break;
  case EXPR_DEF:
    if (typeid(*ast) != typeid(DefExpr)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_DEF");
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
  case EXPR_SEQ:
    if (typeid(*ast) != typeid(SeqExpr)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_SEQ");
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
  case EXPR_FORALL:
    if (typeid(*ast) != typeid(ForallExpr)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_FORALL");
    }
    break;
  case EXPR_LET:
    if (typeid(*ast) != typeid(LetExpr)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_LET");
    }
    break;
  case EXPR_COND:
    if (typeid(*ast) != typeid(CondExpr)) {
      INT_FATAL(ast, "Unexpected AST statement type: EXPR_COND");
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
    break;
  case EXPR_CAST:
    if (typeid(*ast) != typeid(CastExpr)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_CAST");
    }
    break;
  case EXPR_CAST_LIKE:
    if (typeid(*ast) != typeid(CastLikeExpr)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_CAST_LIKE");
    }
    break;
  case EXPR_FNCALL:
    if (typeid(*ast) != typeid(FnCall)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_FNCALL");
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
    break;
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
  case EXPR_NAMED:
    if (typeid(*ast) != typeid(NamedExpr)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_NAMED");
    }
    break;
  case EXPR_WITH:
    if (typeid(*ast) != typeid(WithExpr)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_WITH");
    }
    break;
  case EXPR_USE:
    if (typeid(*ast) != typeid(UseExpr)) {
      INT_FATAL(ast, "Unexpected AST expression type: EXPR_USE");
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
  case SYMBOL_FN:
    if (typeid(*ast) != typeid(FnSymbol)) {
      INT_FATAL(ast, "Unexpected AST symbol type: SYMBOL_FN");
    }
    break;
  case SYMBOL_ENUM:
    if (typeid(*ast) != typeid(EnumSymbol)) {
      INT_FATAL(ast, "Unexpected AST symbol type: SYMBOL_ENUM");
    }
    break;
  case SYMBOL_LABEL:
    if (typeid(*ast) != typeid(LabelSymbol)) {
      INT_FATAL(ast, "Unexpected AST symbol type: SYMBOL_LABEL");
    }
    break;
  case SYMBOL_FORWARDING:
    if (typeid(*ast) != typeid(ForwardingSymbol)) {
      INT_FATAL(ast, "Unexpected AST symbol type: SYMBOL_FORWARDING");
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
  case TYPE_FN:
    if (typeid(*ast) != typeid(FnType)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE_FN");
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
  case TYPE_LIKE:
    if (typeid(*ast) != typeid(LikeType)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE_LIKE");
    }
    break;
  case TYPE_STRUCTURAL:
    if (typeid(*ast) != typeid(StructuralType)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE_STRUCTURAL");
    }
    break;
  case TYPE_CLASS:
    if (typeid(*ast) != typeid(ClassType)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE_CLASS");
    }
    break;
  case TYPE_RECORD:
    if (typeid(*ast) != typeid(RecordType)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE_RECORD");
    }
    break;
  case TYPE_UNION:
    if (typeid(*ast) != typeid(UnionType)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE_UNION");
    }
    break;
  case TYPE_TUPLE:
    if (typeid(*ast) != typeid(TupleType)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE_TUPLE");
    }
    break;
  case TYPE_META:
    if (typeid(*ast) != typeid(MetaType)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE_META");
    }
    break;
  case TYPE_SUM:
    if (typeid(*ast) != typeid(SumType)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE_SUM");
    }
    break;
  case TYPE_VARIABLE:
    if (typeid(*ast) != typeid(VariableType)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE_VARIABLE");
    }
    break;
  case TYPE_UNRESOLVED:
    if (typeid(*ast) != typeid(UnresolvedType)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE_UNRESOLVED");
    }
    break;
  case TYPE_EXPR:
    if (typeid(*ast) != typeid(ExprType)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE_EXPR");
    }
    break;
  case  TYPE_NIL:
    if (typeid(*ast) != typeid(NilType)) {
      INT_FATAL(ast, "Unexpected AST type type: TYPE_NIL");
    }
    break;
  case LIST: 
    break;
  case AST_TYPE_END:
    INT_FATAL(ast, "Unexpected AST type: AST_TYPE_END");
    break;
  }
}


VerifyASTType::VerifyASTType(void) {
  if ((astTypeName[AST_TYPE_END] == NULL) ||
      (strcmp(astTypeName[AST_TYPE_END], "AST_TYPE_END") != 0)) {
    INT_FATAL("Not enough strings in astTypeName (baseAST.cpp");
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
