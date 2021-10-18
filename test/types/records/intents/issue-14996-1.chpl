record R {
  var x: int;
  proc init=(other: R) {
    writeln("copy init");
  }
}
operator =(ref lhs: R, rhs: R) {
  writeln("assign");
}
proc makeR(arg:int) {
  return new R(arg);
}

proc outFn(out arg: R) {
  arg = makeR(1);
}

config param paramOption = false;
config const constOption = false;

proc test1() {
  writeln("test1");
  var x: R;
  if paramOption {
    writeln(x);
  }
  outFn(x);
}
test1();

proc test2() {
  writeln("test2");
  var x: R;
  if constOption {
    writeln(x);
  }
  outFn(x);
}
test2();
