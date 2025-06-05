record R1 {
  proc foo() { return 1; }
}
record R2 {
  proc foo() { return 2; }
}

record Wrapper {
  var r1: R1;
  forwarding something;
}

proc Wrapper.something { return r1; }
proc something { return new R2(); } // production: compiler crash unless this is commented out

var x:Wrapper;
writeln(x.foo());
