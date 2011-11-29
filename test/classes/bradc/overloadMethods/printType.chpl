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

var c = new C();
var d = new D();

var e1 = new E(c);
var e2 = new E(d);

delete c;
delete d;
delete e1;
delete e2;
