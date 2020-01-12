/*
 * Copyright 2004-2020 Cray Inc.
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

// LocaleModelHelpRuntime.chpl
//
// Provides for declarations common to the locale model runtime
// but that do not have to be the same in order to meet the
// interface.

// They are in this file as a practical matter to avoid code
// duplication. If necessary, a locale model using this file
// should feel free to reimplement them in some other way.
module LocaleModelHelpRuntime {
  private use ChapelStandard, SysCTypes;

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

  // runtime stuff about argument bundles
  extern record chpl_comm_on_bundle_t {
  };

  extern record chpl_task_bundle_t {
  };

  extern type chpl_comm_on_bundle_p;

  extern type chpl_task_bundle_p;

  pragma "fn synchronization free"
  extern proc chpl_comm_on_bundle_task_bundle(bundle:chpl_comm_on_bundle_p):chpl_task_bundle_p;

  // We need an explicit copy constructor because the compiler cannot create
  // a correct one for a record type whose members are not known to it.
  pragma "init copy fn"
  pragma "fn synchronization free"
  extern "chpl__initCopy_chpl_rt_localeID_t"
  proc chpl__initCopy(initial: chpl_localeID_t): chpl_localeID_t;

  // Runtime interface for manipulating global locale IDs.
  pragma "fn synchronization free"
  extern
    proc chpl_rt_buildLocaleID(node: chpl_nodeID_t,
                               subloc: chpl_sublocID_t): chpl_localeID_t;

  pragma "fn synchronization free"
  extern
    proc chpl_rt_nodeFromLocaleID(loc: chpl_localeID_t): chpl_nodeID_t;

  pragma "fn synchronization free"
  extern
    proc chpl_rt_sublocFromLocaleID(loc: chpl_localeID_t): chpl_sublocID_t;

  // Compiler (and module code) interface for manipulating global locale IDs..
  pragma "insert line file info"
  export
  proc chpl_buildLocaleID(node: chpl_nodeID_t, subloc: chpl_sublocID_t)
    return chpl_rt_buildLocaleID(node, subloc);

  pragma "insert line file info"
  export
  proc chpl_nodeFromLocaleID(loc: chpl_localeID_t)
    return chpl_rt_nodeFromLocaleID(loc);

  pragma "insert line file info"
  export
  proc chpl_sublocFromLocaleID(loc: chpl_localeID_t)
    return chpl_rt_sublocFromLocaleID(loc);

  //////////////////////////////////////////
  //
  // support for "on" statements
  //

  //
  // runtime interface
  //
  pragma "insert line file info"
  extern proc chpl_comm_execute_on(loc_id: int, subloc_id: int, fn: int,
                                   args: chpl_comm_on_bundle_p, arg_size: size_t);
  pragma "insert line file info"
  extern proc chpl_comm_execute_on_fast(loc_id: int, subloc_id: int, fn: int,
                                        args: chpl_comm_on_bundle_p, args_size: size_t);
  pragma "insert line file info"
  extern proc chpl_comm_execute_on_nb(loc_id: int, subloc_id: int, fn: int,
                                      args: chpl_comm_on_bundle_p, args_size: size_t);
  pragma "insert line file info"
    extern proc chpl_comm_taskCallFTable(fn: int,
                                         args: chpl_comm_on_bundle_p, args_size: size_t,
                                         subloc_id: int): void;
  extern proc chpl_ftable_call(fn: int, args: chpl_comm_on_bundle_p): void;
  extern proc chpl_ftable_call(fn: int, args: chpl_task_bundle_p): void;

  //////////////////////////////////////////
  //
  // support for tasking statements: begin, cobegin, coforall
  //

  //
  // runtime interface
  //
  pragma "insert line file info"
  extern proc chpl_task_addToTaskList(fn: int,
                                      args: chpl_task_bundle_p, args_size: size_t,
                                      subloc_id: int,
                                      ref tlist: c_void_ptr, tlist_node_id: int,
                                      is_begin: bool);
  extern proc chpl_task_executeTasksInList(ref tlist: c_void_ptr);
  extern proc chpl_task_yield();

  //
  // add a task to a list of tasks being built for a begin statement
  //
  pragma "insert line file info"
  export
  proc chpl_taskListAddBegin(subloc_id: int,        // target sublocale
                             fn: int,               // task body function idx
                             args: chpl_task_bundle_p,      // function args
                             args_size: size_t,     // args size
                             ref tlist: c_void_ptr, // task list
                             tlist_node_id: int     // task list owner node
                            ) {
    var tls = chpl_task_getChapelData();
    var isSerial = chpl_task_data_getSerial(tls);
    if isSerial {
      chpl_ftable_call(fn, args);
    } else {
      chpl_task_data_setup(args, tls);
      chpl_task_addToTaskList(fn, args, args_size,
                              subloc_id, tlist, tlist_node_id, true);
    }
  }

  //
  // add a task to a list of tasks being built for a cobegin or coforall
  // statement
  //
  pragma "insert line file info"
  export
  proc chpl_taskListAddCoStmt(subloc_id: int,        // target sublocale
                              fn: int,               // task body function idx
                              args: chpl_task_bundle_p,      // function args
                              args_size: size_t,     // args size
                              ref tlist: c_void_ptr, // task list
                              tlist_node_id: int     // task list owner node
                             ) {
    var tls = chpl_task_getChapelData();
    var isSerial = chpl_task_data_getSerial(tls);
    if isSerial {
      chpl_ftable_call(fn, args);
    } else {
      chpl_task_data_setup(args, tls);
      chpl_task_addToTaskList(fn, args, args_size,
                              subloc_id, tlist, tlist_node_id, false);
     }
  }

  //
  // make sure all tasks in a list have an opportunity to run
  //
  pragma "insert line file info"
  export
  proc chpl_taskListExecute(ref task_list: c_void_ptr) {
    // note: if we're serial, all of the tasks have already
    // been executed. Tasking layers should tolerate empty task
    // lists for this reason.
    chpl_task_executeTasksInList(task_list);
  }

}
