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

// ChapelTaskID.chpl
//
module ChapelTaskID {

  // Define a few operations over chpl_taskID_t
  inline proc =(ref a:chpl_taskID_t, b:chpl_taskID_t) {
    __primitive("=", a, b);
  }

  inline proc ==(a: chpl_taskID_t, b: chpl_taskID_t)
    return __primitive("==", a, b);

  inline proc _cast(type t, x: chpl_taskID_t) where t == int(64)
                                                    || t == uint(64)
    return __primitive("cast", t, x);

}
