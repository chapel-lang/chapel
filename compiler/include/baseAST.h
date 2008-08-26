#ifndef _BASEAST_H_
#define _BASEAST_H_

#include "chpl.h"
#include "stringutil.h"

class AList;
class Symbol;
class ModuleSymbol;
class FnSymbol;
class TypeSymbol;
class VarSymbol;
class Type;
class Expr;

typedef Map<Symbol*,Symbol*> SymbolMap;
typedef MapElem<Symbol*,Symbol*> SymbolMapElem;

extern void update_symbols(BaseAST* ast, SymbolMap* map);

extern Vec<BaseAST*> gAsts;
extern Vec<FnSymbol*> gFns;
extern Vec<TypeSymbol*> gTypes;

void cleanAst(void);
void destroyAst(void);
void printStatistics(const char* pass);

/**
 **  Note: update AstTag and astTagName together always.
 **/
enum AstTag {
  EXPR,
  EXPR_SYM,
  EXPR_DEF,
  EXPR_CALL,
  EXPR_NAMED,
  STMT_BLOCK,
  STMT_COND,
  STMT_GOTO,

  SYMBOL,
  SYMBOL_MODULE,
  SYMBOL_VAR,
  SYMBOL_ARG,
  SYMBOL_TYPE,
  SYMBOL_FN,
  SYMBOL_ENUM,
  SYMBOL_LABEL,

  TYPE,
  TYPE_PRIMITIVE,
  TYPE_ENUM,
  TYPE_CLASS,

  BASE
};

extern const char* astTagName[];

#define DECLARE_COPY(type)                                              \
  virtual type* copy(SymbolMap* map = NULL, bool internal = false) {    \
    SymbolMap localMap;                                                 \
    if (!map)                                                           \
      map = &localMap;                                                  \
    type* _this = copyInner(map);                                       \
    _this->lineno = lineno;                                             \
    if (!internal)                                                      \
      update_symbols(_this, map);                                       \
    return _this;                                                       \
  }                                                                     \
  virtual type* copyInner(SymbolMap* map)

#define DECLARE_SYMBOL_COPY(type)                                       \
  virtual type* copy(SymbolMap* map = NULL, bool internal = false) {    \
    SymbolMap localMap;                                                 \
    if (!map)                                                           \
      map = &localMap;                                                  \
    type* _this = copyInner(map);                                       \
    _this->lineno = lineno;                                             \
    _this->copyFlags(this);                                           \
    map->put(this, _this);                                              \
    if (!internal)                                                      \
      update_symbols(_this, map);                                       \
    return _this;                                                       \
  }                                                                     \
  virtual type* copyInner(SymbolMap* map)

#define COPY_INT(c) (c ? c->copy(map, true) : NULL)

class BaseAST {
 public:
  AstTag astTag;    // BaseAST subclass
  int id;               // Unique ID

  int lineno;           // line number of location

  BaseAST(AstTag type = BASE);
  virtual ~BaseAST() { }
  DECLARE_COPY(BaseAST);

  virtual void verify(); 

  virtual void codegen(FILE* outfile);

  virtual bool inTree(void);
  virtual Type* typeInfo(void);

  const char* stringLoc(void);

  ModuleSymbol* getModule();
  FnSymbol* getFunction();
};

extern int currentLineno;

#define SET_LINENO(ast) currentLineno = ast->lineno;

extern Vec<ModuleSymbol*> allModules;     // Contains all modules
extern Vec<ModuleSymbol*> userModules;    // Contains user modules

void registerModule(ModuleSymbol* mod);

//
// class test macros: determine the dynamic type of a BaseAST*
//
#define isExpr(ast)                                                     \
  ((ast) && (ast)->astTag >= EXPR && (ast)->astTag <= STMT_GOTO)

