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
    proc locale() {
// Now we find we have to create locales early, so _here can be non-nil.
      // compilerError("cannot create instances of the 'locale' class");
    }
  
    //------------------------------------------------------------------------{
    //- Fields and accessors defined for all locale types (not overridable)
    //-

    // Every locale has a parent, except for the root locale.
    // The parent of the root locale is nil (by definition).
    const parent : locale;

    // To be removed from the required interface once legacy code is adjusted.
    const numCores: int;

    // In legacy code, the id accessor is used to obtain the node id, so it
    // should probably be renamed as node_id.
    // As an accessor, it is statically bound, which is also a problem....
    proc id : int return chpl_id();
    proc name return chpl_name();
    //------------------------------------------------------------------------}

    //------------------------------------------------------------------------{
    //- User Interface Methods (overridable)
    //-

    // Returns a globally unique locale identifier.
    // This routine is dynamically dispatched, so it can be overridden in concrete classes.
    proc chpl_id() : int {
      _throwPVFCError();
      return -1;
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


    //------------------------------------------------------------------------{
    //- Compiler Interface Methods (overridable)
    //-

    // Part of the required locale interface.
    // These routines are called by the compiler to implement locale-aware task control.
    // They should be overridden in concrete locale classes as necessary.
    proc taskInit() : void {}
    proc taskExit() : void {}

    proc alloc(nbytes:int) {
      extern proc chpl_malloc(nbytes:int) : opaque;
      return chpl_malloc(nbytes);
    }

    proc calloc(count:int, nbytes:int) {
      extern proc chpl_calloc(count:int, nbytes:int) : opaque;
      return chpl_calloc(count, nbytes);
    }

    proc realloc(x:object, nbytes:int) {
      extern proc chpl_realloc(x:object, nbytes:int) : opaque;
      return chpl_realloc(x, nbytes);
    }

    proc free(x:object) {
      extern proc chpl_free(x:object) : void;
      chpl_free(x);
    }
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

  config const minRootLocaleInitPerTask = 1;
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

    iter initOnLocales(param tag: iterKind)
      where tag==iterKind.leader {
      // Simple blocking of locales.  Consider tree-based start-up
      // We assume locales are number 0..numLocales-1
      extern proc chpl_numCoresOnThisLocale(): int;
      var numChunks = _computeNumChunks(chpl_numCoresOnThisLocale(),
                                        true /* ignoreRunning*/,
                                        minRootLocaleInitPerTask,
                                        numLocales);
      coforall chunk in 0..#numChunks {
        const (lo, hi) = _computeBlock(numLocales, numChunks,
                                       chunk, numLocales-1);
        for locIdx in lo..hi {
          extern var c_sublocid_any:chpl_sublocID_t;
          // sublocale part of locID needs to be any
          var locID = chpl_buildLocaleID(locIdx:chpl_nodeID_t,
                                         c_sublocid_any:chpl_sublocID_t);
          on __primitive("chpl_on_locale_num", locID) {
            yield locIdx;
            rootLocale = origRootLocale;
          }
        }
      }
    }

    iter initOnLocales(param tag: iterKind, followThis)
      where tag==iterKind.follower {
      yield followThis;
    }
  }


  // This function is called in the LocalesArray module to initialize
  // the rootLocale.  It sets up the origRootLocale and also includes
  // set up of the each locale's LocaleModel via DefaulRootLocale:init().
  //
  // The init() function must use chpl_rootLocale_locales() to iterate
  // (in parallel) over the locales to set up the LocaleModel object.
  // In addition, the initial 'here' must be set.
  // 
  proc chpl_init_rootLocale() {
    origRootLocale = new RootLocale();

    (origRootLocale:RootLocale).init();

    // Programs traditionally expect the startup process to run on
    // Locales[0], so this is how we mimic that behavior.
    // Note that this means we have to ask for here.parent or rootLocale
    // to get the root locale of the default architecture.
    var loc = (origRootLocale:RootLocale).getDefaultLocaleArray()[0];
    __primitive("_task_set_here_ptr", loc);
    var locID = chpl_buildLocaleID(0:chpl_nodeID_t, 0:chpl_sublocID_t);
    __primitive("_task_set_locale_id", locID);
  }

  // This function sets up a private copy of rootLocale by replicating
  // origRootLocale and resets the Locales array to point to the local
  // copy.  It should be called on every locale except locale 0 after
  // it is safe to do so (after defaultDist is initialized).
  proc chpl_rootLocaleInitPrivate() {
    if !replicateRootLocale then return;
    // set rootLocale to a local copy
    var newRootLocale = new RootLocale();
    newRootLocale.getDefaultLocaleArray() =
      (origRootLocale:RootLocale).getDefaultLocaleArray();
    rootLocale = newRootLocale;
    // We mimic a private Locales array alias by using the move
    // primitive.  Note that this leaks the original local Locales
    // array wrapper.
    __primitive("move", Locales, (rootLocale:RootLocale).getDefaultLocaleArray());
  }

  // Returns a wide pointer to the locale with the given id.
  proc chpl_localeID_to_locale(id : chpl_localeID_t) : locale {
    if rootLocale then
      return (rootLocale:AbstractRootLocale).localeIDtoLocale(id);
    else
      // For code prior to rootLocale initialization
      return here;
  }



  // This returns the current "here" pointer.
  // It uses a primitive to suck the here pointer out of task-private storage.
  // On local builds, the primitive returns a narrow pointer;
  // otherwise, a wide pointer.
  //
  // Perhaps we can move this into the compiler as a special keyword....
  inline proc here return __primitive("_task_get_here_ptr");
  
  // We need a temporary value for "here" before the architecture is defined.
  // This is due to the fact that "here" is used for memory and task control
  // in setting up the architecture itself.
  // Its type should probably be renamed dummyLocale or something representative.
  // The dummy locale provides system-default tasking and memory management.
  __primitive("_task_set_here_ptr", new locale());

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

  // Here be dragons: If the return type is specified, then normalize.cpp inserts
  // an initializer for the return value which calls its constructor, which calls
  // chpl_here_alloc ad infinitum.  But if the return type is left off, it works!

  // The allocator pragma is used by scalar replacement.
  pragma "allocator"
  pragma "no sync demotion"
  proc chpl_here_alloc(x, md:int(16)) {
    var nbytes = __primitive("sizeof", x);
    chpl_memhook_malloc_pre(1, nbytes, md + chpl_memhook_md_num());
    var mem = __primitive("task_alloc", nbytes);
    chpl_memhook_malloc_post(mem, 1, nbytes, md + chpl_memhook_md_num());
    return __primitive("cast", x.type, mem);
  }

  pragma "allocator"
  pragma "no sync demotion"
  proc chpl_here_calloc(x, number:int, md:int(16)) {
    extern proc chpl_task_calloc(number:int, nbytes:int) : opaque;
    var nbytes = __primitive("sizeof", x);
    chpl_memhook_malloc_pre(number, nbytes, md + chpl_memhook_md_num());
    var mem = chpl_task_calloc(number, nbytes);
    chpl_memhook_malloc_post(mem, number, nbytes, md + chpl_memhook_md_num());
    return __primitive("cast", x.type, mem);
  }

  pragma "allocator"
  pragma "no sync demotion"
  proc chpl_here_realloc(x, md:int(16)) {
    var nbytes = __primitive("sizeof", x);
    chpl_memhook_realloc_pre(x, nbytes, md + chpl_memhook_md_num());
    var mem = __primitive("task_realloc", x:object, nbytes);
    chpl_memhook_realloc_post(mem, x, nbytes, md + chpl_memhook_md_num());
    return __primitive("cast", x.type, mem);
  }

  pragma "no sync demotion"
  proc chpl_here_free(x) {
    // TODO: The pointer should really be of type opaque, but we don't 
    // handle object ==> opaque casts correctly.  (In codegen, opaque behaves 
    // like an lvalue, but in the type system it isn't one.)
    __primitive("local_check", x);
    chpl_memhook_free_pre(__primitive("cast_to_void_star", x));
    __primitive("task_free", x);
  }

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
