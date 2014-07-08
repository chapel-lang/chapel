// chpl-gen-includes.h
//
// Inline functions used in code generation.
//
// TODO: Check if any of these are redundant with functions in chpl-wide-ptr-fns.h.
//

#include "chpl-comm-compiler-macros.h"
#include "chplcgfns.h"
#include "chpl-locale-model.h"
#include "chpl-tasks.h"
#include "chpltypes.h"

//
// Call a function in the compiler-produced function table, passing it
// one argument.
//
static ___always_inline
void chpl_ftable_call(chpl_fn_int_t fid, void* arg)
{
  (*chpl_ftable[fid])(arg);
}


// used for converting between the Chapel idea of a locale ID: chpl_localeID_t
// and the runtime idea of a locale ID: c_localeid_t.
static ___always_inline
c_localeid_t id_pub2rt(chpl_localeID_t s)
{
  return
    ((c_localeid_t) chpl_rt_nodeFromLocaleID(s) << 32) |
    ((c_localeid_t) chpl_rt_sublocFromLocaleID(s) & 0xffffffff);
}

static ___always_inline
chpl_localeID_t id_rt2pub(c_localeid_t i)
{
  return chpl_rt_buildLocaleID(i >> 32, i & 0xffffffff);
}

static ___always_inline
chpl_localeID_t chpl_gen_getLocaleID(void)
{
  return chpl_rt_buildLocaleID(chpl_nodeID, chpl_task_getRequestedSubloc());
}
