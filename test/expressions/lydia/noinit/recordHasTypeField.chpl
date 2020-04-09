record foo {
  type t;
  var i: t;

  proc init(type t) {
    writeln("I default initialized!");
    this.t = t;
    this.i = 3;
  }
}

var bar: foo(int) = noinit; // Should not print message
bar.i = 7;
writeln(bar);
var baz: foo(int);          // Should print message
writeln(baz);
