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
#include "chpl-atomics.h"
#include "chpl-comm.h"
#include "chpl-env-gen.h"
#include "chpl-mem.h"
#include "chpl-prginfo.h"
#include "chpl-error.h"
#include "chpl-linefile-support.h"

#include <string.h>

static chpl_rt_prginfo* chpl_prg_root;

// Class index translation caches, indexed first by the current program ID
// and then by the foreign program ID. Each resulting array maps foreign class
// indices to class indices in the current program.
//
// Example: program 1 has translated class index 3 from program 2 to index 5:
//
//   cid_caches[1]
//       |
//       v
//   foreign program ID     foreign class index
//   +---+---+---+---+      +---+---+---+---+---+---+
//   | 0 | 1 | 2 |...|      | 0 | 1 | 2 | 3 | 4 | 5 |
//   +---+---+-|-+---+      +---+---+---+---+---+---+
//             +----------> | 6 | 0 | 0 | 5 | 0 | 0 |
//                          +---+---+---+---+---+---+
//
// The first entry of the class-index array is its length (6); other entries
// are 0 until looked up. On a cache miss, the runtime finds the class by name
// in the current program and stores its index for subsequent translations.
static chpl_atomic_uintptr_t cid_caches[CHPL_CID_MAX_PROGRAMS];

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
  // The id is stored in the upper bits of every class id.
  if (prg->id >= CHPL_CID_MAX_PROGRAMS) {
    chpl_rt_error("too many Chapel programs loaded to encode class ids", 0, 0);
  }
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
    chpl_rt_error("Failed to map index!\n", 0, 0);
  }

  ret = (chpl_rt_prg_id) got;

  // Set the program's ID. It should be writeable memory.
  prg->id = ret;

  program_registration_common_setup(prg);

  return ret;
}

int chpl_rt_prginfo_register_root_here(chpl_rt_prginfo* prg) {
  if (chpl_prg_root == NULL) {
    for (size_t i = 0; i < CHPL_CID_MAX_PROGRAMS; i++) {
      atomic_init_uintptr_t(&cid_caches[i], 0);
    }

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
    chpl_rt_error("Failed to fetch program data for ID", 0, 0);
  }

  // This should hold...
  assert(ret->id == id);

  return ret;
}

chpl_rt_prg_id chpl_rt_prginfo_id(chpl_rt_prginfo* prg) {
  return prg ? prg->id : CHPL_RT_PRGINFO_NULL_ID;
}

// Returns the array of per-foreign-program caches for 'self'.
static chpl_atomic_uintptr_t* cid_caches_for(chpl_rt_prg_id self) {
  chpl_atomic_uintptr_t* slot = &cid_caches[self];
  uintptr_t p = atomic_load_explicit_uintptr_t(slot, chpl_memory_order_acquire);
  if (p == 0) {
    chpl_atomic_uintptr_t* fresh =
      chpl_mem_allocMany(CHPL_CID_MAX_PROGRAMS, sizeof(*fresh),
                         CHPL_RT_MD_CLASS_ID_CACHE, 0, 0);
    for (size_t i = 0; i < CHPL_CID_MAX_PROGRAMS; i++) {
      atomic_init_uintptr_t(&fresh[i], 0);
    }
    uintptr_t expected = 0;
    if (atomic_compare_exchange_strong_explicit_uintptr_t(
          slot, &expected, (uintptr_t) fresh,
          chpl_memory_order_acq_rel, chpl_memory_order_acquire)) {
      p = (uintptr_t) fresh;
    } else {
      chpl_mem_free(fresh, 0, 0);
      p = expected;
    }
  }
  return (chpl_atomic_uintptr_t*) p;
}

// Returns self's cache for class ids from program 'foreign'.
static chpl_atomic_uint_least32_t*
cid_cache_for(chpl_rt_prginfo* self, chpl_rt_prg_id foreign,
              int32_t lineno, int32_t filenameIdx) {
  chpl_atomic_uintptr_t* slot = &cid_caches_for(self->id)[foreign];
  uintptr_t p = atomic_load_explicit_uintptr_t(slot, chpl_memory_order_acquire);
  if (p == 0) {
    chpl_rt_prginfo* f = chpl_rt_prginfo_from_id_here(foreign);
    if (f == NULL) {
      const char* filename = chpl_rt_lookup_filename(self, filenameIdx);
      chpl_prg_error("class instance belongs to an unknown Chapel program",
                     lineno, filename);
    }
    int n = CHPL_RT_PRGINFO_DATA(f, chpl_classInfoSize);
    chpl_atomic_uint_least32_t* fresh =
      chpl_mem_allocMany(n, sizeof(*fresh), CHPL_RT_MD_CLASS_ID_CACHE,
                         lineno, filenameIdx);
    atomic_init_uint_least32_t(&fresh[0], (uint_least32_t) n);
    for (int i = 1; i < n; i++) {
      atomic_init_uint_least32_t(&fresh[i], 0);
    }
    uintptr_t expected = 0;
    if (atomic_compare_exchange_strong_explicit_uintptr_t(
          slot, &expected, (uintptr_t) fresh,
          chpl_memory_order_acq_rel, chpl_memory_order_acquire)) {
      p = (uintptr_t) fresh;
    } else {
      chpl_mem_free(fresh, lineno, filenameIdx);
      p = expected;
    }
  }
  return (chpl_atomic_uint_least32_t*) p;
}

