use CommDiagnostics;

proc doComm(numtrials) {
  var x: int;
  for 1..numtrials do
    coforall loc in Locales with (ref x) do
      on loc do
        x = 1;
}

manage new verboseCommManager() {
  doComm(1);
}

