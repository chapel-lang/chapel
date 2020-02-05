// Stress some floating point operations that we know have tricky
// implementations on some platforms/configurations.

config const numIters = 10_000;

proc testAddSub(type t) {
  var a: atomic t;
  const numTasks = here.maxTaskPar;

  coforall loc in Locales do on loc do
    coforall 1..numTasks do
      for 1..numIters do
        a.add(1:t);
  assert(isclose(a.read(), (numTasks*numLocales*numIters):t));

  coforall loc in Locales do on loc do
    coforall 1..numTasks do
      for 1..numIters do
        a.sub(1:t);
  assert(isclose(a.read(), 0:t));
}

testAddSub(real(32));
testAddSub(real(64));
