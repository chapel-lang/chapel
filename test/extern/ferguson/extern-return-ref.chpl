proc test1() {
  writeln("test1");
  extern proc extern_ref_identity(ref arg: int(64)) ref :int(64);
  var x: int(64) = 1;

  ref y = extern_ref_identity(x);
  y = 2;

  writeln(x, " ", y);
}
test1();
