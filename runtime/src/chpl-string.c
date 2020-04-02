/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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
  if (x->addr == NULL) return NULL;

  chpl_string s = chpl_mem_alloc(x->size, CHPL_RT_MD_STR_COPY_DATA, lineno, filename);
  chpl_gen_comm_get((void *)s, chpl_rt_nodeFromLocaleID(x->locale),
                    (void *)(x->addr), x->size, CHPL_COMM_UNKNOWN_ID,
                    lineno, filename);
  return s;
}

uint8_t* chpl__getInPlaceBufferData(chpl__inPlaceBuffer* buf) {
  return buf->data;
}

uint8_t* chpl__getInPlaceBufferDataForWrite(chpl__inPlaceBuffer* buf) {
  return chpl__getInPlaceBufferData(buf);
}
