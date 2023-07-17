class C {
  proc foo(i : int, f : real) {
    writeln(i);
    writeln(f);
  }
}

var cOwn = new owned C();
var c = cOwn.borrow();
var i = 2;
var j = 3;
c.foo(i, j);
