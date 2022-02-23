record R {
  proc init() { writeln("init"); }
  proc init=(r: R) { writeln("copy init"); }
  operator =(ref lhs:R, rhs:R) { writeln("assign"); }
}

proc myproc() {
  var r1: R;
  return r1;
}

proc myprocTup() {
  var r1, r2: R;
  return (r1, r2);
}

writeln("Non Tuple");
var r1 = myproc();
writeln("\nTuple");
var (r2, r3) = myprocTup();
