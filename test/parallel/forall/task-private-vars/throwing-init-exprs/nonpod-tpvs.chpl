// The point of the test is to check correct invocations of de-initializers
// for tp1, tp2, tp3 - when one of their initializing expressions, i.e.
// calls to asdf(), throws. Ex. if asdf() throws when initializing tp3,
// then - within that task - the deinits for tp1 and tp2, and not for tp3,
// should be invoked. This is GitHub Issue #9749.
//
// This test can be executed with the various values of 'nt'
// to force asdf() to throw at each of its calls.
// Note that currently there is a set of TPVs created within the main task.
// They would be accessible within the body of the forall if the parallel
// iterator RR() contained a 'yield' outside of any task constructs.
//
// As of this writing, the compiler does not implement this properly.
// Instead it reports an error.

config const nt = 0;

record QQ {
  const jj: int;
  proc init(arp:int) {
    jj = arp;
    this.complete();
    writeln("init ", this);
  }
  proc deinit() {
    writeln("deinit ", this);
  }
}

var cnt: atomic int;
cnt.fetchAdd(1);
writeln("nt ", nt);

iter SSS() {
  yield 2000;
  yield 3000;
}

iter RR() { yield 55; }
iter RR(param tag) {
  coforall sss in SSS() {
    writeln("starting task ", sss);
    yield sss;
    writeln("finished task ", sss);
  }
}

proc asdf(adx: int) throws {
  if adx == nt {
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
      var tp2 = asdf(tp1.jj * 10),
      var tp3 = asdf(tp2.jj * 10)
          )
  {
    writeln(tp1, tp2, tp3, asdf(idx));
  }
}
