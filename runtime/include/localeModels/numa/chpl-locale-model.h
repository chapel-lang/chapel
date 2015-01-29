#ifndef _chpl_locale_model_h_
#define _chpl_locale_model_h_

#include "sys_basic.h"
#include "chpltypes.h"

//
// This is the type of a global locale ID.
//
typedef struct {
  int32_t node;
  int32_t subloc;
} chpl_localeID_t;

//
// This is the initializer for a chpl_localeID_t.  This macro is
// referenced explicitly in the compiler, in symbol.cpp.
//
#define CHPL_LOCALEID_T_INIT  {0, 0}

//
// This is the external copy constructor for a chpl_localeID_t, specified
// by the module code for a numa locale model.
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
  chpl_localeID_t loc = { node, subloc };
  return loc;
}

static ___always_inline
c_nodeid_t chpl_rt_nodeFromLocaleID(chpl_localeID_t loc) {
  return loc.node;
}

static ___always_inline
c_sublocid_t chpl_rt_sublocFromLocaleID(chpl_localeID_t loc) {
  return loc.subloc;
}

#endif // _chpl_locale_model_h_
