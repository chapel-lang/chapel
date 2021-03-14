proc test1() {
  writeln("test1");
  extern proc extern_ref_identity(ref arg: int(64)) ref :int(64);
  var x: int(64) = 1;

  ref y = extern_ref_identity(x);
  y = 2;

  writeln(x, " ", y);
}
test1();

proc test2() {
  writeln("test2");
  extern proc extern_const_ref_identity(const ref arg: int(64)) const ref :int(64);
  var x: int(64) = 1;

  const ref y = extern_const_ref_identity(x);
  x = 3;

  writeln(x, " ", y);
}
test2();

proc test3() {
  writeln("test3");
  extern proc extern_ref_identity(ref arg: int(64)) ref :int(64);
  extern "extern_const_ref_identity" proc extern_ref_identity(const ref arg: int(64)) const ref :int(64);
  var x: int(64) = 1;

  ref y = extern_ref_identity(x);
  y = 4;

  writeln(x, " ", y);
}
test3();

proc test4() {
  writeln("test4");
  extern proc extern_ref_identity(ref arg: int(64)) ref :int(64);
  extern "extern_const_ref_identity" proc extern_ref_identity(const ref arg: int(64)) const ref :int(64);
  var x: int(64) = 1;

  const ref y = extern_ref_identity(x);
  x = 5;

  writeln(x, " ", y);
}
test4();
