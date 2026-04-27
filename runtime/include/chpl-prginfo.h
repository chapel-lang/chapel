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

#ifndef CHPL_RT_PRGINFO_H
#define CHPL_RT_PRGINFO_H

#include "chpl-string-support.h"
#include "chpltypes.h"
#include <inttypes.h>
#include <stddef.h>

#ifndef LAUNCHER
  #include "qio_error.h"
#endif

/**
  This file contains an interface to access critical pieces of data that the
  runtime requires from Chapel programs. This includes data such as
  a program's function table or its broadcast constants table. It also
  includes constants such the value of 'CHPL_HOME' that the program was
  compiled with, 'CHPL_COMM', and additional compiler flags.

  The runtime no longer references these symbols in a compiled Chapel program
  directly (they are not linked against). Instead it references values from a
  'chpl_rt_prginfo' struct which must be filled in by each Chapel program
  that is loaded onto a locale.

  ---

  The 'root' Chapel program is the program that is first loaded into the
  process space on each locale and first initializes the runtime. It has a
  special identifier and can be used to maintain state that you would like
  all loaded Chapel programs to use (e.g., privatized hashtables for
  pointers, global variables, etc).

  The root Chapel program is assumed to persist for the entirety of program
  execution. All other programs are introduced dynamically via dynamic
  loading, and it is possible for their state to be unloaded (or reloaded)
  during process execution.

  ---

  If you have a constant or a callback defined in module code that you need
  to access in the runtime, you can add it to "chpl-prginfo-data-macro.h"
  using the format that is described in that file.

  Then, within a runtime function, you can access the value on the current
  locale by writing (e.g., for a 'chpl_rt_prginfo*' named 'prg' and
  for a data named 'foo'):

  CHPL_RT_PRGINFO_DATA_TEMP(prg, foo);

  This will declare a temporary in the current scope with the name 'foo' that
  has the C type for the constant.

  If for some reason there is a name collision, you can get the value of the
  constant without declaring a variable with the name 'foo' (assume here
  that 'foo' has the type 'int') via:

  int bar = CHPL_RT_PRGINFO_DATA(prg, foo);

  If you don't have a 'chpl_rt_prginfo*' but you do have that program's
  unique ID, you can get the info using:

  chpl_rt_prginfo* prg = CHPL_RT_PRGINFO_FETCH(id);

  Do not declare global variables with values retrieved from these functions!
  Likewise, do not utilize 'static' variables either!

  Multiple Chapel programs can be loaded into a single process space and the
  values they contain for the data constants can differ. There is no guarantee
  that a value retrieved for one program has any meaning for another.
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LAUNCHER
  typedef void syserr;
#endif

// Forward declare some QIO types we don't need a full header for here.
struct qio_channel_s;
typedef struct qio_channel_s qio_channel_t;

// Change the level to embed debugging info.
// LEVELS: 0 = none, 1 = reasonable, 2 = ALL
//
#define CHPL_RT_DEBUG_PRGINFO_ACCESS 0

#ifdef LAUNCHER
  // Except, never bother in the launcher...
  #undef CHPL_RT_DEBUG_PRGINFO_ACCESS
#endif

/** Prefix for typedef generated for each data entry. */
#define CHPL_RT_PRGINFO_DATA_ENTRY_TYPE_PREFIX chpl_rt_

/** Suffix for typedef generated for each data entry. */
#define CHPL_RT_PRGINFO_DATA_ENTRY_TYPE_SUFFIX _type

// Helpers to perform macro token concatenation.
#define CHPL_RT_CONCAT_INNER__(a__, b__) a__ ## b__
#define CHPL_RT_CONCAT__(a__, b__) CHPL_RT_CONCAT_INNER__(a__, b__)
#define CHPL_RT_CONCAT3__(a__, b__, c__) \
  CHPL_RT_CONCAT__(a__, CHPL_RT_CONCAT__(b__, c__))

/** Get the typedef for a specific program data entry. */
#define CHPL_RT_PRGINFO_DATA_ENTRY_TYPE(name__) \
  CHPL_RT_CONCAT3__(CHPL_RT_PRGINFO_DATA_ENTRY_TYPE_PREFIX, name__, \
                    CHPL_RT_PRGINFO_DATA_ENTRY_TYPE_SUFFIX)

