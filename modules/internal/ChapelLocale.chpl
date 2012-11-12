// ChapelLocale.chpl
//
pragma "no use ChapelStandard"
module ChapelLocale {

config const dataParTasksPerLocale = 0;
config const dataParIgnoreRunningTasks = true;
config const dataParMinGranularity: int = 1;

class locale {
  const chpl_id: int(32);
  const numCores: int;

  proc locale() {
    chpl_id = __primitive("chpl_localeID");  // Misnamed.
    // chpl_localeID really returns the current (GASNet) node ID.

    extern proc chpl_numCoresOnThisLocale(): int;
    numCores = chpl_numCoresOnThisLocale();
  }

  // In traditional use, the "locale ID" is used to mean the node ID.
  proc id {
    return chpl_id;
  }

  proc readWriteThis(f) {
    f <~> new ioLiteral("LOCALE") <~> chpl_id;
  }

// Required by the sublocale interface
  proc addChild(child : locale) : void
  { /* Do nothing. */ }

  proc getChildCount() : int { return 0; }

  // By convention, the child corresponding to sublocale 0
  // is the locale itself.
  // In this default case, there are no sublocales, so the index is ignored
  // (assumed to be zero).
  proc getChild(subloc_id : int) : locale { return this; }

  iter getChildren() : locale { yield this; }

  // Also required by the sublocale interface.
  proc initTask() {} // Do nothing.

  proc alloc(type x, md) {
    // The default implementation.
//  var nbytes = __primitive("sizeof", x);
//  var mem = __primitive("chpl_mem_alloc", x, md);
//  return __primitive("cast", x.type, mem);
    return __primitive("chpl_mem_alloc", x, md);
  }

  proc free(x:object) {
    __primitive("chpl_mem_free", x);
  }
}

// Because it is declared in module scope and labelled "private", it exists
// at the node level.
// Every node has a corresponding _here locale, but may contain 
// other locales as well.
pragma "private" var _here: locale = new locale();
// The concept of here is necessary for privatization, 
// but should not generally be used in distributions.

proc here : locale return _here;

  proc locale.name {
    var locName: string;
    on this do locName = __primitive("chpl_localeName");
    return locName;
  }

  proc locale.callStackSize: int {
    // Locales may have differing call stack sizes.
    extern proc chpl_task_getCallStackSize(): int;
    var retval: int;
    on this do retval = chpl_task_getCallStackSize();
    return retval;
  }


proc locale.totalThreads() {
  var totalThreads: int;

  on this do totalThreads = __primitive("chpl_numThreads");

  return totalThreads;
}

proc locale.idleThreads() {
  var idleThreads: int;

  on this do idleThreads = __primitive("chpl_numIdleThreads");

  return idleThreads;
}

proc locale.queuedTasks() {
  var queuedTasks: int;

  on this do queuedTasks = __primitive("chpl_numQueuedTasks");

  return queuedTasks;
}

proc locale.runningTasks() {
  var numTasks: int;

  on this do numTasks = __primitive("chpl_numRunningTasks");

  return numTasks;
}

proc locale.blockedTasks() {
  var blockedTasks: int;

  on this do blockedTasks = __primitive("chpl_numBlockedTasks");

  return blockedTasks;
}

// Does this really belong here? <hilde>
proc chpl_getPrivatizedCopy(type objectType, objectPid:int): objectType
  return __primitive("chpl_getPrivatizedClass", nil:objectType, objectPid);

// Here be dragons: If the return type is specified, then normalize.cpp inserts
// an initializer for the return value, which calls its constructor, which calls
// chpl_here_alloc ad infinitum.  But if the return type is left off, it works!!!
proc chpl_here_alloc(x, md) {
  return here.getChild(__primitive("_get_subloc_id")).alloc(x.type, md);
}

// This one is called from protoIteratorClass().  Can we fix that call and get rid
// of this specialized version?
proc chpl_here_alloc(type x, md) {
  return here.getChild(__primitive("_get_subloc_id")).alloc(x, md);
}

proc chpl_here_free(x) {
  here.getChild(__primitive("_get_subloc_id")).free(x);
}

}
