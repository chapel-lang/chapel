#ifndef _BASEAST_H_
#define _BASEAST_H_

#include "chpl.h"
#include "stringutil.h"

#define foreach_ast_sep(macro, sep)                \
  macro(PrimitiveType) sep                         \
  macro(EnumType) sep                              \
  macro(ClassType) sep                             \
  macro(ModuleSymbol) sep                          \
  macro(VarSymbol) sep                             \
  macro(ArgSymbol) sep                             \
  macro(TypeSymbol) sep                            \
  macro(FnSymbol) sep                              \
  macro(EnumSymbol) sep                            \
  macro(LabelSymbol) sep                           \
  macro(SymExpr) sep                               \
  macro(UnresolvedSymExpr) sep                     \
  macro(DefExpr) sep                               \
  macro(CallExpr) sep                              \
  macro(NamedExpr) sep                             \
  macro(BlockStmt) sep                             \
  macro(CondStmt) sep                              \
  macro(GotoStmt)

#define foreach_ast(macro)                      \
  foreach_ast_sep(macro, ;)

class AList;
class Symbol;
class Type;
class Expr;

//
// prototype ast classes SymExpr, CallExpr, FnSymbol, ...
//
#define proto_classes(type) class type
foreach_ast(proto_classes);
#undef proto_classes

//
// declare global vectors gSymExprs, gCallExprs, gFnSymbols, ...
//
#define decl_gvecs(type) extern Vec<type*> g##type##s
foreach_ast(decl_gvecs);
#undef decl_gvecs

typedef Map<Symbol*,Symbol*> SymbolMap;
typedef MapElem<Symbol*,Symbol*> SymbolMapElem;

extern void update_symbols(BaseAST* ast, SymbolMap* map);

void cleanAst(void);
void destroyAst(void);
void printStatistics(const char* pass);

/**
 **  Note: update AstTag and astTagName together always.
 **/
enum AstTag {
  E_SymExpr,
  E_UnresolvedSymExpr,
  E_DefExpr,
  E_CallExpr,
  E_NamedExpr,
  E_BlockStmt,
  E_CondStmt,
  E_GotoStmt,
  E_Expr,

  E_ModuleSymbol,
  E_VarSymbol,
  E_ArgSymbol,
  E_TypeSymbol,
  E_FnSymbol,
  E_EnumSymbol,
  E_LabelSymbol,
  E_Symbol,

  E_PrimitiveType,
  E_EnumType,
  E_ClassType,
  E_Type,

  E_BaseAST
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

