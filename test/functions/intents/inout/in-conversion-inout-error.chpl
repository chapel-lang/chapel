proc h(in arg: int) {
  writeln("h arg=", arg);
  arg = 22;
}

proc test1() {
  writeln("test1");
  var x: int(8) = 1;
  h(x);
  writeln(x);
}
test1();

proc i(inout arg: int) {
  writeln("i arg=", arg);
  arg = 23;
}

proc test2() {
  writeln("test2");
  var x: int(8) = 2;
  i(x);
  writeln(x);
}
test2();
