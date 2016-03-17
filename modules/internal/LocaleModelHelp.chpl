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

// LocaleModelHelp.chpl
//
// Provides for declarations common to locale models
// but that do not have to be the same in order to meet the
// interface.

// They are in this file as a practical matter to avoid code
// duplication. If necessary, a locale model using this file
// should feel free to reimplement them in some other way.
module LocaleModelHelp {


  use ChapelLocale;
  use DefaultRectangular;
  use ChapelNumLocales;
  use Sys;

  config param debugLocaleModel = false;

  // We would really like a class-static storage class.(C++ nomenclature)
  var doneCreatingLocales: bool = false;

  // The chpl_localeID_t type is used internally.  It should not be exposed to
  // the user.  The runtime defines the actual type, as well as a functional
  // interface for assembling and disassembling chpl_localeID_t values.  This
  // module then provides the interface the compiler-emitted code uses to do
  // the same.

  extern record chpl_localeID_t {
    // We need to know that this is a record type in order to pass it to and
    // return it from runtime functions properly, but we don't need or want
    // to see its contents.
  };

  // We need an explicit copy constructor because the compiler cannot create
  // a correct one for a record type whose members are not known to it.
  pragma "init copy fn"
  extern chpl__initCopy_chpl_rt_localeID_t
  proc chpl__initCopy(initial: chpl_localeID_t): chpl_localeID_t;

  extern var chpl_nodeID: chpl_nodeID_t;

  // Runtime interface for manipulating global locale IDs.
  extern
    proc chpl_rt_buildLocaleID(node: chpl_nodeID_t,
                               subloc: chpl_sublocID_t): chpl_localeID_t;
  extern
    proc chpl_rt_nodeFromLocaleID(loc: chpl_localeID_t): chpl_nodeID_t;

  extern
    proc chpl_rt_sublocFromLocaleID(loc: chpl_localeID_t): chpl_sublocID_t;

  // Compiler (and module code) interface for manipulating global locale IDs..
  pragma "insert line file info"
  proc chpl_buildLocaleID(node: chpl_nodeID_t, subloc: chpl_sublocID_t)
    return chpl_rt_buildLocaleID(node, subloc);

  pragma "insert line file info"
  proc chpl_nodeFromLocaleID(loc: chpl_localeID_t)
    return chpl_rt_nodeFromLocaleID(loc);

  pragma "insert line file info"
  proc chpl_sublocFromLocaleID(loc: chpl_localeID_t)
    return chpl_rt_sublocFromLocaleID(loc);

  record chpl_root_locale_accum {
    var nPUsPhysAcc: atomic int;
    var nPUsPhysAll: atomic int;
    var nPUsLogAcc: atomic int;
    var nPUsLogAll: atomic int;
    var maxTaskPar: atomic int;

    proc accum(loc:locale) {
      nPUsPhysAcc.add(loc.nPUsPhysAcc);
      nPUsPhysAll.add(loc.nPUsPhysAll);
      nPUsLogAcc.add(loc.nPUsLogAcc);
      nPUsLogAll.add(loc.nPUsLogAll);
      maxTaskPar.add(loc.maxTaskPar);
    }
    proc setRootLocaleValues(dst:RootLocale) {
      dst.nPUsPhysAcc = nPUsPhysAcc.read();
      dst.nPUsPhysAll = nPUsPhysAll.read();
      dst.nPUsLogAcc = nPUsLogAcc.read();
      dst.nPUsLogAll = nPUsLogAll.read();
      dst.maxTaskPar = maxTaskPar.read();
    }
  }

  proc helpSetupRootLocaleFlat(dst:RootLocale) {
    var root_accum:chpl_root_locale_accum;

    forall locIdx in dst.chpl_initOnLocales() {
      const node = new LocaleModel(dst);
      dst.myLocales[locIdx] = node;
      root_accum.accum(node);
    }

    root_accum.setRootLocaleValues(dst);
    here.runningTaskCntSet(0);  // locale init parallelism mis-sets this
  }

