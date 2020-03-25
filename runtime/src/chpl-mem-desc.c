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

//
// Shared code for different mem implementations in mem-*/chpl_*_mem.c
//
#include "chplrt.h"

#include "chpl-env.h"
#include "chpl-mem-desc.h"
#include "chpltypes.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>


//
// Define the description strings and track indicators for the memory
// descriptors.
//
#define CHPL_MEMDESC_MACRO(_enum, _str, _track)  { _str, _track }

struct md_desc_type {
  const char* string;
  chpl_bool track;
};

static struct md_desc_type rt_md[] = {
  CHPL_MD_ALL_MEMDESCS(CHPL_MEMDESC_MACRO)
};

#undef CHPL_MEMDESC_MACRO


const char* chpl_mem_descString(chpl_mem_descInt_t mdi) {
  if (mdi < CHPL_RT_MD_NUM)
    return rt_md[mdi].string;
  return chpl_mem_descs[mdi-CHPL_RT_MD_NUM];
}


chpl_bool chpl_mem_descTrack(chpl_mem_descInt_t mdi) {
  //
  // For the runtime-defined descriptor types, for now we either track
  // the ones the static definition in the table says to, or we track
  // them all.  In the future we can imagine wanting to be able to
  // turn them on and off individually, but we can wait for a use case
  // before designing that capability.
  //
  if (mdi < CHPL_RT_MD_NUM) {
    static chpl_bool track_all_mds;
    static volatile chpl_bool track_all_mds_set = false;

    //
    // Init is parallel-safe because all stores are of the same values.
    //
    if (!track_all_mds_set) {
      track_all_mds = chpl_env_rt_get_bool("MEMTRACK_ALL_MDS", false);
      track_all_mds_set = true;
    }

    return track_all_mds || rt_md[mdi].track;
  }

  return true;
}
