use DefaultArithmetic;

class locale {
  const myRealm: realm;
  const chpl_id: int;
  const chpl_uid: int;
  const myNumCores: int;

  def locale(r: realm = nil, id = -1, uid = -1,
             numCores = __primitive("chpl_coresPerLocale")) {
    if doneCreatingLocales {
      halt("locales cannot be created");
    }
    myRealm = r;
    chpl_id = id;
    chpl_uid = uid;
    myNumCores = numCores;
  }

  def id {
    return chpl_id;
  }

  def uid {
    return chpl_uid;
  }

  def name {
    var locName: string;
    on this do locName = __primitive("chpl_localeName");
    return locName;
  }

  def callStackSize: uint(64) {
    _extern def chpl_task_callstacksize(): uint(64);
    return chpl_taskCallStackSize();
  }

  def writeThis(f: Writer) {
    if (numRealms == 1) {
      f.write("LOCALE", id);
    } else {
      f.write("LOCALE", myRealm.id, "-", id);
    }
  }
}

def chpl_setupLocale(uid) {
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

def locale.numCores {
  return myNumCores;
}

def chpl_int_to_locale(in id) {
  for r in Realms {
    if id < r.numLocales then
      return r.Locales[id];
    id -= r.numLocales;
  }
  halt("id out of range in chpl_int_to_locale()");
  return Realms[0].Locales[0];
}


def locale.totalThreads() {
  var totalThreads: uint;

  on this do totalThreads = __primitive("chpl_numThreads");

  return totalThreads;
}

def locale.idleThreads() {
  var idleThreads: uint;

  on this do idleThreads = __primitive("chpl_numIdleThreads");

  return idleThreads;
}

def locale.queuedTasks() {
  var queuedTasks: uint;

  on this do queuedTasks = __primitive("chpl_numQueuedTasks");

  return queuedTasks;
}

def locale.runningTasks() {
  var numTasks: uint;

  on this do numTasks = __primitive("chpl_numRunningTasks");

  return numTasks;
}

def locale.blockedTasks() {
  var blockedTasks: int;

  on this do blockedTasks = __primitive("chpl_numBlockedTasks");

  return blockedTasks;
}


//
// multi-locale diagnostics/debugging support
//
_extern def chpl_startVerboseComm();
_extern def chpl_stopVerboseComm();
_extern def chpl_startVerboseCommHere();
_extern def chpl_stopVerboseCommHere();
_extern def chpl_startCommDiagnostics();
_extern def chpl_stopCommDiagnostics();
_extern def chpl_startCommDiagnosticsHere();
_extern def chpl_stopCommDiagnosticsHere();

def startVerboseComm() { chpl_startVerboseComm(); }
def stopVerboseComm() { chpl_stopVerboseComm(); }
def startVerboseCommHere() { chpl_startVerboseCommHere(); }
def stopVerboseCommHere() { chpl_stopVerboseCommHere(); }

def startCommDiagnostics() { chpl_startCommDiagnostics(); }
def stopCommDiagnostics() { chpl_stopCommDiagnostics(); }
def startCommDiagnosticsHere() { chpl_startCommDiagnosticsHere(); }
def stopCommDiagnosticsHere() { chpl_stopCommDiagnosticsHere(); }


// TODO: generalize this for multiple realms by returning a manhattan
// array
_extern def chpl_numCommGets(): int(32);
_extern def chpl_numCommPuts(): int(32);
_extern def chpl_numCommForks(): int(32);
_extern def chpl_numCommFastForks(): int(32);
_extern def chpl_numCommNBForks(): int(32);

def getCommDiagnostics(realmID: int(32) = 0) {
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

def chpl_startTrackingMemory() {
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
