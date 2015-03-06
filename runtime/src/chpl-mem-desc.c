/*
 * Copyright 2004-2015 Cray Inc.
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

//
// Shared code for different mem implementations in mem-*/chpl_*_mem.c
//
#include "chplrt.h"

#include "chpl-mem-desc.h"
#include "chpltypes.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>


//
// Define the description strings for the memory descriptors.
//
#define CHPL_MEMDESC_DESC(md_name, md_desc)  md_desc
static const char* rt_memDescs[] = {
  CHPL_MD_ALL_MEMDESCS(CHPL_MEMDESC_DESC)
};

const char* chpl_mem_descString(chpl_mem_descInt_t mdi) {
  if (mdi < CHPL_RT_MD_NUM)
    return rt_memDescs[mdi];
  else
    return chpl_mem_descs[mdi-CHPL_RT_MD_NUM];
}
