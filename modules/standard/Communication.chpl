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

  type argType = c_ptr(uint(8));

  /*
   Copy potentially remote data into local memory.
  
   :arg dest: Address in local memory where the data will be copied into
   :arg src: Address of the source in potentially remote memory
   :arg srcLocID: ID of the source locale
   :arg numBytes: Number of bytes to copy
  */
  inline proc get(dest: c_void_ptr, src: c_void_ptr, srcLocID: int,
                  numBytes: integral) {
    __primitive("chpl_comm_get", dest:argType, srcLocID, src:argType, numBytes);
  }

  /*
   Copy local data into potentially remote memory.
  
   :arg dest: Address in potentially remote memory where the data will be copied
              into
   :arg src: Address of the source in local memory
   :arg destLocID: ID of the destination locale
   :arg numBytes: Number of bytes to copy
  */
  inline proc put(dest: c_void_ptr, src: c_void_ptr, destLocID: int,
                  numBytes: integral) {
    __primitive("chpl_comm_put", src:argType, destLocID, dest:argType, numBytes);
  }
}
