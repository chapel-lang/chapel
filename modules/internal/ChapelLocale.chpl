// ChapelLocale.chpl
//
pragma "no use ChapelStandard"
module ChapelLocale {

  use DefaultRectangular;

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

  extern record chpl_localeID_t {
    var node : c_nodeid_t;
    var subloc : c_subloc_t;
  };
  extern proc chpl_localeID_get_node(loc:chpl_localeID_t):int(64);
  extern proc chpl_localeID_get_subloc(loc:chpl_localeID_t):int(64);
  extern proc chpl_localeID_equals(loc:chpl_localeID_t):int(64);

  proc ==(a:chpl_localeID_t, b:chpl_localeID_t)
    return chpl_localeID_equals(a,b) != 0;

  proc !=(a:chpl_localeID_t, b:chpl_localeID_t)
    return chpl_localeID_equals(a,b) == 0;


  const emptyLocaleSpace: domain(1) = {1..0};
  const emptyLocales: [emptyLocaleSpace] locale;

  //
  // An abstract class. Specifies the required locale interface.
  // Each locale implementation must inherit from this class.
  //
  class locale {
    // We may want to let subclasses choose how to implement 'id' and 'name'.
    var chpl_id: int;
    const name: string;
    // To be removed from the required interface once legacy code is adjusted.
    const numCores: int;

    proc locale() {
      compilerError("cannot create instances of the 'locale' class");
    }
  
    // In legacy code, the id accessor is used to obtain the node id, so it
    // should probably be renamed as node_id.
    // As an accessor, it is statically bound, which is also a problem....
    proc id return chpl_id;
  
    proc readWriteThis(f) {
      halt("Pure virtual function called.");
    }

    proc getChildSpace() {
      halt("Pure virtual function called.");
      return emptyLocaleSpace;
    }      
  
    proc getChildCount() : int {
      halt("Pure virtual function called.");
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
      halt("Pure virtual function called.");
    }
  
    proc getChild(idx:int) : locale {
      halt("Pure virtual function called.");
      return this;
    }

// Part of the required locale interface.
// Commented out because presently iterators are statically bound.
//    iter getChildren() : locale  {
//      halt("Pure virtual function called.");
//      yield 0;
//    }

    // This is a special interface used to set up the global Locales array
    // for backward compatibility.  When that array is removed, this routine
    // may be removed as well.
    proc getChildArray() {
      halt("Pure virtual function called.");
      return emptyLocales;
    }

  }

  // TODO: This wants to live in RootLocale.chpl
  var rootLocale : locale = nil;

  pragma "private" var _here: locale;

  proc here return _here;
  

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
  
  proc chpl_getPrivatizedCopy(type objectType, objectPid:int): objectType
    return __primitive("chpl_getPrivatizedClass", nil:objectType, objectPid);
  
}
