#include "baseAST.h"
#include "pragmas.h"
#include "stringutil.h"
#include "symbol.h"


typedef Map<const char*, int> FlagMap;
typedef MapElem<const char*, int> FlagMapElem;
static FlagMap flagMap;


static void
defineFlag(Flag tag, const char* str) {
  flagMap.put(astr(str), tag);
}


int
str2flag(const char* str) {
  return flagMap.get(astr(str));
}


void
initFlags() {
  defineFlag(FLAG_ALLOW_REF, "allow ref");
  defineFlag(FLAG_ARRAY, "array");
  defineFlag(FLAG_ARRAY_ALIAS, "array alias");
  defineFlag(FLAG_ARRAY_TYPE_INFO, "array type info");
  defineFlag(FLAG_AUTO_II, "auto ii");
  defineFlag(FLAG_BASE_ARRAY, "base array");
  defineFlag(FLAG_BEGIN, "begin");
  defineFlag(FLAG_BEGIN_BLOCK, "begin block");
  defineFlag(FLAG_COBEGIN_OR_COFORALL, "cobegin or coforall");
  defineFlag(FLAG_COBEGIN_OR_COFORALL_BLOCK, "cobegin or coforall block");
  defineFlag(FLAG_COMMAND_LINE_SETTING, "command line setting");
  defineFlag(FLAG_CONCURRENTLY_ACCESSED, "concurrently accessed");
  defineFlag(FLAG_CONFIG, "config");
  defineFlag(FLAG_CONST, "const");
  defineFlag(FLAG_DATA_CLASS, "data class");
  defineFlag(FLAG_DATA_SET_ERROR, "data set error");
  defineFlag(FLAG_DEFAULT_CONSTRUCTOR, "default constructor");
  defineFlag(FLAG_DESTRUCTOR, "destructor");
  defineFlag(FLAG_DOMAIN, "domain");
  defineFlag(FLAG_EXPAND_TUPLES_WITH_VALUES, "expand tuples with values");
  defineFlag(FLAG_EXPORT, "export");
  defineFlag(FLAG_EXPR_TEMP, "expr temp");
  defineFlag(FLAG_EXTERN, "extern");
  defineFlag(FLAG_GENERIC, "generic");
  defineFlag(FLAG_HEAP, "heap");
  defineFlag(FLAG_INDEX_VAR, "index var");
  defineFlag(FLAG_INLINE, "inline");
  defineFlag(FLAG_INLINE_ITERATOR, "inline iterator");
  defineFlag(FLAG_IS_MEME, "is meme");
  defineFlag(FLAG_ITERATOR_CLASS, "iterator class");
  defineFlag(FLAG_ITERATOR_CLASS_COPY, "iterator class copy");
  defineFlag(FLAG_ITERATOR_FN, "iterator fn");
  defineFlag(FLAG_LABEL_BREAK, "label break");
  defineFlag(FLAG_LABEL_CONTINUE, "label continue");
  defineFlag(FLAG_LOCAL_BLOCK, "local block");
  defineFlag(FLAG_LOCALIZED, "localized");
  defineFlag(FLAG_MAYBE_PARAM, "maybe param");
  defineFlag(FLAG_MAYBE_TYPE, "maybe type");
  defineFlag(FLAG_METHOD, "method");
  defineFlag(FLAG_NO_DEFAULT_FUNCTIONS, "no default functions");
  defineFlag(FLAG_NO_FORMAL_TMP, "no formal tmp");
  defineFlag(FLAG_NO_HEAP_ALLOCATION, "no heap allocation");
  defineFlag(FLAG_NO_OBJECT, "no object");
  defineFlag(FLAG_NO_PARENS, "no parens");
  defineFlag(FLAG_NO_WIDE_CLASS, "no wide class");
  defineFlag(FLAG_OBJECT_CLASS, "object class");
  defineFlag(FLAG_OMIT_FROM_CONSTRUCTOR, "omit from constructor");
  defineFlag(FLAG_ON, "on");
  defineFlag(FLAG_ON_BLOCK, "on block");
  defineFlag(FLAG_PARAM, "param");
  defineFlag(FLAG_PRIVATE, "private");
  defineFlag(FLAG_PROMOTION_WRAPPER, "promotion wrapper");
  defineFlag(FLAG_RANGE, "range");
  defineFlag(FLAG_REF, "ref");
  defineFlag(FLAG_REF_ITERATOR_CLASS, "ref iterator class");
  defineFlag(FLAG_REF_THIS, "ref this");
  defineFlag(FLAG_SINGLE, "single");
  defineFlag(FLAG_SPECIFIED_RETURN_TYPE, "specified return type");
  defineFlag(FLAG_SUPER_CLASS, "super class");
  defineFlag(FLAG_SYNC, "sync");
  defineFlag(FLAG_TEMP, "temp");
  defineFlag(FLAG_TUPLE, "tuple");
  defineFlag(FLAG_TUPLE_HASH_FUNCTION, "tuple hash function");
  defineFlag(FLAG_TUPLE_INIT, "tuple init");
  defineFlag(FLAG_TYPE_CONSTRUCTOR, "type constructor");
  defineFlag(FLAG_TYPE_VARIABLE, "type variable");
  defineFlag(FLAG_VALID_VAR, "valid var");
  defineFlag(FLAG_INVISIBLE_FN, "invisible fn");
  defineFlag(FLAG_WIDE, "wide");
  defineFlag(FLAG_WIDE_CLASS, "wide class");
  defineFlag(FLAG_WRAPPER, "wrapper");
}


void
viewFlags(BaseAST* ast) {
  if (Symbol* sym = toSymbol(ast)) {
    form_Map(FlagMapElem, e, flagMap) {
      if (sym->flags[e->value]) {
        printf("\"%s\"\n", e->key);
      }
    }
  }
}
