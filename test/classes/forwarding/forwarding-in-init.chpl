record R1 {
  proc foo() { return 1; }
}
record Wrapper {
  var r1: R1;
  forwarding something;
}

proc Wrapper.something { return r1; }

proc Wrapper.init() {
  foo(); // production: error as I would expect
  this.r1 = new R1();
}

var x:Wrapper;
writeln(x.foo());
