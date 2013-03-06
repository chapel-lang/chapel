// ChapelLocale.chpl
//
pragma "no use ChapelStandard"
module ChapelLocale {

  // The chpl_localeID_t type is used internally.  It should not be exposed to the user.
  // It allows the compiler to parse out the fields of the localeID portion
  // of a wide pointer.
  // It simplified the implementation of the various PRIM_WIDE_GET_??? primitives,
  // by allowing the code generation routines for field extraction to be reused.
  extern type c_nodeid_t;
  extern type c_subloc_t;
  // Eventually, it would be desirable to have the architecture specify the allocation
  // of bits in a localeID.  Right now, we borrow the types used for those quantities in
  // the backend, and in that way guarantee that the types agree.

  // Right now, much of the unification of the various runtime facilities is done through the 
  // chpl_ interfaces described in the generic runtime/include files.  If the unification
  // is instead provided in Chapel architectural descriptions, then a greater variety of 
  // runtime facilities can be accommodated.  For example, the single type c_nodeid_t 


  pragma "no object"
  record chpl_localeID_t {
    var node : c_nodeid_t;
    var subloc : c_subloc_t;
  };

  proc ==(a:chpl_localeID_t, b:chpl_localeID_t)
    return __primitive("==", a.node, b.node) &&
           __primitive("==", a.subloc, b.subloc);

  proc !=(a:chpl_localeID_t, b:chpl_localeID_t)
    return ! (a == b);


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
// Needs to be enabled for now, because tasking code calls getChild directly.
// Probably can be reinstated after we call through rootLocale.localeIDtoLocale().
//      _throwPVFCError();
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
    //------------------------------------------------------------------------}
  }

  // TODO: This wants to live in RootLocale.chpl
  // It is here because it needs to be defined before the array return type
  // is used in the definition of class RootLocale.
  var rootLocale : locale = nil;

  // Unfortunately, we need a value for _here before the architecture is defined.
  // This is due to the fact that _here is used for memory and task control already
  // in setting up the default architecture itself.
  // This should probably be renamed _dummyLocale or something representative.
  pragma "private" var _here: locale = new locale();
  __primitive("_task_set_here", _here);

  // This is obsolete.  Here now means the locale on which you are executing,
  // sublocale and all.
  proc here return _here;
  
  proc chpl_getPrivatizedCopy(type objectType, objectPid:int): objectType
    return __primitive("chpl_getPrivatizedClass", nil:objectType, objectPid);
  

//################################################################################{
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

//################################################################################}

}
