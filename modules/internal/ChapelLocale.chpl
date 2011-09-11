

pragma "initialize prelocale" class locale {
  //const myRealm: realm;
  const chpl_id: int;
  const chpl_uid: int;
  const myNumCores: int;


//    extern proc chpl_numCoresOnThisLocale(): int(32);
//    numCores = chpl_numCoresOnThisLocale();
  proc id {
    return chpl_id;
  }

  proc uid {
    return chpl_uid;
  }

  proc name {
    var locName: string;
    on this do locName = __primitive("chpl_localeName");
    return locName;
  }

  proc callStackSize: uint(64) {
    // Locales may have differing call stack sizes.
    extern proc chpl_task_getCallStackSize(): uint(64);
    var retval: uint(64);
    on this do retval = chpl_task_getCallStackSize();
    return retval;
  }

  proc writeThis(f: Writer) {
    //if (numRealms == 1) {
      f.write("LOCALE", id);
    //} else {
    //  f.write("LOCALE", myRealm.id, "-", id);
    //}
  }

  proc getChild() : locale {
    writeln("Cannot get sublocale from locale"); //should we halt?
    return nil;
  }
}

// Prototype for chpl_malloc functions implemented in the runtime.
_extern proc chpl_mem_allocMany(l:int(32), size:int(32), description:int(32), lineno:int(32), filename:string):opaque;
_extern proc chpl_mem_realloc(ptr:opaque, number:int(32), size:int(32), description:int(32), lineno:int(32), filename:string):opaque;
_extern proc chpl_mem_free(ptr:opaque, lineno:int(32), filename:string);
_extern proc sizeof(t) : int(32);

proc locale.numCores {
  return myNumCores;
}

proc locale.alloc(nbytes:int(32), mem_type:int(32), lineno:int(32), filename:string) : opaque { 
  _extern proc printf(str: string);
  printf("l");
  return chpl_mem_allocMany(1, nbytes, mem_type, lineno, filename);
}

proc locale.realloc(ptr: opaque, nbytes:int(32), size:int(32), mem_type:int(32), lineno:int(32), filename:string) : opaque { 
  chpl_mem_realloc(ptr, nbytes, size, mem_type, lineno, filename);
}

proc locale.free(ptr: opaque, lineno:int(32), filename:string) { 
  chpl_mem_free(ptr, lineno, filename);
}

proc locale.totalThreads() {
  var totalThreads: uint;

  on this do totalThreads = __primitive("chpl_numThreads");

  return totalThreads;
}

proc locale.idleThreads() {
  var idleThreads: uint;

  on this do idleThreads = __primitive("chpl_numIdleThreads");

  return idleThreads;
}

proc locale.queuedTasks() {
  var queuedTasks: uint;

  on this do queuedTasks = __primitive("chpl_numQueuedTasks");

  return queuedTasks;
}

proc locale.runningTasks() {
  var numTasks: uint;

  on this do numTasks = __primitive("chpl_numRunningTasks");

  return numTasks;
}

proc locale.blockedTasks() {
  var blockedTasks: int;

  on this do blockedTasks = __primitive("chpl_numBlockedTasks");

  return blockedTasks;
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
  var get: int(32);
  var get_nb: int(32);
  var get_nb_test: int(32);
  var get_nb_wait: int(32);
  var put: int(32);
  var fork: int(32);
  var fork_fast: int(32);
  var fork_nb: int(32);
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

proc resetCommDiagnostics(realmID: int(32) = 0) {
  for loc in Realms(realmID).Locales do on loc do
    resetCommDiagnosticsHere();
}

proc resetCommDiagnosticsHere(realmID: int(32) = 0) {
  chpl_resetCommDiagnosticsHere();
}

// TODO: generalize this for multiple realms by returning a manhattan
// array
// See note above regarding extern records
extern proc chpl_numCommGets(): int(32);
extern proc chpl_numCommNBGets(): int(32);
extern proc chpl_numCommTestNBGets(): int(32);
extern proc chpl_numCommWaitNBGets(): int(32);
extern proc chpl_numCommPuts(): int(32);
extern proc chpl_numCommForks(): int(32);
extern proc chpl_numCommFastForks(): int(32);
extern proc chpl_numCommNBForks(): int(32);

proc getCommDiagnostics(realmID: int(32) = 0) {
  var D: [Realms(realmID).LocaleSpace] commDiagnostics;
  const r = Realms(realmID);
  for loc in r.Locales do on loc {
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


config const
  memTrack: bool = false,
  memStats: bool = false, 
  memLeaks: bool = false,
  memLeaksTable: bool = false,
  memMax: uint(64) = 0,
  memThreshold: uint(64) = 0,
  memLog: string = "";

pragma "no auto destroy"
config const
  memLeaksLog: string = "";

proc chpl_startTrackingMemory() {
  coforall r in Realms {
    if r.Locales(0) == here {
      coforall loc in r.Locales {
        if loc == here {
          __primitive("chpl_setMemFlags", memTrack, memStats, memLeaks, memLeaksTable, memMax, memThreshold, memLog, memLeaksLog);
        } else on loc {
            __primitive("chpl_setMemFlags", memTrack, memStats, memLeaks, memLeaksTable, memMax, memThreshold, memLog, memLeaksLog);
        }
      }
    } else {
      on r {
        coforall loc in r.Locales {
          if loc == here {
            __primitive("chpl_setMemFlags", memTrack, memStats, memLeaks, memLeaksTable, memMax, memThreshold, memLog, memLeaksLog);
          } else on loc {
              __primitive("chpl_setMemFlags", memTrack, memStats, memLeaks, memLeaksTable, memMax, memThreshold, memLog, memLeaksLog);
          }
        }
      }
    }
  }
}
