#include "analysis.h"
#include "baseAST.h"
#include "expr.h"
#include "map.h"
#include "moduleList.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"
#include "yy.h"
#include "../passes/runAnalysis.h"
#include "../traversals/updateSymbols.h"
#include "../traversals/fixup.h"


static long uid = 1;

// This is here so that we can break on the creation of a particular
// BaseAST instance in gdb.
static void checkid(long id) {
}

long BaseAST::getNumIDs(void) {
  return uid;
}

BaseAST::BaseAST(void) {
  INT_FATAL("Base AST must be given a type");
}

BaseAST::BaseAST(astType_t type) :
  astType(type),
  id(uid++),
  prev(NULL),
  next(NULL),
  parentScope(NULL),
  filename(yyfilename), 
  lineno(yylineno),
  traversalInfo(NULL),
  copyInfo(NULL),
  copyFrom(NULL)
{
  checkid(id);
  if (lineno == -1) {
    if (currentTraversal) {
      traversalInfo = copystring(currentTraversal);
    }
    if (currentLineno && currentFilename) {
      lineno = currentLineno;
      filename = copystring(currentFilename);
    }
  }
}


BaseAST*
BaseAST::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  INT_FATAL(this, "copyInner not implemented for BaseAST subclass");
  return NULL;
}


void BaseAST::traverse(Traversal* traversal, bool atTop) {
  INT_FATAL(this, "Cannot traverse BaseAST");
}


void BaseAST::traverseDef(Traversal* traversal, bool atTop) {
  INT_FATAL(this, "Cannot traverse definitions of BaseAST");
}


void BaseAST::print(FILE* outfile) {
  if (!this) {
    INT_FATAL("Calling print() on a Null AST");
  } else {
    INT_FATAL("print() is unimplemented for a subclass of BaseAST");
  }
}


void BaseAST::printDef(FILE* outfile) {
  INT_FATAL("printDef() is unimplemented for a subclass of BaseAST");
}


void BaseAST::println(FILE* outfile) {
  print(outfile);
  fprintf(outfile, "\n");
}


void BaseAST::codegen(FILE* outfile) {
  if (!this) {
    INT_FATAL("Calling codegen() on a Null AST");
  } else {
    INT_FATAL("codegen() is unimplemented for a subclass of BaseAST");
  }
}


void BaseAST::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected call to BaseAST::replaceChild(old, new)");
}


void BaseAST::callReplaceChild(BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected call to BaseAST::callReplaceChild(new)");
}


void BaseAST::remove(void) {
  if (prev || next) {
    if (!prev || !next) {
      INT_FATAL("Ill-formed list in BaseAST::remove");
    }
    next->prev = prev;
    prev->next = next;
    next = NULL;
    prev = NULL;
  } else {
    callReplaceChild(NULL);
  }
}


void BaseAST::replace(BaseAST* new_ast) {
  if (new_ast->prev || new_ast->next) {
    INT_FATAL(new_ast, "Argument is in a list in BaseAST::replace");
  }
  if (prev || next) {
    if (!prev || !next) {
      INT_FATAL(this, "Ill-formed list in BaseAST::replace");
    }
    new_ast->prev = prev;
    new_ast->next = next;
    next->prev = new_ast;
    prev->next = new_ast;
    next = NULL;
    prev = NULL;
  } else {
    callReplaceChild(new_ast);
  }
  fixup(new_ast, this);
}


void BaseAST::insertBefore(BaseAST* new_ast) {
  if (new_ast->prev || new_ast->next) {
    INT_FATAL(new_ast, "Argument is in a list in BaseAST::insertBefore");
  }
  if (prev) {
    new_ast->prev = prev;
    new_ast->next = this;
    prev->next = new_ast;
    prev = new_ast;
  } else {
    INT_FATAL(this, "Ill-formed list in BaseAST::insertBefore");
  }
  fixup(new_ast, this);
}


void BaseAST::insertAfter(BaseAST* new_ast) {
  if (new_ast->prev || new_ast->next) {
    INT_FATAL(new_ast, "Argument is in a list in BaseAST::insertAfter");
  }
  if (next) {
    new_ast->prev = this;
    new_ast->next = next;
    next->prev = new_ast;
    next = new_ast;
  } else {
    INT_FATAL(this, "Ill-formed list in BaseAST::insertAfter");
  }
  fixup(new_ast, this);
}


void BaseAST::preCopy(bool clone,
                      Map<BaseAST*,BaseAST*>*& map,
                      Vec<BaseAST*>* update_list,
                      bool internal) {
  if (!map) {
    map = new Map<BaseAST*,BaseAST*>();
  }
}


