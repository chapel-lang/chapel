use Time;

config const n = 100;
config const niters = 10000000;

config param printTimes = false;

proc main {
  var t = new Timer();

  {
    var A: [0..#n] int;
    t.start();
    for j in 0..#niters {
      for i in 0..#n {
        A[i] += i;
      }
    }
    t.stop();
    for i in 0..#n do
      if A[i] != niters*i then
        writeln("error: A[",i,"] = ", A[i]);
  }

  if printTimes {
    writeln("array ", t.elapsed());
  }

  t.clear();

  {
    var A = _ddata_allocate(int, n);
    t.start();
    for j in 0..#niters {
      for i in 0..#n {
        A[i] += i;
      }
    }
    t.stop();
    for i in 0..#n do
      if A[i] != niters*i then
        writeln("error: A[",i,"] = ", A[i]);
    _ddata_free(A, n);
  }

  writeln(true);
  if printTimes {
    writeln("ddata ", t.elapsed());
  }
}
