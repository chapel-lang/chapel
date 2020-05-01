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

module LocaleModelHelpFlat {

  param localeModelHasSublocales = false;

  use LocaleModelHelpSetup;
  use LocaleModelHelpRuntime;
  private use SysCTypes;

  //////////////////////////////////////////
  //
  // support for "on" statements
  //

  //
  // returns true if an executeOn can be handled directly
  // by running the function in question.
  // Applies to execute on and execute on fast.
  // When performing a blocking on, the compiler will emit this sequence:
  //
  //  if (chpl_doDirectExecuteOn(targetLocale))
  //         onStatementBodyFunction( args ... );
  //  else
  //         chpl_executeOn / chpl_executeOnFast
  //
  export
  proc chpl_doDirectExecuteOn(loc: chpl_localeID_t // target locale
                             ):bool {
    const node = chpl_nodeFromLocaleID(loc);
    return (node == chpl_nodeID);
  }

  //
  // regular "on"
  //
  pragma "insert line file info"
  export
  proc chpl_executeOn(loc: chpl_localeID_t, // target locale
                      fn: int,              // on-body function idx
                      args: chpl_comm_on_bundle_p,     // function args
                      args_size: size_t     // args size
                     ) {
    const node = chpl_nodeFromLocaleID(loc);
    if (node == chpl_nodeID) {
      // don't call the runtime execute_on function if we can stay local
      // one day, we could rely on this always being handled
      // by the compiler's use of doDirectExecuteOn, but for now
      // the compiler calls this version in some cases.
      chpl_ftable_call(fn, args);
    } else {
      var tls = chpl_task_getChapelData();
      chpl_task_data_setup(chpl_comm_on_bundle_task_bundle(args), tls);
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
                          args: chpl_comm_on_bundle_p,     // function args
                          args_size: size_t     // args size
                         ) {
    const node = chpl_nodeFromLocaleID(loc);
    if (node == chpl_nodeID) {
      // don't call the runtime fast execute_on function if we can stay local
      chpl_ftable_call(fn, args);
    } else {
      var tls = chpl_task_getChapelData();
      chpl_task_data_setup(chpl_comm_on_bundle_task_bundle(args), tls);
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
                        args: chpl_comm_on_bundle_p,     // function args
                        args_size: size_t     // args size
                       ) {
    //
    // If we're in serial mode, we should use blocking rather than
    // non-blocking "on" in order to serialize the execute_ons.
    //
    const node = chpl_nodeFromLocaleID(loc);
    var tls = chpl_task_getChapelData();
    var isSerial = chpl_task_data_getSerial(tls);
    if (node == chpl_nodeID) {
      // don't call the runtime nb execute_on function if we can stay local
      if isSerial {
        chpl_ftable_call(fn, args);
      } else {
        chpl_task_data_setup(chpl_comm_on_bundle_task_bundle(args), tls);
        chpl_comm_taskCallFTable(fn, args, args_size, c_sublocid_any);
      }
    } else {
      chpl_task_data_setup(chpl_comm_on_bundle_task_bundle(args), tls);
      if isSerial {
        chpl_comm_execute_on(node, c_sublocid_any, fn, args, args_size);
      } else {
        chpl_comm_execute_on_nb(node, c_sublocid_any, fn, args, args_size);
      }
    }
  }
}
