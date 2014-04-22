use Time;

config const numIterations = 1000000;
config const numTrials = 10000;
config const verbose = false;

extern proc c_trial(): int;

proc main() {
  var t1, t2: Timer;
  var res1, res2 = 0;
  t1.start();
  for i in 1..#numTrials {
    for j in 1..#numIterations {
      if j % 2 == 0 then
        res1 = res1 + 3;
      else
        res1 = res1 / 2;
    }
  }
  t1.stop();
  t2.start();
  var i = 0;
  while (i < numTrials) {
    var j = 0;
    while (j < numIterations) {
      if j % 2 == 1 then
        res2 = res2 + 3;
      else
        res2 = res2 / 2;
      j += 1;
    }
    i += 1;
  }
  t2.stop();
  if (res1 != res2) {
    writeln("These results should have matched, got ", res1, " and ", res2);
  } else {
    writeln("Chapel verification successful");
  }
  
  if verbose {
    writeln("For loop underwent ", numIterations, " iterations ", numTrials,
            " times in ", t1.elapsed(TimeUnits.milliseconds)/1000, " seconds");
    writeln("While loop underwent ", numIterations, " iterations ", numTrials,
            " times in ", t2.elapsed(TimeUnits.milliseconds)/1000, " seconds");
    var res3 = c_trial();
    if (res1 != res3) {
      writeln("Chapel results did not match C results");
    }
  }
}
