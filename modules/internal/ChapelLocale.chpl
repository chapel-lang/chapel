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

/*
  A *locale* is a Chapel abstraction for a piece of a target
  architecture that has processing and storage capabilities.
  Generally speaking, the tasks running within a locale have
  roughly uniform access to values stored in the locale's local
  memory and longer latencies for accessing the memories of other
  locales.  As examples, a single shared memory machine would be
  defined as a single locale, while in a system consisting of a
  group of network-connected multicore nodes or SMPs each node
  would be defined as a locale.

  Chapel provides several predefined methods on locales, as well as
  a few variables that describe the locales upon which a program is
  running.

  In addition to what is documented below, ``numLocales``, ``LocaleSpace``,
  and ``Locales`` are available as global variables.

  ``numLocales`` is the number of top-level (network connected) locales.

  .. code-block:: chapel

    config const numLocales: int;

  ``LocaleSpace`` is the domain over which the global ``Locales`` array is
  defined.

  .. code-block:: chapel

    const LocaleSpace = {0..numLocales-1};

  The global ``Locales`` array contains an entry for each top-level locale.

  .. code-block:: chapel

    const Locales: [LocaleSpace] locale;


  One common code idiom in Chapel is the following, which spreads parallel
  tasks across the network-connected locales upon which the program is running:

    .. code-block:: chapel

      coforall loc in Locales { on loc { ... } }


 */
module ChapelLocale {

  use LocaleModel;
  private use HaltWrappers only;
  private use SysCTypes;

  //
  // Node and sublocale types and special sublocale values.
  //
  pragma "no doc"
  type chpl_nodeID_t = int(32);
  pragma "no doc"
  type chpl_sublocID_t = int(32);

  pragma "no doc"
  extern const c_sublocid_none: chpl_sublocID_t;
  pragma "no doc"
  extern const c_sublocid_any: chpl_sublocID_t;
  pragma "no doc"
  extern const c_sublocid_all: chpl_sublocID_t;

  pragma "no doc"
  inline proc chpl_isActualSublocID(subloc: chpl_sublocID_t)
    return (subloc != c_sublocid_none
            && subloc != c_sublocid_any
            && subloc != c_sublocid_all);

  /*
    ``locale`` is the abstract class from which the various
    implementations inherit.  It specifies the required interface
    and implements part of it, but requires the rest to be provided
    by the corresponding concrete classes.
   */
  class _locale {
    //- Constructor
    pragma "no doc"
    proc init() { }

    pragma "no doc"
    proc init(parent: locale?) {
      this.parent = parent;
    }

    //------------------------------------------------------------------------{
    //- Fields and accessors defined for all locale types (not overridable)
    //-

    // Every locale has a parent, except for the root locale.
    // The parent of the root locale is nil (by definition).
    pragma "no doc"
    const parent : locale?;

    pragma "no doc" var nPUsLogAcc: int;     // HW threads, accessible
    pragma "no doc" var nPUsLogAll: int;     // HW threads, all
    pragma "no doc" var nPUsPhysAcc: int;    // HW cores, accessible
    pragma "no doc" var nPUsPhysAll: int;    // HW cores, all

    /*
      A *processing unit* or *PU* is an instance of the processor
      architecture, basically the thing that executes instructions.
      :proc:`numPUs` tells how many of these are present on this
      locale.  It can count either physical PUs (commonly known as
      *cores*) or hardware threads such as hyperthreads and the like.
      It can also either take into account any OS limits on which PUs
      the program has access to or do its best to ignore such limits.
      By default it returns the number of accessible physical cores.

      :arg logical: Count logical PUs (hyperthreads and the like),
                    or physical ones (cores)?  Defaults to `false`,
                    for cores.
      :type logical: `bool`
      :arg accessible: Count only PUs that can be reached, or all of
                       them?  Defaults to `true`, for accessible PUs.
      :type accessible: `bool`
      :returns: number of PUs
      :rtype: `int`

      There are several things that can cause the OS to limit the
      processor resources available to a Chapel program.  On plain
      Linux systems using the ``taskset(1)`` command will do it.  On
      Cray systems the ``CHPL_LAUNCHER_CORES_PER_LOCALE`` environment
      variable may do it, indirectly via the system job launcher.
      Also on Cray systems, using a system job launcher (``aprun`` or
      ``slurm``) to run a Chapel program manually may do it, as can
      running programs within Cray batch jobs that have been set up
      with limited processor resources.
     */
    inline
    proc numPUs(logical: bool = false, accessible: bool = true)
      return if logical
             then if accessible then nPUsLogAcc else nPUsLogAll
             else if accessible then nPUsPhysAcc else nPUsPhysAll;