  BaseAST(AstTag type);
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
  ((ast) && (ast)->astTag < E_Expr)

#define isSymExpr(ast)   ((ast) && (ast)->astTag == E_SymExpr)
#define isUnresolvedSymExpr(ast)   ((ast) && (ast)->astTag == E_UnresolvedSymExpr)
#define isDefExpr(ast)   ((ast) && (ast)->astTag == E_DefExpr)
#define isCallExpr(ast)  ((ast) && (ast)->astTag == E_CallExpr)
#define isNamedExpr(ast) ((ast) && (ast)->astTag == E_NamedExpr)
#define isBlockStmt(ast) ((ast) && (ast)->astTag == E_BlockStmt)
#define isCondStmt(ast)  ((ast) && (ast)->astTag == E_CondStmt)
#define isGotoStmt(ast)  ((ast) && (ast)->astTag == E_GotoStmt)

#define isSymbol(ast)                                                   \
  ((ast) && (ast)->astTag > E_Expr && (ast)->astTag < E_Symbol)

#define isModuleSymbol(ast)     ((ast) && (ast)->astTag == E_ModuleSymbol)
#define isVarSymbol(ast)        ((ast) && (ast)->astTag == E_VarSymbol)
#define isArgSymbol(ast)        ((ast) && (ast)->astTag == E_ArgSymbol)
#define isTypeSymbol(ast)       ((ast) && (ast)->astTag == E_TypeSymbol)
#define isFnSymbol(ast)         ((ast) && (ast)->astTag == E_FnSymbol)
#define isEnumSymbol(ast)       ((ast) && (ast)->astTag == E_EnumSymbol)
#define isLabelSymbol(ast)      ((ast) && (ast)->astTag == E_LabelSymbol)

#define isType(ast)                                                     \
  ((ast) && (ast)->astTag > E_Symbol && (ast)->astTag < E_Type)

#define isPrimitiveType(ast) ((ast) && (ast)->astTag == E_PrimitiveType)
#define isEnumType(ast)      ((ast) && (ast)->astTag == E_EnumType)
#define isClassType(ast)     ((ast) && (ast)->astTag == E_ClassType)

//
// safe downcast macros: downcast BaseAST*, Expr*, Symbol*, or Type*
//   note: toDerivedClass is equivalent to dynamic_cast<DerivedClass*>
//
#define toExpr(ast)      (isExpr(ast)      ? ((Expr*)(ast))      : NULL)
#define toSymExpr(ast)   (isSymExpr(ast)   ? ((SymExpr*)(ast))   : NULL)
#define toUnresolvedSymExpr(ast)   (isUnresolvedSymExpr(ast)   ? ((UnresolvedSymExpr*)(ast))   : NULL)
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
  case E_CallExpr:                                                       \
    AST_CALL_CHILD(_a, CallExpr, baseExpr, call, __VA_ARGS__);          \
    AST_CALL_LIST(_a, CallExpr, argList, call, __VA_ARGS__);            \
    break;                                                              \
  case E_NamedExpr:                                                      \
    AST_CALL_CHILD(_a, NamedExpr, actual, call, __VA_ARGS__);           \
    break;                                                              \
  case E_DefExpr:                                                        \
    AST_CALL_CHILD(_a, DefExpr, init, call, __VA_ARGS__);               \
    AST_CALL_CHILD(_a, DefExpr, exprType, call, __VA_ARGS__);           \
    AST_CALL_CHILD(_a, DefExpr, sym, call, __VA_ARGS__);                \
    break;                                                              \
  case E_BlockStmt:                                                      \
    AST_CALL_LIST(_a, BlockStmt, body, call, __VA_ARGS__);              \
    AST_CALL_CHILD(_a, BlockStmt, blockInfo, call, __VA_ARGS__);         \
    AST_CALL_CHILD(_a, BlockStmt, modUses, call, __VA_ARGS__);          \
    break;                                                              \
  case E_CondStmt:                                                       \
    AST_CALL_CHILD(_a, CondStmt, condExpr, call, __VA_ARGS__);          \
    AST_CALL_CHILD(_a, CondStmt, thenStmt, call, __VA_ARGS__);          \
    AST_CALL_CHILD(_a, CondStmt, elseStmt, call, __VA_ARGS__);          \
    break;                                                              \
  case E_GotoStmt:                                                       \
    AST_CALL_CHILD(_a, GotoStmt, label, call, __VA_ARGS__);             \
    break;                                                              \
  case E_ModuleSymbol:                                                   \
    AST_CALL_CHILD(_a, ModuleSymbol, block, call, __VA_ARGS__);         \
    break;                                                              \
  case E_ArgSymbol:                                                      \
    AST_CALL_CHILD(_a, ArgSymbol, typeExpr, call, __VA_ARGS__);         \
    AST_CALL_CHILD(_a, ArgSymbol, defaultExpr, call, __VA_ARGS__);      \
    AST_CALL_CHILD(_a, ArgSymbol, variableExpr, call, __VA_ARGS__);     \
    break;                                                              \
  case E_TypeSymbol:                                                     \
    AST_CALL_CHILD(_a, Symbol, type, call, __VA_ARGS__);                \
    break;                                                              \
  case E_FnSymbol:                                                       \
    AST_CALL_LIST(_a, FnSymbol, formals, call, __VA_ARGS__);            \
    AST_CALL_CHILD(_a, FnSymbol, setter, call, __VA_ARGS__);            \
    AST_CALL_CHILD(_a, FnSymbol, body, call, __VA_ARGS__);              \
    AST_CALL_CHILD(_a, FnSymbol, where, call, __VA_ARGS__);             \
    AST_CALL_CHILD(_a, FnSymbol, retExprType, call, __VA_ARGS__);       \
    break;                                                              \
  case E_EnumType:                                                       \
    AST_CALL_LIST(_a, EnumType, constants, call, __VA_ARGS__);          \
    break;                                                              \
  case E_ClassType:                                                      \
    AST_CALL_LIST(_a, ClassType, fields, call, __VA_ARGS__);            \
    AST_CALL_LIST(_a, ClassType, inherits, call, __VA_ARGS__);          \
    break;                                                              \
  default:                                                              \
    break;                                                              \
  }

#endif
