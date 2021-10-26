record R {
  var x: int;
  proc init() {
    writeln("default init");
  }
  proc init(arg: int) {
    writeln("init ", arg);
    this.x = arg;
  }
  proc deinit() {
    writeln("deinit");
  }
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

config param paramOption = false;
config const constOption = false;

proc outFn4(out arg: R) {
  if constOption then
    return;
  arg = makeR(1);
}

proc test4() {
  writeln("test4");
  var x: R;
  outFn4(x);
  writeln(x);
}
test4();

proc outFn5(out arg: R) {
  if !constOption then
    return;
  arg = makeR(1);
}

proc test5() {
  writeln("test5");
  var x: R;
  outFn4(x);
  writeln(x);
}
test5();
