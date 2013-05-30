use Time;

config const n = 100;
config const niters = 10000000;

config param printTimes = false;

proc main {
  var A: [0..#n] int;
  var AA = _ddata_allocate(int, n);
  var t = new Timer();
  t.start();
  for j in 0..#niters {
    for i in 0..#n {
      A[i] += i;
    }
  }
  t.stop();

  if printTimes {
    writeln("array ", t.elapsed());
  }

  t.clear();

  t.start();
  for j in 0..#niters {
    for i in 0..#n {
      AA[i] += i;
    }
  }
  t.stop();
  writeln(AA[n-1] == A[n-1]);
  if printTimes {
    writeln("ddata ", t.elapsed());
  }
}
