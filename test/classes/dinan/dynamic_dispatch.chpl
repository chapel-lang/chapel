class A {
  proc fcn() {
    return "In A";
  }
}

class B: A {
  override proc fcn() {
    return "In B";
  }
}

var a  = new borrowed A();
var b  = new borrowed B();
var ba = b:borrowed A;

writeln("a   says: ", a.fcn());
writeln("b   says: ", b.fcn());
writeln("b:A says: ", (b:borrowed A).fcn());
writeln("ba  says: ", ba.fcn());
