#ifndef _BASEAST_H_
#define _BASEAST_H_

#include "chpl.h"

typedef Map<BaseAST*,BaseAST*> ASTMap;
typedef MapElem<BaseAST*,BaseAST*> ASTMapElem;

extern void update_symbols(BaseAST* ast, ASTMap* map);

char* canonicalize_string(const char *s);
char* astr(const char* s1, const char* s2 = NULL,
           const char* s3 = NULL, const char* s4 = NULL);
char* istr(int i);

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
 **  Note: update astType_t and astTypeName together always.
 **/
enum astType_t {
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

  BASE,

  AST_TYPE_END 
};

extern const char* astTypeName[];

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

#define COPY(c) (c ? c->copy() : NULL)
#define COPY_INT(c) (c ? c->copy(map, true) : NULL)

typedef struct _ASTContext {
  SymScope* parentScope;
  Symbol* parentSymbol;
  Expr* parentExpr;
} ASTContext;

class BaseAST {
 public:
  astType_t astType;    // BaseAST subclass
  int id;               // Unique ID

  SymScope* parentScope;

  const char* filename;       // filename of location
  int lineno;           // line number of location

  BaseAST(astType_t type = BASE);
  virtual ~BaseAST() { }
  COPY_DEF(BaseAST);

  virtual void verify(); 
  virtual void clean();

  virtual void codegen(FILE* outfile);

  virtual bool inTree(void);
  virtual Type* typeInfo(void);

  char* stringLoc(void);
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
extern ModuleSymbol* theProgram;
void registerModule(ModuleSymbol* mod);

#endif
