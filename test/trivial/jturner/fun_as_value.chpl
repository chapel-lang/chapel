proc bob() {
  proc i(j:int) { return j; }

  class call_i {
    proc this(j:int) {
      return i(j);
    }
  }

  var k = new borrowed call_i();
  writeln(k(6));
}
bob();

proc bob2() {
  proc i2(j2:int) { writeln(j2); return;}
  var k2 = i2;
  k2(6);
}
bob2();

proc bob3() {
  proc i3(j3:int) { writeln(j3); return 3;}
  var k3 = i3;
  k3(6);
}
bob3();

proc bob4() {
  proc i4(j4:int) { writeln(j4); }
  var k4 = i4;
  k4(6);
}
bob4();
