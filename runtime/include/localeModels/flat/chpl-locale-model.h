#ifndef _chpl_locale_model_h_
#define _chpl_locale_model_h_

#include "sys_basic.h"
#include "chpltypes.h"

//
// The flat locale model doesn't have sublocales.  It only has top
// level (network-connected) locales.
//

//
// This is the type of a global locale ID.
//
typedef struct {
  int32_t node;
} chpl_localeID_t;

//
// This is the initializer for a chpl_localeID_t.  This macro is
// referenced explicitly in the compiler, in symbol.cpp.
//
#define CHPL_LOCALEID_T_INIT  {0}

//
// This is the external copy constructor for a chpl_localeID_t, specified
// by the module code for a flat locale model.
//
static ___always_inline
chpl_localeID_t chpl__initCopy_chpl_rt_localeID_t(chpl_localeID_t initial) {
  return initial;
}

//
// These functions are used by the module code to assemble and
// disassemble global locale IDs.
//
static ___always_inline
chpl_localeID_t chpl_rt_buildLocaleID(c_nodeid_t node, c_sublocid_t subloc) {
  chpl_localeID_t loc = { node };
  //assert(subloc == c_sublocid_any);
  return loc;
}

static ___always_inline
c_nodeid_t chpl_rt_nodeFromLocaleID(chpl_localeID_t loc) {
  return loc.node;
}

static ___always_inline
c_sublocid_t chpl_rt_sublocFromLocaleID(chpl_localeID_t loc) {
  return c_sublocid_any;
}

//
// Force the tasking layer to say there are no sublocales even if it
// knows otherwise (NUMA, e.g.).
//
#define CHPL_LOCALE_MODEL_NUM_SUBLOCALES 0

#endif // _chpl_locale_model_h_
