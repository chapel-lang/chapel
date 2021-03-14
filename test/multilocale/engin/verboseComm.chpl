use CommDiagnostics;
var a = 0;

startVerboseComm();
on Locales[1] {
  a = 1;
}
stopVerboseComm();
writeln(a);
