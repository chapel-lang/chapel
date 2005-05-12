#include "baseAST.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "map.h"
#include "analysis.h"
#include "../passes/runAnalysis.h"
#include "../traversals/updateSymbols.h"


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


void BaseAST::copySupport(BaseAST* copy,
                          bool clone,
                          Map<BaseAST*,BaseAST*>* map,
                          CloneCallback* analysis_clone,
                          Vec<BaseAST*>* update_list) {
  copy->lineno = lineno;
  copy->filename = filename;
  if (!RunAnalysis::isRunning) {
    //    copy->ainfo = ainfo;  // if we need this, we have to go to expr
  }
  if (analysis_clone) {
    analysis_clone->clone(this, copy);
  }
  map->put(this, copy);
}


void BaseAST::copySupportTopLevel(BaseAST* copy,
                                  bool clone,
                                  Map<BaseAST*,BaseAST*>* map,
                                  CloneCallback* analysis_clone,
                                  Vec<BaseAST*>* update_list) {
  if (update_list) {
    for (int j = 0; j < update_list->n; j++) {
      for (int i = 0; i < map->n; i++) {
        if (update_list->v[j] == map->v[i].key) {
          update_list->v[j] = map->v[i].value;
        }
      }
    }
  }
  TRAVERSE(copy, new UpdateSymbols(map), true);
}


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
  "DefStmt",
  "ExprStmt",
  "ReturnStmt",
  "WithStmt",
  "UseStmt",
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
  "UserInit",
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
  "CastLike",
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
  "ForwardingSymbol",

  "Type",
  "BuiltinType",
  "FnType",
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
