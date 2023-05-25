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

var c = (new owned C()).borrow();

c.foo() = 2;
writeln(c.foo());
