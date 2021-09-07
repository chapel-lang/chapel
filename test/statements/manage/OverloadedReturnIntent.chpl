//
// Use this as a benchmark as for how overloaded return intents should
// be resolved (context manager resources should roughly match this).
//

use TrackingRecord;

var gR = new r();

proc makeR(): r {
  writeln('proc makeR(): r');
  return new r();
}

proc makeR() ref: r {
  writeln('proc makeR() ref: r');
  return gR;
}

proc makeR() const ref: r {
  writeln('proc makeR() const ref: r');
  return gR;
}

proc test1() {
  writeln('T1: store in var');
  var x = makeR();
}

proc test2() {
  writeln('T2: store in ref');
  ref x = makeR();
}

proc test3() {
  writeln('T3: store in const ref');
  const ref x = makeR();
}

proc test4() {
  writeln('T4: store in const');
  const x = makeR();
}

proc test5() {
  writeln('T5: call without assignment');
  makeR();
}

proc main() {
  writeln();
  for test in [test1, test2, test3, test4, test5] {
    test();
    writeln();
  }
}