    /*
      This is the maximum task concurrency that one can expect to
      achieve on this locale.  The value is an estimate by the
      runtime tasking layer.  Typically it is the number of physical
      processor cores available to the program.  Creating more tasks
      than this will probably increase walltime rather than decrease
      it.
     */
    var maxTaskPar: int;

    /*
      ``callStackSize`` holds the size of a task stack on a given
      locale.  Thus, ``here.callStackSize`` is the size of the call
      stack for any task on the current locale, including the
      caller.
    */
    var callStackSize: size_t;

    /*
      Get the integer identifier for this locale.

      :returns: locale number, in the range ``0..numLocales-1``
      :rtype: int
     */
    proc id : int return chpl_nodeFromLocaleID(__primitive("_wide_get_locale", this));

    pragma "no doc"
    proc localeid : chpl_localeID_t return __primitive("_wide_get_locale", this);

    /*
      Get the hostname of this locale.

      :returns: the hostname of the compute node associated with the locale
      :rtype: string
    */
    proc hostname: string {
      extern proc chpl_nodeName(): c_string;
      var hname: string;
      on this {
        try! {
          hname = createStringWithNewBuffer(chpl_nodeName());
        }
      }
      return hname;
    }

    /*
      Get the name of this locale.  In practice, this is often the
      same as the hostname, though in some cases (like when using
      local launchers), it may be modified.

      :returns: locale name
      :rtype: string
     */
    proc name return chpl_name() : string;

    // This many tasks are running on this locale.
    //
    // Note handling runningTaskCounter <= 0 in runningTaskCnt().  The
    // references to it are done via here.runningTasks(), and currently
    // "here" means the requested locale, not the actual or concrete
    // one.  But this may be "any", and the running task count in it may
    // be zero, which in turn leads to overindexing the locales array
    // after the subtraction of 1 in locale.runningTasks(), below.  In
    // the long run we probably need to go through all the "here" refs
    // in the module code and differentiate more carefully which need
    // the requested locale and which need the concrete one.  But for
    // now, we make the assumption that all requests for the number of
    // running tasks want the count from the locale the calling task is
    // running on, so the minimum possible value must be 1.
    //
    // This field should only be accessed locally, so we will have better
    // performance if we always use a processor atomic.
    pragma "no doc"
    var runningTaskCounter : chpl__processorAtomicType(int);

    pragma "no doc"
    inline proc runningTaskCntSet(val : int) {
      runningTaskCounter.write(val, memoryOrder.relaxed);
    }

    pragma "no doc"
    inline proc runningTaskCntAdd(val : int) {
      runningTaskCounter.add(val, memoryOrder.relaxed);
    }

    pragma "no doc"
    inline proc runningTaskCntSub(val : int) {
      runningTaskCounter.sub(val, memoryOrder.relaxed);
    }

    pragma "no doc"
    inline proc runningTaskCnt() {
      var rtc = runningTaskCounter.read(memoryOrder.relaxed);
      return if (rtc <= 0) then 1 else rtc;
    }
    //------------------------------------------------------------------------}

    //------------------------------------------------------------------------{
    //- User Interface Methods (overridable)
    //-

