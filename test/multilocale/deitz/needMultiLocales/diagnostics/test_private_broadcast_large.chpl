use CommDiagnostics;

proc f(param p: int) {
  var x: p*int;
  for param i in 0..p-1 do
    x(i) = i+1;
  return x;
}

const x = f(256);

startVerboseComm();

extern proc printf(fmt: c_string, x...);

on Locales(1) {
  for param i in 0..255 do
    printf("%s\n", (x(i):string).c_str());
}

stopVerboseComm();
