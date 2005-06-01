#ifndef _BASEAST_H_
#define _BASEAST_H_

#include "link.h"
#include "map.h"

class Symbol;
class Type;

/**
 **  Note: update astType_t and astTypeName together always.
 **/
enum astType_t {
  STMT = 0,
  STMT_NOOP,
  STMT_DEF,
  STMT_EXPR,
  STMT_RETURN,
  STMT_WITH,
  STMT_USE,
  STMT_BLOCK,
  STMT_WHILELOOP,
  STMT_FORLOOP,
  STMT_COND,
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
  EXPR_SPECIALBINOP,
  EXPR_ASSIGNOP,
  EXPR_SEQ,
  EXPR_SIMPLESEQ,
  EXPR_FLOOD,
  EXPR_COMPLETEDIM,
  EXPR_LET,
  EXPR_COND,
  EXPR_FORALL,
  EXPR_SIZEOF,
  EXPR_PARENOP,
  EXPR_CAST,
  EXPR_CAST_LIKE,
  EXPR_FNCALL,
  EXPR_ARRAYREF,
  EXPR_TUPLESELECT,
  EXPR_MEMBERACCESS,
  EXPR_REDUCE,
  EXPR_TUPLE,
  EXPR_NAMED,

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
  TYPE_SEQ,
  TYPE_ARRAY,
  TYPE_USER,
  TYPE_LIKE,
  TYPE_STRUCTURAL,
  TYPE_CLASS,
  TYPE_RECORD,
  TYPE_UNION,
  TYPE_TUPLE,
  TYPE_META,
  TYPE_SUM,
  TYPE_VARIABLE,
  TYPE_UNRESOLVED,
  TYPE_NIL,

  LIST,

  PRAGMA,

  AST_TYPE_END 
};

extern char* astTypeName[];

#define isSomeStmt(_x) (((_x) >= STMT) && (_x) < EXPR)
#define isSomeExpr(_x) (((_x) >= EXPR) && (_x) < SYMBOL)
#define isSomeSymbol(_x) (((_x) >= SYMBOL) && (_x) < TYPE)
#define isSomeType(_x) (((_x) >= TYPE) && (_x) < AST_TYPE_END)

class BaseAST : public ILink {
 public:
  //what class this class instance is
  astType_t astType;
  long id;

  char* traversalInfo; // holds the traversal where this was created
  Vec<char*>* copyInfo; // holds a vec of traversals where this was copied

  static long getNumIDs(void);

  BaseAST(void);
  BaseAST(astType_t type);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  virtual void callReplaceChild(BaseAST* new_ast);

  BaseAST* extract(void);

// need to put this as default value to copy for new interface
//    new Map<BaseAST*,BaseAST*>();
  void copySupport(BaseAST* copy, bool clone, Map<BaseAST*,BaseAST*>* map, Vec<BaseAST*>* update_list);
  void copySupportTopLevel(BaseAST* copy, bool clone, Map<BaseAST*,BaseAST*>* map, Vec<BaseAST*>* update_list);
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

