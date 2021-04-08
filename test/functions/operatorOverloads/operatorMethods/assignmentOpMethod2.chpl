record Foo {
  var x: int;

  proc init(x: int) {
    writeln("In user init");
    this.x = x;
  }

  proc init=(other: Foo) {
    writeln("In user init=");
    this.x = other.x;
  }

  proc init=(other: int) {
    writeln("In user init=");
    this.x = other;
  }
}
operator Foo.=(ref lhs: Foo, val: Foo) {
  writeln("In user =");
  lhs.x = val.x;
}

inline operator :(src: int, type toType: Foo) {
  return new Foo(src);
}
proc main() {
  var x: Foo = new Foo(7);
  var y: Foo = 3;
  x = y;
  writeln(x);
}
