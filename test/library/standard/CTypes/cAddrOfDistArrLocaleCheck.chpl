use CommDiagnostics;
use CTypes;
use BlockDist;

var Arr = blockDist.createArray(1..10, int);
Arr = -1;

// Arr record will be rvf'ed and `c_addrOf` will use that
on Locales.last {
  var Arr_p = c_addrOf(Arr);

  startCommDiagnosticsHere();
  for i in 1..10 do
    Arr_p.deref()[i] = i;
  stopCommDiagnosticsHere();
}

printCommDiagnosticsTable();
writeln(Arr);
