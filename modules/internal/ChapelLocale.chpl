// ChapelLocale.chpl
//
pragma "no use ChapelStandard"
module ChapelLocale {
// TODO: Check if these are these needed by the LLVM codegen routines
// and uncomment them if so.
//  extern proc chpl_localeID_get_node(loc:chpl_localeID_t):int(64);
//  extern proc chpl_localeID_get_subloc(loc:chpl_localeID_t):int(64);
//  extern proc chpl_localeID_equals(loc:chpl_localeID_t):int(64);

//  proc ==(a:chpl_localeID_t, b:chpl_localeID_t)
//    return chpl_localeID_equals(a,b) != 0;

//  proc !=(a:chpl_localeID_t, b:chpl_localeID_t)
//    return chpl_localeID_equals(a,b) == 0;


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

  class RootLocale : locale {
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
  }

  var rootLocale : locale = nil;

  // Returns a wide pointer to the locale with the given id.
  // When hierarchical locales are fully implemented, the lookup may be
  // done mostly in the runtime (through the sublocale registry).
  proc chpl_localeID_to_locale(id : chpl_localeID_t) : locale {
    // The _is_here test examines only localeIDs, so is local and very fast.
    // Evaluating "here" is also local and very fast.
    if __primitive("_is_here", id) then return here;
    var ret:locale;
    on rootLocale do ret = (rootLocale:RootLocale).localeIDtoLocale(id);
    return ret;
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
