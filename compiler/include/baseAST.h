#ifndef _BASEAST_H_
#define _BASEAST_H_

#include "chpl.h"

typedef Map<BaseAST*,BaseAST*> ASTMap;
typedef MapElem<BaseAST*,BaseAST*> ASTMapElem;

extern void update_symbols(BaseAST* ast, ASTMap* map);

char* canonicalize_string(char *s);
char* astr(char* s1, char* s2 = NULL, char* s3 = NULL, char* s4 = NULL);

class AList;
class Symbol;
class ModuleSymbol;
class FnSymbol;
class TypeSymbol;
class VarSymbol;
class Type;
class Stmt;
class Expr;
class SymScope;

extern Vec<BaseAST*> gAsts;
extern Vec<FnSymbol*> gFns;
extern Vec<TypeSymbol*> gTypes;

/**
 **  Note: update astType_t and astTypeName together always.
 **/
enum astType_t {
  STMT = 0,
  STMT_EXPR,
  STMT_RETURN,
  STMT_BLOCK,
  STMT_COND,
  STMT_GOTO,

  EXPR,
  EXPR_SYM,
  EXPR_DEF,
  EXPR_CALL,
  EXPR_NAMED,

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

extern char* astTypeName[];

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
  Stmt* parentStmt;
  Expr* parentExpr;
} ASTContext;

class BaseAST {
 public:
  astType_t astType;    // BaseAST subclass
  int id;               // Unique ID

  BaseAST* prev;        // List previous pointer
  BaseAST* next;        // List next pointer
  AList* list;          // List pointer

  SymScope* parentScope;
  Symbol* parentSymbol;

  char* filename;       // filename of location
  int lineno;           // line number of location

  BaseAST(astType_t type = BASE);
  virtual ~BaseAST() { }
  COPY_DEF(BaseAST);

  virtual void verify(); 
  virtual void clean();

  virtual void print(FILE* outfile);
  virtual void printDef(FILE* outfile);
  void println(FILE* outfile);

  virtual void codegen(FILE* outfile);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  virtual void callReplaceChild(BaseAST* new_ast);

  virtual ASTContext getContext(void);
  virtual bool inTree(void);

  BaseAST* remove(void);
  void replace(BaseAST* new_ast);
  virtual void insertBefore(BaseAST* new_ast);
  virtual void insertAfter(BaseAST* new_ast);

  char* stringLoc(void);
  void printLoc(FILE* outfile);

  void addPragma(char* str);
  void addPragmas(Vec<char*>* srcPragmas);
  void copyPragmas(BaseAST* ast);

  ModuleSymbol* getModule();
  FnSymbol* getFunction();

  Symbol* lookup(char*);
  Symbol* lookup(BaseAST*);
  TypeSymbol* lookupType(char*);
  TypeSymbol* lookupType(BaseAST*);
  VarSymbol* lookupVar(char*);
  VarSymbol* lookupVar(BaseAST*);
};

#define forv_BaseAST(_p, _v) forv_Vec(BaseAST, _p, _v)

int compar_baseast(const void *ai, const void *aj);
// for use with qsort

void get_ast_children(BaseAST *a, Vec<BaseAST *> &asts);

extern int currentLineno;
extern char* currentFilename;
extern char* currentTraversal;

extern SymScope* rootScope; // contains intrinsics and all modules

extern Vec<ModuleSymbol*> allModules;     // Contains all modules
extern Vec<ModuleSymbol*> userModules;    // Contains user modules
void registerModule(ModuleSymbol* mod);

#endif
