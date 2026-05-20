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

// Needed to access 'dladdr' and 'Dl_info'.
#ifndef _GNU_SOURCE
  #define _GNU_SOURCE 1
#endif
#ifdef __FreeBSD__
  #include <link.h>
#endif
#include <dlfcn.h>

#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl-prginfo.h"

#include <string.h>

static chpl_rt_prginfo* chpl_prg_root;

// TODO: Have to switch to linking in separate object files?
#ifndef CHPL_RT_IS_BUILT_AS_DYNAMIC_LIBRARY
  int chpl_rt_is_dynamic_library = 0;
#else
  int chpl_rt_is_dynamic_library = 1;
#endif

/** Define setters that module code can call to set up program data. */
#define CONCAT(a__, b__) a__##b__
#define PREFIX chpl_rt_prginfo_data_entry_set_
#define SETTER(prefix__, name__)                                            \
  void CONCAT(prefix__, name__)(chpl_rt_prginfo* prg, const void* data) {   \
    size_t sz = sizeof(CHPL_RT_PRGINFO_DATA_ENTRY_TYPE(name__));            \
    memcpy(&prg->data.name__, data, sz);                                    \
  }
#define E_CONSTANT(name__, type__) SETTER(PREFIX, name__)
#define E_CALLBACK(name__, ret_type__, ...) SETTER(PREFIX, name__)
#include "chpl-prginfo-data-macro-adapter.h"
#undef PREFIX
#undef SETTER
#undef CONCAT

static void program_registration_common_setup(chpl_rt_prginfo* prg) {
}

chpl_rt_prg_id
chpl_rt_prginfo_register_here_nosync(chpl_rt_prg_id id, chpl_rt_prginfo* prg) {
  chpl_rt_prg_id ret = CHPL_RT_PRGINFO_NULL_ID;

  // ERROR: The root program hasn't even been set yet...
  if (chpl_prg_root == NULL) return ret;

  // ERROR: This ID is reserved...
  if (id == CHPL_RT_PRGINFO_ROOT_ID) return ret;

  // ERROR: The program's ID is already set!
  if (prg->id != CHPL_RT_PRGINFO_NULL_ID) return ret;

  // Map the pointer to the index using code in the root program.
  chpl_bool requestingNewIdx = (id == CHPL_RT_PRGINFO_NULL_ID);
  int64_t idxToUse = requestingNewIdx ? 0 : ((int64_t) id);

  // Fetch and invoke callback from the root program specifically.
  CHPL_RT_PRGINFO_DECLARE(CHPL_RT_PRGINFO_ROOT, chpl_mapPtrToIdxHere);
  int64_t got = chpl_mapPtrToIdxHere(prg, idxToUse);

  if (!requestingNewIdx && idxToUse != got) {
    // TODO: Propagate errors outwards from this function?
    chpl_error("Failed to map index!\n", 0, 0);
  }

  ret = (chpl_rt_prg_id) got;

  // Set the program's ID. It should be writeable memory.
  prg->id = ret;

  program_registration_common_setup(prg);

  return ret;
}

int chpl_rt_prginfo_register_root_here(chpl_rt_prginfo* prg) {
  if (chpl_prg_root == NULL) {
    // Bind and set the ID.
    chpl_prg_root = prg;
    prg->id = CHPL_RT_PRGINFO_ROOT_ID;

    program_registration_common_setup(prg);

    return 1;
  }

  return 0;
}

//
// HEADER-DECLARED SYMBOLS
// (Available for both module code and runtime code to call.)
//

chpl_rt_prginfo* chpl_rt_prginfo_from_id_here(chpl_rt_prg_id id) {
  if (id == CHPL_RT_PRGINFO_NULL_ID) return NULL;
  if (id == CHPL_RT_PRGINFO_ROOT_ID) return chpl_prg_root;

  // Fetch pointers from the root program.
  CHPL_RT_PRGINFO_DECLARE(CHPL_RT_PRGINFO_ROOT, chpl_getPtrForIdxHere);
  int64_t idx = (int64_t) id;
  chpl_rt_prginfo* ret = chpl_getPtrForIdxHere(idx);

  if (ret == NULL) {
    chpl_error("Failed to fetch program data for ID", 0, 0);
  }

  // This should hold...
  assert(ret->id == id);

  return ret;
}