  proc helpSetupRootLocaleNUMA(dst:RootLocale) {
    var root_accum:chpl_root_locale_accum;

    forall locIdx in dst.chpl_initOnLocales() {
      chpl_task_setSubloc(c_sublocid_any);
      const node = new LocaleModel(dst);
      dst.myLocales[locIdx] = node;
      root_accum.accum(node);
    }

    root_accum.setRootLocaleValues(dst);
    here.runningTaskCntSet(0);  // locale init parallelism mis-sets this
  }


  proc helpSetupLocaleFlat(dst:LocaleModel, out local_name:string) {
    const _node_id = chpl_nodeID: int;

    // chpl_nodeName is defined in chplsys.c.
    // It supplies a node name obtained by running uname(3) on the
    // current node.  For this reason (as well), the constructor (or
    // at least this init method) must be run on the node it is
    // intended to describe.
    var comm, spawnfn : c_string;
    extern proc chpl_nodeName() : c_string;
    // sys_getenv returns zero on success.
    if sys_getenv(c"CHPL_COMM", comm) == 0 && comm == c"gasnet" &&
      sys_getenv(c"GASNET_SPAWNFN", spawnfn) == 0 && spawnfn == c"L"
    then local_name = chpl_nodeName() + "-" + _node_id : string;
    else local_name = chpl_nodeName():string;

    extern proc chpl_task_getCallStackSize(): size_t;
    dst.callStackSize = chpl_task_getCallStackSize();

    extern proc chpl_getNumPhysicalCpus(accessible_only: bool): c_int;
    dst.nPUsPhysAcc = chpl_getNumPhysicalCpus(true);
    dst.nPUsPhysAll = chpl_getNumPhysicalCpus(false);

    extern proc chpl_getNumLogicalCpus(accessible_only: bool): c_int;
    dst.nPUsLogAcc = chpl_getNumLogicalCpus(true);
    dst.nPUsLogAll = chpl_getNumLogicalCpus(false);

    extern proc chpl_task_getMaxPar(): uint(32);
    dst.maxTaskPar = chpl_task_getMaxPar();
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



  //////////////////////////////////////////
  //
  // support for tasking statements: begin, cobegin, coforall
  //

  //
  // runtime interface
  //
  pragma "insert line file info"
  extern proc chpl_task_addToTaskList(fn: int, args: c_void_ptr, subloc_id: int,
                                      ref tlist: c_void_ptr, tlist_node_id: int,
                                      is_begin: bool);
  extern proc chpl_task_executeTasksInList(ref tlist: c_void_ptr);

  //
  // add a task to a list of tasks being built for a begin statement
  //
  pragma "insert line file info"
  export
  proc chpl_taskListAddBegin(subloc_id: int,        // target sublocale
                             fn: int,               // task body function idx
                             args: c_void_ptr,      // function args
                             ref tlist: c_void_ptr, // task list
                             tlist_node_id: int     // task list owner node
                            ) {
    chpl_task_addToTaskList(fn, args, subloc_id, tlist, tlist_node_id, true);
  }

  //
  // add a task to a list of tasks being built for a cobegin or coforall
  // statement
  //
  pragma "insert line file info"
  export
  proc chpl_taskListAddCoStmt(subloc_id: int,        // target sublocale
                              fn: int,               // task body function idx
                              args: c_void_ptr,      // function args
                              ref tlist: c_void_ptr, // task list
                              tlist_node_id: int     // task list owner node
                             ) {
    chpl_task_addToTaskList(fn, args, subloc_id, tlist, tlist_node_id, false);
  }

  //
  // make sure all tasks in a list have an opportunity to run
  //
  pragma "insert line file info"
  export
  proc chpl_taskListExecute(ref task_list: c_void_ptr) {
    chpl_task_executeTasksInList(task_list);
  }

}
