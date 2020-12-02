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

module LocaleModelHelpNUMA {
  use SysCTypes;

  param localeModelHasSublocales = true;

  public use LocaleModelHelpSetup;
  public use LocaleModelHelpRuntime;

  //////////////////////////////////////////
  //
  // utilities
  //
  inline
  proc chpl_getSubloc() {
    extern proc chpl_task_getSubloc(): chpl_sublocID_t;
    return chpl_task_getSubloc();
  }

  //////////////////////////////////////////
  //
  // support for "on" statements
  //

  //
  // runtime interface
  //
  extern proc chpl_task_setSubloc(subloc: int(32));

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
  proc chpl_doDirectExecuteOn(in loc: chpl_localeID_t // target locale
                             ):bool {
    const dnode =  chpl_nodeFromLocaleID(loc);
    const dsubloc =  chpl_sublocFromLocaleID(loc);

    if (dnode != chpl_nodeID) {
      return false; // need to move to different node
    } else {
      var origSubloc = chpl_task_getRequestedSubloc();
      if (dsubloc==origSubloc ||
          (dsubloc==c_sublocid_any &&
           chpl_localeModel_sublocToExecutionSubloc(origSubloc)==origSubloc)) {
        return true;
      } else {
        return false; // need to move to different sublocale
      }
    }
  }

  //
  // regular "on"
  //
  pragma "insert line file info"
  export
  proc chpl_executeOn(in loc: chpl_localeID_t, // target locale
                      fn: int,              // on-body function idx
                      args: chpl_comm_on_bundle_p,     // function args
                      args_size: size_t     // args size
                     ) {
    const dnode =  chpl_nodeFromLocaleID(loc);
    const dsubloc =  chpl_sublocFromLocaleID(loc);
    if dnode != chpl_nodeID {
      var tls = chpl_task_getInfoChapel();
      chpl_task_data_setup(chpl_comm_on_bundle_task_bundle(args), tls);
      chpl_comm_execute_on(dnode, dsubloc, fn, args, args_size);
    } else {
      // run directly on this node
      var origSubloc = chpl_task_getRequestedSubloc();
      if (dsubloc==origSubloc ||
          (dsubloc==c_sublocid_any &&
           chpl_localeModel_sublocToExecutionSubloc(origSubloc)==origSubloc)) {
        chpl_ftable_call(fn, args);
      } else {
        // move to a different sublocale
        chpl_task_setSubloc(dsubloc);
        chpl_ftable_call(fn, args);
        chpl_task_setSubloc(origSubloc);
      }
    }
  }

  //
  // fast "on" (doesn't do anything that could deadlock a comm layer,
  // in the Active Messages sense)
  //
  pragma "insert line file info"
  export
  proc chpl_executeOnFast(in loc: chpl_localeID_t, // target locale
                          fn: int,              // on-body function idx
                          args: chpl_comm_on_bundle_p,     // function args
                          args_size: size_t     // args size
                         ) {
    const dnode =  chpl_nodeFromLocaleID(loc);
    const dsubloc =  chpl_sublocFromLocaleID(loc);
    if dnode != chpl_nodeID {
      var tls = chpl_task_getInfoChapel();
      chpl_task_data_setup(chpl_comm_on_bundle_task_bundle(args), tls);
      chpl_comm_execute_on_fast(dnode, dsubloc, fn, args, args_size);
    } else {
      var origSubloc = chpl_task_getRequestedSubloc();
      if (dsubloc==origSubloc ||
          (dsubloc==c_sublocid_any &&
           chpl_localeModel_sublocToExecutionSubloc(origSubloc)==origSubloc)) {
        chpl_ftable_call(fn, args);
      } else {
        // move to a different sublocale
        chpl_task_setSubloc(dsubloc);
        chpl_ftable_call(fn, args);
        chpl_task_setSubloc(origSubloc);
      }
    }
  }

  //
  // nonblocking "on" (doesn't wait for completion)
  //
  pragma "insert line file info"
  export
  proc chpl_executeOnNB(in loc: chpl_localeID_t, // target locale
                        fn: int,              // on-body function idx
                        args: chpl_comm_on_bundle_p,     // function args
                        args_size: size_t     // args size
                       ) {
    //
    // If we're in serial mode, we should use blocking rather than
    // non-blocking "on" in order to serialize the execute_ons.
    //
    const dnode =  chpl_nodeFromLocaleID(loc);
    const dsubloc =  chpl_sublocFromLocaleID(loc);
    var tls = chpl_task_getInfoChapel();
    var isSerial = chpl_task_data_getSerial(tls);
    if dnode == chpl_nodeID {
      if isSerial {
        chpl_ftable_call(fn, args);
      } else {
        chpl_task_data_setup(chpl_comm_on_bundle_task_bundle(args), tls);
        chpl_comm_taskCallFTable(fn, args, args_size, dsubloc);
      }
    } else {
      chpl_task_data_setup(chpl_comm_on_bundle_task_bundle(args), tls);
      if isSerial {
        chpl_comm_execute_on(dnode, dsubloc, fn, args, args_size);
      } else {
        chpl_comm_execute_on_nb(dnode, dsubloc, fn, args, args_size);
      }
    }
  }
}
