use DefaultRectangular;

class locale {
  const myRealm: realm;
  const chpl_id: int;
  const chpl_uid: int;
  const myNumCores: int;

  proc locale(r: realm = nil, id = -1, uid = -1,
             numCores = __primitive("chpl_coresPerLocale")) {
    if doneCreatingLocales {
      halt("locales cannot be created");
    }
    myRealm = r;
    chpl_id = id;
    chpl_uid = uid;
    myNumCores = numCores;
  }

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
    _extern proc chpl_task_getCallStackSize(): uint(64);
    return chpl_task_getCallStackSize();
  }

  proc writeThis(f: Writer) {
    if (numRealms == 1) {
      f.write("LOCALE", id);
    } else {
      f.write("LOCALE", myRealm.id, "-", id);
    }
  }
}

proc chpl_setupLocale(uid) {
  var tmp: locale;
  on __primitive("chpl_on_locale_num", uid) {
    tmp = new locale(uid=uid);
    _here = tmp;
    if (defaultDist._value == nil) {
      defaultDist = new dmap(new DefaultDist());
    }
  }
  return tmp;
}

proc locale.numCores {
  return myNumCores;
}

proc chpl_int_to_locale(in id) {
  for r in Realms {
    if id < r.numLocales then
      return r.Locales[id];
    id -= r.numLocales;
  }
  halt("id out of range in chpl_int_to_locale()");
  return Realms[0].Locales[0];
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


//
// multi-locale diagnostics/debugging support
//
_extern proc chpl_startVerboseComm();
_extern proc chpl_stopVerboseComm();
_extern proc chpl_startVerboseCommHere();
_extern proc chpl_stopVerboseCommHere();
_extern proc chpl_startCommDiagnostics();
_extern proc chpl_stopCommDiagnostics();
_extern proc chpl_startCommDiagnosticsHere();
_extern proc chpl_stopCommDiagnosticsHere();

proc startVerboseComm() { chpl_startVerboseComm(); }
proc stopVerboseComm() { chpl_stopVerboseComm(); }
proc startVerboseCommHere() { chpl_startVerboseCommHere(); }
proc stopVerboseCommHere() { chpl_stopVerboseCommHere(); }

proc startCommDiagnostics() { chpl_startCommDiagnostics(); }
proc stopCommDiagnostics() { chpl_stopCommDiagnostics(); }
proc startCommDiagnosticsHere() { chpl_startCommDiagnosticsHere(); }
proc stopCommDiagnosticsHere() { chpl_stopCommDiagnosticsHere(); }


// TODO: generalize this for multiple realms by returning a manhattan
// array
_extern proc chpl_numCommGets(): int(32);
_extern proc chpl_numCommPuts(): int(32);
_extern proc chpl_numCommForks(): int(32);
_extern proc chpl_numCommFastForks(): int(32);
_extern proc chpl_numCommNBForks(): int(32);

proc getCommDiagnostics(realmID: int(32) = 0) {
  var D: [Realms(realmID).LocaleSpace] 5*int;
  const r = Realms(realmID);
  for loc in r.Locales do on loc {
    const gets = chpl_numCommGets();
    const puts = chpl_numCommPuts();
    const forks = chpl_numCommForks();
    const fforks = chpl_numCommFastForks();
    const nbforks = chpl_numCommNBForks();
    D(loc.id) = (gets, puts, forks, fforks, nbforks);
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
