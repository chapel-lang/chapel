class A {
  def fcn() {
    return "In A";
  }
}

class B: A {
  def fcn() {
    return "In B";
  }
}

var a  = A();
var b  = B();
var ba = b:A;

writeln("a   says: ", a.fcn());
writeln("b   says: ", b.fcn());
writeln("b:A says: ", (b:A).fcn());
writeln("ba  says: ", ba.fcn());
