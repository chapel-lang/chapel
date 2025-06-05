record R {
  proc foo() { return 1; }
}

record Wrapper {
  var r: R;
  forwarding something();
}

proc Wrapper.something() { return r; }

var x:Wrapper;
writeln(x.foo());
