use CommDiagnostics;

record commDiagnosticsManager: contextManager {
  var autoPrint: bool;
  @chpldoc.nodoc
  var diagnostics: [LocaleSpace] commDiagnostics;

  proc init(autoPrint: bool = false) {
    this.autoPrint = autoPrint;
  }

  proc get() {
    return diagnostics;
  }

  proc enterContext() {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  proc ref exitContext(in e: owned Error?) {
    stopCommDiagnostics();
    diagnostics = getCommDiagnostics();

    if autoPrint {
      printCommDiagnosticsTable(diagnostics=diagnostics);
    }
  }
}


proc doComm(numtrials) {
  var x: int;
  for 1..numtrials do
    coforall loc in Locales with (ref x) do
      on loc do
        x = 1;
}

manage new commDiagnosticsManager(autoPrint=true) {
  doComm(1);
}
writeln("finished\n");



var m = new commDiagnosticsManager();
printCommDiagnosticsTable(m.get());
manage m {
  doComm(1);
}
writeln("\nafter context");
printCommDiagnosticsTable(m.get());
