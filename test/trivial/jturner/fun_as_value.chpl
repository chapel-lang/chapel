def bob() {
  def i(j:int) { return j; }

  class call_i {
    def this(j:int) {
      return i(j);
    }
  }

  var k = new call_i();
  writeln(k(6));
}
bob();

def bob2() {
  def i2(j2:int) { writeln(j2); return;}
  var k2 = i2;
  k2(6);
}
bob2();

def bob3() {
  def i3(j3:int) { writeln(j3); return 3;}
  var k3 = i3;
  k3(6);
}
bob3();

def bob4() {
  def i4(j4:int) { writeln(j4); }
  var k4 = i4;
  k4(6);
}
bob4();
