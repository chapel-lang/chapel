// ChapelLocale.chpl
//
pragma "no use ChapelStandard"
module ChapelLocale {

use DefaultRectangular;
use ChapelNumLocales;

// would like this to be the following, but it breaks about 20 tests:
//const LocaleSpace: domain(1) distributed(OnePer) = [0..numLocales-1];
const LocaleSpace: domain(1) = [0..numLocales-1];

var doneCreatingLocales: bool;
var localeSerialNumber = 0;

class locale {
  const chpl_id: int = -1;
  const _numCores: int = -1;
  var parent: locale = nil;

  proc initialize() {
    if (_numCores < 0) {
      extern proc chpl_numCoresOnThisLocale(): int;
      _numCores = chpl_numCoresOnThisLocale();
    }
    // This only works on a non-distributed system.
    // It is not thread-safe and also very inefficient on a distributed system.
    // Instead of having a shared global variable, we need something like a GUID --
    // that is generated locally and yet guaranteed to be unique system-wide.
    if (chpl_id < 0) {
      chpl_id = localeSerialNumber;
      localeSerialNumber += 1;
    }
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
    f & new ioLiteral("LOCALE") & chpl_id;
  }
}

pragma "private" var _here: locale;
proc here return _here;

const Locales: [LocaleSpace] locale;


//proc locale.addChild(l: locale) {
//  LocaleSpace.expand(1);	// Expand by 1.
//  Locales[numLocales] = l;
//  numLocales += 1;
//}

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

proc set_here(loc : locale) {
  _here = loc;
 //task-private accessor here ...
}

proc set_here_by_id(locID: int) {
  set_here(Locales[locID]);
}

// Prototype for chpl_malloc functions implemented in the runtime.
extern proc chpl_mem_allocMany(l:int(32), size:int(32), description:int(32), lineno:int(32), filename:string):opaque;
extern proc chpl_mem_realloc(ptr:opaque, number:int(32), size:int(32), description:int(32), lineno:int(32), filename:string):opaque;
extern proc chpl_mem_free(ptr:opaque, lineno:int(32), filename:string);
extern proc sizeof(t) : int(32);


export locale_alloc proc locale.alloc(nbytes:int(32), mem_type:int(32), lineno:int(32), filename:string) : opaque { 
  extern proc printf(str: string);
  printf("l");
  return chpl_mem_allocMany(1, nbytes, mem_type, lineno, filename);
}

proc locale.realloc(ptr: opaque, nbytes:int(32), size:int(32), mem_type:int(32), lineno:int(32), filename:string) : opaque { 
  chpl_mem_realloc(ptr, nbytes, size, mem_type, lineno, filename);
}

proc locale.free(ptr: opaque, lineno:int(32), filename:string) { 
  chpl_mem_free(ptr, lineno, filename);
}

proc locale.numCores {
  return _numCores;
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

proc chpl_int_to_locale(id) {
  // Internal representation is not necessarily a dense rectangular array.
  return Locales(id);
}

proc chpl_getPrivatizedCopy(type objectType, objectPid:int): objectType
  return __primitive("chpl_getPrivatizedClass", nil:objectType, objectPid);


//
// multi-locale diagnostics/debugging support
//

// There should be a type like this declared in chpl-comm.h with a single
// function that returns the C struct.  We're not doing it that way yet
// due to some shortcomings in our extern records implementation.
// Once that gets sorted out, we can turn this into an extern record,
// and remove the 8 or so individual functions below that return the
// various counters.
record chpl_commDiagnostics {
  var get: uint(64);
  var get_nb: uint(64);
  var get_nb_test: uint(64);
  var get_nb_wait: uint(64);
  var put: uint(64);
  var fork: uint(64);
  var fork_fast: uint(64);
  var fork_nb: uint(64);
};

type commDiagnostics = chpl_commDiagnostics;

extern proc chpl_startVerboseComm();
extern proc chpl_stopVerboseComm();
extern proc chpl_startVerboseCommHere();
extern proc chpl_stopVerboseCommHere();
extern proc chpl_startCommDiagnostics();
extern proc chpl_stopCommDiagnostics();
extern proc chpl_startCommDiagnosticsHere();
extern proc chpl_stopCommDiagnosticsHere();
extern proc chpl_resetCommDiagnosticsHere();
extern proc chpl_getCommDiagnosticsHere(out cd: commDiagnostics);

proc startVerboseComm() { chpl_startVerboseComm(); }
proc stopVerboseComm() { chpl_stopVerboseComm(); }
proc startVerboseCommHere() { chpl_startVerboseCommHere(); }
proc stopVerboseCommHere() { chpl_stopVerboseCommHere(); }

proc startCommDiagnostics() { chpl_startCommDiagnostics(); }
proc stopCommDiagnostics() { chpl_stopCommDiagnostics(); }
proc startCommDiagnosticsHere() { chpl_startCommDiagnosticsHere(); }
proc stopCommDiagnosticsHere() { chpl_stopCommDiagnosticsHere(); }

proc resetCommDiagnostics() {
  for loc in Locales do on loc do
    resetCommDiagnosticsHere();
}

inline proc resetCommDiagnosticsHere() {
  chpl_resetCommDiagnosticsHere();
}

// See note above regarding extern records
extern proc chpl_numCommGets(): uint(64);
extern proc chpl_numCommNBGets(): uint(64);
extern proc chpl_numCommTestNBGets(): uint(64);
extern proc chpl_numCommWaitNBGets(): uint(64);
extern proc chpl_numCommPuts(): uint(64);
extern proc chpl_numCommForks(): uint(64);
extern proc chpl_numCommFastForks(): uint(64);
extern proc chpl_numCommNBForks(): uint(64);

proc getCommDiagnostics() {
  var D: [LocaleSpace] commDiagnostics;
  for loc in Locales do on loc {
    // See note above regarding extern records
    D(loc.id).get = chpl_numCommGets();
    D(loc.id).put = chpl_numCommPuts();
    D(loc.id).fork = chpl_numCommForks();
    D(loc.id).fork_fast = chpl_numCommFastForks();
    D(loc.id).fork_nb = chpl_numCommNBForks();
    D(loc.id).get_nb = chpl_numCommNBGets();
    D(loc.id).get_nb_test = chpl_numCommTestNBGets();
    D(loc.id).get_nb_wait = chpl_numCommWaitNBGets();
  }
  return D;
}

proc getCommDiagnosticsHere() {
  var cd: commDiagnostics;
  cd.get = chpl_numCommGets();
  cd.put = chpl_numCommPuts();
  cd.fork = chpl_numCommForks();
  cd.fork_fast = chpl_numCommFastForks();
  cd.fork_nb = chpl_numCommNBForks();
  cd.get_nb = chpl_numCommNBGets();
  cd.get_nb_test = chpl_numCommTestNBGets();
  cd.get_nb_wait = chpl_numCommWaitNBGets();
  return cd;
}

config const
  memTrack: bool = false,
  memStats: bool = false, 
  memLeaks: bool = false,
  memLeaksTable: bool = false,
  memMax: int = 0,
  memThreshold: int = 0,
  memLog: string = "";

pragma "no auto destroy"
config const
  memLeaksLog: string = "";

proc chpl_startTrackingMemory() {
  if Locales(0) == here {
    coforall loc in Locales {
      if loc == here {
        __primitive("chpl_setMemFlags", memTrack, memStats, memLeaks, memLeaksTable, memMax, memThreshold, memLog, memLeaksLog);
      } else on loc {
          __primitive("chpl_setMemFlags", memTrack, memStats, memLeaks, memLeaksTable, memMax, memThreshold, memLog, memLeaksLog);
      }
    }
  }
}

} // End of ChapelLocale module
