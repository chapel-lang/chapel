use CommDiagnostics;

record commCounter {
  param isLocal: bool;
  var _resetCountsOnExit: bool;
  var _stats: [LocaleSpace] commDiagnostics;
  var _verbose = false;

  proc init() {
    this.isLocal = false;
    this._resetCountsOnExit = true;
  }

  proc init(param isLocal: bool, resetCountsOnExit: bool) {
    this.isLocal = isLocal;
    this._resetCountsOnExit = resetCountsOnExit;
  }

  inline proc _startCounting() {
    if isLocal then startCommDiagnosticsHere();
               else startCommDiagnostics();
  }

  inline proc _stopCounting() {
    if isLocal then stopCommDiagnosticsHere();
               else stopCommDiagnostics();
  }

  inline proc _resetCounts() {
    if isLocal then resetCommDiagnosticsHere();
               else resetCommDiagnostics();
  }

  inline proc _startVerbose() {
    if isLocal then startVerboseCommHere();
               else startVerboseComm();
  }

  inline proc _stopVerbose() {
    if isLocal then stopVerboseCommHere();
               else stopVerboseComm();
  }

  proc ref enterContext(): void {
    _startCounting();
  }

  proc ref exitContext(in e: owned Error?) {
    if e then halt(e!.message());
    if _verbose then _stopVerbose();
    _stopCounting();
    _stats = getCommDiagnostics();
    if _resetCountsOnExit then _resetCounts();
  }

  inline proc stats const ref do return _stats;
  inline proc statsHere const ref do return _stats[here.id];

  inline proc verbose() ref {
    _verbose = true;
    _startVerbose();
    return this;
  }

  inline proc printStats() {
    for idx in 0..<stats.size {
      writeln('LOCALE-', idx, ': ', stats[idx]);
    }
  }

  inline proc printStatsHere() {
    writeln('LOCALE-', here.id, ': ', statsHere);
  }
}

commCounter implements contextManager;
