#include "chplrt.h"

#include "chplfp.h"
#include "chpl-mem.h"
#include "chpl-mem-desc.h"
#include "chplcgfns.h"
#include "chpl-comm.h"
#include "chpl-comm-compiler-macros.h"
#include "error.h"

#include <inttypes.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*** REMOVE ME ***
const char* _default_format_write_complex64 = "%g + %gi";
const char* _default_format_write_complex128 = "%g + %gi";
***/

// Uses the system allocator.  Should not be used to create user-visible data
// (error messages are OK).
char* chpl_glom_strings(int numstrings, ...) {
  va_list ap;
  int i, len;
  char* str;

  va_start(ap, numstrings);
  len = 0;
  for (i=0; i<numstrings; i++)
    len += strlen(va_arg(ap, char*));
  va_end(ap);

  str = (char*)chpl_mem_allocMany(len+1, sizeof(char),
                                  CHPL_RT_MD_GLOM_STRINGS_DATA, 0, 0);

  va_start(ap, numstrings);
  str[0] = '\0';
  for (i=0; i<numstrings; i++)
    strcat(str, va_arg(ap, char*));
  va_end(ap);

  return str;
}


c_string chpl_format(c_string format, ...) {
  va_list ap;
  char z[128];

  va_start(ap, format);
  if (vsnprintf(z, sizeof(z), format, ap) >= sizeof(z))
    chpl_error("overflow encountered in format", 0, 0);
  va_end(ap);
  return string_copy(z, 0, 0);
}


// TODO: This should be placed in a separate file never included in the launcher build.
// Maybe rename chpl-gen-includes and place this in the corresponding C file....
#ifndef LAUNCHER
#include "chpl-gen-includes.h"

struct chpl_chpl____wide_chpl_string_s {
  chpl_localeID_t locale;
  chpl_string addr;
  int64_t size;
};
typedef struct chpl_chpl____wide_chpl_string_s chpl____wide_chpl_string;