#define isSymExpr(ast)   ((ast) && (ast)->astTag == EXPR_SYM)
#define isDefExpr(ast)   ((ast) && (ast)->astTag == EXPR_DEF)
#define isCallExpr(ast)  ((ast) && (ast)->astTag == EXPR_CALL)
#define isNamedExpr(ast) ((ast) && (ast)->astTag == EXPR_NAMED)
#define isBlockStmt(ast) ((ast) && (ast)->astTag == STMT_BLOCK)
#define isCondStmt(ast)  ((ast) && (ast)->astTag == STMT_COND)
#define isGotoStmt(ast)  ((ast) && (ast)->astTag == STMT_GOTO)

#define isSymbol(ast)                                                   \
  ((ast) && (ast)->astTag >= SYMBOL && (ast)->astTag <= SYMBOL_LABEL)

#define isModuleSymbol(ast)     ((ast) && (ast)->astTag == SYMBOL_MODULE)
#define isVarSymbol(ast)        ((ast) && (ast)->astTag == SYMBOL_VAR)
#define isArgSymbol(ast)        ((ast) && (ast)->astTag == SYMBOL_ARG)
#define isTypeSymbol(ast)       ((ast) && (ast)->astTag == SYMBOL_TYPE)
#define isFnSymbol(ast)         ((ast) && (ast)->astTag == SYMBOL_FN)
#define isEnumSymbol(ast)       ((ast) && (ast)->astTag == SYMBOL_ENUM)
#define isLabelSymbol(ast)      ((ast) && (ast)->astTag == SYMBOL_LABEL)

#define isType(ast)                                                     \
  ((ast) && (ast)->astTag >= TYPE && (ast)->astTag <= TYPE_CLASS)

#define isPrimitiveType(ast) ((ast) && (ast)->astTag == TYPE_PRIMITIVE)
#define isEnumType(ast)      ((ast) && (ast)->astTag == TYPE_ENUM)
#define isOpaqueType(ast)    (ast == dtOpaque)
#define isClassType(ast)     ((ast) && (ast)->astTag == TYPE_CLASS)

//
// safe downcast macros: downcast BaseAST*, Expr*, Symbol*, or Type*
//   note: toDerivedClass is equivalent to dynamic_cast<DerivedClass*>
//
#define toExpr(ast)      (isExpr(ast)      ? ((Expr*)(ast))      : NULL)
#define toSymExpr(ast)   (isSymExpr(ast)   ? ((SymExpr*)(ast))   : NULL)
#define toDefExpr(ast)   (isDefExpr(ast)   ? ((DefExpr*)(ast))   : NULL)
#define toCallExpr(ast)  (isCallExpr(ast)  ? ((CallExpr*)(ast))  : NULL)
#define toNamedExpr(ast) (isNamedExpr(ast) ? ((NamedExpr*)(ast)) : NULL)
#define toBlockStmt(ast) (isBlockStmt(ast) ? ((BlockStmt*)(ast)) : NULL)
#define toCondStmt(ast)  (isCondStmt(ast)  ? ((CondStmt*)(ast))  : NULL)
#define toGotoStmt(ast)  (isGotoStmt(ast)  ? ((GotoStmt*)(ast))  : NULL)

#define toSymbol(ast)           (isSymbol(ast)           ? ((Symbol*)(ast))           : NULL)
#define toModuleSymbol(ast)     (isModuleSymbol(ast)     ? ((ModuleSymbol*)(ast))     : NULL)
#define toVarSymbol(ast)        (isVarSymbol(ast)        ? ((VarSymbol*)(ast))        : NULL)
#define toArgSymbol(ast)        (isArgSymbol(ast)        ? ((ArgSymbol*)(ast))        : NULL)
#define toTypeSymbol(ast)       (isTypeSymbol(ast)       ? ((TypeSymbol*)(ast))       : NULL)
#define toFnSymbol(ast)         (isFnSymbol(ast)         ? ((FnSymbol*)(ast))         : NULL)
#define toEnumSymbol(ast)       (isEnumSymbol(ast)       ? ((EnumSymbol*)(ast))       : NULL)
#define toLabelSymbol(ast)      (isLabelSymbol(ast)      ? ((LabelSymbol*)(ast))      : NULL)

