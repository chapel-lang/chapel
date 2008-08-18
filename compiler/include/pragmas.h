#ifndef CHPL_PRAGMAS_H_
#define CHPL_PRAGMAS_H_


//
// HOW TO ADD A PRAGMA: Add an entry (sorted alphabetically) to the
// enumeration PragmaTag in this file and add a call to definePragma
// in initPragmas() in pragmas.cpp.  Then to add the pragma to a
// symbol, use Symbol::addPragma(PragmaTag) and, to check whether a
// symbol has a pragma, use Symbol::hasPragma(PragmaTag).  Pragmas can
// be added to Chapel code using the pragma production as in
//
//   pragma "my pragma" def FunctionName
//   pragma "my pragma" var VariableName
//   pragma "my pragma" class ClassName
//
// HOW TO VIEW A PRAGMA IN GDB: Use the command "pragmas ast" where
// ast is a Symbol.  If you just know the id of the ast, use the
// command "pragmas aid(id)" where id is the integral symbol id.
//


#include "chpl.h"


class BaseAST;


enum PragmaTag {
  PRAG_UNKNOWN = 0,
  PRAG_ALLOW_REF,
  PRAG_ARRAY,
  PRAG_ARRAY_ALIAS, // array alias declared via => syntax
  PRAG_ARRAY_TYPE_INFO,
  PRAG_AUTO_II,
  PRAG_BASE_ARRAY,
  PRAG_BEGIN,
  PRAG_BEGIN_BLOCK,
  PRAG_COBEGIN_OR_COFORALL,
  PRAG_COBEGIN_OR_COFORALL_BLOCK,
  PRAG_COMMAND_LINE_SETTING,
  PRAG_CONCURRENTLY_ACCESSED, // local variables accessed by multiple threads
  PRAG_DATA_CLASS,
  PRAG_DATA_SET_ERROR,
  PRAG_DEFAULT_CONSTRUCTOR,
  PRAG_DESTRUCTOR,
  PRAG_DOMAIN,
  PRAG_EXPAND_TUPLES_WITH_VALUES,
  PRAG_EXPORT,
  PRAG_EXPR_TEMP, // temporary that stores the result of an expression
  PRAG_EXTERN, // extern variables, types, and functions
  PRAG_HEAP,
  PRAG_INDEX_VAR,
  PRAG_INLINE,
  PRAG_IS_MEME,
  PRAG_ITERATOR_CLASS,
  PRAG_ITERATOR_CLASS_COPY,
  PRAG_LOCAL_BLOCK,
  PRAG_LOCALIZED,
  PRAG_NO_DEFAULT_FUNCTIONS,
  PRAG_NO_FORMAL_TMP,
  PRAG_NO_HEAP_ALLOCATION,
  PRAG_NO_OBJECT,
  PRAG_NO_PARENS, // function without parentheses
  PRAG_NO_WIDE_CLASS,
  PRAG_OBJECT_CLASS,
  PRAG_OMIT_FROM_CONSTRUCTOR,
  PRAG_ON,
  PRAG_ON_BLOCK,
  PRAG_PRIVATE,
  PRAG_PROMOTION_WRAPPER,
  PRAG_RANGE,
  PRAG_REF,
  PRAG_REF_ITERATOR_CLASS,
  PRAG_REF_THIS,
  PRAG_SINGLE,
  PRAG_SPECIFIED_RETURN_TYPE,
  PRAG_SUPER_CLASS,
  PRAG_SYNC,
  PRAG_TUPLE,
  PRAG_TUPLE_HASH_FUNCTION,
  PRAG_TUPLE_INIT,
  PRAG_TYPE_CONSTRUCTOR,
  PRAG_VALID_VAR,
  PRAG_WIDE,
  PRAG_WIDE_CLASS,
  NUM_PRAGMAS
};


int str2prag(const char* str);
void initPragmas();
void viewPragmas(BaseAST* sym);


#endif
