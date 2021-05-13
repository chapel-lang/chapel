/*
This tests the situation where a reduce intent and/or in intent
occurs in a forall loop where the parallel iterator itself
contains a parallel for loop.

This test is a copy of reduce-with-forall-in-par-iter.chpl
except for using a for-loop instead of a forall-loop
in pariter1 and pariter2.
*/

config const nn = 3;
config const mm = 3;

//
// myiter() iterator ensures deterministic execution (using cnt$)
// while still creating bona fide parallel tasks
//

iter myiter() {
  yield 555;
}

iter myiter(param tag: iterKind) where tag == iterKind.standalone {
  var cnt$: sync int = 1;
  coforall ooo in 1..nn {
    const current = cnt$.readFE();
    writef("myiter start %t\n", current);
    for jjj in 1..mm {
      yield current * 100 + jjj;
    }
    writef("myiter done  %t\n", current);
    cnt$.writeEF(current + 1);
  }
}

//
// pariter1 and pariter2 are the same except 1 vs. 2 yields
// test1 and test2 are the same except testing pariter1 vs. pariter2
//

iter pariter1() {
  yield 333;
}

iter pariter1(param tag: iterKind) {
  writeln("starting pariter1");
  for kkk in myiter(iterKind.standalone) {
    yield kkk;
  }
  writeln("finishing pariter1");
}

iter pariter2() {
  yield 333;
}

iter pariter2(param tag: iterKind) {
  writeln("starting pariter2");
  for kkk in myiter(iterKind.standalone) {
    yield kkk;
    yield 10000 + kkk;
  }
  writeln("finishing pariter2");
}

proc test1 {
  var result1 = 10000, result2 = 7;
  forall iii in pariter1() with (in result1, + reduce result2) {
    const before1 = result1;
    const before2 = result2;
    result1 += iii;
    result2 += iii;
    writef("loop %5i    %5i -> %5i    %5i -> %5i\n",
           iii, before1, result1, before2, result2);
  }
  writeln("result1 = ", result1);
  writeln("result2 = ", result2);
}

proc test2 {
  var result1 = 20000, result2 = 9;
  forall iii in pariter2() with (in result1, + reduce result2) {
    const before1 = result1;
    const before2 = result2;
    result1 += iii;
    result2 += iii;
    writef("loop %5i    %5i -> %5i    %5i -> %5i\n",
           iii, before1, result1, before2, result2);
  }
  writeln("result1 = ", result1);
  writeln("result2 = ", result2);
}

proc main {
  test1;
  test2;
  writeln(+ reduce pariter1());
//todo:  writeln(+ reduce pariter2());
}
