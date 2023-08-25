use GpuDiagnostics;
use Time;

config const timeLimit = 5; //seconds
config const validate = false;
config const report = false;

config const N = 30_000;
config param M = 10;
config const P = 100;

// ----------------------------------------------------------------------------

inline proc trial(ref A) {
  for i in 0..N {
    for param j in 0..<M {
      foreach k in 0..<P { A[k] = i; }
    }
  }
}

var t = new stopwatch();

if validate then startGpuDiagnostics();
var numTrials = 0;

on here.gpus[0] {
  var A : [0..<M] int;

  t.start();
  do {
    trial(A);
    numTrials += 1;
  } while t.elapsed() < timeLimit;
  t.stop();
}

if validate {
  stopGpuDiagnostics();
  assert(getGpuDiagnostics()[0].kernel_launch > 0);
}

if report {
  const perTrialTime = t.elapsed()/numTrials;
  writeln("Total test time (s): ", t.elapsed());
  writeln("Number of trials: ", numTrials);
  writeln("Average time per trial (ms): ", perTrialTime*1_000);
}
