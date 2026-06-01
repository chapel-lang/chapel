/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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

module ChapelRuntimeInterface {
  use ChapelBase, ChapelProgramRegistration, CTypes;

  extern type wide_ptr_t;

  inline proc ptrToPrgInfoHere do return chpl_programInfoHere.asPtr();

  // The compiler will generate code that calls this procedure in order to
  // deterministically populate the 'chpl_globals_registry' on all locales.
  export proc
  chpl_registerGlobalVar(idx: int(32), ptrToWidePtr: c_ptr(wide_ptr_t)) {
    param cname = 'chpl_rt_comm_register_global_var';
    extern cname proc fn(prg: c_ptr(chpl_rt_prginfo),
                         idx: int(32),
                         ptr_to_wide_ptr: c_ptr(wide_ptr_t)): void;
    fn(ptrToPrgInfoHere, idx, ptrToWidePtr);
  }

  // These pragmas will be useful for debugging what is going on when a RT
  // shim is called. As well, some runtime functions need this information
  // to be passed.
  //
  // TODO: Replace me with a new pragma that means the same thing.
  pragma "insert line file info"
  pragma "always propagate line file info"
  export proc chpl_broadcastGlobalVars() {
    param cname = 'chpl_rt_comm_broadcast_global_vars';
    extern cname proc fn(prg: c_ptr(chpl_rt_prginfo)): void;
    fn(ptrToPrgInfoHere);
  }

  pragma "insert line file info"
  pragma "always propagate line file info"
  export proc chpl_privateBroadcast(idx: int(32), size: c_size_t) {
    param cname = 'chpl_rt_comm_private_broadcast';
    extern cname proc fn(prg: c_ptr(chpl_rt_prginfo),
                         idx: int(32),
                         size: c_size_t): void;
    fn(ptrToPrgInfoHere, idx, size);
  }
}