    // These are dynamically dispatched, so they can be overridden in
    // concrete classes.
    pragma "no doc"
    proc chpl_id() : int {
      HaltWrappers.pureVirtualMethodHalt();
      return -1;
    }

    pragma "no doc"
    proc chpl_localeid() : chpl_localeID_t {
      HaltWrappers.pureVirtualMethodHalt();
      return chpl_buildLocaleID(-1:chpl_nodeID_t, c_sublocid_none);
    }

    pragma "no doc"
    proc chpl_name() : string {
      HaltWrappers.pureVirtualMethodHalt();
      return "";
    }

    //
    // Support for different types of memory:
    // large, low latency, and high bandwidth
    //
    pragma "no doc"
    proc defaultMemory() : locale {
      HaltWrappers.pureVirtualMethodHalt();
      return this;
    }

    pragma "no doc"
    proc largeMemory() : locale {
      HaltWrappers.pureVirtualMethodHalt();
      return this;
    }

    pragma "no doc"
    proc lowLatencyMemory() : locale {
      HaltWrappers.pureVirtualMethodHalt();
      return this;
    }

    pragma "no doc"
    proc highBandwidthMemory() : locale {
      HaltWrappers.pureVirtualMethodHalt();
      return this;
    }

    pragma "no doc"
    proc getChildCount() : int {
      HaltWrappers.pureVirtualMethodHalt();
      return 0;
    }
  
// Part of the required locale interface.
// Commented out because presently iterators are statically bound.
//    iter getChildIndices() : int {
//      for idx in this.getChildSpace do
//        yield idx;
//    }
  
    pragma "no doc"
    proc addChild(loc:locale)
    {
      HaltWrappers.pureVirtualMethodHalt();
    }
  
    pragma "no doc"
    proc getChild(idx:int) : locale {
      HaltWrappers.pureVirtualMethodHalt();
      return this;
    }

// Part of the required locale interface.
// Commented out because presently iterators are statically bound.
//    iter getChildren() : locale  {
//    HaltWrappers.pureVirtualMethodHalt();
//      yield 0;
//    }

    //------------------------------------------------------------------------}
  }

  /* This class is used during initialization and is returned when
     'here' is used before the locale hierarchy is initialized.
   */
  pragma "no doc"
  class DummyLocale : locale {
    proc init() { }

    override proc chpl_id() : int {
      return -1;
    }
    override proc chpl_localeid() : chpl_localeID_t {
      return chpl_buildLocaleID(-1:chpl_nodeID_t, c_sublocid_none);
    }
    override proc chpl_name() : string {
      return "dummy-locale";
    }
    override proc getChildCount() : int {
      return 0;
    }
    override proc getChild(idx:int) : locale {
      return this;
    }
    override proc addChild(loc:locale)
    {
      halt("addChild on DummyLocale");
    }
  }


  pragma "no doc"
  class AbstractLocaleModel : locale {
    // This will be used for interfaces that will be common to all
    // (non-RootLocale) locale models
    proc init(parent_loc : locale) {
      super.init(parent_loc);
    }

    proc init() {  }
  }

  // rootLocale is declared to be of type locale rather than
  // RootLocale because doing so would make locales depend on arrays
  // which depend on locales, etc.
  //
  // If we had a way to express domain and array return types abstractly,
  // that problem would go away. <hilde>
  //
  // The rootLocale is private to each locale.  It cannot be
  // initialized until LocaleModel is initialized.  To disable this
  // replication, set replicateRootLocale to false.
  pragma "no doc"
  pragma "locale private" var rootLocale : unmanaged locale? = nil;
  pragma "no doc"
  pragma "locale private" var rootLocaleInitialized = false;

  pragma "no doc"
  config param replicateRootLocale = true;

