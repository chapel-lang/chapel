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

module LocaleModelHelpNUMA {

  param localeModelHasSublocales = true;

  use LocaleModelHelp;

  proc helpSetupLocaleNUMA(dst:LocaleModel, out local_name:string, out numSublocales) {
    helpSetupLocaleFlat(dst, local_name);

    extern proc chpl_task_getNumSublocales(): int(32);
    numSublocales = chpl_task_getNumSublocales();

    extern proc chpl_task_getMaxPar(): uint(32);

    if numSublocales >= 1 {
      dst.childSpace = {0..#numSublocales};
      // These nPUs* values are estimates only; better values await
      // full hwloc support. In particular it assumes a homogeneous node
      const nPUsPhysAccPerSubloc = dst.nPUsPhysAcc/numSublocales;
      const nPUsPhysAllPerSubloc = dst.nPUsPhysAll/numSublocales;
      const nPUsLogAccPerSubloc = dst.nPUsLogAcc/numSublocales;
      const nPUsLogAllPerSubloc = dst.nPUsLogAll/numSublocales;
      const maxTaskParPerSubloc = chpl_task_getMaxPar()/numSublocales;
      const origSubloc = chpl_task_getRequestedSubloc(); // this should be any
      for i in dst.childSpace {
        // allocate the structure on the proper sublocale
        chpl_task_setSubloc(i:chpl_sublocID_t);
        dst.childLocales[i] = new NumaDomain(i:chpl_sublocID_t, dst);
        dst.childLocales[i].nPUsPhysAcc = nPUsPhysAccPerSubloc;
        dst.childLocales[i].nPUsPhysAll = nPUsPhysAllPerSubloc;
        dst.childLocales[i].nPUsLogAcc = nPUsLogAccPerSubloc;
        dst.childLocales[i].nPUsLogAll = nPUsLogAllPerSubloc;
        dst.childLocales[i].maxTaskPar = maxTaskParPerSubloc;
      }
      chpl_task_setSubloc(origSubloc);
    }
  }

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
  extern proc chpl_comm_execute_on(loc_id: int, subloc_id: int, fn: int,
                                   args: c_void_ptr, arg_size: size_t);
  extern proc chpl_comm_execute_on_fast(loc_id: int, subloc_id: int, fn: int,
                                        args: c_void_ptr, args_size: size_t);
  extern proc chpl_comm_execute_on_nb(loc_id: int, subloc_id: int, fn: int,
                                      args: c_void_ptr, args_size: size_t);
  extern proc chpl_ftable_call(fn: int, args: c_void_ptr): void;
  extern proc chpl_task_setSubloc(subloc: int(32));

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
    const dnode =  chpl_nodeFromLocaleID(loc);
    const dsubloc =  chpl_sublocFromLocaleID(loc);
    if dnode != chpl_nodeID {
      chpl_comm_execute_on(dnode, dsubloc, fn, args, args_size);
    } else {
      // run directly on this node
      var origSubloc = chpl_task_getRequestedSubloc();
      if (dsubloc==c_sublocid_any || dsubloc==origSubloc) {
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
  proc chpl_executeOnFast(loc: chpl_localeID_t, // target locale
                          fn: int,              // on-body function idx
                          args: c_void_ptr,     // function args
                          args_size: size_t     // args size
                         ) {
    const dnode =  chpl_nodeFromLocaleID(loc);
    const dsubloc =  chpl_sublocFromLocaleID(loc);
    if dnode != chpl_nodeID {
      chpl_comm_execute_on_fast(dnode, dsubloc, fn, args, args_size);
    } else {
      var origSubloc = chpl_task_getRequestedSubloc();
      if (dsubloc==c_sublocid_any || dsubloc==origSubloc) {
        chpl_ftable_call(fn, args);
      } else {
        // move to a different sublocale
        chpl_task_setSubloc(dsubloc);
        chpl_ftable_call(fn, args);
        chpl_task_setSubloc(origSubloc);
      }
    }
  }

  // Unused for now due to bug when using 'begin'
  inline proc chpl_executeOnNBAux(fn: int,         // on-body function idx
                                  args: c_void_ptr // function args
                                  ) {
    if __primitive("task_get_serial") then
      chpl_ftable_call(fn, args);
    else
      begin chpl_ftable_call(fn, args);
  }
  //
  // nonblocking "on" (doesn't wait for completion)
  //
  param useBegin = false;
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
    const dnode =  chpl_nodeFromLocaleID(loc);
    const dsubloc =  chpl_sublocFromLocaleID(loc);
    if dnode != chpl_nodeID {
      if __primitive("task_get_serial") then
        chpl_comm_execute_on(dnode, dsubloc, fn, args, args_size);
      else
        chpl_comm_execute_on_nb(dnode, dsubloc, fn, args, args_size);
    } else {
      var origSubloc = chpl_task_getRequestedSubloc();
      // We'd like to call chpl_executeOnNBaux() here, but the begin
      //  statement seems to cause a problem
      if (dsubloc==c_sublocid_any || dsubloc==origSubloc) {
        // run on this sublocale
        if useBegin {
          chpl_executeOnNBAux(fn, args);
        } else {
          if __primitive("task_get_serial") then
            chpl_ftable_call(fn, args);
          else
            // begin chpl_ftable_call(fn, args);
            chpl_comm_execute_on_nb(dnode, dsubloc, fn, args, args_size);
        }
      } else {
        // move to a different sublocale
        chpl_task_setSubloc(dsubloc);
        if useBegin {
          chpl_executeOnNBAux(fn, args);
        } else {
          if __primitive("task_get_serial") then
            chpl_ftable_call(fn, args);
          else
            // begin chpl_ftable_call(fn, args);
            chpl_comm_execute_on_nb(dnode, dsubloc, fn, args, args_size);
        }
        chpl_task_setSubloc(origSubloc);
      }
    }
  }
}
