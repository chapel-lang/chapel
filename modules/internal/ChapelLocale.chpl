// ChapelLocale.chpl
//
pragma "no use ChapelStandard"
module ChapelLocale {

config const dataParTasksPerLocale = 0;
config const dataParIgnoreRunningTasks = true;
config const dataParMinGranularity: int = 1;

class locale {
  const chpl_id: int;
  const numCores: int;

  proc locale() {
    chpl_id = __primitive("chpl_localeID");  // Misnamed.
    // chpl_localeID really returns the current (GASNet) node ID.

    extern proc chpl_numCoresOnThisLocale(): int;
    numCores = chpl_numCoresOnThisLocale();
  }

  proc id {
    return chpl_id;
  }

  proc readWriteThis(f) {
    f <~> new ioLiteral("LOCALE") <~> chpl_id;
  }

// Required by the sublocale interface
  proc addChild(child : locale) : void
  { /* Do nothing. */ }

  // By convention, the child corresponding to sublocale 0
  // is the locale itself.
  // In this default case, there are no sublocales, so the index is ignored
  // (assumed to be zero).
  proc getChild(subloc_id : int) : locale { return this; }

  // Also required by the sublocale interface.
  proc initTask() {} // Do nothing.
}

// Would like to do away with _here altogether.
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

// Break out into separate module?
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
  for loc in rootLocale.getLocales() do on loc do
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
  var D: [rootLocale.getLocaleSpace()] commDiagnostics;
  for loc in rootLocale.getLocales() do on loc {
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
  if rootLocale.getLocale(0) == here {
    coforall loc in rootLocale.getLocales() {
      if loc == here {
        __primitive("chpl_setMemFlags", memTrack, memStats, memLeaks, memLeaksTable, memMax, memThreshold, memLog, memLeaksLog);
      } else on loc {
          __primitive("chpl_setMemFlags", memTrack, memStats, memLeaks, memLeaksTable, memMax, memThreshold, memLog, memLeaksLog);
      }
    }
  }
}

}
