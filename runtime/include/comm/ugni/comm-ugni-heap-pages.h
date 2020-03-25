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

#ifndef _comm_ugni_heap_pages_h_
#define _comm_ugni_heap_pages_h_

//
// System and heap page sizes, for the uGNI communication interface.
//

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

#include "chplrt.h"
#include "error.h"

size_t chpl_comm_ugni_getSysPageSize(void);
size_t chpl_comm_ugni_getHeapPageSize(void);

#ifdef CHPL_JEMALLOC_PREFIX
static inline
char* chpl_comm_ugni_jemalloc_conf_ev_name(void) {
#define _STRFY(s)                    #s
#define _EXPAND_THEN_STRFY(s)        _STRFY(s)
#define _STRFY_CHPL_JEMALLOC_PREFIX  _EXPAND_THEN_STRFY(CHPL_JEMALLOC_PREFIX)

  static char evn[100] = "";

  // safe because always called serially
  if (evn[0] != '\0')
    return evn;

  if (snprintf(evn, sizeof(evn), "%sMALLOC_CONF", _STRFY_CHPL_JEMALLOC_PREFIX)
      >= (int) sizeof(evn)) {
    chpl_internal_error("jemalloc conf env var name buffer is too small");
  }

  for (int i = 0; evn[i] != '\0'; i++) {
    if (islower(evn[i]))
      evn[i] = toupper(evn[i]);
  }

  return evn;

#undef _STRFY_CHPL_JEMALLOC_PREFIX
#undef _EXPAND_THEN_STRFY
#undef _STRFY
}
#else
static inline
char* chpl_comm_ugni_jemalloc_conf_ev_name(void) { return "MALLOC_CONF"; }
#endif

#endif // _comm_ugni_heap_pages_h_
