use CommDiagnostics;
config param doVerboseComm = false;
config const noisy = false;
config const target = numLocales-1;
config const printForkCnt = false;
config const printGetPutCnt = false;

// Although this test does the wrong thing the majority of the time, it can
// sometimes happen upon the right comm counts once in a while. By running
// multiple trials we have comm counts that are much less likely to
// accidentally match the .good file.
config const numtrials = 10;

proc foo() {
  if noisy then writeln("hi");
}

proc printDiags() {
  const diags = getCommDiagnostics();
  for i in 0..numLocales-1 do {
    write(i, ": ");
    if printForkCnt then
      write((diags[i].fork, diags[i].fork_fast, diags[i].fork_nb));
    if printGetPutCnt then
      write((diags[i].get, diags[i].get_nb, diags[i].put));
    writeln();
  }
}

proc main() {
  resetCommDiagnostics();
  startCommDiagnostics();
  if doVerboseComm then startVerboseComm();
  for i in 1..numtrials {
    on Locales[numLocales-1] do if here.id==target then begin foo();
  }
  if doVerboseComm then stopVerboseComm();
  stopCommDiagnostics();
  printDiags();

  resetCommDiagnostics();
  startCommDiagnostics();
  if doVerboseComm then startVerboseComm();
  for i in 1..numtrials {
    coforall l in Locales[1..numLocales-1] do on l {
        if l.id==target then begin foo();
      }
  }
  if doVerboseComm then stopVerboseComm();
  stopCommDiagnostics();
  printDiags();

  resetCommDiagnostics();
  startCommDiagnostics();
  if doVerboseComm then startVerboseComm();
  for i in 1..numtrials {
    cobegin {
      on Locales[0] do if here.id==target then begin foo();
      on Locales[numLocales-1] do if here.id==target then begin foo();
    }
  }
  if doVerboseComm then stopVerboseComm();
  stopCommDiagnostics();
  printDiags();
}
