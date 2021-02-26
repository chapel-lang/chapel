use BlockDist;
use CommDiagnostics;

const dom = {1..10} dmapped Block({1..10});


for l in Locales do on l {
  writeln(l);
  var isConstRec: bool;
  var isConstIns: bool;

  startCommDiagnosticsHere();
  isConstRec = dom.definedConst;
  isConstIns = dom._value.definedConst;
  stopCommDiagnosticsHere();

  writeln(getCommDiagnosticsHere());
  writeln(isConstRec);
  writeln(isConstIns);
}

