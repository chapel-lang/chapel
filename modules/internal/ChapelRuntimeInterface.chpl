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

  // Records with empty bodies are intentionally opaque.
  extern record chpl_localeID_t {};
  extern record chpl_comm_on_bundle_t {};
  extern record chpl_task_bundle_t {};
  extern type chpl_comm_on_bundle_p;
  extern type chpl_task_bundle_p;
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

  export proc chpl_initProgramStandardModules() {
    param cname = 'chpl_rt_init_program_standard_modules';
    extern cname proc fn(prg: c_ptr(chpl_rt_prginfo)): void;
    fn(ptrToPrgInfoHere);
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

  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_executeOn(node: int, subloc: int, fid: int,
                            arg: chpl_comm_on_bundle_p,
                            arg_size: c_size_t) {
    param cname = 'chpl_rt_comm_execute_on';
    pragma "insert line file info"
    pragma "always propagate line file info"
    extern cname proc fn(prg: c_ptr(chpl_rt_prginfo), node: int,
                         subloc: int, fid: int,
                         arg: chpl_comm_on_bundle_p,
                         arg_size: c_size_t): void;
    fn(ptrToPrgInfoHere, node, subloc, fid, arg, arg_size);
  }

  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_executeOnFast(node: int, subloc: int, fid: int,
                          arg: chpl_comm_on_bundle_p,
                          arg_size: c_size_t) {
    param cname = 'chpl_rt_comm_execute_on_fast';
    pragma "insert line file info"
    pragma "always propagate line file info"
    extern cname proc fn(prg: c_ptr(chpl_rt_prginfo), node: int,
                         subloc: int, fid: int,
                         arg: chpl_comm_on_bundle_p,
                         arg_size: c_size_t): void;
    fn(ptrToPrgInfoHere, node, subloc, fid, arg, arg_size);
  }

  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_executeOnNb(node: int, subloc: int, fid: int,
                        arg: chpl_comm_on_bundle_p,
                        arg_size: c_size_t) {
    param cname = 'chpl_rt_comm_execute_on_nb';
    pragma "insert line file info"
    pragma "always propagate line file info"
    extern cname proc fn(prg: c_ptr(chpl_rt_prginfo), node: int,
                         subloc: int, fid: int,
                         arg: chpl_comm_on_bundle_p,
                         arg_size: c_size_t): void;
    fn(ptrToPrgInfoHere, node, subloc, fid, arg, arg_size);
  }

  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_addTask(fid: int, args: chpl_task_bundle_p,
                    args_size: c_size_t,
                    subloc_id: int) {
    param cname = 'chpl_rt_task_add_task';
    pragma "insert line file info"
    pragma "always propagate line file info"
    extern cname proc fn(prg: c_ptr(chpl_rt_prginfo), fid: int,
                         args: chpl_task_bundle_p,
                         args_size: c_size_t,
                         subloc_id: int): void;
    fn(ptrToPrgInfoHere, fid, args, args_size, subloc_id);
  }

  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_commTaskFtableCall(fid: int, args: chpl_comm_on_bundle_p,
                               args_size: c_size_t,
                               subloc_id: int) {
    param cname = 'chpl_rt_comm_task_ftable_call';
    pragma "insert line file info"
    pragma "always propagate line file info"
    extern cname proc fn(prg: c_ptr(chpl_rt_prginfo), fid: int,
                         args: chpl_comm_on_bundle_p,
                         args_size: c_size_t,
                         subloc_id: int): void;
    fn(ptrToPrgInfoHere, fid, args, args_size, subloc_id);
  }

  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_ftableCall(fid: int, args: c_ptr(void)) {
    param cname = 'chpl_rt_ftable_call';
    // TODO: No need to actually call into the runtime to do this.
    extern cname proc fn(prg: c_ptr(chpl_rt_prginfo), fid: int,
                         bundle: c_ptr(void)): void;
    fn(ptrToPrgInfoHere, fid, args);
  }

  inline proc chpl_ftableCall(fid: int, args: chpl_comm_on_bundle_p) {
    const ptr = __primitive('cast', c_ptr(void), args);
    chpl_ftableCall(fid, ptr);
  }

  inline proc chpl_ftableCall(fid: int, args: chpl_task_bundle_p) {
    const ptr = __primitive('cast', c_ptr(void), args);
    chpl_ftableCall(fid, ptr);
  }
}
