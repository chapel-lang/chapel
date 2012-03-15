//
// The BaseAST class is the parent class of all AST node types.
//
// Notes on adding, removing, or changing AST node types
// -----------------------------------------------------
//
// 1. When adding or removing AST node types, update AstTag and
//    astTagName so that they are consistent.
//
// 2. Update the traversal macros as necessary.  The traversal must be
//    able to touch all AST nodes when traversing the AST.
//

#ifndef _BASEAST_H_
#define _BASEAST_H_

#include "chpl.h"
#include "stringutil.h"

//
// foreach_ast_sep: invoke a 'macro' for every AST node type,
//                  separating invocations by 'sep'
// foreach_ast: invoke a macro for every AST node type, separating
//              invocations by ;
//
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

//
// prototype Symbol, Type, Expr, and all AST node types
//
class Symbol;
class Type;
class Expr;
#define proto_classes(type) class type
foreach_ast(proto_classes);
#undef proto_classes

//
// declare global vectors for all AST node types
//
// These global vectors, named gSymExprs, gCallExprs, gFnSymbols, ...,
// contain all existing nodes of the given AST node type; they are
// updated automatically as new AST nodes are constructed.  Nodes are
// removed from these vectors between passes.
//
#define decl_gvecs(type) extern Vec<type*> g##type##s
foreach_ast(decl_gvecs);
#undef decl_gvecs

//
// type definitions for common maps
//
typedef Map<Symbol*,Symbol*> SymbolMap;
typedef MapElem<Symbol*,Symbol*> SymbolMapElem;

// get the current AST node id
extern int lastNodeIDUsed();

// trace various AST node removals
extern void trace_remove(BaseAST* ast, char flag);

// how an AST node knows its location in the source code
// (assumed to get copied upon assignment and parameter passing)
struct astlocT {
  const char* filename;  // filename of location
  int lineno;    // line number of location
  astlocT(int linenoArg, const char* filenameArg):
    filename(filenameArg), lineno(linenoArg)
    {}
};

//
// enumerated type of all AST node types (and superclass types)
//
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

//
// string names of all AST node types (used for debugging)
//
extern const char* astTagName[];

//
// macros used to define the copy method on all AST node types, and to
// prototype the copyInner method
//
// The copyInner method is used to do the copy specific to any node.
// The outermost call to copy invokes the copyInner method used to
// implement the recursive copy.
//
#define DECLARE_COPY(type)                                              \
  type* copy(SymbolMap* map = NULL, bool internal = false) {            \
    SymbolMap localMap;                                                 \
    if (!map)                                                           \
      map = &localMap;                                                  \
    type* _this = copyInner(map);                                       \
    _this->astloc = astloc;                                             \
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
    _this->astloc = astloc;                                             \
    _this->copyFlags(this);                                             \
    map->put(this, _this);                                              \
    if (!internal)                                                      \
      update_symbols(_this, map);                                       \
    return _this;                                                       \
  }                                                                     \
  virtual type* copyInner(SymbolMap* map)

//
// macro used to call copy from inside the copyInner method
//
#define COPY_INT(c) (c ? c->copy(map, true) : NULL)

//
// abstract parent of all AST node types
//
class BaseAST {
 public:
  AstTag astTag; // BaseAST subclass
  int id;        // Unique ID
  astlocT astloc; // Location of this node in the source code

  BaseAST(AstTag type);
  virtual ~BaseAST() { }
  virtual void verify() = 0;
  virtual BaseAST* copy(SymbolMap* map = NULL, bool internal = false) = 0;
  virtual BaseAST* copyInner(SymbolMap* map) = 0;
  virtual bool inTree(void) = 0;
  virtual void codegen(FILE* outfile) = 0;

  const char* stringLoc(void);
  ModuleSymbol* getModule();
  FnSymbol* getFunction();
  int linenum() { return astloc.lineno; }
  const char* fname() { return astloc.filename; }

  virtual Type* typeInfo(void) = 0;
  Type* getValType();
  Type* getRefType();
  Type* getWideRefType();
};

//
// macro to update the global line number used to set the line number
// of an AST node when it is constructed
//
// This should be used before constructing new nodes to make sure the
// line number is correctly set.
//
#define SET_LINENO(ast) currentAstLoc = ast->astloc;
extern astlocT currentAstLoc;

//
// vectors of modules
//
extern Vec<ModuleSymbol*> allModules;  // contains all modules
extern Vec<ModuleSymbol*> userModules; // contains main + user modules
extern Vec<ModuleSymbol*> mainModules; // contains main modules

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
// These macros are used to implement the functions that collect all
// of the nodes in some part of the AST, e.g., collectSymExprs.  They
// can also be used to define recursive functions that work over a
// portion of the AST.  See collectSymExprs for a simple example.
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

//
// clean IR between passes by clearing some back pointers to dead AST
// nodes and removing dead AST nodes from the global vectors of AST
// nodes. "dead" means !isAlive && !isRootModule.
//
void cleanAst(void);

//
// reclaim memory associated with all AST nodes (called at the end)
//
void destroyAst(void);

//
// print memory-related statistics about the IR (called between passes
// if using --print-statistics)
//
void printStatistics(const char* pass);

void registerModule(ModuleSymbol* mod);

//
// update_symbols: substitute symbol uses in 'ast' using 'map'
//
// This function is primarily used when AST::copy is called.  If a
// Block containing a def and a use of that def is copied, then the
// def will be copied, and this routine will update the use to the new
// def.
//
void update_symbols(BaseAST* ast, SymbolMap* map);

#endif
