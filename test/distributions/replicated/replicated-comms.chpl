use Time, ReplicatedDist, CommDiagnostics;
enum Mode {timer, comm, verboseComm}; use Mode;
config const mode = Mode.comm;

var t: Timer;
proc startDiag(name) {
  select mode {
    when timer {
      t.start();
    }
    when comm {
      startCommDiagnostics();
    }
    when verboseComm {
      writeln("ENTER ", name);
      startVerboseComm();
    }
  }
}
proc endDiag(name) {
  select mode {
    when timer {
      t.stop();
      writef("%s: %.2dr\n", name, t.elapsed());
      t.clear();
    }
    when comm {
      stopCommDiagnostics();
      writeln(name, " ", getCommDiagnostics()[1]);
      resetCommDiagnostics();
    }
    when verboseComm {
      stopVerboseComm();
      writeln("EXIT ", name);
    }
  }
}

startDiag("Build replicated dist");
const MyDist = new dmap(new Replicated(Locales));
endDiag("Build replicated dist");

startDiag("Build replicated domain");
const elemPerLocDom = {1..1} dmapped MyDist;
endDiag("Build replicated domain");

startDiag("Build replicated arrays");
var elemPerLoc: [elemPerLocDom] int;
var inputReady$: [elemPerLocDom] sync bool;
var outputReady$: [elemPerLocDom] sync bool;
endDiag("Build replicated arrays");
