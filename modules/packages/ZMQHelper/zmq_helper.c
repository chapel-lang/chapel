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

#include "ZMQHelper/zmq_helper.h"

// Used when the option specified for zmq_getsockopt would modify a char*,
// due to c_strings in Chapel being const char*.
int zmq_getsockopt_string_helper(void* s, int option, const char** res) {
  size_t len = 256;
  char* resbuf = (char *)chpl_calloc(len, sizeof(char));
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
