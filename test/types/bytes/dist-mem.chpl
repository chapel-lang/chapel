use CommDiagnostics;
config const length = 20;
var b: bytes = "b ";
b = b*length;
var otherB: bytes;

on Locales[1] {
  startCommDiagnosticsHere();
  var locB = b.toUpper();
  stopCommDiagnosticsHere();
  writeln(locB);
  writeln(getCommDiagnosticsHere());
}

resetCommDiagnostics();

on Locales[1] {
  startCommDiagnosticsHere();
  otherB = b.toUpper();
  stopCommDiagnosticsHere();
  writeln(otherB);
  writeln(getCommDiagnosticsHere());
}
writeln(otherB);
