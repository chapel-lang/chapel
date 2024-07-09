class C {
  var x: int;
  proc foo() {
    writeln("C: ", this);
  }
}

class D: C {
  var y: int;
  override proc foo() {
    writeln("D: ", this);
  }
}

var ownD = new owned D();
var d: borrowed D = ownD.borrow();

(d:borrowed C).foo();
