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
  use ChapelBase, CTypes;

  extern type wide_ptr_t;

  export
  proc chpl_registerGlobalVar(idx: c_int, ptrToWidePtr: c_ptr(wide_ptr_t)) {
    // Extern, but declared by the compiler and lives in program code.
    extern var chpl_globals_registry: c_ptr(c_ptr(wide_ptr_t));
    chpl_globals_registry[idx] = ptrToWidePtr;
  }
}
