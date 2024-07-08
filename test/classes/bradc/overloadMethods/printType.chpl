class C {
  proc printType() {
    writeln("C");
    return "C";
  }
}

class D : C {
  override proc printType() {
    writeln("D");
    return "D";
  }
}


class E : C {
  var myC: borrowed C;
  var myCType = myC.printType();
}

var cOwn = new owned C();
var c = cOwn.borrow();
var dOwn = new owned D();
var d = dOwn.borrow();

var e1Own = new owned E(c);
var e1 = e1Own.borrow();
var e2Own = new owned E(d);
var e2 = e2Own.borrow();