// Expand out unique typedefs for each data entry.
#define E_CONSTANT(name__, type__) \
  typedef type__ CHPL_RT_PRGINFO_DATA_ENTRY_TYPE(name__);
#define E_CALLBACK(name__, ret_type__, ...) \
  typedef ret_type__ (*CHPL_RT_PRGINFO_DATA_ENTRY_TYPE(name__))(__VA_ARGS__);
#include "chpl-prginfo-data-macro-adapter.h"

// TODO: Where the heck does this live (i.e., launcher or us?). It was
//       declared in 'chplcgfns.h', but clearly that's not correct as
//       it is not code-generated (and that header is going away).
extern char* chpl_executionCommand;

/** Whether or not the runtime is compiled as a dynamic library or not. */
extern int chpl_rt_is_dynamic_library;

// The type of a unique program identifier. These are assigned by the runtime
// as it registers Chapel programs. The first Chapel program (that initial-
// izes the runtime) will have the special ID 'CHPL_RT_PRGINFO_ROOT'. Others
// will have unique IDs with no other ordering guarantees.
typedef uint64_t chpl_rt_prg_id;

/** There will never be a Chapel program that is assigned this ID value. */
#define CHPL_RT_PRGINFO_NULL_ID  (0)

/** The Chapel program that initializes the runtime is given this ID. */
#define CHPL_RT_PRGINFO_ROOT_ID  (1)

/** Get a pointer to the root program info. */
#define CHPL_RT_PRGINFO_ROOT \
  (chpl_rt_prginfo_from_id_here(CHPL_RT_PRGINFO_ROOT_ID))

/** Retrieve a program's info on the current locale given an ID. */
#define CHPL_RT_PRGINFO_FETCH(id__) (chpl_rt_prginfo_from_id_here(id__))

/** Retrieve data from a program. */
#if !defined(CHPL_RT_DEBUG_PRGINFO_ACCESS) || \
             CHPL_RT_DEBUG_PRGINFO_ACCESS <= 0
  #define CHPL_RT_PRGINFO_DATA(prg__, data_name__) (prg__->data.data_name__)
#else
  #define CHPL_RT_PRGINFO_DATA(prg__, data_name__)                          \
    (*((CHPL_RT_PRGINFO_DATA_ENTRY_TYPE(data_name__))                       \
      chpl_rt_prginfo_data_debug_print_hook(CHPL_RT_DEBUG_PRGINFO_ACCESS,   \
                                            prg__,                          \
                                            &(prg__->data.data_name__),     \
                                            #data_name__,                   \
                                            __FILE__,                       \
                                            __FUNCTION__,                   \
                                            __LINE__)))
#endif

/** Declares a local that is a copy of a program data, with the same name. */
#define CHPL_RT_PRGINFO_DATA_TEMP(prg__, data_name__)             \
  CHPL_RT_PRGINFO_DATA_ENTRY_TYPE(data_name__) data_name__ =      \
      CHPL_RT_PRGINFO_DATA(prg__, data_name__)

/** Contains data that the runtime needs to execute Chapel code. */
typedef struct chpl_rt_prginfo {
  chpl_rt_prg_id id;

  // This structure contains "data entries" which must be supplied by each
  // compiled Chapel program. Currently it is unorganized and per-locale.
  struct chpl_rt_prginfo_data {
    #define E_CONSTANT(name__, type__) type__ name__;
    #define E_CALLBACK(name__, ret_type__, ...) \
      CHPL_RT_PRGINFO_DATA_ENTRY_TYPE(name__) name__;
    #include "chpl-prginfo-data-macro-adapter.h"
  } data;
} chpl_rt_prginfo;

/** Get the program info on a given locale given a program's unique ID. */
chpl_rt_prginfo* chpl_rt_prginfo_from_id_here(chpl_rt_prg_id id);

/** Get the ID of a program info. */
chpl_rt_prg_id chpl_rt_prginfo_id(chpl_rt_prginfo* prg);

/** Get the load path of a program info. */
const char* chpl_rt_prginfo_load_path(chpl_rt_prginfo* prg);

// Private implementation details.
#include "chpl-prginfo-detail.h"

#ifdef __cplusplus
}
#endif

#endif
