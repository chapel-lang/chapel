// chpl-gen-includes.h
//
// Inline functions used in code generation.
//

#include "chpl-comm-compiler-macros.h"

// used for converting between the Chapel idea of a locale ID: chpl_localeID_t
// and the runtime idea of a locale ID: c_locale_t.
typedef union {
  chpl_localeID_t as_struct;
  c_locale_t as_int;
} localeID_u;

static ___always_inline
chpl_localeID_t chpl_gen_getLocaleID(void)
{
  return ((localeID_u){.as_int = chpl_task_getLocaleID()}).as_struct;
}

static ___always_inline
void chpl_gen_setLocaleID(chpl_localeID_t locale)
{
  chpl_task_setLocaleID(((localeID_u){.as_struct = locale}).as_int);
}

static ___always_inline
int64_t chpl_gen_localeID_to_int(chpl_localeID_t locale)
{
  return ((localeID_u){.as_struct = locale}).as_int;
}

static ___always_inline
chpl_bool chpl_is_here(chpl_localeID_t locale)
{
  return chpl_gen_localeID_to_int(locale) == chpl_task_getLocaleID();
}




