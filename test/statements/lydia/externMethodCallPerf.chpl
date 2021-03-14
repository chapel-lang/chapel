use Time, externMethod;

config const numIters = 10000;
config const printStuff = false;

extern proc blah2(x:int): int;

proc blah1(num) {
  var res = num % 17;
  res = res + 4;
  res = res * 3;
  res = res - 7;
  return res/8;
}

proc main() {
  var t1, t2, t3: Timer;
  var res1, res2, res3: int;
  
  t1.start();
  for i in 1..numIters {
    res1 += blah1(i);
  }
  t1.stop();
  
  t2.start();
  for i in 1..numIters {
    res2 += blah2(i);
  }
  t2.stop();

  t3.start();
  for i in 1..numIters {
    res3 += blah3(i);
  }
  t3.stop();
  
  if (res1 != res2 || res1 != res3) {
    writeln("res1 ", res1, " res2 ", res2, " res3 ", res3);
  } else {
    writeln("Validation successful");
  }
  
  if (printStuff) {
    writeln("Chapel method call took ", t1.elapsed(TimeUnits.milliseconds)/1000,
            " seconds");
    writeln("Chapel extern C method call took ",
            t2.elapsed(TimeUnits.milliseconds)/1000, " seconds");
    writeln("Chapel extern method call took ",
            t3.elapsed(TimeUnits.milliseconds)/1000, " seconds");
  }
}
