module foo {
  var x = 5;
}

use this.foo;

proc main() {
  writeln(x);
}
