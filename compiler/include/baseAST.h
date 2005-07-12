#ifndef _BASEAST_H_
#define _BASEAST_H_

#include "map.h"

class Symbol;
class Type;
class Traversal;
class SymScope;

/**
 **  Note: update astType_t and astTypeName together always.
 **/
enum astType_t {
  STMT = 0,
  STMT_NOOP,
  STMT_DEF,
  STMT_EXPR,
  STMT_RETURN,
  STMT_BLOCK,
  STMT_WHILELOOP,
  STMT_FORLOOP,
  STMT_COND,
  STMT_WHEN,
  STMT_SELECT,
  STMT_LABEL,
  STMT_GOTO,

  EXPR,
  EXPR_LITERAL,
  EXPR_BOOLLITERAL,
  EXPR_INTLITERAL,
  EXPR_FLOATLITERAL,
  EXPR_COMPLEXLITERAL,
  EXPR_STRINGLITERAL,
  EXPR_VARIABLE,
  EXPR_VARINIT,
  EXPR_USERINIT,
  EXPR_DEF,
  EXPR_UNOP,
  EXPR_BINOP,
  EXPR_ASSIGNOP,
  EXPR_SEQ,
  EXPR_SIMPLESEQ,
  EXPR_LET,
  EXPR_COND,
  EXPR_FORALL,
  EXPR_SIZEOF,
  EXPR_PARENOP,
  EXPR_CAST,
  EXPR_CAST_LIKE,
  EXPR_FNCALL,
  EXPR_ARRAYREF,
  EXPR_MEMBERACCESS,
  EXPR_REDUCE,
  EXPR_TUPLE,
  EXPR_NAMED,
  EXPR_WITH,
  EXPR_USE,

  SYMBOL,
  SYMBOL_UNRESOLVED,
  SYMBOL_MODULE,
  SYMBOL_VAR,
  SYMBOL_PARAM,
  SYMBOL_TYPE,
  SYMBOL_FN,
  SYMBOL_ENUM,
  SYMBOL_LABEL,
  SYMBOL_FORWARDING,

  TYPE,
  TYPE_BUILTIN,
  TYPE_FN,
  TYPE_ENUM,
  TYPE_DOMAIN,
  TYPE_INDEX,
  TYPE_ARRAY,
  TYPE_USER,
  TYPE_STRUCTURAL,
  TYPE_CLASS,
  TYPE_RECORD,
  TYPE_UNION,
  TYPE_TUPLE,
  TYPE_META,
  TYPE_SUM,
  TYPE_VARIABLE,
  TYPE_UNRESOLVED,
  TYPE_EXPR,
  TYPE_NIL,

  LIST,

  AST_TYPE_END 
};

extern char* astTypeName[];

#define isSomeStmt(_x) (((_x) >= STMT) && (_x) < EXPR)
#define isSomeExpr(_x) (((_x) >= EXPR) && (_x) < SYMBOL)
#define isSomeSymbol(_x) (((_x) >= SYMBOL) && (_x) < TYPE)
#define isSomeType(_x) (((_x) >= TYPE) && (_x) < LIST)

#define COPY_DEF(type)                                   \
  virtual type* copy(bool clone = false,                 \
                     Map<BaseAST*,BaseAST*>* map = NULL, \
                     Vec<BaseAST*>* update_list = NULL,  \
                     bool internal = false) {            \
    preCopy(clone, map, update_list, internal);          \
    type* _this = copyInner(clone, map);                 \
    postCopy(_this, clone, map, update_list, internal);  \
    return _this;                                        \
  }                                                      \
  virtual type* copyInner(bool clone,                    \
                          Map<BaseAST*,BaseAST*>* map)

#define COPY(c) (c ? c->copy() : NULL)
#define COPY_INTERNAL(c) (c ? c->copy(clone, map, NULL, true) : NULL)
#define CLONE(c) (c ? c->copy(true) : NULL)
#define CLONE_INTERNAL(c) (c ? c->copy(true, map, NULL, true) : NULL)

class BaseAST : public gc {
 public:
  astType_t astType;    // BaseAST subclass
  long id;              // Unique ID

  BaseAST* prev;        // List previous pointer
  BaseAST* next;        // List next pointer

  SymScope* parentScope;

  char* filename;       // filename of location
  int lineno;           // line number of location
  char* traversalInfo;  // traversal where this was created
  Vec<char*>* copyInfo; // traversals where this was copied
  BaseAST* copyFrom;    // if this was created in a copy, from what?

  Vec<char*> pragmas;

  static long getNumIDs(void);

  BaseAST(void);
  BaseAST(astType_t type);
  COPY_DEF(BaseAST);
  virtual void traverse(Traversal* traversal, bool atTop = true);
  virtual void traverseDef(Traversal* traversal, bool atTop = true);

  virtual void print(FILE* outfile);
  virtual void printDef(FILE* outfile);
  void println(FILE* outfile);

  virtual void codegen(FILE* outfile);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  virtual void callReplaceChild(BaseAST* new_ast);

  void remove(void);
  void replace(BaseAST* new_ast);
  void insertBefore(BaseAST* new_ast);
  void insertAfter(BaseAST* new_ast);

// need to put this as default value to copy for new interface
//    new Map<BaseAST*,BaseAST*>();
  void preCopy(bool clone, Map<BaseAST*,BaseAST*>*& map, Vec<BaseAST*>* update_list, bool internal);
  void postCopy(BaseAST* copy, bool clone, Map<BaseAST*,BaseAST*>* map, Vec<BaseAST*>* update_list, bool internal);

  char* stringLoc(void);
  void printLoc(FILE* outfile);

  char* hasPragma(char* str);
  void addPragma(char* str);
  void copyPragmas(Vec<char*> srcPragmas);
};

#define forv_BaseAST(_p, _v) forv_Vec(BaseAST, _p, _v)

class FnSymbol;

void collect_symbols(Vec<Symbol*>* syms);
void collect_symbols(Vec<Symbol*>* syms, FnSymbol* function);
void collect_symbols(Vec<Symbol*>* syms, Type* type);
// USAGE:
//   Vec<Symbol*> all_syms;
//   collect_symbols(&all_syms);
//   FnSymbol* function = ...;
//   collect_symbols(&all_syms, function);

void collect_functions(Vec<FnSymbol*>* functions);
// USAGE:
//   Vec<FnSymbol*> all_functions;
//   collect_functions(&all_functions);

void collect_asts(Vec<BaseAST*>* asts, FnSymbol* function);
// USAGE:
//   Vec<BaseAST*> all_asts;
//   FnSymbol* function = ...;
//   collect_asts(&all_asts, function);

int compar_baseast(const void *ai, const void *aj);
// for use with qsort

void get_ast_children(BaseAST *a, Vec<BaseAST *> &asts, int all = 0);

extern int currentLineno;
extern char* currentFilename;
extern char* currentTraversal;

#endif

