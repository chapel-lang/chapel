proc assign(A, n: int(32)) {
  for i in 1..n {
    A(i) = i;
  }
}


config const arrsize: int(32) = 100000000;
config const ntrials = 100;

extern proc c_trial();

config param compareTimes = false;

proc main {
  use Time;
  var A: [1..arrsize] int(32);
  var t: Timer;
  t.start();
  for i in 1..ntrials {
    assign(A, arrsize);
  }
  t.stop();

  if compareTimes {
    writeln("chpl wrote ", arrsize, " elements ", ntrials, " times in ",
            t.elapsed(TimeUnits.milliseconds)/1000, " seconds");
    c_trial();
  }
}
