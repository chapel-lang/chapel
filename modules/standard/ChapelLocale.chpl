use DefaultArithmetic;

class locale {
  const myRealm: realm;
  const chpl_id: int;
  const chpl_uid: int;

  def locale(r: realm = nil, id = -1, uid = -1) {
    if doneCreatingLocales {
      halt("locales cannot be created");
    }
    myRealm = r;
    chpl_id = id;
    chpl_uid = uid;
  }

  def id {
    return chpl_id;
  }

  def name {
    var locName: string;
    on this do locName = __primitive("chpl_localeName");
    return locName;
  }

  def writeThis(f: Writer) {
    if (numRealms == 1) {
      f.write("LOCALE", id);
    } else {
      f.write("LOCALE", myRealm.id, "-", chpl_id);
    }
  }
}

def chpl_setupLocale(r, id, baseid) {
  var tmp: locale;
  on __primitive("chpl_on_locale_num", id) {
    tmp = new locale(r, id, baseid);
    _here = tmp;
    if (defaultDist == nil) {
      defaultDist = new DefaultDist();
    }
  }
  return tmp;
}

def locale.numCores {
  var numCores: int;
  on this do numCores = __primitive("chpl_coresPerLocale");
  return numCores;
}

def chpl_int_to_locale(id) {
  return Locales(id);
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
def startVerboseComm() { __primitive("chpl_startVerboseComm"); }
def stopVerboseComm() { __primitive("chpl_stopVerboseComm"); }
def startVerboseCommHere() { __primitive("chpl_startVerboseCommHere"); }
def stopVerboseCommHere() { __primitive("chpl_stopVerboseCommHere"); }

def startCommDiagnostics() { __primitive("chpl_startCommDiagnostics"); }
def stopCommDiagnostics() { __primitive("chpl_stopCommDiagnostics"); }
def startCommDiagnosticsHere() { __primitive("chpl_startCommDiagnosticsHere"); }
def stopCommDiagnosticsHere() { __primitive("chpl_stopCommDiagnosticsHere"); }


// TODO: generalize this for multiple realms by returning a manhattan
// array
def getCommDiagnostics(realmID: int(32) = 0) {
  var D: [Realms(realmID).LocaleSpace] 4*int;
  const r = Realms(realmID);
  for loc in r.Locales do on loc {
    const gets = __primitive("chpl_numCommGets");
    const puts = __primitive("chpl_numCommPuts");
    const forks = __primitive("chpl_numCommForks");
    const nbforks = __primitive("chpl_numCommNBForks");
    D(loc.id) = (gets, puts, forks, nbforks);
  }
  return D;
}


config const
  memTrack: bool = false,
  memStats: bool = false, 
  memLeaks: bool = false,
  memMax: uint(64) = 0,
  memThreshold: uint(64) = 0,
  memLog: string = "",
  memLeaksLog: string = "";

def chpl_startTrackingMemory() {
  coforall r in Realms do
    on r do
      coforall loc in r.Locales {
        if loc == here {
          __primitive("chpl_setMemFlags", memTrack, memStats, memLeaks, memMax, memThreshold, memLog, memLeaksLog);
        } else on loc {
            __primitive("chpl_setMemFlags", memTrack, memStats, memLeaks, memMax, memThreshold, memLog, memLeaksLog);
        }
      }
}
