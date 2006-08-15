def foo(type t) {
  writeln("foo(type t) called");
  var x : t;
  writeln("  ", x);
}

def foo(x : int) {
  writeln("foo(int) called");
  writeln("  ", x);
}

foo(2);
foo(3.0);
foo(int);
