// ChapelLocale.chpl
//
pragma "no use ChapelStandard"
module ChapelLocale {

  //
  // An abstract class. Specifies the required locale interface.
  // Each locale implementation must inherit from this class.
  //
  class locale {
    //- Constructor
    proc locale() { }
  
    //------------------------------------------------------------------------{
    //- Fields and accessors defined for all locale types (not overridable)
    //-

    // Every locale has a parent, except for the root locale.
    // The parent of the root locale is nil (by definition).
    const parent : locale;

    // To be removed from the required interface once legacy code is adjusted.
    const numCores: int;

    proc id : int return chpl_id();  // just the node part
    proc localeid : chpl_localeID_t return chpl_localeid(); // full locale id
    proc name return chpl_name();
    //------------------------------------------------------------------------}

    //------------------------------------------------------------------------{
    //- User Interface Methods (overridable)
    //-

    // These are dynamically dispatched, so they can be overridden in
    // concrete classes.
    proc chpl_id() : int {
      _throwPVFCError();
      return -1;
    }

    proc chpl_localeid() : chpl_localeID_t {
      _throwPVFCError();
      extern const c_sublocid_none: chpl_sublocID_t;
      return chpl_buildLocaleID(-1:chpl_nodeID_t, c_sublocid_none);
    }

    proc chpl_name() : string {
      _throwPVFCError();
      return "";
    }

    // A useful default definition is provided (not pure virtual).
    proc readWriteThis(f) {
      f <~> name;
    }

    proc getChildCount() : int {
      _throwPVFCError();
      return 0;
    }      
  
// Part of the required locale interface.
// Commented out because presently iterators are statically bound.
//    iter getChildIndices() : int {
//      for idx in this.getChildSpace do
//        yield idx;
//    }
  
    proc addChild(loc:locale)
    {
      _throwPVFCError();
    }
  
    proc getChild(idx:int) : locale {
      _throwPVFCError();
      return this;
    }

// Part of the required locale interface.
// Commented out because presently iterators are statically bound.
//    iter getChildren() : locale  {
//    _throwPVFCError();
//      yield 0;
//    }

    //------------------------------------------------------------------------}
  }

  class AbstractLocaleModel : locale {
    // This will be used for interfaces that will be common to all
    // (non-RootLocale) locale models
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
  pragma "private" var rootLocale : locale = nil;
  config param replicateRootLocale = true;

  // The rootLocale needs to be initalized on all locales prior to
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
  var origRootLocale : locale = nil;

  class AbstractRootLocale : locale {
    // These functions are used to establish values for Locales[] and
    // LocaleSpace -- an array of locales and its correponding domain
    // which are used as the default set of targetLocales in many
    // distributions.
    proc getDefaultLocaleSpace() {
      _throwPVFCError();
      const emptyLocaleSpace: domain(1) = {1..0};
      return emptyLocaleSpace;
    }

    proc getDefaultLocaleArray() {
      _throwPVFCError();
      const emptyLocaleSpace: domain(1) = {1..0};
      const emptyLocales: [emptyLocaleSpace] locale;
      return emptyLocales;
    }

    proc localeIDtoLocale(id : chpl_localeID_t) : locale {
      _throwPVFCError();
      return this;
    }

    // These iterators are to be used by RootLocale:init() to
    // initialize the LocaleModel.  The calling loop body cannot
    // contain any non-local code, since the rootLocale is not yet
    // initialized.
    iter initOnLocales() {
      if numLocales > 1 then
        halt("The locales must be initialized in parallel");
      for locIdx in (origRootLocale:RootLocale).getDefaultLocaleSpace() {
        yield locIdx;
        rootLocale = origRootLocale;
      }
    }

    // Since we are going on to all the locales here, we use the
    // opportunity to initialize any global private variables we
    // either need (e.g., defaultDist) or can do at this point in
    // initialization (e.g., rootLocale).
    iter initOnLocales(param tag: iterKind)
      where tag==iterKind.leader {
      // Simple locales barrier, see implementation below for notes
      var b: localesBarrier;
      var flags: [1..#numLocales-1] localesSignal;
      extern const c_sublocid_any: chpl_sublocID_t;
      coforall locIdx in 0..#numLocales /*ref(b)*/ {
        on __primitive("chpl_on_locale_num",
                       chpl_buildLocaleID(locIdx:chpl_nodeID_t,
                                          c_sublocid_any)) {
          chpl_defaultDistInitPrivate();
          yield locIdx;
          b.wait(locIdx, flags);
          chpl_rootLocaleInitPrivate(locIdx);
        }
      }
    }