  // The rootLocale needs to be initialized on all locales prior to
  // initializing the Locales array.  Unfortunately, the rootLocale
  // cannot be properly replicated until DefaultRectangular can be
  // initialized (as the private copies of the defaultDist are
  // needed).  We remedy this by initializing a rootLocale on locale 0
  // (called origRootLocale), and setting all locales' rootLocale to
  // origRootLocale.  Later, after DefaultRectangular can be
  // initialized, we create local copies of the rootLocale (and the
  // Locales array).  This is being done in the InitPrivateGlobals
  // module.
  //
  pragma "no doc"
  var origRootLocale : unmanaged locale? = nil;

  pragma "no doc"
  class AbstractRootLocale : locale {
    proc init() { }

    proc init(parent_loc : locale?) {
      super.init(parent_loc);
    }

    // These functions are used to establish values for Locales[] and
    // LocaleSpace -- an array of locales and its corresponding domain
    // which are used as the default set of targetLocales in many
    // distributions.
    proc getDefaultLocaleSpace() const ref {
      HaltWrappers.pureVirtualMethodHalt();
      return chpl_emptyLocaleSpace;
    }

    proc getDefaultLocaleArray() const ref {
      HaltWrappers.pureVirtualMethodHalt();
      return chpl_emptyLocales;
    }

    proc localeIDtoLocale(id : chpl_localeID_t) : locale {
      HaltWrappers.pureVirtualMethodHalt();
      return this;
    }

    // These iterators are to be used by RootLocale:setup() to
    // initialize the LocaleModel.  The calling loop body cannot
    // contain any non-local code, since the rootLocale is not yet
    // initialized.
    iter chpl_initOnLocales() {
      if numLocales > 1 then
        halt("The locales must be initialized in parallel");
      for locIdx in (origRootLocale:borrowed RootLocale?)!.getDefaultLocaleSpace() {
        yield locIdx;
        rootLocale = origRootLocale;
        rootLocaleInitialized = true;
      }
    }

    // Since we are going on to all the locales here, we use the
    // opportunity to initialize any global private variables we
    // either need (e.g., defaultDist) or can do at this point in
    // initialization (e.g., rootLocale).
    iter chpl_initOnLocales(param tag: iterKind)
      where tag==iterKind.standalone {
      // Simple locales barrier, see implementation below for notes
      var b: localesBarrier;
      var flags: [1..#numLocales-1] unmanaged localesSignal;
      coforall locIdx in 0..#numLocales /*ref(b)*/ {
        on __primitive("chpl_on_locale_num",
                       chpl_buildLocaleID(locIdx:chpl_nodeID_t,
                                          c_sublocid_any)) {
          chpl_defaultDistInitPrivate();
          yield locIdx;
          b.wait(locIdx, flags);
          chpl_rootLocaleInitPrivate(locIdx);
          warmupRuntime();
        }
      }
    }
  }