#define toType(ast)          (isType(ast)          ? ((Type*)(ast))          : NULL)
#define toPrimitiveType(ast) (isPrimitiveType(ast) ? ((PrimitiveType*)(ast)) : NULL)
#define toEnumType(ast)      (isEnumType(ast)      ? ((EnumType*)(ast))      : NULL)
#define toClassType(ast)     (isClassType(ast)     ? ((ClassType*)(ast))     : NULL)



#define AST_CALL_CHILD(_a, _t, _m, call, ...)                           \
  if (((_t*)_a)->_m) {                                                  \
    BaseAST* next_ast = ((_t*)_a)->_m;                                  \
    call(next_ast, __VA_ARGS__);                                        \
  }

#define AST_CALL_LIST(_a, _t, _m, call, ...)                            \
  for_alist(next_ast, ((_t*)_a)->_m) {                                  \
    call(next_ast, __VA_ARGS__);                                        \
  }

#define AST_CHILDREN_CALL(_a, call, ...)                                \
  switch (_a->astTag) {                                                 \
  case EXPR_CALL:                                                       \
    AST_CALL_CHILD(_a, CallExpr, baseExpr, call, __VA_ARGS__);          \
    AST_CALL_LIST(_a, CallExpr, argList, call, __VA_ARGS__);            \
    break;                                                              \
  case EXPR_NAMED:                                                      \
    AST_CALL_CHILD(_a, NamedExpr, actual, call, __VA_ARGS__);           \
    break;                                                              \
  case EXPR_DEF:                                                        \
    AST_CALL_CHILD(_a, DefExpr, init, call, __VA_ARGS__);               \
    AST_CALL_CHILD(_a, DefExpr, exprType, call, __VA_ARGS__);           \
    AST_CALL_CHILD(_a, DefExpr, sym, call, __VA_ARGS__);                \
    break;                                                              \
  case STMT_BLOCK:                                                      \
    AST_CALL_LIST(_a, BlockStmt, body, call, __VA_ARGS__);              \
    AST_CALL_CHILD(_a, BlockStmt, blockInfo, call, __VA_ARGS__);         \
    AST_CALL_CHILD(_a, BlockStmt, modUses, call, __VA_ARGS__);          \
    break;                                                              \
  case STMT_COND:                                                       \
    AST_CALL_CHILD(_a, CondStmt, condExpr, call, __VA_ARGS__);          \
    AST_CALL_CHILD(_a, CondStmt, thenStmt, call, __VA_ARGS__);          \
    AST_CALL_CHILD(_a, CondStmt, elseStmt, call, __VA_ARGS__);          \
    break;                                                              \
  case STMT_GOTO:                                                       \
    AST_CALL_CHILD(_a, GotoStmt, label, call, __VA_ARGS__);             \
    break;                                                              \
  case SYMBOL_MODULE:                                                   \
    AST_CALL_CHILD(_a, ModuleSymbol, block, call, __VA_ARGS__);         \
    break;                                                              \
  case SYMBOL_ARG:                                                      \
    AST_CALL_CHILD(_a, ArgSymbol, typeExpr, call, __VA_ARGS__);         \
    AST_CALL_CHILD(_a, ArgSymbol, defaultExpr, call, __VA_ARGS__);      \
    AST_CALL_CHILD(_a, ArgSymbol, variableExpr, call, __VA_ARGS__);     \
    break;                                                              \
  case SYMBOL_TYPE:                                                     \
    AST_CALL_CHILD(_a, Symbol, type, call, __VA_ARGS__);                \
    break;                                                              \
  case SYMBOL_FN:                                                       \
    AST_CALL_LIST(_a, FnSymbol, formals, call, __VA_ARGS__);            \
    AST_CALL_CHILD(_a, FnSymbol, setter, call, __VA_ARGS__);            \
    AST_CALL_CHILD(_a, FnSymbol, body, call, __VA_ARGS__);              \
    AST_CALL_CHILD(_a, FnSymbol, where, call, __VA_ARGS__);             \
    AST_CALL_CHILD(_a, FnSymbol, retExprType, call, __VA_ARGS__);       \
    break;                                                              \
  case TYPE_ENUM:                                                       \
    AST_CALL_LIST(_a, EnumType, constants, call, __VA_ARGS__);          \
    break;                                                              \
  case TYPE_CLASS:                                                      \
    AST_CALL_LIST(_a, ClassType, fields, call, __VA_ARGS__);            \
    AST_CALL_LIST(_a, ClassType, inherits, call, __VA_ARGS__);          \
    break;                                                              \
  default:                                                              \
    break;                                                              \
  }

