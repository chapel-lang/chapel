proc h(in arg: bool) {
  writeln("h arg=", arg);
  arg = true;
}

proc test1() {
  writeln("test1");
  var x: bool(8) = false;
  h(x);
  writeln(x);
}
test1();

proc i(inout arg: bool) {
  writeln("i arg=", arg);
  arg = true;
}

proc test2() {
  writeln("test2");
  var x: bool(8) = false;
  i(x); // works because bool(8) can be converted to bool, and then
        // x: bool(8) can be assigned to a bool
  writeln(x);
}
test2();
