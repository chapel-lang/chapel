/*
 * Copyright 2004-2015 Cray Inc.
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

  param localeModelHasSublocales = true;

  use ChapelLocale;
  use DefaultRectangular;
  use ChapelNumLocales;
  use Sys;

  extern proc chpl_task_getRequestedSubloc(): int(32);

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


  //
  // The NUMA sublocale model
  //
  class NumaDomain : AbstractLocaleModel {
    const sid: chpl_sublocID_t;
    const name: string;

    proc chpl_id() return (parent:LocaleModel)._node_id; // top-level node id
    proc chpl_localeid() {
      return chpl_buildLocaleID((parent:LocaleModel)._node_id:chpl_nodeID_t,
                                sid); 
    }
    proc chpl_name() return name;

    proc NumaDomain() {
    }

    proc NumaDomain(_sid, _parent) {
      sid = _sid;
      name = "ND"+sid;
      parent = _parent;
    }

    proc readWriteThis(f) {
      parent.readWriteThis(f);
      f <~> '.'+name;
    }

    proc getChildCount(): int { return 0; }
    iter getChildIndices() : int {
      halt("No children to iterate over.");
      yield -1;
    }
    proc addChild(loc:locale) { halt("Cannot add children to this locale type."); }
    proc getChild(idx:int) : locale { return nil; }

    // This is commented out b/c it leads to an internal error during
    // the resolveIntents pass.  See
    // test/functions/iterators/sungeun/iterInClass.future
    //
    // iter getChildren() : locale {
    //  halt("No children to iterate over.");
    //  yield nil;
    // }
  }

  //
  // The node model
  //
  class LocaleModel : AbstractLocaleModel {
    const callStackSize: size_t;
    const _node_id : int;
    const local_name : string;

    const numSublocales: int;
    var childSpace: domain(1);
    var childLocales: [childSpace] NumaDomain;

    // This constructor must be invoked "on" the node
    // that it is intended to represent.  This trick is used
    // to establish the equivalence the "locale" field of the locale object
    // and the node ID portion of any wide pointer referring to it.
    proc LocaleModel() {
      if doneCreatingLocales {
        halt("Cannot create additional LocaleModel instances");
      }
      init();
    }

    proc LocaleModel(parent_loc : locale) {
      if doneCreatingLocales {
        halt("Cannot create additional LocaleModel instances");
      }
      parent = parent_loc;
      init();
    }

    proc chpl_id() return _node_id;     // top-level locale (node) number
    proc chpl_localeid() {
      return chpl_buildLocaleID(_node_id:chpl_nodeID_t, c_sublocid_any); 
    }
    proc chpl_name() return local_name;


    proc readWriteThis(f) {
      // Most classes will define it like this:
//      f <~> name;
      // but here it is defined thus for backward compatibility.
      f <~> new ioLiteral("LOCALE") <~> _node_id;
    }

    proc getChildSpace() return childSpace;

    proc getChildCount() return numSublocales;

    iter getChildIndices() : int {
      for idx in childSpace do
        yield idx;
    }

    proc getChild(idx:int) : locale {
      if boundsChecking then
        if (idx < 0) || (idx >= numSublocales) then
          halt("sublocale child index out of bounds (",idx,")");
      return childLocales[idx];
    }

    iter getChildren() : locale  {
      for loc in childLocales do
        yield loc;
    }

    proc getChildArray() {
      return childLocales;
    }

    //------------------------------------------------------------------------{
    //- Implementation (private)
    //-
    proc init() {
      _node_id = chpl_nodeID: int;

      // chpl_nodeName is defined in chplsys.c.
      // It supplies a node name obtained by running uname(3) on the
      // current node.  For this reason (as well), the constructor (or
      // at least this init method) must be run on the node it is
      // intended to describe.
      var comm, spawnfn : c_string;
      extern proc chpl_nodeName() : c_string;
      // sys_getenv returns zero on success.
      if sys_getenv("CHPL_COMM".c_str(), comm) == 0 && comm == "gasnet" &&
        sys_getenv("GASNET_SPAWNFN".c_str(), spawnfn) == 0 && spawnfn == "L"
      then local_name = chpl_nodeName():string + "-" + _node_id : string;
      else local_name = chpl_nodeName():string;

      extern proc chpl_task_getCallStackSize(): size_t;
      callStackSize = chpl_task_getCallStackSize();

      extern proc chpl_getNumLogicalCpus(accessible_only: bool): c_int;
      numCores = chpl_getNumLogicalCpus(true);

      extern proc chpl_task_getNumSublocales(): int(32);
      numSublocales = chpl_task_getNumSublocales();

      extern proc chpl_task_getMaxPar(): uint(32);
      maxTaskPar = if numSublocales==0 then chpl_task_getMaxPar()
                                       else numSublocales;

      if numSublocales >= 1 {
        childSpace = {0..#numSublocales};
        const numCoresPerNumaDomain = numCores/numSublocales;
        const maxTaskParPerNumaDomain = chpl_task_getMaxPar()/numSublocales;
        const origSubloc = chpl_task_getRequestedSubloc(); // this should be any
        for i in childSpace {
          // allocate the structure on the proper sublocale
          chpl_task_setSubloc(i:chpl_sublocID_t);
          childLocales[i] = new NumaDomain(i:chpl_sublocID_t, this);
          childLocales[i].numCores = numCoresPerNumaDomain;
          childLocales[i].maxTaskPar = maxTaskParPerNumaDomain;
        }
        chpl_task_setSubloc(origSubloc);
      }
    }
    //------------------------------------------------------------------------}

    inline proc subloc return c_sublocid_any;
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
      numCores = 0;
      maxTaskPar = 0;
    }

    // The init() function must use chpl_initOnLocales() to iterate (in
    // parallel) over the locales to set up the LocaleModel object.
    // In addition, the initial 'here' must be set.
    proc init() {
      forall locIdx in chpl_initOnLocales() {
        chpl_task_setSubloc(c_sublocid_any);
        const node = new LocaleModel(this);
        myLocales[locIdx] = node;
        numCores += node.numCores;
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
    proc local_name() return "rootLocale":string;

    proc readWriteThis(f) {
      f <~> name;
    }

    proc getChildCount() return this.myLocaleSpace.numIndices;

    proc getChildSpace() return this.myLocaleSpace;

    iter getChildIndices() : int {
      for idx in this.myLocaleSpace do
        yield idx;
    }

    proc getChild(idx:int) return this.myLocales[idx];

    iter getChlidren() : locale  {
      for loc in this.myLocales do
        yield loc;
    }

    proc getDefaultLocaleSpace() return this.myLocaleSpace;
    proc getDefaultLocaleArray() return myLocales;

    proc localeIDtoLocale(id : chpl_localeID_t) {
      const node = chpl_nodeFromLocaleID(id);
      const subloc = chpl_sublocFromLocaleID(id);
      if (subloc == c_sublocid_none) || (subloc == c_sublocid_any) then
        return (myLocales[node:int]):locale;
      else
        return (myLocales[node:int].getChild(subloc:int)):locale;
    }
  }

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
  // support for memory management
  //

  // The allocator pragma is used by scalar replacement.
  pragma "allocator"
  pragma "locale model alloc"
  proc chpl_here_alloc(size:int, md:int(16)) {
    pragma "insert line file info"
      extern proc chpl_mem_alloc(size:int, md:int(16)) : opaque;
    return chpl_mem_alloc(size, md + chpl_memhook_md_num());
  }

  pragma "allocator"
  proc chpl_here_calloc(size:int, number:int, md:int(16)) {
    pragma "insert line file info"
      extern proc chpl_mem_calloc(number:int, size:int, md:int(16)) : opaque;
    return chpl_mem_calloc(number, size, md + chpl_memhook_md_num());
  }

  pragma "allocator"
  proc chpl_here_realloc(ptr:opaque, size:int, md:int(16)) {
    pragma "insert line file info"
      extern proc chpl_mem_realloc(ptr:opaque, size:int, md:int(16)) : opaque;
    return chpl_mem_realloc(ptr, size, md + chpl_memhook_md_num());
  }

  pragma "locale model free"
  proc chpl_here_free(ptr:opaque) {
    pragma "insert line file info"
      extern proc chpl_mem_free(ptr:opaque): void;
    chpl_mem_free(ptr);
  }


  //////////////////////////////////////////
  //
  // support for "on" statements
  //

  //
  // runtime interface
  //
  extern proc chpl_comm_fork(loc_id: int, subloc_id: int,
                             fn: int, args: c_void_ptr, arg_size: int(32));
  extern proc chpl_comm_fork_fast(loc_id: int, subloc_id: int,
                                  fn: int, args: c_void_ptr, args_size: int(32));
  extern proc chpl_comm_fork_nb(loc_id: int, subloc_id: int,
                                fn: int, args: c_void_ptr, args_size: int(32));
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
                      args_size: int(32)    // args size
                     ) {
    const dnode =  chpl_nodeFromLocaleID(loc);
    const dsubloc =  chpl_sublocFromLocaleID(loc);
    if dnode != chpl_nodeID {
      chpl_comm_fork(dnode, dsubloc, fn, args, args_size);
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
                          args_size: int(32)    // args size
                         ) {
    const dnode =  chpl_nodeFromLocaleID(loc);
    const dsubloc =  chpl_sublocFromLocaleID(loc);
    if dnode != chpl_nodeID {
      chpl_comm_fork_fast(dnode, dsubloc, fn, args, args_size);
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
                        args_size: int(32)    // args size
                       ) {
    //
    // If we're in serial mode, we should use blocking rather than
    // non-blocking "on" in order to serialize the forks.
    //
    const dnode =  chpl_nodeFromLocaleID(loc);
    const dsubloc =  chpl_sublocFromLocaleID(loc);
    if dnode != chpl_nodeID {
      if __primitive("task_get_serial") then
        chpl_comm_fork(dnode, dsubloc, fn, args, args_size);
      else
        chpl_comm_fork_nb(dnode, dsubloc, fn, args, args_size);
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
            chpl_comm_fork_nb(dnode, dsubloc, fn, args, args_size);
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
            chpl_comm_fork_nb(dnode, dsubloc, fn, args, args_size);
        }
        chpl_task_setSubloc(origSubloc);
      }
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
                                      ref tlist: _task_list, tlist_node_id: int,
                                      is_begin: bool);
  extern proc chpl_task_processTaskList(tlist: _task_list);
  extern proc chpl_task_executeTasksInList(tlist: _task_list);
  extern proc chpl_task_freeTaskList(tlist: _task_list);

  //
  // add a task to a list of tasks being built for a begin statement
  //
  pragma "insert line file info"
  export
  proc chpl_taskListAddBegin(subloc_id: int,        // target sublocale
                             fn: int,               // task body function idx
                             args: c_void_ptr,      // function args
                             ref tlist: _task_list, // task list
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
                              ref tlist: _task_list, // task list
                              tlist_node_id: int     // task list owner node
                             ) {
    chpl_task_addToTaskList(fn, args, subloc_id, tlist, tlist_node_id, false);
  }

  //
  // make sure all tasks in a list are known to the tasking layer
  //
  pragma "insert line file info"
  export
  proc chpl_taskListProcess(task_list: _task_list) {
    chpl_task_processTaskList(task_list);
  }

  //
  // make sure all tasks in a list have an opportunity to run
  //
  pragma "insert line file info"
  export
  proc chpl_taskListExecute(task_list: _task_list) {
    chpl_task_executeTasksInList(task_list);
  }

  //
  // do final cleanup for a task list
  //
  pragma "insert line file info"
  export
  proc chpl_taskListFree(task_list: _task_list) {
    chpl_task_freeTaskList(task_list);
  }
}
