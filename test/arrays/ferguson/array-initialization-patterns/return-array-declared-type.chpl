// this test is intended as a reproducer for the
// problem of too much memory allocated
// shown in issue #25741

config const n = 100_000;

proc myfn(nn: int) : [0..<nn] int {
  var ret: [0..<nn] int;
  return ret;
}

proc main() {
  writeln("alloc int test begins");
  var x = myfn(n);
  writeln("alloc int test ends");
}
