use MemDiagnostics;
use driver;

writeln("start");

for l in Locales do on l {
  writeln("testing on ", here);
  const m1 = memoryUsed();
  testit(false);
  const m2 = memoryUsed();
  writeln("memory delta ", m2 - m1);
  testit(true);
}

writeln("done");

compilerWarning("Dist1D is a ", Dist1D.type:string);

proc testit(printit:bool) {
  var Dom1D: domain(1) dmapped Dist1D;
  var a: [Dom1D] int;
  Dom1D = Space1;
  ref b = a;
  ref c = a;
  ref cc = c;
  ref bb = b;
  ref bbb = bb;
  ref ccc = cc;
  Dom1D = {1..max(n1/2,10)};
  ref e = a;
  ref ee = e;
  ref eee = ee;

  ccc[3] = 333;
  a[8] = 888;

  if printit then
    for (i, v) in zip(Dom1D, ccc) do
      if bbb[i] != 0 then
        writeln((i, v, a[i], b[i], cc[i], eee[i]));
}
