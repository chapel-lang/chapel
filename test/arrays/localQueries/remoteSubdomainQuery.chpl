use CommDiagnostics;



config const modify = false;

var DR = {1..10};
var DA = {3.4};
var DS: sparse subdomain(DR) = {7};

var AR: [DR] real;
var AA: [DA] real;
var AS: [DS] real;

testit(DR);
testit(DA);
testit(DS);
testit(AR);
testit(AA);
testit(AS);

proc testit(ref D) {
  if isDomain(D) then
    if modify then D.clear(); // thwart rvf
  testitHelp(D, numLocales-1);
  if (numLocales > 1) {
    writeln("From locale ", numLocales-1, ":");
    on Locales[numLocales-1] do
      testitHelp(D, 0);
  }
}

proc testitHelp(ref D, other) {
  resetCommDiagnostics();
  
  if isDomain(D) then
    if modify then D.clear(); // thwart rvf
  writeln(D.type:string, ":");
  if (D.hasSingleLocalSubdomain()) {
    startCommDiagnosticsHere();
    const locSubD = D.localSubdomain();
    const remSubD = D.localSubdomain(Locales[other]);
    stopCommDiagnosticsHere();
    writeln("I own:   ", locSubD);
    writeln("It owns: ", remSubD);
  }

  startCommDiagnosticsHere();
  for ls in D.localSubdomains() {
    stopCommDiagnosticsHere();
    writeln("I own:   ", ls);
    startCommDiagnosticsHere();
  }
  stopCommDiagnosticsHere();

  startCommDiagnosticsHere();
  for ls in D.localSubdomains(Locales[other]) {
    stopCommDiagnosticsHere();
    writeln("It owns: ", ls);
    startCommDiagnosticsHere();
  }
  stopCommDiagnosticsHere();

  // only verify that there's no communication when we own D
  if D.locale == here then writeln(getCommDiagnosticsHere());
  writeln();
}

