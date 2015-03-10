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

#ifndef _chpl_mem_desc_H_
#define _chpl_mem_desc_H_

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

#define CHPL_MD_ALL_MEMDESCS(m)                                         \
        m(UNKNOWN = 0,                                                  \
          "unknown"),                                                   \
        m(EXECUTION_COMMAND,                                            \
          "chapel execution command buffer"),                           \
        m(ARRAY_ELEMENTS,                                               \
          "array elements"),                                            \
        m(SET_WIDE_STRING,                                              \
          "set wide string"),                                           \
        m(GET_WIDE_STRING,                                              \
          "get wide string"),                                           \
        m(COMMAND_BUFFER,                                               \
          "command buffer"),                                            \
        m(COMM_XMIT_RECV_BUF,                                           \
          "comm layer transmit/receive buffer"),                        \
        m(COMM_XMIT_RECV_STATUS,                                        \
          "comm layer transmit/receive status"),                        \
        m(COMM_FORK_SEND_INFO,                                          \
          "comm layer sent remote fork info"),                          \
        m(COMM_FORK_SEND_NB_INFO,                                       \
          "comm layer sent non-blocking remote fork info"),             \
        m(COMM_FORK_SEND_LARGE_ARG,                                     \
          "comm layer sent remote fork large fncall arg"),              \
        m(COMM_FORK_SEND_NB_LARGE_ARG,                                  \
          "comm layer sent non-blocking remote fork large fncall arg"), \
        m(COMM_FORK_SEND_RESPONSE_DATA,                                 \
          "comm layer sent remote fork response data"),                 \
        m(COMM_FORK_RECV_INFO,                                          \
          "comm layer received remote fork info"),                      \
        m(COMM_FORK_RECV_LARGE_INFO,                                    \
          "comm layer received remote fork large info"),                \
        m(COMM_FORK_RECV_NB_INFO,                                       \
          "comm layer received non-blocking remote fork info"),         \
        m(COMM_FORK_RECV_NB_LARGE_INFO,                                 \
          "comm layer received non-blocking remote fork large info"),   \
        m(COMM_FORK_RECV_LARGE_ARG,                                     \
          "comm layer received remote fork large fncall arg"),          \
        m(COMM_FORK_RECV_NB_LARGE_ARG,                                  \
          "comm layer received non-blocking remote fork large fncall arg "),\
        m(COMM_FORK_DONE_FLAG,                                          \
          "comm layer remote fork done flag(s)"),                       \
        m(COMM_PER_LOCALE_INFO,                                         \
          "comm layer per-locale information"),                         \
        m(COMM_PRIVATE_OBJECTS_ARRAY,                                   \
          "comm layer private objects array"),                          \
        m(COMM_PRIVATE_BROADCAST_DATA,                                  \
          "comm layer private broadcast data"),                         \
        m(GLOM_STRINGS_DATA,                                            \
          "glom strings data"),                                         \
        m(STRING_COPY_DATA,                                             \
          "string copy data"),                                          \
        m(STRING_COPY_REMOTE,                                           \
          "remote string copy"),                                        \
        m(STRING_CONCAT_DATA,                                           \
          "string concat data"),                                        \
        m(STRING_MOVE_DATA,                                             \
          "string move data"),                                          \
        m(STRING_SELECT_DATA,                                           \
          "string select data"),                                        \
        m(OS_LAYER_RETURN_DATA,                                         \
          "os layer return data"),                                      \
        m(CONFIG_ARG_COPY_DATA,                                         \
          "config arg copy data"),                                      \
        m(CONFIG_TABLE_DATA,                                            \
          "config table data"),                                         \
        m(LOCALE_NAME_BUFFER,                                           \
          "locale name buffer"),                                        \
        m(TASK_DESCRIPTOR,                                              \
          "task descriptor"),                                           \
        m(TASK_DESCRIPTOR_LINK,                                         \
          "task descriptor link"),                                      \
        m(TASK_STACK,                                                   \
          "task stack"),                                                \
        m(MUTEX,                                                        \
          "mutex"),                                                     \
        m(LOCK_REPORT_DATA,                                             \
          "lock report data"),                                          \
        m(TASK_POOL_DESCRIPTOR,                                         \
          "task pool descriptor"),                                      \
        m(TASK_LIST_DESCRIPTOR,                                         \
          "task list descriptor"),                                      \
        m(THREAD_PRIVATE_DATA,                                          \
          "thread private data"),                                       \
        m(THREAD_LIST_DESCRIPTOR,                                       \
          "thread list descriptor"),                                    \
        m(IO_BUFFER,                                                    \
          "io buffer or bytes"),                                        \
        m(GMP,                                                          \
          "gmp data"),                                                  \
        m(GETS_PUTS_STRIDES,                                            \
          "put_strd/get_strd array of strides"),                        \
        m(GETS_PUTS_COUNTS,                                             \
          "put_strd/get_strd array of count"),                          \
        m(NUM, "")                      // this must be the last entry


//
// Define the numeration constants for the memory descriptors.
//
#define CHPL_MEMDESC_ENUM(md_name, md_desc)  CHPL_RT_MD_ ## md_name
typedef enum {
  CHPL_MD_ALL_MEMDESCS(CHPL_MEMDESC_ENUM)
} chpl_mem_rtMemDesc_t;


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

#endif

