#include "baseAST.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"

static long uid = 1;

long BaseAST::getNumIDs(void) {
  return uid;
}

BaseAST::BaseAST(void) {
  INT_FATAL("Base AST must be given a type");
}

BaseAST::BaseAST(astType_t type) :
  astType(type),
  id(uid++)
{}

int compar_baseast(const void *ai, const void *aj) {
  BaseAST *i = *(BaseAST**)ai;
  BaseAST *j = *(BaseAST**)aj;
  if (i->id > j->id)
    return 1;
  else if (i->id < j->id)
    return -1;
  return 0;
}

char* astTypeName[AST_TYPE_END+1] = {
  "Stmt",
  "NoOpStmt",
  "WithStmt",
  "DefStmt",
  "ExprStmt",
  "ReturnStmt",
  "BlockStmt",
  "WhileLoopStmt",
  "ForLoopStmt",
  "CondStmt",
  "LabelStmt",
  "GotoStmt",

  "Expr",
  "Literal",
  "BoolLiteral",
  "IntLiteral",
  "FloatLiteral",
  "ComplexLiteral",
  "StringLiteral",
  "Variable",
  "VarInit",
  "DefExpr",
  "UnOp",
  "BinOp",
  "SpecialBinOp",
  "AssignOp",
  "Seq",
  "SimpleSeq",
  "Flood",
  "CompleteDim",
  "Let",
  "Forall",
  "Sizeof",
  "ParenOp",
  "Cast",
  "FnCall",
  "IOCall",
  "ArrayRef",
  "TupleSelect",
  "MemberAccess",
  "Reduce",
  "Tuple",
  "NamedExpr",

  "Symbol",
  "UnresolvedSymbol",
  "ModuleSymbol",
  "VarSymbol",
  "ParamSymbol",
  "TypeSymbol",
  "FnSymbol",
  "EnumSymbol",
  "LabelSymbol",

  "Type",
  "BuiltinType",
  "EnumType",
  "DomainType",
  "IndexType",
  "SeqType",
  "ArrayType",
  "UserType",
  "LikeType",
  "StructuralType",
  "ClassType",
  "RecordType",
  "UnionType",
  "TupleType",
  "SumType",
  "VariableType",
  "UnresolvedType",
  "NilType",

  "AST_TYPE_END"
};
