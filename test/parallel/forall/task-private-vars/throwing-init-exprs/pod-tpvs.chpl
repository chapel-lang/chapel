// The point of the test is to check correct handling of errors
// thrown by task-private variables' initializing expressions, i.e.
// calls to asdf(), throws. 
//
// This test can be executed with the various values of 'nt'
// to force asdf() to throw at each of its calls.
//
// A set of TPVs is created within the main task. They are accessed
// from the forall loop body for the iteration caused by 'yield 1000'.

config const nt = 0;
config const nt2 = 0;

record QQ {
  const jj: int;
}

var cnt: atomic int;
cnt.fetchAdd(1);
if nt2 then writeln("nt ", nt, "   nt2 ", nt2);
else        writeln("nt ", nt);

iter SSS() {
  yield 2000;
  yield 3000;
}
iter RR() { yield 55; }
iter RR(param tag) {
  yield 1000;
  coforall sss in SSS() {
    writeln("starting task");
    yield sss;
    writeln("finished task");
  }
}

proc asdf(adx: int) throws {
  if adx == nt || adx == nt2 {
    writeln("asdf(", adx, "), throwing");
    throw new IllegalArgumentError(adx:string);
  }

  writeln("asdf(", adx, "), returning");
  return new QQ(adx);
}

proc main {
  forall idx in RR()
    with (
      var tp1 = asdf(cnt.fetchAdd(1)),
      var tp2 = asdf(tp1.jj * 10)
          )
  {
    writeln(tp1.jj, tp2.jj, asdf(tp2.jj*100).jj);
  }
}
