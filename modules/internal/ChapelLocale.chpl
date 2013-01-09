// ChapelLocale.chpl
//
pragma "no use ChapelStandard"
module ChapelLocale {
  
  use DefaultRectangular;
  
  // would like this to be the following, but it breaks about 20 tests:
  //const LocaleSpace: domain(1) distributed(OnePer) = [0..numLocales-1];
  const LocaleSpace: domain(1) = {0..numLocales-1};
  
  var doneCreatingLocales: bool;
  
  class locale {
    const chpl_id: int;
    const numCores: int;
  
    proc locale(id = -1) {
      if doneCreatingLocales {
        halt("locales cannot be created");
      }
      chpl_id = id;
  
      extern proc chpl_numCoresOnThisLocale(): int;
      numCores = chpl_numCoresOnThisLocale();
    }
  
    proc id {
      return chpl_id;
    }
  
    proc name {
      var locName: string;
      on this do locName = __primitive("chpl_localeName");
      return locName;
    }
  
    proc callStackSize: int {
      // Locales may have differing call stack sizes.
      extern proc chpl_task_getCallStackSize(): int;
      var retval: int;
      on this do retval = chpl_task_getCallStackSize();
      return retval;
    }
  
    proc readWriteThis(f) {
      f <~> new ioLiteral("LOCALE") <~> chpl_id;
    }
  }
  
  pragma "private" var _here: locale;
  
  proc here return _here;
  
  // Perform locale-specific initialization.
  // This is where global variables declared 'pragma "private"' are initialized.
  // That initialization is not currently arranged automatically by the compiler.
  proc chpl_setupLocale(id) {
    var tmp: locale;
    on __primitive("chpl_on_locale_num", id) {
      tmp = new locale(id);
      _here = tmp;
      if (defaultDist._value == nil) {
        defaultDist = new dmap(new DefaultDist());
      }
    }
    return tmp;
  }
  
  const Locales: [LocaleSpace] locale;
  // We cannot use a forall here because the default leader iterator will
  // access data structures that are not yet initialized (i.e., Locales
  // array/here).  An alternative would be to use a coforall+on and refactor
  // chpl_setupLocale().
  for loc in LocaleSpace do
    Locales(loc) = chpl_setupLocale(loc);
  
  doneCreatingLocales = true;
  
  //
  // tree for recursive task invocation during privatization
  //
  record chpl_localeTreeRecord {
    var left, right: locale;
  }
  pragma "private" var chpl_localeTree: chpl_localeTreeRecord;
  
  proc chpl_initLocaleTree() {
    for i in LocaleSpace {
      var left: locale = nil;
      var right: locale = nil;
      var child = (i+1)*2-1;
      if child < numLocales {
        left = Locales[child];
        child += 1;
        if child < numLocales then
          right = Locales[child];
      }
      on Locales(i) {
        chpl_localeTree.left = left;
        chpl_localeTree.right = right;
      }
    }
  }
  
  chpl_initLocaleTree();
  
  //proc locale.numCores {
  //  var numCores: int;
  //  on this do numCores = __primitive("chpl_coresPerLocale");
  //  return numCores;
  //}
  
  proc chpl_int_to_locale(id) {
    return Locales(id);
  }
  
  
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
