/*
 * Copyright 2004-2016 Cray Inc.
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

module LocaleModelHelpFlat {

  param localeModelHasSublocales = false;

  use LocaleModelHelp;

  //////////////////////////////////////////
  //
  // support for "on" statements
  //

  //
  // runtime interface
  //
  extern proc chpl_comm_execute_on(loc_id: int, subloc_id: int, fn: int,
                                   args: c_void_ptr, arg_size: size_t);
  extern proc chpl_comm_execute_on_fast(loc_id: int, subloc_id: int, fn: int,
                                        args: c_void_ptr, args_size: size_t);
  extern proc chpl_comm_execute_on_nb(loc_id: int, subloc_id: int, fn: int,
                                      args: c_void_ptr, args_size: size_t);
  extern proc chpl_ftable_call(fn: int, args: c_void_ptr): void;
  //
  // regular "on"
  //
  pragma "insert line file info"
  export
  proc chpl_executeOn(loc: chpl_localeID_t, // target locale
                      fn: int,              // on-body function idx
                      args: c_void_ptr,     // function args
                      args_size: size_t     // args size
                     ) {
    const node = chpl_nodeFromLocaleID(loc);
    if (node == chpl_nodeID) {
      // don't call the runtime execute_on function if we can stay local
      chpl_ftable_call(fn, args);
    } else {
      chpl_comm_execute_on(node, chpl_sublocFromLocaleID(loc),
                     fn, args, args_size);
    }
  }

  //
  // fast "on" (doesn't do anything that could deadlock a comm layer,
  // in the Active Messages sense)
  //
  pragma "insert line file info"
  export
  proc chpl_executeOnFast(loc: chpl_localeID_t, // target locale
                          fn: int,              // on-body function idx
                          args: c_void_ptr,     // function args
                          args_size: size_t     // args size
                         ) {
    const node = chpl_nodeFromLocaleID(loc);
    if (node == chpl_nodeID) {
      // don't call the runtime fast execute_on function if we can stay local
      chpl_ftable_call(fn, args);
    } else {
      chpl_comm_execute_on_fast(node, chpl_sublocFromLocaleID(loc),
                          fn, args, args_size);
    }
  }

  //
  // nonblocking "on" (doesn't wait for completion)
  //
  pragma "insert line file info"
  export
  proc chpl_executeOnNB(loc: chpl_localeID_t, // target locale
                        fn: int,              // on-body function idx
                        args: c_void_ptr,     // function args
                        args_size: size_t     // args size
                       ) {
    //
    // If we're in serial mode, we should use blocking rather than
    // non-blocking "on" in order to serialize the execute_ons.
    //
    const node = chpl_nodeFromLocaleID(loc);
    if (node == chpl_nodeID) {
      if __primitive("task_get_serial") then
        // don't call the runtime nb execute_on function if we can stay local
        chpl_ftable_call(fn, args);
      else
        // We'd like to use a begin, but unfortunately doing so as
        // follows does not compile for --no-local:
        // begin chpl_ftable_call(fn, args);
        chpl_comm_execute_on_nb(node, chpl_sublocFromLocaleID(loc),
                          fn, args, args_size);
    } else {
      const subloc = chpl_sublocFromLocaleID(loc);
      if __primitive("task_get_serial") then
        chpl_comm_execute_on(node, subloc, fn, args, args_size);
      else
        chpl_comm_execute_on_nb(node, subloc, fn, args, args_size);
    }
  }

}

