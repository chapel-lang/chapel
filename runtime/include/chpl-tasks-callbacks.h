/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#ifndef _chpl_tasks_callbacks_h_
#define _chpl_tasks_callbacks_h_

#include <stdint.h>
#include "chpltypes.h"

#ifdef __cplusplus
extern "C" {
#endif

//
// Tasking callback support.
//
// NAME
//
//   chpl_task_install_callback   - install tasking callback function
//   chpl_task_uninstall_callback - remove tasking callback function
//
//
// SYNOPSIS
//
//     #include "chpl-tasks-callbacks.h"
//
//     int chpl_task_install_callback(chpl_task_cb_event_kind_t event_kind,
//                                    chpl_task_cb_info_kind_t info_kind,
//                                    chpl_task_cb_fn_t cb_fn);
//     int chpl_task_uninstall_callback(chpl_task_cb_event_kind_t event_kind,
//                                      chpl_task_cb_fn_t cb_fn);
//
//
// DESCRIPTION
//
//   These runtime functions are used to install and remove functions
//   called when certain events occur in the runtime tasking layer.
//
//   The 'event_kind' argument specifies the tasking layer event that
//   is of interest.  The 'info_kind' argument specifies what kind of
//   information to send to the specified callback function when that
//   event occurs.  The 'cb_fn' argument is a pointer to a callback
//   function for that event.  The callback functions installed for
//   each event will be called, in the order in which they were
//   installed, each time the corresponding event occurs in the
//   tasking layer of the runtime.
//
//   The event and info kind types, chpl_task_cb_event_kind_t and
//   chpl_task_cb_info_kind_t, are defined below.
//
//   The type of a tasking layer callback function pointer is:
//
//     typedef void (*chpl_task_cb_fn_t)(chpl_task_cb_info_t* info);
//
//   The single argument passed to it is a pointer to information of
//   type chpl_task_cb_info_t, describing the task and/or event.  This
//   type is also defined below.
//
//
// RETURN VALUE
//
//   The returned value is 0 if no errors occurred and some other value
//   (with errno set) if any errors did occur.
//
//
// ERRORS
//
//   The following errors can occur with chpl_task_install_callback():
//
//     ENOMEM:  No room to install another callback function for this
//              event.  At present there is a static limit of 10 installed
//              callback functions for each event.
//     ERANGE:  The specified 'kind' is too large.
//
//   The following errors can occur with chpl_task_uninstall_callback():
//
//     ENOENT:  The given pointer was not found in the list of installed
//              callback functions for the given event.
//     ERANGE:  The specified 'kind' is too large.
//
//
// NOTES
//
//   Task callbacks are local to the top-level locale.  In multi-locale
//   programs, callback functions must be installed separately on each
//   locale where they are wanted.
//
//   The task callback system is not thread safe.  Calling the install
//   and/or uninstall functions simultaneously from more than one thread
//   on a single locale can corrupt internal data structures and lead to
//   chaos.  The installed callback functions themselves may be called
//   by more than one thread on the same locale simultaneously; if they
//   need concurrency control they must provide for it themselves.
//
//   The callback function should not depend upon the allocation
//   status of the pointed-to info or any member within it, past the
//   point when it returns.  For example, it must not store a copy of
//   the info pointer of an event_kind==chpl_task_cb_event_kind_begin
//   call, for use when the corresponding chpl_task_cb_event_kind_end
//   call occurs.  It should also not store a copy of the filename
//   pointer from an info_kind==full call, to be used in a subsequent
//   info_kind==id_only call.  If the info as a whole or the filename
//   are wanted later, the callback function must allocate memory to
//   hold a copy of the pointed-to data and duplicate it itself.
//

typedef enum {
  chpl_task_cb_event_kind_create,
  chpl_task_cb_event_kind_begin,
  chpl_task_cb_event_kind_end,
  chpl_task_cb_num_event_kinds
} chpl_task_cb_event_kind_t;

typedef enum {
  chpl_task_cb_info_kind_full,
  chpl_task_cb_info_kind_id_only,
  chpl_task_cb_num_info_kinds
} chpl_task_cb_info_kind_t;

typedef struct {
  uint64_t nodeID;              // top-level node ID
  chpl_task_cb_event_kind_t
          event_kind;           // kind of event this describes
  chpl_task_cb_info_kind_t
          info_kind;            // kind of info in union below
  union {

    struct chpl_task_info_full {
                                // chpl_task_cb_info_kind_full:
      chpl_fn_int_t fid;        //   number of function to call
      int32_t filename;         //   source file of task definition
      int lineno;               //   source line of task definition
      uint64_t id;              //   unique ID, within locale
      int is_executeOn;         //   !=0: task is for executeOn body
    } full;
    struct chpl_task_info_id_only {
                                // chpl_task_cb_info_kind_id_only:
      uint64_t id;              //   unique ID, within locale
    } id_only;
  } iu;
} chpl_task_cb_info_t;

typedef void (*chpl_task_cb_fn_t)(const chpl_task_cb_info_t*);

int chpl_task_install_callback(chpl_task_cb_event_kind_t,
                               chpl_task_cb_info_kind_t,
                               chpl_task_cb_fn_t);
int chpl_task_uninstall_callback(chpl_task_cb_event_kind_t,
                                 chpl_task_cb_fn_t);

#ifdef __cplusplus
} // end extern "C"
#endif

#endif // _chpl_tasks_callbacks_h_
