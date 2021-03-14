record R {
  var x = 42;

  proc init=(other: R) {
    writeln("In copy init");
    this.x = other.x + 1;
  }
}

proc =(ref lhs: R, rhs: R) {
  writeln("In assignment");
  lhs.x = rhs.x + 5;
}

proc foo(args...) {
  for a in args do
    writeln(a);
}

var tup = (1, 2.3, "hi", new R());

foo(1, 2.3, "hi", new R());
