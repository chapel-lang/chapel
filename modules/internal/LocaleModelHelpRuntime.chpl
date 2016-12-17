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

// LocaleModelHelpRuntime.chpl
//
// Provides for declarations common to the locale model runtime
// but that do not have to be the same in order to meet the
// interface.

// They are in this file as a practical matter to avoid code
// duplication. If necessary, a locale model using this file
// should feel free to reimplement them in some other way.
module LocaleModelHelpRuntime {

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

  extern record chpl_comm_on_bundle_t {
  };

  extern record chpl_task_bundle_t {
  };

  extern type chpl_comm_on_bundle_p;

  extern type chpl_task_bundle_p;

  extern proc chpl_comm_on_bundle_task_bundle(bundle:chpl_comm_on_bundle_p):chpl_task_bundle_p;

  // We need an explicit copy constructor because the compiler cannot create
  // a correct one for a record type whose members are not known to it.
  pragma "init copy fn"
  extern chpl__initCopy_chpl_rt_localeID_t
  proc chpl__initCopy(initial: chpl_localeID_t): chpl_localeID_t;

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
  // support for memory management
  //

  private extern proc chpl_memhook_md_num(): chpl_mem_descInt_t;

  // The allocator pragma is used by scalar replacement.

  // Note that there are 2 nearly identical chpl_here_alloc() functions. This
  // one takes an int(64) size and is marked with "locale model alloc" while
  // the second version takes a generic `integral` size and is not marked
  // "locale model alloc". Calls to the "locale model alloc" version are
  // inserted by the compiler (sometimes after resolution) for class/record
  // allocations. As a result, there can only be a single function with "locale
  // model alloc" in any compilation and the function must be fully specified.
  pragma "allocator"
  pragma "locale model alloc"
  proc chpl_here_alloc(size:int(64), md:chpl_mem_descInt_t): c_void_ptr {
    pragma "insert line file info"
      extern proc chpl_mem_alloc(size:size_t, md:chpl_mem_descInt_t) : c_void_ptr;
    return chpl_mem_alloc(size.safeCast(size_t), md + chpl_memhook_md_num());
  }

  pragma "allocator"
  proc chpl_here_alloc(size:integral, md:chpl_mem_descInt_t): c_void_ptr {
    pragma "insert line file info"
      extern proc chpl_mem_alloc(size:size_t, md:chpl_mem_descInt_t) : c_void_ptr;
    return chpl_mem_alloc(size.safeCast(size_t), md + chpl_memhook_md_num());
  }

  pragma "allocator"
  proc chpl_here_calloc(size:integral, number:integral, md:chpl_mem_descInt_t): c_void_ptr {
    pragma "insert line file info"
      extern proc chpl_mem_calloc(number:size_t, size:size_t, md:chpl_mem_descInt_t) : c_void_ptr;
    return chpl_mem_calloc(number.safeCast(size_t), size.safeCast(size_t), md + chpl_memhook_md_num());
  }

  pragma "allocator"
  proc chpl_here_realloc(ptr:c_void_ptr, size:integral, md:chpl_mem_descInt_t): c_void_ptr {
    pragma "insert line file info"
      extern proc chpl_mem_realloc(ptr:c_void_ptr, size:size_t, md:chpl_mem_descInt_t) : c_void_ptr;
    return chpl_mem_realloc(ptr, size.safeCast(size_t), md + chpl_memhook_md_num());
  }

  proc chpl_here_good_alloc_size(min_size:integral): min_size.type {
    pragma "insert line file info"
      extern proc chpl_mem_good_alloc_size(min_size:size_t) : size_t;
    return chpl_mem_good_alloc_size(min_size.safeCast(size_t)).safeCast(min_size.type);
  }

  pragma "locale model free"
  proc chpl_here_free(ptr:c_void_ptr): void {
    pragma "insert line file info"
      extern proc chpl_mem_free(ptr:c_void_ptr) : void;
    chpl_mem_free(ptr);
  }

  //////////////////////////////////////////
  //
  // support for "on" statements
  //

  //
  // runtime interface
  //
  extern proc chpl_comm_execute_on(loc_id: int, subloc_id: int, fn: int,
                                   args: chpl_comm_on_bundle_p, arg_size: size_t);
  extern proc chpl_comm_execute_on_fast(loc_id: int, subloc_id: int, fn: int,
                                        args: chpl_comm_on_bundle_p, args_size: size_t);
  extern proc chpl_comm_execute_on_nb(loc_id: int, subloc_id: int, fn: int,
                                      args: chpl_comm_on_bundle_p, args_size: size_t);
  pragma "insert line file info"
    extern proc chpl_comm_taskCallFTable(fn: int,
                                         args: chpl_comm_on_bundle_p, args_size: size_t,
                                         subloc_id: int): void;
  extern proc chpl_ftable_call(fn: int, args: chpl_comm_on_bundle_p): void;

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
    chpl_task_addToTaskList(fn, args, args_size,
                            subloc_id, tlist, tlist_node_id, true);
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
    chpl_task_addToTaskList(fn, args, args_size,
                            subloc_id, tlist, tlist_node_id, false);
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
