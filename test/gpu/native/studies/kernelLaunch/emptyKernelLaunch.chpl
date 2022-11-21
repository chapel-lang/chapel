use GPUDiagnostics;
use Time;

config const timeLimit = 5; //seconds
config const validate = false;
config const report = false;

config const granularity = 1000;
config const n = 1;

var t = new stopwatch();

if validate then startGPUDiagnostics();
var numLaunches = 0;

on here.gpus[0] {
  t.start();
  do {
    for useless in 1..granularity {
      foreach i in 1..n { }
    }
    numLaunches += granularity;
  } while t.elapsed() < timeLimit;
  t.stop();
}

if validate {
  stopGPUDiagnostics();
  assert(getGPUDiagnostics()[0].kernel_launch == numLaunches);
}

if report {
  const perLaunchTime = t.elapsed()/numLaunches;
  writeln("Total test time (s): ", t.elapsed());
  writeln("Number of launches: ", numLaunches);
  writeln("Average kernel launch time (ms): ", perLaunchTime*1_000);
}
