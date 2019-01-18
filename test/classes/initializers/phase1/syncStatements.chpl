// Tests having field initialization within a sync statement.  Sync statements
// themselves do not introduce parallelism or loops, so it should be no problem
// to have a field initialization within them.
class Foo {
  var x: int;

  proc init(xVal) {
    sync {
      x = xVal;
    }
  }
}

var foo = new Foo(13);
writeln(foo);
delete foo;
