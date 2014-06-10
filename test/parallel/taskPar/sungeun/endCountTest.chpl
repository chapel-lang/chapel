use CommDiagnostics;
config param doVerboseComm = false;
config const noisy = false;
config const target = numLocales-1;
config const printForkCnt = false;
config const printGetPutCnt = false;

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
  on Locales[numLocales-1] do if here.id==target then begin foo();
  if doVerboseComm then stopVerboseComm();
  stopCommDiagnostics();
  printDiags();

  resetCommDiagnostics();
  startCommDiagnostics();
  if doVerboseComm then startVerboseComm();
  coforall l in Locales[1..numLocales-1] do on l {
      if l.id==target then begin foo();
    }
  if doVerboseComm then stopVerboseComm();
  stopCommDiagnostics();
  printDiags();

  resetCommDiagnostics();
  startCommDiagnostics();
  if doVerboseComm then startVerboseComm();
  cobegin {
    on Locales[0] do if here.id==target then begin foo();
    on Locales[numLocales-1] do if here.id==target then begin foo();
  }
  if doVerboseComm then stopVerboseComm();
  stopCommDiagnostics();
  printDiags();
}
