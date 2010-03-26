#include "baseAST.h"
#include "flags.h"
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
  defineFlag(FLAG_HAS_RUNTIME_TYPE, "has runtime type");
  defineFlag(FLAG_RUNTIME_TYPE_VALUE, "runtime type value");
  defineFlag(FLAG_AUTO_II, "auto ii");
  defineFlag(FLAG_BASE_ARRAY, "base array");
  defineFlag(FLAG_BASE_DIST, "base dist");
  defineFlag(FLAG_BEGIN, "begin");
  defineFlag(FLAG_BEGIN_BLOCK, "begin block");
  defineFlag(FLAG_CALLS_CONSTRUCTOR, "calls constructor");
  defineFlag(FLAG_COBEGIN_OR_COFORALL, "cobegin or coforall");
  defineFlag(FLAG_COBEGIN_OR_COFORALL_BLOCK, "cobegin or coforall block");
  defineFlag(FLAG_COMMAND_LINE_SETTING, "command line setting");
  defineFlag(FLAG_COMPILER_NESTED_FUNCTION, "compiler nested function");
  defineFlag(FLAG_CONCURRENTLY_ACCESSED, "concurrently accessed");
  defineFlag(FLAG_CONFIG, "config");
  defineFlag(FLAG_CONST, "const");
  defineFlag(FLAG_DATA_CLASS, "data class");
  defineFlag(FLAG_DATA_SET_ERROR, "data set error");
  defineFlag(FLAG_DEFAULT_CONSTRUCTOR, "default constructor");
  defineFlag(FLAG_DESTRUCTOR, "destructor");
  defineFlag(FLAG_DOMAIN, "domain");
  defineFlag(FLAG_DONT_DISABLE_REMOTE_VALUE_FORWARDING, "dont disable remote value forwarding");
  defineFlag(FLAG_DISTRIBUTION, "distribution");
  defineFlag(FLAG_EXPAND_TUPLES_WITH_VALUES, "expand tuples with values");
  defineFlag(FLAG_EXPORT, "export");
  defineFlag(FLAG_EXPR_TEMP, "expr temp");
  defineFlag(FLAG_EXTERN, "extern");
  defineFlag(FLAG_GENERIC, "generic");
  defineFlag(FLAG_HEAP, "heap");
  defineFlag(FLAG_HEAP_ALLOCATE, "heap allocate");
  defineFlag(FLAG_STAR_TUPLE, "star tuple");
  defineFlag(FLAG_IMPLICIT_ALIAS_FIELD, "implicit alias field");
  defineFlag(FLAG_INDEX_VAR, "index var");
  defineFlag(FLAG_INLINE, "inline");
  defineFlag(FLAG_INLINE_ITERATOR, "inline iterator");
  defineFlag(FLAG_INVISIBLE_FN, "invisible fn");
  defineFlag(FLAG_INSERT_AUTO_COPY, "insert auto copy");
  defineFlag(FLAG_INSERT_AUTO_DESTROY, "insert auto destroy");
  defineFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW, "insert auto destroy for explicit new");
  defineFlag(FLAG_IS_MEME, "is meme");
  defineFlag(FLAG_ITERATOR_CLASS, "iterator class");
  defineFlag(FLAG_ITERATOR_FN, "iterator fn");
  defineFlag(FLAG_ITERATOR_RECORD, "iterator record");
  defineFlag(FLAG_LABEL_BREAK, "label break");
  defineFlag(FLAG_LABEL_CONTINUE, "label continue");
  defineFlag(FLAG_LOOP_BODY_ARGUMENT_CLASS, "loop body argument class");
  defineFlag(FLAG_MAYBE_PARAM, "maybe param");
  defineFlag(FLAG_MAYBE_TYPE, "maybe type");
  defineFlag(FLAG_METHOD, "method");
  defineFlag(FLAG_NO_AUTO_DESTROY, "no auto destroy");
  defineFlag(FLAG_NO_COPY, "no copy");
  defineFlag(FLAG_NO_DEFAULT_FUNCTIONS, "no default functions");
  defineFlag(FLAG_NO_FORMAL_TMP, "no formal tmp");
  defineFlag(FLAG_NO_IMPLICIT_COPY, "no implicit copy");
  defineFlag(FLAG_NO_OBJECT, "no object");
  defineFlag(FLAG_NO_PARENS, "no parens");
  defineFlag(FLAG_NO_WIDE_CLASS, "no wide class");
  defineFlag(FLAG_NON_BLOCKING, "non blocking");
  defineFlag(FLAG_OBJECT_CLASS, "object class");
  defineFlag(FLAG_OMIT_FROM_CONSTRUCTOR, "omit from constructor");
  defineFlag(FLAG_ON, "on");
  defineFlag(FLAG_ON_BLOCK, "on block");
  defineFlag(FLAG_PARAM, "param");
  defineFlag(FLAG_PRIVATE, "private");
  defineFlag(FLAG_PRIVATIZED_CLASS, "privatized class");
  defineFlag(FLAG_PROMOTION_WRAPPER, "promotion wrapper");
  defineFlag(FLAG_RANGE, "range");
  defineFlag(FLAG_REF, "ref");
  defineFlag(FLAG_REF_ITERATOR_CLASS, "ref iterator class");
  defineFlag(FLAG_REF_THIS, "ref this");
  defineFlag(FLAG_REMOVABLE_AUTO_COPY, "removable auto copy");
  defineFlag(FLAG_REMOVABLE_AUTO_DESTROY, "removable auto destroy");
  defineFlag(FLAG_SINGLE, "single");
  defineFlag(FLAG_SPECIFIED_RETURN_TYPE, "specified return type");
  defineFlag(FLAG_SUPER_CLASS, "super class");
  defineFlag(FLAG_SYNC, "sync");
  defineFlag(FLAG_SYNTACTIC_DISTRIBUTION, "syntactic distribution");
  defineFlag(FLAG_TEMP, "temp");
  defineFlag(FLAG_TUPLE, "tuple");
  defineFlag(FLAG_TYPE_CONSTRUCTOR, "type constructor");
  defineFlag(FLAG_TYPE_VARIABLE, "type variable");
  defineFlag(FLAG_VIRTUAL, "virtual");
  defineFlag(FLAG_WIDE, "wide");
  defineFlag(FLAG_WIDE_CLASS, "wide class");
  defineFlag(FLAG_WRAPPER, "wrapper");
  defineFlag(FLAG_WRAP_OUT_INTENT, "wrap out or inout intent formal");
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


TypeSymbol*
getDataClassType(TypeSymbol* ts) {
  form_Map(SymbolMapElem, e, ts->type->substitutions) {
    if (TypeSymbol* ets = toTypeSymbol(e->value))
      return ets;
  }
  return NULL;
}


void
setDataClassType(TypeSymbol* ts, TypeSymbol* ets) {
  form_Map(SymbolMapElem, e, ts->type->substitutions) {
    if (isTypeSymbol(e->value))
      e->value = ets;
  }
}
