#include "baseAST.h"
#include "pragmas.h"
#include "stringutil.h"
#include "symbol.h"


typedef Map<const char*, int> PragmaMap;
typedef MapElem<const char*, int> PragmaMapElem;
static PragmaMap pragmaMap;


static void
definePragma(PragmaTag tag, const char* str) {
  pragmaMap.put(astr(str), tag);
}


int
str2prag(const char* str) {
  return pragmaMap.get(astr(str));
}


void
initPragmas() {
  definePragma(PRAG_ALLOW_REF, "allow ref");
  definePragma(PRAG_ARRAY, "array");
  definePragma(PRAG_ARRAY_ALIAS, "array alias");
  definePragma(PRAG_ARRAY_TYPE_INFO, "array type info");
  definePragma(PRAG_AUTO_II, "auto ii");
  definePragma(PRAG_BASE_ARRAY, "base array");
  definePragma(PRAG_BEGIN, "begin");
  definePragma(PRAG_BEGIN_BLOCK, "begin block");
  definePragma(PRAG_COBEGIN_OR_COFORALL, "cobegin or coforall");
  definePragma(PRAG_COBEGIN_OR_COFORALL_BLOCK, "cobegin or coforall block");
  definePragma(PRAG_COMMAND_LINE_SETTING, "command line setting");
  definePragma(PRAG_CONCURRENTLY_ACCESSED, "concurrently accessed");
  definePragma(PRAG_DATA_CLASS, "data class");
  definePragma(PRAG_DATA_SET_ERROR, "data set error");
  definePragma(PRAG_DEFAULT_CONSTRUCTOR, "default constructor");
  definePragma(PRAG_DESTRUCTOR, "destructor");
  definePragma(PRAG_DOMAIN, "domain");
  definePragma(PRAG_EXPAND_TUPLES_WITH_VALUES, "expand tuples with values");
  definePragma(PRAG_EXPORT, "export");
  definePragma(PRAG_EXPR_TEMP, "expr temp");
  definePragma(PRAG_EXTERN, "extern");
  definePragma(PRAG_GENERIC, "generic");
  definePragma(PRAG_HEAP, "heap");
  definePragma(PRAG_INDEX_VAR, "index var");
  definePragma(PRAG_INLINE, "inline");
  definePragma(PRAG_IS_MEME, "is meme");
  definePragma(PRAG_ITERATOR_CLASS, "iterator class");
  definePragma(PRAG_ITERATOR_CLASS_COPY, "iterator class copy");
  definePragma(PRAG_LABEL_BREAK, "label break");
  definePragma(PRAG_LABEL_CONTINUE, "label continue");
  definePragma(PRAG_LOCAL_BLOCK, "local block");
  definePragma(PRAG_LOCALIZED, "localized");
  definePragma(PRAG_NO_DEFAULT_FUNCTIONS, "no default functions");
  definePragma(PRAG_NO_FORMAL_TMP, "no formal tmp");
  definePragma(PRAG_NO_HEAP_ALLOCATION, "no heap allocation");
  definePragma(PRAG_NO_OBJECT, "no object");
  definePragma(PRAG_NO_PARENS, "no parens");
  definePragma(PRAG_NO_WIDE_CLASS, "no wide class");
  definePragma(PRAG_OBJECT_CLASS, "object class");
  definePragma(PRAG_OMIT_FROM_CONSTRUCTOR, "omit from constructor");
  definePragma(PRAG_ON, "on");
  definePragma(PRAG_ON_BLOCK, "on block");
  definePragma(PRAG_PRIVATE, "private");
  definePragma(PRAG_PROMOTION_WRAPPER, "promotion wrapper");
  definePragma(PRAG_RANGE, "range");
  definePragma(PRAG_REF, "ref");
  definePragma(PRAG_REF_ITERATOR_CLASS, "ref iterator class");
  definePragma(PRAG_REF_THIS, "ref this");
  definePragma(PRAG_SINGLE, "single");
  definePragma(PRAG_SPECIFIED_RETURN_TYPE, "specified return type");
  definePragma(PRAG_SUPER_CLASS, "super class");
  definePragma(PRAG_SYNC, "sync");
  definePragma(PRAG_TUPLE, "tuple");
  definePragma(PRAG_TUPLE_HASH_FUNCTION, "tuple hash function");
  definePragma(PRAG_TUPLE_INIT, "tuple init");
  definePragma(PRAG_TYPE_CONSTRUCTOR, "type constructor");
  definePragma(PRAG_VALID_VAR, "valid var");
  definePragma(PRAG_INVISIBLE_FN, "invisible fn");
  definePragma(PRAG_WIDE, "wide");
  definePragma(PRAG_WIDE_CLASS, "wide class");
  definePragma(PRAG_WRAPPER, "wrapper");
}


void
viewPragmas(BaseAST* ast) {
  if (Symbol* sym = toSymbol(ast)) {
    form_Map(PragmaMapElem, e, pragmaMap) {
      if (sym->pragmas[e->value]) {
        printf("\"%s\"\n", e->key);
      }
    }
  }
}
