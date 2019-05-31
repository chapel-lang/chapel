use Time;

config const n = 1000,
       printTiming = false;

var A1: [1..n*n] real;
var A2: [1..n, 1..n] real;

proc runTrial(dim, A) {
  var timer: Timer;
  timer.start();
  A += 1.0;
  timer.stop();
  if printTiming then
    writeln(dim, "D op= time: ", timer.elapsed());
  writeln("Verification ", dim, ": ", ((+ reduce A) == (n*n): real));
}

runTrial("1", A1);
runTrial("2", A2);
