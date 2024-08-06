record r1 {
  var x: int;
}

record r2 {
  var x: int;

  proc init() {
    writeln("Default-initializing r2");
    this.x = 0;
  }

  proc init=(other: r1) {
    writeln("Calling init= from r1 to r2");
    this.x = other.x;
  }
}

operator :(ref lhs: r1, type rhs: r2) {
  writeln("Calling : from r1 to r2");
  return new rhs(lhs.x);
}

on Locales.last var x: r2 = new r1(42);
writeln(x);
writeln(x.locale == Locales.last);
