class A {
  proc fcn() {
    return "In A";
  }
}

class B: A {
  proc fcn() {
    return "In B";
  }
}

var a  = new A();
var b  = new B();
var ba = b:A;

writeln("a   says: ", a.fcn());
writeln("b   says: ", b.fcn());
writeln("b:A says: ", (b:A).fcn());
writeln("ba  says: ", ba.fcn());
