use CommDiagnostics;

var a = 0;

startVerboseComm();
on Locales[1] {
  for i in 0..#5 do
    a += 1;
}
stopVerboseComm();

writeln(a);
