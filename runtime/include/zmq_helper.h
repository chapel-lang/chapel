/*
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

#ifndef _ZMQ_HELPER_H_
#define _ZMQ_HELPER_H_

#include "zmq.h"
#include "chplrt.h"
#include "qio.h"

#include <stdio.h>

int zmq_getsockopt_string_helper(void* s, int option, const char** res);
int zmq_getsockopt_int_helper(void* s, int option, int* res);

// Lydia NOTE 2019-02-26: the helper function implementations need to be defined
// in this header file.  Otherwise, we expect any .c file to be built as part of
// the runtime, but this theoretical .c file would need to #include zmq.h and
// we can't guarantee that will be present on a user's system.  So keep it in
// this header, which is `require`d by the ZMQ module and thus will only be
// included when ZMQ is present.

// Used when the option specified for zmq_getsockopt would modify a char*,
// due to c_strings in Chapel being const char*.
int zmq_getsockopt_string_helper(void* s, int option, const char** res) {
  size_t len = 256;
  char* resbuf = (char *)qio_malloc(len*sizeof(char));
  int err = zmq_getsockopt(s, option, resbuf, &len);
  *res = resbuf;
  return err;
}

// Used when the option specified for zmq_getsockopt would modify the int
// argument.  Mostly done for symmetry, this version could likely have been
// called directly from Chapel.
int zmq_getsockopt_int_helper(void* s, int option, int* res) {
  size_t intsize = sizeof(*res);
  int err = zmq_getsockopt(s, option, res, &intsize);
  return err;
}

#endif
