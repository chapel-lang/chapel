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
  type* copy(SymbolMap* map = NULL, bool internal = false) {            \
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
  type* copy(SymbolMap* map = NULL, bool internal = false) {            \
    SymbolMap localMap;                                                 \
    if (!map)                                                           \
      map = &localMap;                                                  \
    type* _this = copyInner(map);                                       \
    _this->lineno = lineno;                                             \
    _this->copyFlags(this);                                             \
    map->put(this, _this);                                              \
    if (!internal)                                                      \
      update_symbols(_this, map);                                       \
    return _this;                                                       \
  }                                                                     \
  virtual type* copyInner(SymbolMap* map)

#define COPY_INT(c) (c ? c->copy(map, true) : NULL)

class BaseAST {
 public:
  AstTag astTag; // BaseAST subclass
  int id;        // Unique ID
  int lineno;    // line number of location

  BaseAST(AstTag type);
  virtual ~BaseAST() { }
  virtual void verify() = 0;
  virtual BaseAST* copy(SymbolMap* map = NULL, bool internal = false) = 0;
  virtual BaseAST* copyInner(SymbolMap* map) = 0;
  virtual bool inTree(void) = 0;
  virtual Type* typeInfo(void) = 0;
  virtual void codegen(FILE* outfile) = 0;

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
#define isExpr(a)   ((a) && (a)->astTag < E_Expr)
#define isSymbol(a) ((a) && (a)->astTag > E_Expr && (a)->astTag < E_Symbol)
#define isType(a)   ((a) && (a)->astTag > E_Symbol && (a)->astTag < E_Type)

#define isSymExpr(a)           ((a) && (a)->astTag == E_SymExpr)
#define isUnresolvedSymExpr(a) ((a) && (a)->astTag == E_UnresolvedSymExpr)
#define isDefExpr(a)           ((a) && (a)->astTag == E_DefExpr)
#define isCallExpr(a)          ((a) && (a)->astTag == E_CallExpr)
#define isNamedExpr(a)         ((a) && (a)->astTag == E_NamedExpr)
#define isBlockStmt(a)         ((a) && (a)->astTag == E_BlockStmt)
#define isCondStmt(a)          ((a) && (a)->astTag == E_CondStmt)
#define isGotoStmt(a)          ((a) && (a)->astTag == E_GotoStmt)
#define isModuleSymbol(a)      ((a) && (a)->astTag == E_ModuleSymbol)
#define isVarSymbol(a)         ((a) && (a)->astTag == E_VarSymbol)
#define isArgSymbol(a)         ((a) && (a)->astTag == E_ArgSymbol)
#define isTypeSymbol(a)        ((a) && (a)->astTag == E_TypeSymbol)
#define isFnSymbol(a)          ((a) && (a)->astTag == E_FnSymbol)
#define isEnumSymbol(a)        ((a) && (a)->astTag == E_EnumSymbol)
#define isLabelSymbol(a)       ((a) && (a)->astTag == E_LabelSymbol)
#define isPrimitiveType(a)     ((a) && (a)->astTag == E_PrimitiveType)
#define isEnumType(a)          ((a) && (a)->astTag == E_EnumType)
#define isClassType(a)         ((a) && (a)->astTag == E_ClassType)

//
// safe downcast macros: downcast BaseAST*, Expr*, Symbol*, or Type*
//   note: toDerivedClass is equivalent to dynamic_cast<DerivedClass*>
//
#define def_to_ast(Type, a)    (is##Type(a) ? ((Type*)(a)) : NULL)
#define toSymExpr(a)           def_to_ast(SymExpr, a)
#define toUnresolvedSymExpr(a) def_to_ast(UnresolvedSymExpr, a)
#define toDefExpr(a)           def_to_ast(DefExpr, a)
#define toCallExpr(a)          def_to_ast(CallExpr, a)
#define toNamedExpr(a)         def_to_ast(NamedExpr, a)
#define toBlockStmt(a)         def_to_ast(BlockStmt, a)
#define toCondStmt(a)          def_to_ast(CondStmt, a)
#define toGotoStmt(a)          def_to_ast(GotoStmt, a)
#define toExpr(a)              def_to_ast(Expr, a)
#define toModuleSymbol(a)      def_to_ast(ModuleSymbol, a)
#define toVarSymbol(a)         def_to_ast(VarSymbol, a)
#define toArgSymbol(a)         def_to_ast(ArgSymbol, a)
#define toTypeSymbol(a)        def_to_ast(TypeSymbol, a)
#define toFnSymbol(a)          def_to_ast(FnSymbol, a)
#define toEnumSymbol(a)        def_to_ast(EnumSymbol, a)
#define toLabelSymbol(a)       def_to_ast(LabelSymbol, a)
#define toSymbol(a)            def_to_ast(Symbol, a)
#define toPrimitiveType(a)     def_to_ast(PrimitiveType, a)
#define toEnumType(a)          def_to_ast(EnumType, a)
#define toClassType(a)         def_to_ast(ClassType, a)
#define toType(a)              def_to_ast(Type, a)

//
// traversal macros
//
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
  case E_CallExpr:                                                      \
    AST_CALL_CHILD(_a, CallExpr, baseExpr, call, __VA_ARGS__);          \
    AST_CALL_LIST(_a, CallExpr, argList, call, __VA_ARGS__);            \
    break;                                                              \
  case E_NamedExpr:                                                     \
    AST_CALL_CHILD(_a, NamedExpr, actual, call, __VA_ARGS__);           \
    break;                                                              \
  case E_DefExpr:                                                       \
    AST_CALL_CHILD(_a, DefExpr, init, call, __VA_ARGS__);               \
    AST_CALL_CHILD(_a, DefExpr, exprType, call, __VA_ARGS__);           \
    AST_CALL_CHILD(_a, DefExpr, sym, call, __VA_ARGS__);                \
    break;                                                              \
  case E_BlockStmt:                                                     \
    AST_CALL_LIST(_a, BlockStmt, body, call, __VA_ARGS__);              \
    AST_CALL_CHILD(_a, BlockStmt, blockInfo, call, __VA_ARGS__);        \
    AST_CALL_CHILD(_a, BlockStmt, modUses, call, __VA_ARGS__);          \
    break;                                                              \
  case E_CondStmt:                                                      \
    AST_CALL_CHILD(_a, CondStmt, condExpr, call, __VA_ARGS__);          \
    AST_CALL_CHILD(_a, CondStmt, thenStmt, call, __VA_ARGS__);          \
    AST_CALL_CHILD(_a, CondStmt, elseStmt, call, __VA_ARGS__);          \
    break;                                                              \
  case E_GotoStmt:                                                      \
    AST_CALL_CHILD(_a, GotoStmt, label, call, __VA_ARGS__);             \
    break;                                                              \
  case E_ModuleSymbol:                                                  \
    AST_CALL_CHILD(_a, ModuleSymbol, block, call, __VA_ARGS__);         \
    break;                                                              \
  case E_ArgSymbol:                                                     \
    AST_CALL_CHILD(_a, ArgSymbol, typeExpr, call, __VA_ARGS__);         \
    AST_CALL_CHILD(_a, ArgSymbol, defaultExpr, call, __VA_ARGS__);      \
    AST_CALL_CHILD(_a, ArgSymbol, variableExpr, call, __VA_ARGS__);     \
    break;                                                              \
  case E_TypeSymbol:                                                    \
    AST_CALL_CHILD(_a, Symbol, type, call, __VA_ARGS__);                \
    break;                                                              \
  case E_FnSymbol:                                                      \
    AST_CALL_LIST(_a, FnSymbol, formals, call, __VA_ARGS__);            \
    AST_CALL_CHILD(_a, FnSymbol, setter, call, __VA_ARGS__);            \
    AST_CALL_CHILD(_a, FnSymbol, body, call, __VA_ARGS__);              \
    AST_CALL_CHILD(_a, FnSymbol, where, call, __VA_ARGS__);             \
    AST_CALL_CHILD(_a, FnSymbol, retExprType, call, __VA_ARGS__);       \
    break;                                                              \
  case E_EnumType:                                                      \
    AST_CALL_LIST(_a, EnumType, constants, call, __VA_ARGS__);          \
    break;                                                              \
  case E_ClassType:                                                     \
    AST_CALL_LIST(_a, ClassType, fields, call, __VA_ARGS__);            \
    AST_CALL_LIST(_a, ClassType, inherits, call, __VA_ARGS__);          \
    break;                                                              \
  default:                                                              \
    break;                                                              \
  }

#endif
