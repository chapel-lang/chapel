use Time;
use Memory.Diagnostics;
use CommDiagnostics;

use BlockDist;

type elemType = int;

enum diagMode { performance, correctness, commCount, verboseComm, verboseMem };
config const mode = diagMode.commCount;

config const reportDist = true;
config const reportDom = true;
config const reportArr = true;

config const reportInit = true;
config const reportDeinit = false;

const nElems = numLocales;

var t = new Timer();

inline proc shouldRunDiag(name) {
  if !reportInit && name.endsWith("Init") then
    return false;
  if !reportDeinit && name.endsWith("Deinit") then
    return false;

  return true;
}

proc startDiag(name) {
  if !shouldRunDiag(name) then return;

  select(mode) {
    when diagMode.performance {
      t.start();
    }
    when diagMode.correctness { }
    when diagMode.commCount {
      startCommDiagnostics();
    }
    when diagMode.verboseComm {
      startVerboseComm();
    }
    when diagMode.verboseMem {
      startVerboseMem();
    }
    otherwise {
      halt("Unrecognized diagMode");
    }
  }
}

proc endDiag(name) {
  if !shouldRunDiag(name) then return;

  select(mode) {
    when diagMode.performance {
      t.stop();
      writeln(name, ": ", t.elapsed());
      t.clear();
    }
    when diagMode.correctness { }
    when diagMode.commCount {
      stopCommDiagnostics();
      writeln(name);
      printCommDiagnosticsTable();
      resetCommDiagnostics();
    }
    when diagMode.verboseComm {
      stopVerboseComm();
    }
    when diagMode.verboseMem {
      stopVerboseMem();
    }
    otherwise {
      halt("Unrecognized diagMode");
    }
  }
}

const localDom = {1..nElems};

proc main() {
  { // weird blocks are necessary to measure deinit performance
    if reportDist then startDiag("distInit");
    const Dist = new dmap(new Block(boundingBox=localDom));
    if reportDist then endDiag("distInit");
    {
      if reportDom then startDiag("domInit");
      const Dom = localDom dmapped Dist;
      if reportDom then endDiag("domInit");
      {
        if reportArr then startDiag("arrInit");
        var Arr: [Dom] elemType;
        if reportArr then endDiag("arrInit");

        if reportArr then startDiag("arrDeinit");
      }
      if reportArr then endDiag("arrDeinit");

      if reportDom then startDiag("domDeinit");
    }
    if reportDom then endDiag("domDeinit");

    if reportDist then startDiag("distDeinit");
  }
  if reportDist then endDiag("distDeinit");
}
