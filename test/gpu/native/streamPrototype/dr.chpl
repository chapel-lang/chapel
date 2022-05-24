use GPUDiagnostics;

config const n = 10;

startGPUDiagnostics();
on here.gpus[0] {
  var a, b: [0..n] int;
  var value = 20;

  // one array
  forall  i in 0..n { a[i] += i+10;       } writeln(a);
  forall  i in 0..n { a[i] += i+value;    } writeln(a);
  foreach i in 0..n { a[i] += i+10;       } writeln(a);
  foreach i in 0..n { a[i] += i+value;    } writeln(a);

  // two arrays
  forall  i in 0..n { b[i] += a[i]*10;    } writeln(b);
  forall  i in 0..n { b[i] += a[i]*value; } writeln(b);
  foreach i in 0..n { b[i] += a[i]*10;    } writeln(b);
  foreach i in 0..n { b[i] += a[i]*value; } writeln(b);
}
stopGPUDiagnostics();
writeln(getGPUDiagnostics());