chpl__class_id chpl_cid_translate(chpl_rt_prginfo* self, chpl__class_id cid,
                                  int32_t lineno, int32_t filenameIdx) {
  chpl_rt_prg_id foreign = cid >> CHPL_CID_IDX_BITS;
  chpl__class_id idx = cid & ((1u << CHPL_CID_IDX_BITS) - 1);

  chpl_atomic_uint_least32_t* cache =
    cid_cache_for(self, foreign, lineno, filenameIdx);

  uint_least32_t n =
    atomic_load_explicit_uint_least32_t(&cache[0], chpl_memory_order_relaxed);
  if (idx == 0 || idx >= n) {
    const char* filename = chpl_rt_lookup_filename(self, filenameIdx);
    chpl_prg_error("invalid class id", lineno, filename);
  }

  chpl__class_id ret =
    atomic_load_explicit_uint_least32_t(&cache[idx], chpl_memory_order_relaxed);
  if (ret != 0) return ret;

  // TODO: compare mangled names instead, and avoid the linear search
  chpl_rt_prginfo* f = chpl_rt_prginfo_from_id_here(foreign);
  const chpl_class_info* fTable = CHPL_RT_PRGINFO_DATA(f, chpl_classInfo);
  const char* fqName = fTable[idx].fqName;

  if (fqName != NULL) {
    const chpl_class_info* sTable = CHPL_RT_PRGINFO_DATA(self, chpl_classInfo);
    int sN = CHPL_RT_PRGINFO_DATA(self, chpl_classInfoSize);
    for (int i = 1; i < sN; i++) {
      if (sTable[i].fqName != NULL && strcmp(sTable[i].fqName, fqName) == 0) {
        ret = (chpl__class_id) i;
        break;
      }
    }
  }

  if (ret == 0) {
    char msg[512];
    const char* foreignPath = chpl_rt_prginfo_load_path(f);
    const char* selfPath = chpl_rt_prginfo_load_path(self);
    if (foreignPath == NULL) {
      foreignPath = "<unknown>";
    } else {
      const char* slash = strrchr(foreignPath, '/');
      if (slash != NULL) foreignPath = slash + 1;
    }
    if (selfPath == NULL) {
      selfPath = "<unknown>";
    } else {
      const char* slash = strrchr(selfPath, '/');
      if (slash != NULL) selfPath = slash + 1;
    }
    snprintf(msg, sizeof(msg),
             "class '%s' from Chapel program %" PRIu64 " (%s)"
             " is not known to Chapel program %" PRIu64 " (%s)",
             fqName ? fqName : "<unknown>",
             (uint64_t) foreign, foreignPath, (uint64_t) self->id, selfPath);
    const char* filename = chpl_rt_lookup_filename(self, filenameIdx);
    chpl_prg_error(msg, lineno, filename);
  }

  atomic_store_explicit_uint_least32_t(&cache[idx], ret,
                                       chpl_memory_order_relaxed);
  return ret;
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

// This is defined as a macro in the 'chpl-env-gen.h' header.
static const char* lookup_table[][2] = CHPL_RT_ENV_LOOKUP_TABLE_INIT_EXPR;

int chpl_rt_num_build_constants(void) {
  return CHPL_RT_ENV_LOOKUP_TABLE_SIZE;
}

const char* chpl_rt_build_constant_by_idx(int idx, const char** out_val) {
  assert(0 <= idx && idx < CHPL_RT_ENV_LOOKUP_TABLE_SIZE);
  if (out_val) *out_val = lookup_table[idx][1];
  return lookup_table[idx][0];
}

int chpl_rt_num_program_only_constants(void) {
  enum size_enum {
    #define E_CONSTANT(name__, type__) size_enum_##name__,
    #include "chpl-prginfo-data-macro-adapter.h"
    ENUM_SIZE
  };
  return ENUM_SIZE;
}

static const char* program_only_constant_name_table[] = {
  #define E_CONSTANT(name__, type__) #name__,
  #include "chpl-prginfo-data-macro-adapter.h"
  NULL
};

const char* chpl_rt_program_only_constant_by_idx(int idx) {
  int num_program_only_constants = chpl_rt_num_program_only_constants();
  assert(0 <= idx && idx < num_program_only_constants);
  return program_only_constant_name_table[idx];
}

static bool compare_c_string(const char* s1, const char* s2) {
  return !strcmp(s1, s2);
}

bool chpl_rt_program_only_constant_matches_root(chpl_rt_prginfo* prg,
                                                const char* constant,
                                                char** out_error_msg) {
  chpl_rt_prginfo* p1 = CHPL_RT_PRGINFO_ROOT;
  chpl_rt_prginfo* p2 = prg;
  bool matches = false;
  char* buffer = NULL;
  int buffer_size = 0;

  #define E_CONSTANT(name__, type__) do {                                     \
    if (!strcmp(name__, constant)) {                                          \
      type__ v1 = CHPL_RT_PRGINFO_DATA(p1, constant);                         \
      type__ v2 = CHPL_RT_PRGINFO_DATA(p2, constant);                         \
      if (!strcmp(type__, "const char*") || !strcmp(type__, "c_string")) {    \
        matches = compare_c_string(v1, v2);                                   \
      } else if (NULL != strchar(type__, '*')) {                              \
        chpl_internal_error("Unhandled pointer type!");                       \
      } else {                                                                \
        matches = (v1 == v2);                                                 \
      }                                                                       \
    }                                                                         \
    if (!matches) {
      chpl_append_to_cmd(&buffer, &buffer_size,
                         "the value of the field %s does not match"
    }
  } while (0)

  #include "chpl-prginfo-data-macro-adapter.h"

  return false;
}
