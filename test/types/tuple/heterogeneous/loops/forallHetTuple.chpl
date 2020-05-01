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

var tup = (1, 2.3, "hi", new R());

forall t in tup do
  writeln(t);
