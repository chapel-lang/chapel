use Time;
// Different access: multiplication/division, with similar < > comparisons

config const numIters = 100000;
config const numTrials = 100;
config const verbose = false;

var res1 = 0;
var t1: Timer;

t1.start();
for i in 1..#numTrials {
  for j in 1..#numIters {
    if (j < i) {
      res1 += j * 2;
    } else {
      res1 -= j / 2;
    }
  }
}
t1.stop();
method();

proc method() {
  var res2 = 0;
  var t2: Timer;
  t2.start();
  for i in 1..#numTrials {
    for j in 1..#numIters {
      if (j < i) {
        res2 += j * 2;
      } else {
        res2 -= j / 2;
      }
    }
  }
  t2.stop();
  if (res1 != res2) {
    writeln("Error, res1 did not match res2!");
  } else {
    writeln("Success!");
  }
  if verbose {
    writeln ("Module level access took ",
             t1.elapsed(TimeUnits.milliseconds)/1000);
    writeln ("Method level access took ",
             t2.elapsed(TimeUnits.milliseconds)/1000);
  }
}

