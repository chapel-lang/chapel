/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

/*
 This module provides low-level communication procedures.
*/
module Communication {
  private use CTypes;

  inline proc get(dest: c_void_ptr, src: c_void_ptr, srcLocID: int,
                  numBytes: integral) {
    __primitive("chpl_comm_get", dest, srcLocID, src, numBytes);
  }

  inline proc put(dest: c_void_ptr, src: c_void_ptr, destLocID: int,
                  numBytes: integral) {
    __primitive("chpl_comm_put", src, destLocID, dest, numBytes);
  }
}