chpl_rt_prg_id chpl_rt_prginfo_id(chpl_rt_prginfo* prg) {
  return prg ? prg->id : CHPL_RT_PRGINFO_NULL_ID;
}

const char* chpl_rt_prginfo_load_path(chpl_rt_prginfo* prg) {
  Dl_info info;
  int ecode = 0;

  // TODO: Note that 'dladdr' is NOT portable...if there is a situation where
  //       we run into a platform that does NOT support it, then we can fall
  //       back on having the program info record the path that was used when
  //       it was created (perhaps should be doing that anyways).
  // NOTE: But it is portable under POSIX 2024 :D.
  ecode = dladdr(prg, &info);
  if (ecode == 0 || info.dli_fname == NULL) return NULL;

  // This should be valid as long as 'prg' is loaded.
  return info.dli_fname;
}

#define STR_INNER(x__) #x__
#define STR(x__) STR_INNER(x__)

// Counts and returns the number of '*' in a type string.
static int type_pointer_depth(const char* type) {
  int ret = 0;
  for (size_t i = 0; i < strlen(type); i++) {
    if (type[i] == '*') ret += 1;
  }
  return ret;
}

static void dump_prginfo_data_entry(FILE* fp, chpl_rt_prginfo* prg,
                                    const char* name,
                                    const char* type,
                                    void* addr,
                                    int is_callback,
                                    int show_address) {

  bool is_array = type_pointer_depth(type) > 0;

  // If true then 'addr' is a pointer to a pointer, so deref it.
  void* ptr = (is_callback || is_array) ? *((void**) addr) : NULL;

  fprintf(fp, "%s : %s = ", name, type);

  if (is_callback && show_address) {
    fprintf(fp, "%p", ptr);

  } else if (is_callback) {
    fprintf(fp, "%s", ptr == NULL ? "nil" : "non-nil");

  } else {
    // TODO: Replace me with more sophisticated debugging machinery?
    if (!strcmp(type, "int")) {
      fprintf(fp, "%d", *((int*) addr));
    } else if (!strcmp(type, "int64_t")) {
      fprintf(fp, "%" PRId64, *((int64_t*) addr));
    } else if (!strcmp(type, "int32_t")) {
      fprintf(fp, "%" PRId32, *((int32_t*) addr));
    } else if (!strcmp(type, "const char*") || !strcmp(type, "c_string")) {
      fprintf(fp, "%s", addr != NULL ? *((const char**) addr) : "nil");
    } else if (is_array) {
      fprintf(fp, "%s", ptr == NULL ? "nil" : "non-nil");
    } else {
      fprintf(fp, "<value of unknown type '%s'>", type);
    }
  }

  fprintf(fp, "\n");
}

void chpl_rt_prginfo_dump_data_entries(chpl_rt_prginfo* prg,
                                       int show_addresses) {
  FILE* fp = stdout;

  fprintf(fp, "Data entries for P%d@L%d (%s)\n",
              ((int) prg->id),
              ((int) chpl_nodeID),
              chpl_rt_prginfo_load_path(prg));

  #define E_CONSTANT(name__, type__) do {                                   \
    const char* name = STR(name__);                                         \
    const char* type = STR(type__);                                         \
    void* addr = &(CHPL_RT_PRGINFO_DATA(prg, name__));                      \
    int is_callback = 0;                                                    \
    dump_prginfo_data_entry(fp, prg, name, type, addr, is_callback,         \
                            show_addresses);                                \
  } while (0);

  #define E_CALLBACK(name__, ret_type__, ...) do {                          \
    const char* name = STR(name__);                                         \
    const char* type = STR(CHPL_RT_PRGINFO_CALLBACK_TYPE(name__,            \
                                                         ret_type__,        \
                                                         __VA_ARGS__));     \
    void * addr = &(CHPL_RT_PRGINFO_DATA(prg, name__));                     \
    int is_callback = 1;                                                    \
    dump_prginfo_data_entry(fp, prg, name, type, addr, is_callback,         \
                            show_addresses);                                \
  } while (0);

  #include "chpl-prginfo-data-macro-adapter.h"
}

chpl_main_argument* chpl_rt_prginfo_main_argument(chpl_rt_prginfo* prg) {
  CHPL_RT_PRGINFO_DECLARE(prg, chpl_genMainArgPtr);
  chpl_main_argument* ret = chpl_genMainArgPtr();
  return ret;
}
