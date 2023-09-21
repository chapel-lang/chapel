class C {
  var x: int;
  proc foo() ref {
    writeln(true);
    return x;
  }
  proc foo() {
    writeln(false);
    return x;
  }

}

var ownC = new owned C();
var c = ownC.borrow();

c.foo() = 2;
writeln(c.foo());
