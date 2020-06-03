class C {
  var xx: int = 0;
}

proc out5(out arg: owned C) {
  arg = new owned C();
}

proc test6() {
  writeln("test6");
  var c: C;
  out5(c);
}
test6();
