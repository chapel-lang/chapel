record R {
  var x = 42;

  proc init=(other: R) {
    writeln("In copy init");
    this.x = other.x + 1;
  }
}

operator R.=(ref lhs: R, rhs: R) {
  writeln("In assignment");
  lhs.x = rhs.x + 5;
}

const tup = (1, 2.3, "hi", new R());

for t in tup {
  var x: t.type;
  t = x;
}

writeln(tup);
