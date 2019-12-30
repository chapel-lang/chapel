/*
 * Copyright 2004-2020 Cray Inc.
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

#ifndef _chpl_mem_desc_H_
#define _chpl_mem_desc_H_

#include "chpltypes.h"

//
// When defining a new allocation type (for new instances of
// chpl_mem_allocMany or chpl_mem_realloc in the runtime), add an
// entry to the macro below giving both its enumeration constant
// (without the leading CHPL_MD_RT_) and its string description.
// Then you can use your enumeration in a call to chpl_mem_*().
//
// The macro CHPL_MD_ALL_MEMDESCS defines applies some other given
// macro (its argument) to all of the enum constant + desc string
// pairs.  Below, we define CHPL_MEMDESC_ENUM() to yield the enum
// constant name for each memory description, and we apply that to
// all the pairs to create the enum type.  In chpl_mem_descString()
// in chpl-mem-desc.c, we define CHPL_MEMDESC_DESC() to yield the
// description string, and we apply that to all the pairs to create
// that table of description strings.  This technique lets us keep
// the enum constant names and descriptions together, but make use
// of them separately to define the things we need.
//

#define CHPL_MD_ALL_MEMDESCS(m)                                               \
  m(UNKNOWN = 0,          "unknown",                                  true ), \
  m(EXECUTION_COMMAND,    "chapel execution command buffer",          true ), \
  m(ARRAY_ELEMENTS,       "array elements",                           true ), \
  m(SET_WIDE_STRING,      "set wide string",                          true ), \
  m(GET_WIDE_STRING,      "get wide string",                          true ), \
  m(COMMAND_BUFFER,       "command buffer",                           true ), \
  m(COMM_UTIL,            "comm layer utility space",                 false), \
  m(COMM_XMIT_RCV_BUF,    "comm layer transmit/receive buffer",       false), \
  m(COMM_FRK_SND_INFO,    "comm layer sent remote fork info",         false), \
  m(COMM_FRK_SND_ARG,     "comm layer sent remote fork arg",          false), \
  m(COMM_FRK_RCV_INFO,    "comm layer received remote fork info",     false), \
  m(COMM_FRK_RCV_ARG,     "comm layer received remote fork arg",      false), \
  m(COMM_FRK_DONE_FLAG,   "comm layer remote fork done flag(s)",      false), \
  m(COMM_PER_LOC_INFO,    "comm layer per-locale information",        false), \
  m(COMM_PRV_OBJ_ARRAY,   "comm layer private objects array",         false), \
  m(COMM_PRV_BCAST_DATA,  "comm layer private broadcast data",        false), \
  m(MEM_HEAP_SPACE,       "mem layer heap expansion space",           false), \
  m(GLOM_STRINGS_DATA,    "glom strings data",                        true ), \
  m(STR_COPY_DATA,        "string copy data",                         true ), \
  m(STR_COPY_REMOTE,      "remote string copy",                       true ), \
  m(STR_CONCAT_DATA,      "string concat data",                       true ), \
  m(STR_MOVE_DATA,        "string move data",                         true ), \
  m(STR_SELECT_DATA,      "string select data",                       true ), \
  m(CFG_ARG_COPY_DATA,    "config arg copy data",                     true ), \
  m(CF_TABLE_DATA,        "config table data",                        true ), \
  m(LOCALE_NAME_BUF,      "locale name buffer",                       true ), \
  m(TASK_DESC,            "task descriptor",                          false), \
  m(TASK_DESC_LINK,       "task descriptor link",                     false), \
  m(TASK_ARG,             "task body argument",                       false), \
  m(TASK_STACK,           "task stack",                               false), \
  m(MUTEX,                "mutex",                                    false), \
  m(LOCK_REPORT_DATA,     "lock report data",                         false), \
  m(TASK_POOL_DESC,       "task pool descriptor",                     false), \
  m(TASK_ARG_AND_POOL_DESC, "task body argument and pool descriptor", false), \
  m(TASK_LIST_DESC,       "task list descriptor",                     false), \
  m(TASK_LAYER_UNSPEC,    "tasking layer unspecified data",           false), \
  m(THREAD_PRV_DATA,      "thread private data",                      false), \
  m(THREAD_LIST_DESC,     "thread list descriptor",                   false), \
  m(THREAD_STACK_DESC,    "thread stack descriptor",                  false), \
  m(IO_BUFFER,            "io buffer or bytes",                       true ), \
  m(OS_LAYER_TMP_DATA,    "OS layer temporary data",                  true ), \
  m(GMP,                  "gmp data",                                 true ), \
  m(GETS_PUTS_STRIDES,    "put_strd/get_strd array of strides",       true ), \
  m(NUM,                  "*** this must be the last entry ***",      true )


//
// Define the enumeration constants for the memory descriptors.
//
#define CHPL_MEMDESC_MACRO(_enum, _str, _track)  CHPL_RT_MD_ ## _enum
typedef enum {
  CHPL_MD_ALL_MEMDESCS(CHPL_MEMDESC_MACRO)
} chpl_mem_rtMemDesc_t;

#undef CHPL_MEMDESC_MACRO


//
// The compiler generates a separate array of descriptions for the
// allocation types it defines.  Indices into that compiler-generated
// array conceptually start after the CHPL_RT_MD_NUM enum value in
// chpl-mem.h).  This is that compiler-generated array, and how many
// entries it has (also defined in the generated code).
//
extern const char* chpl_mem_descs[];
extern const int chpl_mem_numDescs;


typedef int16_t chpl_mem_descInt_t;


const char* chpl_mem_descString(chpl_mem_descInt_t mdi);
chpl_bool chpl_mem_descTrack(chpl_mem_descInt_t mdi);

#endif
