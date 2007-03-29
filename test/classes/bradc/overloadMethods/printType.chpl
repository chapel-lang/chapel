class C {
  def printType() {
    writeln("C");
    return "C";
  }
}

class D : C {
  def printType() {
    writeln("D");
    return "D";
  }
}


class E : C {
  var myC: C;
  var myCType = myC.printType();
}

var c = C();
var d = D();

var e1 = E(c);
var e2 = E(d);