  // Warm up runtime components. For tasking layers that have a fixed number of
  // threads, we create a task on each thread to warm it up (e.g. grab some
  // initial call stacks.) We also warm up the memory layer, since allocators
  // tend to have per thread pools/arenas.
  private proc warmupRuntime() {
    extern proc chpl_task_getFixedNumThreads(): uint(32);
    coforall i in 0..#chpl_task_getFixedNumThreads() {
      var p = c_malloc(int, 1);
      p[0] = i;
      c_free(p);
    }
  }

  //
  // Simple locales barrier
  //
  // This sits outside of the abstract root locale definition above
  // because the compiler cannot resolve the constructor (known issues
  // with nested classes/records).  In addition, we cannot have the
  // flags array in the record, because the initCopy function needs
  // the dataPar* configs declared in ChapelDistribution.
  //
  // Each non-0 locale increments the count, and then waits on a
  // *local* atomic.  This is done by creating an array of type
  // class localesSignal, one per locale, and allocating each locale's
  // copy before updating the count.  Locale 0 waits for the others to
  // arrive and then set the signals to true.  We can't do anything
  // too complicated this early on, so we are using a for loop to
  // broadcast that we are done.
  pragma "no doc"
  class localesSignal {
    var s: atomic bool;
  }
  pragma "no doc"
  record localesBarrier {
    proc wait(locIdx, flags) {
      if locIdx==0 {
        // locale 0 has nothing else to do, so check flags
        while (true) {
          // This fence ensures that writes to the count variables
          // are available to this task. (Note that they aren't
          // atomic if they're 128-bit writes though - so we
          // have some risk of getting part of a wide pointer).
          // Without this fence, there is a race condition on
          // a weakly-ordered architecture.
          atomicFence();
          var count = 0;
          for f in flags do
            if f then count += 1;
          if count==numLocales-1 then break;
          // Give time to other tasks/threads/processes
          // like we do in waitFor
          chpl_task_yield();
        }
        // Let the others go
        for f in flags do
          f!.s.testAndSet();
      } else {
        var f = new unmanaged localesSignal();
        // expose my flag to locale 0
        flags[locIdx] = f;
        // wait (locally) for locale 0 to set my flag
        f.s.waitFor(true);
        // clean up
        delete f;
      }
    }
  }


  // This function is called in the LocaleArray module to initialize
  // the rootLocale.  It sets up the origRootLocale and also includes
  // set up of the each locale's LocaleModel via RootLocale:init().
  //
  // The init() function must use the chpl_initOnLocales() iterator above
  // to iterate in parallel over the locales to set up the LocaleModel
  // object.
  pragma "no doc"
  proc chpl_init_rootLocale() {
    if numLocales > 1 && _local then
      halt("Cannot run a program compiled with --local in more than 1 locale");

    origRootLocale = new unmanaged RootLocale();
    (origRootLocale:borrowed RootLocale?)!.setup();
  }

  // This function sets up a private copy of rootLocale by replicating
  // origRootLocale and resets the Locales array to point to the local
  // copy on all but locale 0 (which is done in LocalesArray.chpl as
  // part of the declaration).
  pragma "no doc"
  proc chpl_rootLocaleInitPrivate(locIdx) {
    // Even when not replicating the rootLocale, we must temporarily
    // set the rootLocale to the original version on locale 0, because
    // the initialization below needs to get/set locale ids.
    rootLocale = origRootLocale;
    if replicateRootLocale && locIdx!=0 {
      // Create a new local rootLocale
      var newRootLocale = new unmanaged RootLocale();
      // We don't want to be doing unnecessary ref count updates here
      // as they require additional tasks.  We know we don't need them
      // so tell the compiler to not insert them.
      pragma "no copy" pragma "no auto destroy"
      const ref origLocales = (origRootLocale:borrowed RootLocale?)!.getDefaultLocaleArray();
      var origRL = origLocales._value.theData;
      var newRL = newRootLocale.getDefaultLocaleArray()._value.theData;
      // We must directly implement a bulk copy here, as the mechanisms
      // for doing so via a whole array assignment are not initialized
      // yet and copying element-by-element via a for loop is costly.
      __primitive("chpl_comm_array_get",
                  __primitive("array_get", newRL, 0),
                  0 /* locale 0 */,
                  __primitive("array_get", origRL, 0), 
                  numLocales:size_t);
      // Set the rootLocale to the local copy
      rootLocale = newRootLocale;
    }
    if locIdx!=0 {
      // We mimic a private Locales array alias by using the move
      // primitive.
      pragma "no auto destroy"
      const ref tmp = (rootLocale:borrowed RootLocale?)!.getDefaultLocaleArray();
      __primitive("move", Locales, tmp);
    }
    rootLocaleInitialized = true;
  }

  // We need a temporary value for "here" before the architecture is defined.
  // This is due to the fact that "here" is used for memory and task control
  // in setting up the architecture itself.
  // Its type should probably be renamed dummyLocale or something
  // representative.
  // The dummy locale provides system-default tasking and memory management.
  pragma "no doc"
  const dummyLocale = new unmanaged DummyLocale();

  pragma "fn synchronization free"
  pragma "no doc"
  extern proc chpl_task_getRequestedSubloc(): chpl_sublocID_t;

  pragma "no doc"
  pragma "insert line file info"
  export
  proc chpl_getLocaleID(ref localeID: chpl_localeID_t) {
    localeID = here_id;
  }

  // Return the locale ID of the current locale
  pragma "no doc"
  inline proc here_id {
    if localeModelHasSublocales then
      return chpl_rt_buildLocaleID(chpl_nodeID, chpl_task_getRequestedSubloc());
    else
      return chpl_rt_buildLocaleID(chpl_nodeID, c_sublocid_any);
  }

  /*
    This returns the locale from which the call is made.

    :return: current locale
    :rtype: locale
  */
  inline proc here {
    return chpl_localeID_to_locale(here_id);
  }
  
  // Returns a wide pointer to the locale with the given id.
  pragma "no doc"
  pragma "fn returns infinite lifetime"
  proc chpl_localeID_to_locale(id : chpl_localeID_t) : locale {
    if rootLocale then
      return (rootLocale:borrowed AbstractRootLocale?)!.localeIDtoLocale(id);
    else
      // For code prior to rootLocale initialization
      return dummyLocale;
  }

  // the type of elements in chpl_privateObjects.
  extern record chpl_privateObject_t {
    var obj:c_void_ptr;
  }
  extern var chpl_privateObjects:c_ptr(chpl_privateObject_t);

  pragma "no doc"
  pragma "fn returns infinite lifetime"
  // should this use pragma "local args"?
  // Why is the compiler making the objectType argument wide?
  inline
  proc chpl_getPrivatizedCopy(type objectType, objectPid:int): objectType {
    return __primitive("cast", objectType, chpl_privateObjects[objectPid].obj);
  }

