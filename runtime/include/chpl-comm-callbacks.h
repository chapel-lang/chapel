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

#ifndef _chpl_comm_callbacks_h_
#define _chpl_comm_callbacks_h_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//
// Communication layer callback support.
//
// NAME
//
//   chpl_comm_install_callback   - install communication callback function
//   chpl_comm_uninstall_callback - remove communication callback function
//
//
// SYNOPSIS
//
//     #include "chpl-comm-callbacks.h"
//
//     int chpl_comm_install_callback(chpl_comm_cb_event_kind_t event_kind,
//                                    chpl_comm_cb_fn_t cb_fn);
//     int chpl_comm_uninstall_callback(chpl_comm_cb_event_kind_t event_kind,
//                                      chpl_comm_cb_fn_t cb_fn);
//
//
// DESCRIPTION
//
//   These runtime functions are used to install and remove functions
//   called when certain events occur in the runtime communication layer.
//
//   The 'event_kind' argument specifies the communication layer event that
//   is of interest. The 'cb_fn' argument is a pointer to a callback
//   function for that event.  The callback functions installed for
//   each event will be called, in the order in which they were
//   installed, each time the corresponding event occurs in the
//   communication layer of the runtime.
//
//   The event type, chpl_comm_cb_event_kind_t is defined below.
//
//   The type of a communication layer callback function pointer is:
//
//     typedef void (*chpl_comm_cb_fn_t)(chpl_comm_cb_info_t* info);
//
//   The single argument passed to it is a pointer to information of
//   type chpl_comm_cb_info_t, describing the communication operation.  This
//   type is also defined below.  It contains a union of three different
//   information sets depending only on the chpl_comm_cb_event_kind_t
//   value.   
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
//   The following errors can occur with chpl_comm_install_callback():
//
//     ENOMEM:  No room to install another callback function for this
//              event.  At present there is a static limit of 10 installed
//              callback functions for each event.
//     ERANGE:  The specified 'kind' is too large.
//
//   The following errors can occur with chpl_comm_uninstall_callback():
//
//     ENOENT:  The given pointer was not found in the list of installed
//              callback functions for the given event.
//     ERANGE:  The specified 'kind' is too large.
//
//
// NOTES
//
//   Communication callbacks are local to the top-level locale.  In 
//   multi-locale programs, callback functions must be installed separately
//   on each locale where they are wanted.
//
//   The communication callback system is not thread safe.  Calling the install
//   and/or uninstall functions simultaneously from more than one thread
//   on a single locale can corrupt internal data structures and lead to
//   chaos.  The installed callback functions themselves may be called
//   by more than one thread on the same locale simultaneously; if they
//   need concurrency control they must provide for it themselves.
//
//   The callback function should not depend upon the allocation
//   status of the pointed-to info or any member within it, past the
//   point when it returns.  For example, it must not store a copy of
//   the info pointer for an event call for use when any other event
//   call occurs.
//

typedef enum {
  chpl_comm_cb_event_kind_put,       // Simple put
  chpl_comm_cb_event_kind_put_nb,    // Non-blocking put
  chpl_comm_cb_event_kind_put_strd,  // Strided put
  chpl_comm_cb_event_kind_get,       // Simple get
  chpl_comm_cb_event_kind_get_nb,    // Non-blocking get
  chpl_comm_cb_event_kind_get_strd,  // Strided get
  chpl_comm_cb_event_kind_executeOn,      // regular executeOn
  chpl_comm_cb_event_kind_executeOn_nb,   // Non-blocking executeOn
  chpl_comm_cb_event_kind_executeOn_fast, // Fast executeOn
  chpl_comm_cb_num_event_kinds
} chpl_comm_cb_event_kind_t;

typedef struct {
  chpl_comm_cb_event_kind_t 
             event_kind;          // kind of event this describes
  c_nodeid_t localNodeID;         // The node doing the communication
  c_nodeid_t remoteNodeID;        // The node to which the communication is going
  union {

    struct chpl_comm_info_comm { // put, put_nb, get, get_nb
      void *addr;                // Source address
      void *raddr;               // Destination address
      size_t size;               // Size of communication
      int32_t commID;            // unique identifier for this get/put
      int lineno;                // source line of communication
      int32_t filename;          // source file of communication
    } comm;

    struct chpl_comm_info_comm_strd { // put_strd, get_strd
      void* srcaddr;            // Source Address
      void* srcstrides;         // Source strides

      void* dstaddr;            // Destination Address
      void* dststrides;         // Destination strides
      size_t *count;            // Counts for the above
      int32_t stridelevels;
      size_t elemSize;
      int32_t commID;           // unique identifier for this get/put
      int lineno;               // source line of communication
      int32_t filename;         // source file of communication
    } comm_strd;

    struct chpl_comm_info_comm_executeOn {
      c_sublocid_t subloc;      //  Sub-location
      chpl_fn_int_t fid;        //  Function ID
      void *arg;                //  Function arg pointer
      size_t arg_size;          //  Function arg size
      int lineno;               // source line of communication
      int32_t filename;         // source file of communication
    } executeOn;

  } iu;
} chpl_comm_cb_info_t;

typedef void (*chpl_comm_cb_fn_t)(const chpl_comm_cb_info_t*);

int chpl_comm_install_callback(chpl_comm_cb_event_kind_t,
                               chpl_comm_cb_fn_t);
int chpl_comm_uninstall_callback(chpl_comm_cb_event_kind_t,
                                 chpl_comm_cb_fn_t);

#ifdef __cplusplus
} // end extern "C"
#endif

#endif // _chpl_comm_callbacks_h_
