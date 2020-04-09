config const iters = 100000;
config param n = 10;

config param printTimes = false;

proc main  {
  use Time;
  var AAA: [1..n] real;
  var TTT: n*real;
  var Atimer: Timer();
  var Ttimer: Timer();
  Atimer.start(); 
  for i in 1..iters {
    for j in 1..n {
      AAA(j) += i - 1.0;
    }
  }
  Atimer.stop();

  Ttimer.start();
  for i in 1..iters {
    for j in 0..#n {
      TTT(j) += i - 1.0;
    }
  }
  Ttimer.stop();

  writeln((AAA(1), AAA(n)), (TTT(0), TTT(n-1)));

  if printTimes {
    writeln("Array time: ", Atimer.elapsed());
    writeln("Tuple time: ", Ttimer.elapsed());
    writeln("A/T: ", Atimer.elapsed() / Ttimer.elapsed());
  }
}
