/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

   .. warning::

    These procedures will perform very rudimentary error checking with checks
    enabled. Namely, only locale IDs and data sizes are checked to make sure
    they have reasonable values.  Validity of the addresses is not confirmed
    under any circumstance.

*/
module Communication {
  private use CTypes;

  // This module may be used from internal modules that are resolved before
  // Locale-related modules. So, we can't use `numLocales` or `Locales` here.
  pragma "no doc"
  private extern const chpl_numNodes: int(32);

  /*
   Copy potentially remote data into local memory.

   :arg dest: Address in local memory where the data will be copied into
   :arg src: Address of the source in potentially remote memory
   :arg srcLocID: ID of the source locale
   :arg numBytes: Number of bytes to copy
  */
  inline proc get(dest: c_void_ptr, src: c_void_ptr, srcLocID: int,
                  numBytes: integral) {
    if boundsChecking then {
      if srcLocID < 0 || srcLocID >= chpl_numNodes {
        halt("The source locale ID (", srcLocID,
             ") for Communication.get is out of bounds.");
      }

      if numBytes < 0 {
        halt("Number of bytes (", numBytes,
             ") for Communication.get is negative.");
      }
    }

    __primitive("chpl_comm_get", dest:c_ptr(uint(8)), srcLocID,
                src:c_ptr(uint(8)), numBytes);
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
    if boundsChecking then {
      if destLocID < 0 || destLocID >= chpl_numNodes {
        halt("The destination locale ID (", destLocID,
             ") for Communication.put is out of bounds.");
      }

      if numBytes < 0 {
        halt("Number of bytes (", numBytes,
             ") for Communication.put is negative.");
      }
    }
    __primitive("chpl_comm_put", src:c_ptr(uint(8)), destLocID,
                dest:c_ptr(uint(8)), numBytes);
  }
}
