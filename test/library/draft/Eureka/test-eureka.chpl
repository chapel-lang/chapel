// todo: set it up so that one of 'e.check()' throws reliably,
//       to illustrate this aspect of the eureka feature


use Eureka;

config const n = 8;
config const p = 3;

proc testForall() throws {
  var e: eureka;
  forall idx in 1..n {
    e.check();
    if idx == p then
      e.notify();
  }
}

proc testCoforall() throws {
  var e: eureka;
  coforall idx in 1..n with (in e) {
    e.check();
    if idx == p then
      e.notify();
  }
}

proc main {
  try { testForall(); }
  catch errForall { verifyEureka("testForall", errForall); }

  try { testCoforall(); }
  catch errCoforall { verifyEureka("testCoforall", errCoforall); }
}

proc verifyEureka(msg, taskErr) {
  for err in taskErr: borrowed TaskErrors do
    if err: EurekaNotified? then
      writeln(msg, ": got EurekaNotified");
}