#define AST_ADD_CHILD(_asts, _a, _t, _m)                                \
  if (((_t*)_a)->_m) {                                                  \
    _asts.add(((_t*)_a)->_m);                                           \
  }

#define AST_ADD_LIST(_asts, _a, _t, _m)                                 \
  for_alist(next_ast, ((_t*)_a)->_m) {                                  \
    _asts.add(next_ast);                                                \
  }

#define AST_CHILDREN_PUSH(_asts, _a)                                    \
  switch (_a->astTag) {                                                 \
  case EXPR_CALL:                                                       \
    AST_ADD_CHILD(_asts, _a, CallExpr, baseExpr);                       \
    AST_ADD_LIST(_asts, _a, CallExpr, argList);                         \
    break;                                                              \
  case EXPR_NAMED:                                                      \
    AST_ADD_CHILD(_asts, _a, NamedExpr, actual);                        \
    break;                                                              \
  case EXPR_DEF:                                                        \
    AST_ADD_CHILD(_asts, _a, DefExpr, init);                            \
    AST_ADD_CHILD(_asts, _a, DefExpr, exprType);                        \
    AST_ADD_CHILD(_asts, _a, DefExpr, sym);                             \
    break;                                                              \
  case STMT_BLOCK:                                                      \
    AST_ADD_LIST(_asts, _a, BlockStmt, body);                           \
    AST_ADD_CHILD(_asts, _a, BlockStmt, blockInfo);                      \
    break;                                                              \
  case STMT_COND:                                                       \
    AST_ADD_CHILD(_asts, _a, CondStmt, condExpr);                       \
    AST_ADD_CHILD(_asts, _a, CondStmt, thenStmt);                       \
    AST_ADD_CHILD(_asts, _a, CondStmt, elseStmt);                       \
    break;                                                              \
  case STMT_GOTO:                                                       \
    AST_ADD_CHILD(_asts, _a, GotoStmt, label);                          \
    break;                                                              \
  case SYMBOL_MODULE:                                                   \
    AST_ADD_CHILD(_asts, _a, ModuleSymbol, block);                      \
    break;                                                              \
  case SYMBOL_ARG:                                                      \
    AST_ADD_CHILD(_asts, _a, ArgSymbol, typeExpr);                      \
    AST_ADD_CHILD(_asts, _a, ArgSymbol, defaultExpr);                   \
    AST_ADD_CHILD(_asts, _a, ArgSymbol, variableExpr);                  \
    break;                                                              \
  case SYMBOL_TYPE:                                                     \
    AST_ADD_CHILD(_asts, _a, Symbol, type);                             \
    break;                                                              \
  case SYMBOL_FN:                                                       \
    AST_ADD_LIST(_asts, _a, FnSymbol, formals);                         \
    AST_ADD_CHILD(_asts, _a, FnSymbol, setter);                         \
    AST_ADD_CHILD(_asts, _a, FnSymbol, body);                           \
    AST_ADD_CHILD(_asts, _a, FnSymbol, where);                          \
    AST_ADD_CHILD(_asts, _a, FnSymbol, retExprType);                    \
    break;                                                              \
  case TYPE_ENUM:                                                       \
    AST_ADD_LIST(_asts, _a, EnumType, constants);                       \
    break;                                                              \
  case TYPE_CLASS:                                                      \
    AST_ADD_LIST(_asts, _a, ClassType, fields);                         \
    AST_ADD_LIST(_asts, _a, ClassType, inherits);                       \
    break;                                                              \
  default:                                                              \
    break;                                                              \
  }

#define AST_CHILDREN_POP(_asts, _a)             \
  _a = _asts.pop()                              \

#endif