chpl_string
chpl_wide_string_copy(chpl____wide_chpl_string* x, int32_t lineno, chpl_string filename) {
  if (chpl_rt_nodeFromLocaleID(x->locale) == chpl_nodeID)
    return string_copy(x->addr, lineno, filename);
  else {
    chpl_string s;
    chpl_comm_wide_get_string(&s, x,
                              -CHPL_TYPE_chpl_string /* this is unfortunate */,
                              lineno, filename);
    return s;
  }
}

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
void chpl_gen_comm_wide_string_get(void* addr,
  c_nodeid_t node, void* raddr, int32_t elemSize, int32_t typeIndex, int32_t len,
  int ln, chpl_string fn)
{
  // This part just copies the descriptor.
  if (chpl_nodeID == node) {
    memcpy(addr, raddr, elemSize*len);
  } else {
    chpl_gen_comm_get(addr, node, raddr, elemSize, typeIndex, len, ln, fn);
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

// un-macro'd CHPL_WIDEN_STRING
void
chpl_string_widen(chpl____wide_chpl_string* x, chpl_string from, int32_t lineno, chpl_string filename)
{
  size_t len = strlen(from) + 1;
  x->locale = chpl_gen_getLocaleID();
  x->addr = chpl_mem_calloc(len, CHPL_RT_MD_SET_WIDE_STRING, lineno, filename);
  strncpy((char*)x->addr, from, len);
  if (*((len-1)+(char*)x->addr) != '\0')
    chpl_internal_error("String missing terminating NUL.");
  x->size = len;    // This size includes the terminating NUL.
}

// un-macro'd CHPL_COMM_WIDE_GET_STRING
void
chpl_comm_wide_get_string(chpl_string* local, struct chpl_chpl____wide_chpl_string_s* x, int32_t tid, int32_t lineno, chpl_string filename)
{
  char* chpl_macro_tmp =
      chpl_mem_calloc(x->size, CHPL_RT_MD_GET_WIDE_STRING, lineno, filename);
  if (chpl_nodeID == chpl_rt_nodeFromLocaleID(x->locale))
    memcpy(chpl_macro_tmp, x->addr, x->size);
  else
    chpl_gen_comm_get((void*) &(*chpl_macro_tmp),
                  chpl_rt_nodeFromLocaleID(x->locale),
                  (void*)(x->addr),
                  sizeof(char), tid, x->size, lineno, filename);
  *local = chpl_macro_tmp;
}

void string_from_c_string(chpl_string *ret, c_string str, int haslen, int64_t len, int32_t lineno, chpl_string filename)
{
  char* s;

  if( str == NULL ) {
    *ret = NULL;
    return;
  }
  if( ! haslen ) len = strlen(str);

  s = (char*)chpl_mem_alloc(len+1, CHPL_RT_MD_STRING_COPY_DATA,
                              lineno, filename);
  memcpy(s, str, len);
  s[len] = '\0';
  *ret = s;
}
void wide_string_from_c_string(chpl____wide_chpl_string *ret, c_string str, int haslen, int64_t len, int32_t lineno, chpl_string filename)
{
  char* s;

  ret->locale = chpl_gen_getLocaleID();
  if( str == NULL ) {
    ret->addr = NULL;
    ret->size = 0;
    return;
  }
  if( ! haslen ) len = strlen(str);

  s = chpl_mem_alloc(len+1, CHPL_RT_MD_STRING_COPY_DATA, lineno, filename);
  memcpy(s, str, len);
  s[len] = '\0';

  ret->addr = s;
  ret->size = len + 1; // this size includes the terminating NUL
}
void c_string_from_string(c_string* ret, chpl_string* str, int32_t lineno, chpl_string filename)
{
  *ret = *str;
}
void c_string_from_wide_string(c_string* ret, chpl____wide_chpl_string* str, int32_t lineno, chpl_string filename)
{
  if( chpl_nodeID != chpl_rt_nodeFromLocaleID(str->locale) ) {
    chpl_error("cannot create a C string from a remote string",
               lineno, filename);
  }
  *ret = str->addr;
}



#endif


//
// We need an allocator for the rest of the code, but for the user
// program it needs to be a locale-aware one with tracking, while for
// the launcher the regular system one will do.
//
static ___always_inline void*
chpltypes_malloc(size_t size, chpl_mem_descInt_t description,
                 int32_t lineno, chpl_string filename) {
#ifndef LAUNCHER
  return chpl_mem_alloc(size, description, lineno, filename);
#else
  return malloc(size);
#endif
}


c_string
string_copy(c_string x, int32_t lineno, c_string filename)
{
  char *z;

  // If the input string is null, just return null.
  if (x == NULL)
    return NULL;

  z = (char*)chpltypes_malloc(strlen(x)+1, CHPL_RT_MD_STRING_COPY_DATA,
                              lineno, filename);
  return strcpy(z, x);
}


c_string
string_concat(c_string x, c_string y, int32_t lineno, c_string filename) {
  char *z = (char*)chpltypes_malloc(strlen(x)+strlen(y)+1,
                                    CHPL_RT_MD_STRING_CONCAT_DATA,
                                    lineno, filename);
  z[0] = '\0';
  strcat(z, x);
  strcat(z, y);
  return z;
}

// Returns the index of the first occurrence of a substring within a string, or
// 0 if the substring is not in the string.
int string_index_of(c_string haystack, c_string needle) {
  chpl_string substring = strstr(haystack, needle);
  return substring ? (int) (substring-haystack)+1 : 0;
}

// It is up to the caller to make sure low and high are within the string
// bounds and that stride is not 0.
// FIXME: This can't return a statically allocated empty string once strings
// are garbage collected.
c_string
string_select(c_string x, int low, int high, int stride, int32_t lineno, c_string filename) {
  char* result = NULL;
  char* dst = NULL;
  int size = high-low+1;
  c_string src;

  if (low  < 1) low = 1;
  if (high < 1) return "";

  src = stride > 0 ? x + low - 1 : x + high - 1;
  result = chpltypes_malloc(size + 1, CHPL_RT_MD_STRING_SELECT_DATA,
                            lineno, filename);
  dst = result;
  if (stride == 1) {
    memcpy(result, src, size);
    dst = result + size;
  } else if (stride > 0) {
    while (src - x <= high - 1) {
      *dst++ = *src;
      src += stride;
    }
  } else {
    while (src - x >= low - 1) {
      *dst++ = *src;
      src += stride;
    }
  }

  *dst = '\0';
  return result;
}

// Returns a string containing the character at the given index of the input
// string, or an empty string if the index is out of bounds.
// FIXME: This can't return a statically allocated empty string once strings
// are garbage collected.
c_string
string_index(c_string x, int i, int32_t lineno, c_string filename) {
  char* buffer;

  if (i-1 < 0 || i-1 >= string_length(x))
    return "";
  buffer = chpltypes_malloc(2, CHPL_RT_MD_STRING_COPY_DATA,
                                  lineno, filename);
  sprintf(buffer, "%c", x[i-1]);
  return buffer;
}


chpl_bool
string_contains(c_string x, c_string y) {
  if (strstr(x, y))
    return true;
  else
    return false;
}


int32_t string_compare(c_string x, c_string y) {
  return (int32_t)strcmp(x, y);
}


int64_t
string_length(c_string x) {
  return strlen(x);
}

