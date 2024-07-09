class C {
  var i : int;
  proc foo() ref {
    return i;
  }
}

var ownC = new owned C();
var c : borrowed C = ownC.borrow();
c.i = 2;
writeln(c);
writeln(c.foo());
c.foo() = 3;
writeln(c);
