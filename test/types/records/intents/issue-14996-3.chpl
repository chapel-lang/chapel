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

proc outFn1(out arg: R) {
  if paramOption then
    return;
  arg = makeR(1);
}

proc test1() {
  writeln("test1");
  var x: R;
  outFn1(x);
  writeln(x);
}
test1();

proc outFn2(out arg: R) {
  if !paramOption then
    return;
  arg = makeR(1);
}

proc test2() {
  writeln("test2");
  var x: R;
  outFn2(x);
  writeln(x);
}
test2();

proc outFn3(out arg: R) {
  return;
  arg = makeR(1);
}

proc test3() {
  writeln("test3");
  var x: R;
  outFn3(x);
  writeln(x);
}
test3();
