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

struct chpl_chpl____wide_chpl_string_s; // TODO: Include from somewhere.

// This copies the remote string data into a local wide string representation
// of the same.
// This routine performs a deep copy of the character array data 
// after fetching the string descriptor from the remote node.  (The char*
// field in the local copy of the remote descriptor has no meaning in the 
// context of the local node, since it refers to elements in the address 
// space on the remote node.)  
// In chpl_comm_wide_get_string() a buffer of the right size is allocated 
// to receive the bytes copied from the remote node.  This buffer will be leaked,
// since no corresponding free is added to the generated code.
static ___always_inline
void chpl_gen_comm_wide_string_get(void* addr,
  int32_t node, void* raddr, int32_t elemSize, int32_t typeIndex, int32_t len,
  int ln, chpl_string fn)
{
  // This part just copies the descriptor.
  if (chpl_nodeID == node) {
    memcpy(addr, raddr, elemSize*len);
  } else {
#ifdef CHPL_TASK_COMM_GET
    chpl_task_comm_get(addr, node, raddr, elemSize, typeIndex, len, ln, fn);
#else
    chpl_comm_get(addr, node, raddr, elemSize, typeIndex, len, ln, fn);
#endif
  }

  // And now we copy the bytes in the string itself.
  {
    struct chpl_chpl____wide_chpl_string_s* local_str =
      (struct chpl_chpl____wide_chpl_string_s*) addr;
    // Accessing the addr field of the incomplete struct declaration
    // would not work in this context except that this function
    // is always inlined.
    chpl_comm_wide_get_string((chpl_string*) &(local_str->addr),
                              local_str, typeIndex, ln, fn);
    // The bytes live locally, so we have to update the locale.
    local_str->locale = chpl_gen_getLocaleID();
  }
}




