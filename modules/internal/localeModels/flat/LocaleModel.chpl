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

// LocaleModel.chpl
//
// This provides a flat locale model architectural description.  The
// locales contain memory and a multi-core processor with homogeneous
// cores, and we ignore any affinity (NUMA effects) between the
// processor cores and the memory.  This architectural description is
// backward compatible with the architecture implicitly provided by
// releases 1.6 and preceding.
//
module LocaleModel {

  param localeModelHasSublocales = false;

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

  const chpl_emptyLocaleSpace: domain(1) = {1..0};
  const chpl_emptyLocales: [chpl_emptyLocaleSpace] locale;

  //
  // A concrete class representing the nodes in this architecture.
  //
  class LocaleModel : AbstractLocaleModel {
    const _node_id : int;
    const local_name : string;

    // This constructor must be invoked "on" the node
    // that it is intended to represent.  This trick is used
    // to establish the equivalence the "locale" field of the locale object
    // and the node ID portion of any wide pointer referring to it.
    proc LocaleModel() {
      if doneCreatingLocales {
        halt("Cannot create additional LocaleModel instances");
      }
      setup();
    }

    proc LocaleModel(parent_loc : locale) {
      if doneCreatingLocales {
        halt("Cannot create additional LocaleModel instances");
      }
      parent = parent_loc;
      setup();
    }

    proc chpl_id() return _node_id; // top-level node number
    proc chpl_localeid() {
      return chpl_buildLocaleID(_node_id:chpl_nodeID_t, c_sublocid_any);
    }
    proc chpl_name() return local_name;


    proc writeThis(f) {
      // Most classes will define it like this:
//      f <~> name;
      // but here it is defined thus for backward compatibility.
      f <~> new ioLiteral("LOCALE") <~> _node_id;
    }

    proc getChildSpace() return chpl_emptyLocaleSpace;

    proc getChildCount() return 0;

    iter getChildIndices() : int {
      for idx in chpl_emptyLocaleSpace do
        yield idx;
    }

    proc getChild(idx:int) : locale {
      if boundsChecking then
        halt("requesting a child from a LocaleModel locale");
      return nil;
    }

    iter getChildren() : locale  {
      for loc in chpl_emptyLocales do
        yield loc;
    }

    proc getChildArray() {
      return chpl_emptyLocales;
    }

    //------------------------------------------------------------------------{
    //- Implementation (private)
    //-
    proc setup() {
      _node_id = chpl_nodeID: int;

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
      then local_name = chpl_nodeName():string + "-" + _node_id:string;
      else local_name = chpl_nodeName():string;

      extern proc chpl_task_getCallStackSize(): size_t;
      callStackSize = chpl_task_getCallStackSize();

      extern proc chpl_getNumPhysicalCpus(accessible_only: bool): c_int;
      nPUsPhysAcc = chpl_getNumPhysicalCpus(true);
      nPUsPhysAll = chpl_getNumPhysicalCpus(false);

      extern proc chpl_getNumLogicalCpus(accessible_only: bool): c_int;
      nPUsLogAcc = chpl_getNumLogicalCpus(true);
      nPUsLogAll = chpl_getNumLogicalCpus(false);

      extern proc chpl_task_getMaxPar(): uint(32);
      maxTaskPar = chpl_task_getMaxPar();
    }
    //------------------------------------------------------------------------}
  }

  //
  // An instance of this class is the default contents 'rootLocale'.
  //
  // In the current implementation a platform-specific architectural description
  // may overwrite this instance or any of its children to establish a more customized
  // representation of the system resources.
  //
  class RootLocale : AbstractRootLocale {

    const myLocaleSpace: domain(1) = {0..numLocales-1};
    var myLocales: [myLocaleSpace] locale;

    proc RootLocale() {
      parent = nil;
      nPUsPhysAcc = 0;
      nPUsPhysAll = 0;
      nPUsLogAcc = 0;
      nPUsLogAll = 0;
      maxTaskPar = 0;
    }

    // The init() function must use chpl_initOnLocales() to iterate (in
    // parallel) over the locales to set up the LocaleModel object.
    // In addition, the initial 'here' must be set.
    proc setup() {
      forall locIdx in chpl_initOnLocales() {
        const node = new LocaleModel(this);
        myLocales[locIdx] = node;
        nPUsPhysAcc += node.nPUsPhysAcc;
        nPUsPhysAll += node.nPUsPhysAll;
        nPUsLogAcc += node.nPUsLogAcc;
        nPUsLogAll += node.nPUsLogAll;
        maxTaskPar += node.maxTaskPar;
      }

      here.runningTaskCntSet(0);  // locale init parallelism mis-sets this
    }

    // Has to be globally unique and not equal to a node ID.
    // We return numLocales for now, since we expect nodes to be
    // numbered less than this.
    // -1 is used in the abstract locale class to specify an invalid node ID.
    proc chpl_id() return numLocales;
    proc chpl_localeid() {
      return chpl_buildLocaleID(numLocales:chpl_nodeID_t, c_sublocid_none);
    }
    proc chpl_name() return local_name();
    proc local_name() return "rootLocale";

    proc writeThis(f) {
      f <~> name;
    }

    proc getChildCount() return this.myLocaleSpace.numIndices;

    proc getChildSpace() return this.myLocaleSpace;

    iter getChildIndices() : int {
      for idx in this.myLocaleSpace do
        yield idx;
    }

    proc getChild(idx:int) return this.myLocales[idx];

    iter getChildren() : locale  {
      for loc in this.myLocales do
        yield loc;
    }

    proc getDefaultLocaleSpace() const ref return this.myLocaleSpace;
    proc getDefaultLocaleArray() const ref return myLocales;

    proc localeIDtoLocale(id : chpl_localeID_t) {
      // In the default architecture, there are only nodes and no sublocales.
      // What is more, the nodeID portion of a wide pointer is the same as
      // the index into myLocales that yields the locale representing that
      // node.
      return myLocales[chpl_rt_nodeFromLocaleID(id)];
    }
  }

  //////////////////////////////////////////
  //
  // utilities
  //
  inline
  proc chpl_getSubloc() {
    compilerError("must not call chpl_getSubloc() ",
                  "when locale model lacks sublocales");
    return c_sublocid_none;
  }

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
                                   args: c_void_ptr, arg_size: size_t);
  extern proc chpl_comm_execute_on_fast(loc_id: int, subloc_id: int, fn: int,
                                        args: c_void_ptr, args_size: size_t);
  extern proc chpl_comm_execute_on_nb(loc_id: int, subloc_id: int, fn: int,
                                      args: c_void_ptr, args_size: size_t);
  pragma "insert line file info"
    extern proc chpl_task_taskCallFTable(fn: int,
                                         args: c_void_ptr, args_size: size_t,
                                         subloc_id: int): void;
  extern proc chpl_ftable_call(fn: int, args: c_void_ptr): void;

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
                      args: c_void_ptr,     // function args
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
        chpl_ftable_call(fn, args);
      else
        chpl_task_taskCallFTable(fn, args, args_size, c_sublocid_any);
    } else {
      if __primitive("task_get_serial") then
        chpl_comm_execute_on(node, c_sublocid_any, fn, args, args_size);
      else
        chpl_comm_execute_on_nb(node, c_sublocid_any, fn, args, args_size);
    }
  }

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
