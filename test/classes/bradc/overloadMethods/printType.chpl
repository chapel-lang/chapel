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

var c = (new owned C()).borrow();
var d = (new owned D()).borrow();

var e1 = (new owned E(c)).borrow();
var e2 = (new owned E(d)).borrow();
