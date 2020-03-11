class C {
  var xx: int = 0;
}

proc out1(out arg: owned C) {
  arg = new owned C();
}

proc test1() {
  writeln("test1");
  var c: owned C;
  writeln(c);
  out1(c);
}
test1();
