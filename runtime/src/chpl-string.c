/*
 * Copyright 2004-2014 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "chplrt.h"
#include "chpl-string.h"
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

//
// Support for the new string record implementation
//
// NOTE: strings of length 0 are assumed to be the literal ""

/* This function copies src into dest.  If dest == "", allocate a new
 * buffer for the string.  If dest is specified, it is the caller's
 * responsibility to make sure that dest is large enough to hold src.
 * Return the moved string.
 */
c_string stringMove(c_string dest, c_string src, int64_t len,
                    int32_t lineno, c_string filename) {
  char *ret;
  assert(src);

  if (!strcmp(dest, "")) {
    ret = chpl_mem_alloc(len+1, CHPL_RT_MD_STRING_MOVE_DATA, lineno, filename);
  } else {
    // reuse the buffer
    ret = (char *) dest;
  }

  snprintf(ret, len+1, "%s", src);
  return (c_string) ret;
}

/* This function returns a string from src_locale located at src_addr.
 *
 *     src_locale: node id
 *     src_addr: string address on remote node
 *     src_len: length
 *
 */
c_string remoteStringCopy(c_nodeid_t src_locale,
                          c_string src_addr, int64_t src_len,
                          int32_t lineno, c_string filename) {
  char* ret;
  if (src_addr == NULL) return NULL;
  ret = chpl_mem_alloc(src_len+1, CHPL_RT_MD_STRING_COPY_REMOTE,
                       lineno, filename);
  chpl_gen_comm_get((void*)ret, src_locale, (void*)src_addr, sizeof(char),
                    CHPL_TYPE_uint8_t, src_len+1, lineno, filename);
  return (c_string)ret;
}