    iter initOnLocales(param tag: iterKind, followThis)
      where tag==iterKind.follower {
      yield followThis;
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
  class localesSignal {
    var s: atomic bool;
  }
  record localesBarrier {
    proc wait(locIdx, flags) {
      if locIdx==0 {
        // locale 0 has nothing else to do, so check flags
        while (true) {
          var count = 0;
          for f in flags do
            if f then count += 1;
          if count==numLocales-1 then break;
        }
        // Let the others go
        for f in flags do
          f.s.testAndSet();
      } else {
        var f = new localesSignal();
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
  // The init() function must use the initOnLocales() iterator above
  // to iterate in parallel over the locales to set up the LocaleModel
  // object.
  proc chpl_init_rootLocale() {
    origRootLocale = new RootLocale();
    (origRootLocale:RootLocale).init();
  }

  // This function sets up a private copy of rootLocale by replicating
  // origRootLocale and resets the Locales array to point to the local
  // copy on all but locale 0 (which is done in LocalesArray.chpl as
  // part of the declaration).
  proc chpl_rootLocaleInitPrivate(locIdx) {
    // Even when not replicating the rootLocale, we must temporarily
    // set the rootLocale to the original version on locale 0, because
    // the initialization below needs to get/set locale ids.
    rootLocale = origRootLocale;
    if replicateRootLocale && locIdx!=0 {
      // Create a new local rootLocale
      var newRootLocale = new RootLocale();
      // We don't want to be doing unnecessary ref count updates here
      // as they require additional tasks.  We know we don't need them
      // so tell the compiler to not insert them.
      pragma "no copy" pragma "no auto destroy"
      const origLocales => (origRootLocale:RootLocale).getDefaultLocaleArray();
      var origRL = origLocales._value.theData;
      var newRL = newRootLocale.getDefaultLocaleArray()._value.theData;
      // We must directly implement a bulk copy here, as the mechanisms
      // for doing so via a whole array assignment are not initialized
      // yet and copying element-by-element via a for loop is is costly.
      __primitive("chpl_comm_get",
                  __primitive("array_get", newRL, 0),
                  0 /* locale 0 */,
                  __primitive("array_get", origRL, 0), numLocales);
      // Set the rootLocale to the local copy
      rootLocale = newRootLocale;
    }
    if locIdx!=0 {
      // We mimic a private Locales array alias by using the move
      // primitive.
      __primitive("move", Locales,
                  (rootLocale:RootLocale).getDefaultLocaleArray());
    }
  }

  // We need a temporary value for "here" before the architecture is defined.
  // This is due to the fact that "here" is used for memory and task control
  // in setting up the architecture itself.
  // Its type should probably be renamed dummyLocale or something
  // representative.
  // The dummy locale provides system-default tasking and memory management.
  const dummyLocale = new locale();

  extern proc chpl_task_getRequestedSubloc(): chpl_sublocID_t;
  extern var chpl_nodeID: int(32);
  // Return the locale ID of the current locale
  inline proc here_id {
    return chpl_buildLocaleID(chpl_nodeID,chpl_task_getRequestedSubloc());
  }
  // Return the current locale
  inline proc here {
    return chpl_localeID_to_locale(here_id);
  }
  
  // Returns a wide pointer to the locale with the given id.
  proc chpl_localeID_to_locale(id : chpl_localeID_t) : locale {
    if rootLocale then
      return (rootLocale:AbstractRootLocale).localeIDtoLocale(id);
    else
      // For code prior to rootLocale initialization
      return dummyLocale;
  }

  pragma "insert line file info"
  extern proc chpl_memhook_malloc_pre(number:int, size:int, md:int(16)): void;
  pragma "insert line file info"
  extern proc chpl_memhook_malloc_post(ptr:opaque, number:int,
                                       size:int, md:int(16)): void;
  pragma "insert line file info"
  extern proc chpl_memhook_realloc_pre(ptr:object, size:int, md:int(16)): void;
  pragma "insert line file info"
  extern proc chpl_memhook_realloc_post(newPtr:opaque, ptr:object,
                                        size:int, md:int(16)): void;
  pragma "insert line file info"
  extern proc chpl_memhook_free_pre(ptr:opaque): void;
  extern proc chpl_memhook_md_num(): int(16);

  proc chpl_getPrivatizedCopy(type objectType, objectPid:int): objectType
    return __primitive("chpl_getPrivatizedClass", nil:objectType, objectPid);
  

//########################################################################{
//# Locale diagnostics
//#

  proc locale.totalThreads() {
    var totalThreads: int;
    extern proc chpl_task_getNumThreads() : int;
    on this do totalThreads = chpl_task_getNumThreads();
    return totalThreads;
  }
  
  proc locale.idleThreads() {
    var idleThreads: int;
    extern proc chpl_task_getNumIdleThreads() : int;
    on this do idleThreads = chpl_task_getNumIdleThreads();
    return idleThreads;
  }
  
  proc locale.queuedTasks() {
    var queuedTasks: int;
    extern proc chpl_task_getNumQueuedTasks() : int;
    on this do queuedTasks = chpl_task_getNumQueuedTasks();
    return queuedTasks;
  }
  
  proc locale.runningTasks() {
    var numTasks: int;
    extern proc chpl_task_getNumRunningTasks() : int;
    on this do numTasks = chpl_task_getNumRunningTasks();
    return numTasks;
  }
  
  proc locale.blockedTasks() {
    var blockedTasks: int;
    extern proc chpl_task_getNumBlockedTasks() : int;
    on this do blockedTasks = chpl_task_getNumBlockedTasks();
    return blockedTasks;
  }

//########################################################################}

}
