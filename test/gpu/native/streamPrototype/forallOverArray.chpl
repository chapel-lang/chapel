use GpuDiagnostics;

proc verifyLaunches() {
  use ChplConfig;
  param expected = if CHPL_GPU_MEM_STRATEGY == "unified_memory" then 4 else 5;
  const actual = getGpuDiagnostics()[0].kernel_launch;
  assert(actual == expected,
         "observed ", actual, " launches instead of ", expected);
}


config const start = 1;
config const end = 10;

startGpuDiagnostics();
on here.gpus[0] {
  var a: [start..end] int;
  var value = 20;

  // one array
  forall  elem in a { elem += 10;          } writeln(a);
  forall  elem in a { elem += value;       } writeln(a);
  foreach elem in a { elem += 10;          } writeln(a);
  foreach elem in a { elem += value;       } writeln(a);
}
stopGpuDiagnostics();
verifyLaunches();
