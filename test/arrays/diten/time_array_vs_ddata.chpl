use Time;

config const n = 100;
config const niters = 10_000_000;

config param printTimes = true;

config const validate = false;

proc main {
  var t = new Timer();

  {
    var A: [0..#n] int;
    t.start();
    for j in 0..#niters {
      for (a,i) in zip(A, 0..#n) {
        a += i;
      }
    }
    t.stop();

    if validate {
      for i in 0..#n do
        if A[i] != niters*i then
          writeln("error: A[",i,"] = ", A[i]);
    }
  }

  if printTimes {
    writeln("array ", t.elapsed());
  }

  t.clear();

}
