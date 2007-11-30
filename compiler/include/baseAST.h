#ifndef _BASEAST_H_
#define _BASEAST_H_

#include "chpl.h"
#include "stringutil.h"

typedef Map<BaseAST*,BaseAST*> ASTMap;
typedef MapElem<BaseAST*,BaseAST*> ASTMapElem;

extern void update_symbols(BaseAST* ast, ASTMap* map);
void cleanAst(void);
void destroyAst(void);

void printStatistics(const char* pass);

class AList;
class Symbol;
class ModuleSymbol;
class FnSymbol;
class TypeSymbol;
class VarSymbol;
class Type;
class Expr;
class SymScope;

extern Vec<BaseAST*> gAsts;
extern Vec<FnSymbol*> gFns;
extern Vec<TypeSymbol*> gTypes;

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
  SYMBOL_UNRESOLVED,
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
  TYPE_USER,
  TYPE_CLASS,

  BASE
};

extern const char* astTagName[];

#define COPY_DEF(type)                                                  \
  virtual type* copy(ASTMap* map = NULL, bool internal = false) {       \
    ASTMap localMap;                                                    \
    if (!map)                                                           \
      map = &localMap;                                                  \
    type* _this = copyInner(map);                                       \
    _this->lineno = lineno;                                             \
    _this->filename = filename;                                         \
    _this->copyPragmas(this);                                           \
    map->put(this, _this);                                              \
    if (!internal)                                                      \
      update_symbols(_this, map);                                       \
    return _this;                                                       \
  }                                                                     \
  virtual type* copyInner(ASTMap* map)

#define COPY_INT(c) (c ? c->copy(map, true) : NULL)

typedef struct _ASTContext {
  SymScope* parentScope;
  Symbol* parentSymbol;
  Expr* parentExpr;
} ASTContext;

class BaseAST {
 public:
  AstTag astTag;    // BaseAST subclass
  int id;               // Unique ID

  SymScope* parentScope;

  const char* filename;       // filename of location
  int lineno;           // line number of location

  BaseAST(AstTag type = BASE);
  virtual ~BaseAST() { }
  COPY_DEF(BaseAST);

  virtual void verify(); 

  virtual void codegen(FILE* outfile);

  virtual bool inTree(void);
  virtual Type* typeInfo(void);

  const char* stringLoc(void);
  void printLoc(FILE* outfile);

  void addPragma(const char* str);
  void addPragmas(Vec<const char*>* srcPragmas);
  void copyPragmas(BaseAST* ast);

  ModuleSymbol* getModule();
  FnSymbol* getFunction();

  Symbol* lookup(const char*);
  Symbol* lookup(BaseAST*);
  TypeSymbol* lookupType(const char*);
  TypeSymbol* lookupType(BaseAST*);
  VarSymbol* lookupVar(const char*);
  VarSymbol* lookupVar(BaseAST*);
};

#define forv_BaseAST(_p, _v) forv_Vec(BaseAST, _p, _v)

int compar_baseast(const void *ai, const void *aj);
// for use with qsort

void get_ast_children(BaseAST *a, Vec<BaseAST *> &asts);

extern int currentLineno;
extern const char* currentFilename;

extern SymScope* rootScope; // contains intrinsics and all modules

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

#define isUnresolvedSymbol(ast) ((ast) && (ast)->astTag == SYMBOL_UNRESOLVED)
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
#define isUserType(ast)      ((ast) && (ast)->astTag == TYPE_USER)
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
#define toUnresolvedSymbol(ast) (isUnresolvedSymbol(ast) ? ((UnresolvedSymbol*)(ast)) : NULL)
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
#define toUserType(ast)      (isUserType(ast)      ? ((UserType*)(ast))      : NULL)
#define toClassType(ast)     (isClassType(ast)     ? ((ClassType*)(ast))     : NULL)

#endif
