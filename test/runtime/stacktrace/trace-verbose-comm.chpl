use CommDiagnostics;
var a = 0;

startVerboseComm();
on Locales[1] {
  callSetA();
}
stopVerboseComm();
writeln(a);

proc callSetA() {
  setA();
}
proc setA() {
  a = 1;
}