void BaseAST::postCopy(BaseAST* copy,
                       bool clone,
                       Map<BaseAST*,BaseAST*>* map,
                       Vec<BaseAST*>* update_list,
                       bool internal) {
  copy->copyFrom = this;
  copy->lineno = lineno;
  copy->filename = filename;
  copy->copyPragmas(pragmas);
  if (!RunAnalysis::isRunning) {
    Expr* expr = dynamic_cast<Expr*>(this);
    Expr* exprCopy = dynamic_cast<Expr*>(copy);
    if (expr && exprCopy) {
      exprCopy->ainfo = expr->ainfo;
    }
    Stmt* stmt = dynamic_cast<Stmt*>(this);
    Stmt* stmtCopy = dynamic_cast<Stmt*>(copy);
    if (stmt && stmtCopy) {
      stmtCopy->ainfo = stmt->ainfo;
    }
  }
  map->put(this, copy);
  if (!internal) {
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
}


char* BaseAST::stringLoc(void) {
  const int tmpBuffSize = 64;
  char tmpBuff[tmpBuffSize];

  snprintf(tmpBuff, tmpBuffSize, "%s:%d", filename, lineno);
  return copystring(tmpBuff);
}


void BaseAST::printLoc(FILE* outfile) {
  fprintf(outfile, "%s:%d", filename, lineno);
}


char* BaseAST::hasPragma(char* str) {
  forv_Vec(char, pragma, pragmas) {
    if (!strncmp(pragma, str, strlen(str))) {
      return pragma;
    }
  }
  return NULL;
}


void BaseAST::addPragma(char* str) {
  pragmas.add(copystring(str));
}


void BaseAST::copyPragmas(Vec<char*> srcPragmas) {
  forv_Vec(char, srcPragma, srcPragmas) {
    addPragma(srcPragma);
  }
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
  "BlockStmt",
  "WhileLoopStmt",
  "ForLoopStmt",
  "CondStmt",
  "WhenStmt",
  "SelectStmt",
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
  "CondExpr",
  "Forall",
  "Sizeof",
  "ParenOp",
  "Cast",
  "CastLike",
  "FnCall",
  "ArrayRef",
  "TupleSelect",
  "MemberAccess",
  "Reduce",
  "Tuple",
  "NamedExpr",
  "WithExpr",
  "UseExpr",

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
  "ArrayType",
  "UserType",
  "LikeType",
  "StructuralType",
  "ClassType",
  "RecordType",
  "UnionType",
  "TupleType",
  "MetaType",
  "SumType",
  "VariableType",
  "UnresolvedType",
  "ExprType",
  "NilType",

  "List",

  "AST_TYPE_END"
};

int currentLineno = 0;
char* currentFilename = NULL;
char* currentTraversal = NULL;

#define AST_ADD_CHILD(_t, _m) if (((_t*)a)->_m) asts.add(((_t*)a)->_m)
#define AST_ADD_LIST(_t, _m, _mt) \
  for (_mt* tmp = ((_t*)a)->_m->first(); tmp; tmp = ((_t*)a)->_m->next()) \
      asts.add(tmp)
#define AST_ADD_VEC(_t, _m, _mt) \
    forv_Vec(_mt, c, ((_t*)a)->_m) asts.add(c)

#define ADD_CHILD(_t, _m) if (all) AST_ADD_CHILD(_t, _m)
#define ADD_LIST(_t, _m, _mt) if (all) AST_ADD_LIST(_t, _m, _mt)
#define ADD_VEC(_t, _m, _mt) if (all) AST_ADD_VEC(_t, _m, _mt)

void
get_ast_children(BaseAST *a, Vec<BaseAST *> &asts, int all) {
  switch (a->astType) {
  case STMT:
  LStmtCommon:
    break;
  case STMT_NOOP:
    goto LStmtCommon;
  case STMT_DEF:
    AST_ADD_LIST(DefStmt, defExprls, DefExpr);
    goto LStmtCommon;
  case STMT_EXPR:
  LExprStmtCommon:
    AST_ADD_CHILD(ExprStmt, expr);
    goto LStmtCommon;
  case STMT_RETURN:
    goto LExprStmtCommon;
  case STMT_BLOCK:
  LBlockStmtCommon:
    AST_ADD_LIST(BlockStmt, body, Stmt);
    goto LStmtCommon;
  case STMT_WHILELOOP:
    AST_ADD_CHILD(WhileLoopStmt, condition);
    goto LBlockStmtCommon;
  case STMT_FORLOOP:
    AST_ADD_LIST(ForLoopStmt, indices, DefExpr);
    AST_ADD_CHILD(ForLoopStmt, domain);
    goto LBlockStmtCommon;
  case STMT_COND:
    AST_ADD_CHILD(CondStmt, condExpr);
    AST_ADD_CHILD(CondStmt, thenStmt);
    AST_ADD_CHILD(CondStmt, elseStmt);
    goto LStmtCommon;
  case STMT_WHEN:
    AST_ADD_LIST(WhenStmt, caseExprs, Expr);
    AST_ADD_CHILD(WhenStmt, doStmt);
    goto LStmtCommon;
  case STMT_SELECT:
    AST_ADD_CHILD(SelectStmt, caseExpr);
    AST_ADD_LIST(SelectStmt, whenStmts, WhenStmt);
    goto LStmtCommon;
  case STMT_LABEL:
    ADD_CHILD(LabelStmt, label);
    AST_ADD_CHILD(LabelStmt, stmt);
    goto LStmtCommon;
  case STMT_GOTO:
    ADD_CHILD(GotoStmt, label);
    goto LStmtCommon;
  case EXPR:
    LExprCommon:
    break;
  case EXPR_LITERAL:
  case EXPR_BOOLLITERAL:
  case EXPR_INTLITERAL:
  case EXPR_FLOATLITERAL:
  case EXPR_COMPLEXLITERAL:
  case EXPR_STRINGLITERAL:
    goto LExprCommon;
  case EXPR_VARIABLE:
    ADD_CHILD(Variable, var);
    ADD_CHILD(Variable, forward);
    goto LExprCommon;
  case EXPR_VARINIT:
    AST_ADD_CHILD(VarInitExpr, expr);
    goto LExprCommon;
  case EXPR_USE:
  case EXPR_WITH:
  case EXPR_USERINIT:
    AST_ADD_CHILD(UserInitExpr, expr);
    goto LExprCommon;
  case EXPR_DEF:
    ADD_CHILD(DefExpr, sym);
    AST_ADD_CHILD(DefExpr, init);
    AST_ADD_CHILD(DefExpr, exprType);
    goto LExprCommon;
  case EXPR_UNOP:
    AST_ADD_CHILD(UnOp, operand);
    goto LExprCommon;
  case EXPR_BINOP:
  LBinOpCommon:
    AST_ADD_CHILD(BinOp, left);
    AST_ADD_CHILD(BinOp, right);
    goto LExprCommon;
  case EXPR_SPECIALBINOP:
    goto LBinOpCommon;
  case EXPR_ASSIGNOP:
    goto LBinOpCommon;
  case EXPR_SEQ:
    AST_ADD_LIST(SeqExpr, exprls, Expr);
    goto LExprCommon;
  case EXPR_SIMPLESEQ:
    AST_ADD_CHILD(SimpleSeqExpr, lo);
    AST_ADD_CHILD(SimpleSeqExpr, hi);
    AST_ADD_CHILD(SimpleSeqExpr, str);
    goto LExprCommon;
  case EXPR_FLOOD:
    goto LExprCommon;
  case EXPR_COMPLETEDIM:
    goto LExprCommon;
  case EXPR_LET:
    AST_ADD_LIST(LetExpr, symDefs, DefExpr);
    AST_ADD_CHILD(LetExpr, innerExpr);
    goto LExprCommon;
  case EXPR_COND:
    AST_ADD_CHILD(CondExpr, condExpr);
    AST_ADD_CHILD(CondExpr, thenExpr);
    AST_ADD_CHILD(CondExpr, elseExpr);
    goto LExprCommon;
  case EXPR_FORALL:
    AST_ADD_LIST(ForallExpr, domains, Expr);
    AST_ADD_LIST(ForallExpr, indices, Expr);
    AST_ADD_CHILD(ForallExpr, forallExpr);
    goto LExprCommon;
  case EXPR_SIZEOF:
    AST_ADD_CHILD(SizeofExpr, variable);
    goto LExprCommon;
  case EXPR_PARENOP:
  LParenOpCommon:
    AST_ADD_CHILD(ParenOpExpr, baseExpr);
    AST_ADD_LIST(ParenOpExpr, argList, Expr);
    goto LExprCommon;
  case EXPR_CAST:
    ADD_CHILD(CastExpr, newType);
    AST_ADD_CHILD(CastExpr, expr);
    goto LExprCommon;
  case EXPR_CAST_LIKE:
    AST_ADD_CHILD(CastLikeExpr, variable);
    AST_ADD_CHILD(CastLikeExpr, expr);
    goto LExprCommon;
  case EXPR_FNCALL:
  case EXPR_ARRAYREF:
  case EXPR_TUPLESELECT:
    goto LParenOpCommon;
  case EXPR_MEMBERACCESS:
    AST_ADD_CHILD(MemberAccess, base);
    ADD_CHILD(MemberAccess, member);
    ADD_CHILD(MemberAccess, member_type);
    goto LExprCommon;
  case EXPR_REDUCE:
    ADD_CHILD(ReduceExpr, reduceType);
    AST_ADD_LIST(ReduceExpr, redDim, Expr);
    AST_ADD_CHILD(ReduceExpr, argExpr);
    goto LExprCommon;
  case EXPR_TUPLE:
    AST_ADD_LIST(Tuple, exprs, Expr);
    goto LExprCommon;
  case EXPR_NAMED:
    AST_ADD_CHILD(NamedExpr, actual);
    goto LExprCommon;
  case SYMBOL: case SYMBOL_UNRESOLVED: 
  LSymbolCommon:
    ADD_CHILD(Symbol, type);
    break;
  case SYMBOL_MODULE:
    ADD_LIST(ModuleSymbol, stmts, Stmt);
    ADD_CHILD(ModuleSymbol, initFn);
    goto LSymbolCommon;
  case SYMBOL_VAR: 
    ADD_CHILD(VarSymbol, aspect);
    goto LSymbolCommon;
  case SYMBOL_PARAM: 
    ADD_CHILD(ParamSymbol, typeVariable);
    goto LSymbolCommon;
  case SYMBOL_TYPE: goto LSymbolCommon;
  case SYMBOL_FN:
    ADD_LIST(FnSymbol, formals, DefExpr);
    AST_ADD_CHILD(FnSymbol, body);
    ADD_CHILD(FnSymbol, retType);
    ADD_CHILD(FnSymbol, _this);
    ADD_CHILD(FnSymbol, _setter);
    ADD_CHILD(FnSymbol, _getter);
    AST_ADD_CHILD(FnSymbol, whereExpr);
    goto LSymbolCommon;
  case SYMBOL_ENUM: case SYMBOL_LABEL: case SYMBOL_FORWARDING:
    goto LSymbolCommon;
  case TYPE:
  LTypeCommon:
    ADD_CHILD(Type, symbol);
    ADD_CHILD(Type, defaultVal);
    ADD_CHILD(Type, defaultConstructor);
    ADD_CHILD(Type, parentType);
    ADD_CHILD(Type, metaType);
    break;
  case TYPE_BUILTIN:
  case TYPE_FN:
    goto LTypeCommon;
  case TYPE_ENUM:
    ADD_LIST(EnumType, valList, EnumSymbol);
    goto LTypeCommon;
  case TYPE_DOMAIN:
    ADD_CHILD(DomainType, parent);
    ADD_CHILD(DomainType, initExpr);
    ADD_CHILD(DomainType, idxType);
    goto LTypeCommon;
  case TYPE_INDEX:
    ADD_CHILD(IndexType, idxExpr);
    ADD_CHILD(IndexType, domainType);
    ADD_CHILD(IndexType, idxType);
    goto LTypeCommon;
  case TYPE_ARRAY:
    ADD_CHILD(ArrayType, domain);
    ADD_CHILD(ArrayType, domainType);
    ADD_CHILD(ArrayType, elementType);
    goto LTypeCommon;
  case TYPE_USER:
    ADD_CHILD(UserType, definition);
    goto LTypeCommon;
  case TYPE_LIKE:
    ADD_CHILD(LikeType, expr);
    goto LTypeCommon;
  case TYPE_STRUCTURAL:
  LStructuralTypeCommon:
    ADD_LIST(StructuralType, declarationList, Stmt);
    ADD_CHILD(StructuralType, parentStruct);
    ADD_VEC(StructuralType, fields, VarSymbol);
    ADD_VEC(StructuralType, types, TypeSymbol);
    goto LTypeCommon;
  case TYPE_CLASS:
    ADD_VEC(ClassType, parentClasses, ClassType);
    goto LStructuralTypeCommon;
  case TYPE_RECORD:
    goto LStructuralTypeCommon;
  case TYPE_UNION:
    ADD_CHILD(UnionType, fieldSelector);
    goto LStructuralTypeCommon;
  case TYPE_TUPLE:
    ADD_VEC(TupleType, components, Type);
    goto LStructuralTypeCommon;
  case TYPE_META:
    ADD_CHILD(MetaType, base);
    goto LTypeCommon;
  case TYPE_SUM:
    ADD_VEC(SumType, components, Type);
    goto LTypeCommon;
  case TYPE_VARIABLE:
    ADD_CHILD(VariableType, type);
    goto LTypeCommon;
  case TYPE_EXPR:
    ADD_CHILD(ExprType, expr);
    goto LTypeCommon;
  case TYPE_UNRESOLVED:
  case TYPE_NIL:
    goto LTypeCommon;
  case AST_TYPE_END: break;
  case LIST: 
    INT_FATAL(a, "Unexpected case in AST_GET_CHILDREN (LIST)");
    break;
  }
}
#undef AST_ADD_CHILD
#undef AST_ADD_LIST
#undef AST_ADD_VEC
#undef ADD_CHILD
#undef ADD_LIST
#undef ADD_VEC


