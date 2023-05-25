use GpuDiagnostics;

proc verifyLaunches() {
  use ChplConfig;
  param expected = if CHPL_GPU_MEM_STRATEGY == "unified_memory" then 8 else 10;
  const actual = getGpuDiagnostics()[0].kernel_launch;
  assert(actual == expected,
         "observed ", actual, " launches instead of ", expected);
}


config const start = 1;
config const end = 10;

startGpuDiagnostics();
on here.gpus[0] {
  var a, b: [start..end] int;
  var value = 20;

  // one array
  forall  i in a.domain { a[i] += i+10;       } writeln(a);
  forall  i in a.domain { a[i] += i+value;    } writeln(a);
  foreach i in a.domain { a[i] += i+10;       } writeln(a);
  foreach i in a.domain { a[i] += i+value;    } writeln(a);

  // two arrays
  forall  i in a.domain { b[i] += a[i]*10;    } writeln(b);
  forall  i in a.domain { b[i] += a[i]*value; } writeln(b);
  foreach i in a.domain { b[i] += a[i]*10;    } writeln(b);
  foreach i in a.domain { b[i] += a[i]*value; } writeln(b);
}
stopGpuDiagnostics();
verifyLaunches();
