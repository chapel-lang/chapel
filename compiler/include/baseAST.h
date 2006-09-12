#ifndef _BASEAST_H_
#define _BASEAST_H_

#include "chpl.h"

char* canonicalize_string(char *s);

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
  TYPE_FN,
  TYPE_ENUM,
  TYPE_USER,
  TYPE_CLASS,

  AST_TYPE_END 
};

extern char* astTypeName[];

#define COPY_DEF(type)                                   \
  virtual type* copy(Map<BaseAST*,BaseAST*>* map = NULL, \
                     bool internal = false) {            \
    preCopy(map, internal);                              \
    type* _this = copyInner(map);                        \
    postCopy(_this, map, internal);                      \
    return _this;                                        \
  }                                                      \
  virtual type* copyInner(Map<BaseAST*,BaseAST*>* map)

#define COPY(c) (c ? c->copy() : NULL)
#define COPY_INT(c) (c ? c->copy(map, true) : NULL)

typedef struct _ASTContext {
  SymScope* parentScope;
  Symbol* parentSymbol;
  Stmt* parentStmt;
  Expr* parentExpr;
} ASTContext;

class BaseAST : public gc {
 public:
  astType_t astType;    // BaseAST subclass
  long id;              // Unique ID

  BaseAST* prev;        // List previous pointer
  BaseAST* next;        // List next pointer

  SymScope* parentScope;
  Symbol* parentSymbol;

  char* filename;       // filename of location
  int lineno;           // line number of location

  Vec<ModuleSymbol*> modUses; // modules used (BlockStmt,FnSymbol,...)
                              // via SymScope::astParent

  static long getNumIDs(void);

  BaseAST(void);
  BaseAST(astType_t type);
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

  BaseAST* remove(void);
  void replace(BaseAST* new_ast);
  void insertBefore(BaseAST* new_ast);
  void insertAfter(BaseAST* new_ast);

// need to put this as default value to copy for new interface
//    new ASTMap();
  void preCopy(Map<BaseAST*,BaseAST*>*& map, bool internal);
  void postCopy(BaseAST* copy, Map<BaseAST*,BaseAST*>* map, bool internal);

  char* stringLoc(void);
  void printLoc(FILE* outfile);

  void addPragma(char* str);
  void addPragmas(Vec<char*>* srcPragmas);

  ModuleSymbol* getModule();
  FnSymbol* getFunction();

  Symbol* lookup(char*);
  Symbol* lookup(BaseAST*);
  TypeSymbol* lookupType(char*);
  TypeSymbol* lookupType(BaseAST*);
  VarSymbol* lookupVar(char*);
  VarSymbol* lookupVar(BaseAST*);
};

typedef Map<BaseAST*,BaseAST*> ASTMap;
typedef MapElem<BaseAST*,BaseAST*> ASTMapElem;

#define forv_BaseAST(_p, _v) forv_Vec(BaseAST, _p, _v)

int compar_baseast(const void *ai, const void *aj);
// for use with qsort

void get_ast_children(BaseAST *a, Vec<BaseAST *> &asts, int sentinels = 0);

extern int currentLineno;
extern char* currentFilename;
extern char* currentTraversal;

extern SymScope* rootScope; // contains intrinsics and all modules

extern Vec<ModuleSymbol*> allModules;     // Contains all modules
extern Vec<ModuleSymbol*> userModules;    // Contains user modules
void registerModule(ModuleSymbol* mod);

#endif
