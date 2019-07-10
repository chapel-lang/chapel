/*
 * Copyright 2004-2019 Cray Inc.
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
chpl_wide_string_copy(chpl____wide_chpl_string* x, int32_t lineno, int32_t filename) {
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

// un-macro'd CHPL_COMM_WIDE_GET_STRING
void
chpl_comm_wide_get_string(chpl_string* local, struct chpl_chpl____wide_chpl_string_s* x, int32_t tid, int32_t lineno, int32_t filename)
{
  char* chpl_macro_tmp;

  if (x->addr == NULL)
  {
    *local = NULL;
    return;
  }

  chpl_macro_tmp =
      chpl_mem_calloc(1, x->size, CHPL_RT_MD_GET_WIDE_STRING, lineno, filename);
  if (chpl_nodeID == chpl_rt_nodeFromLocaleID(x->locale))
    chpl_memmove(chpl_macro_tmp, x->addr, x->size);
  else
    chpl_gen_comm_get((void *)&(*chpl_macro_tmp),
                      chpl_rt_nodeFromLocaleID(x->locale), (void *)(x->addr),
                      sizeof(char) * x->size, tid,
                      CHPL_COMM_UNKNOWN_ID, lineno, filename);
  *local = chpl_macro_tmp;
}

uint8_t* chpl__getInPlaceBufferData(chpl__inPlaceBuffer* buf) {
  return buf->data;
}

uint8_t* chpl__getInPlaceBufferDataForWrite(chpl__inPlaceBuffer* buf) {
  return chpl__getInPlaceBufferData(buf);
}