//########################################################################{
//# Locale diagnostics
//#

  pragma "no doc"
  proc locale.totalThreads() {
    var totalThreads: int;
    extern proc chpl_task_getNumThreads() : uint(32);
    on this do totalThreads = chpl_task_getNumThreads();
    return totalThreads;
  }
  
  pragma "no doc"
  proc locale.idleThreads() {
    var idleThreads: int;
    extern proc chpl_task_getNumIdleThreads() : uint(32);
    on this do idleThreads = chpl_task_getNumIdleThreads();
    return idleThreads;
  }
  
  pragma "no doc"
  proc locale.queuedTasks() {
    var queuedTasks: int;
    extern proc chpl_task_getNumQueuedTasks() : uint(32);
    on this do queuedTasks = chpl_task_getNumQueuedTasks();
    return queuedTasks;
  }

  pragma "fn synchronization free"
  pragma "no doc"
  proc locale.runningTasks() {
    return this.runningTaskCnt();
  }
  
  pragma "no doc"
  proc locale.blockedTasks() {
    var blockedTasks: int;
    extern proc chpl_task_getNumBlockedTasks() : int(32);
    on this do blockedTasks = chpl_task_getNumBlockedTasks();
    return blockedTasks;
  }

//########################################################################}

  //
  // Increment and decrement the task count.
  //
  // Elsewhere in the module code we adjust the running task count
  // directly, but at least for now the runtime also needs to be
  // able to do so.  These functions support that.
  //
  pragma "no doc"
  pragma "insert line file info"
  pragma "inc running task"
  export
  proc chpl_taskRunningCntInc() {
    if rootLocaleInitialized {
      here.runningTaskCntAdd(1);
    }
  }

  pragma "no doc"
  pragma "insert line file info"
  pragma "dec running task"
  export
  proc chpl_taskRunningCntDec() {
    if rootLocaleInitialized {
      here.runningTaskCntSub(1);
    }
  }

  pragma "no doc"
  pragma "insert line file info"
  export
  proc chpl_taskRunningCntReset() {
    here.runningTaskCntSet(0);
  }

  //
  // Free the original root locale when the program is being torn down
  //
  // Be careful to free only origRootLocale, and never the copy in
  // rootLocale, or the same locales will be torn down twice.
  //
  pragma "no doc"
  proc deinit() {
    delete origRootLocale;
  }
}
