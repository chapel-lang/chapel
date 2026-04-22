/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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
#include "chpl-prginfo.h"
#include <stdlib.h>
#include <string.h>

#ifndef LAUNCHER
  #error "Should not be possible!"
#endif

chpl_rt_prginfo chpl_rt_prginfo_singleton;

// Callbacks are expanded to a 'void (void)' function, which is OK as we just
// need their address. Constants are expanded normally, and both categories
// are expanded to 'extern' symbols that should be linked into the launcher.
#define E_CONSTANT(name__, type__) extern type__ name__;
#define E_CALLBACK(name__) extern void name__(void);
#include "chpl-prginfo-data-macro-adapter.h"

static
void prepare_launcher_info_via_external_symbols(chpl_rt_prginfo* prg) {
  // Unlike the runtime, it is OK to link symbols into the launcher, as its
  // purpose is just to launch the real program, and there is one launcher
  // per Chapel root program. So we prepare the fields of the program info by
  // expanding out the X-macro pattern again and assigning the extern symbols
  // that we declared above.
  #define E_CONSTANT(name__, type__) prg->data.name__ = name__;
  #define E_CALLBACK(name__) prg->data.name__ = (name__##_type) &name__;
  #include "chpl-prginfo-data-macro-adapter.h"
}

chpl_rt_prginfo* chpl_rt_prginfo_from_id_here(chpl_rt_prg_id id) {
  if (id == CHPL_RT_PRGINFO_NULL_ID) return NULL;

  if (id == CHPL_RT_PRGINFO_ROOT_ID) {
    chpl_rt_prginfo* ret = &chpl_rt_prginfo_singleton;

    if (ret->id == CHPL_RT_PRGINFO_NULL_ID) {
      // Only prepare the entries if the ID is not set.
      prepare_launcher_info_via_external_symbols(ret);
      ret->id = CHPL_RT_PRGINFO_ROOT_ID;
    }

    return ret;
  }

  // Should never reach here as the launcher. Only 'root' ID is valid.
  abort();

  return NULL;
}
