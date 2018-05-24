class C {
  proc printType() {
    writeln("C");
    return "C";
  }
}

class D : C {
  proc printType() {
    writeln("D");
    return "D";
  }
}


class E : C {
  var myC: C;
  var myCType = myC.printType();
}

var c = new borrowed C();
var d = new borrowed D();

var e1 = new borrowed E(c);
var e2 = new borrowed E(d);
