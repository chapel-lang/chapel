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

var aOwn = new owned A();
var a  = aOwn.borrow();
var bOwn = new owned B();
var b  = bOwn.borrow();
var ba = b:borrowed A;

writeln("a   says: ", a.fcn());
writeln("b   says: ", b.fcn());
writeln("b:A says: ", (b:borrowed A).fcn());
writeln("ba  says: ", ba.fcn());
