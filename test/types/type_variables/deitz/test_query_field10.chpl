def foo(type t, x: range(t, bounded, ?stridable) ... ?k) {
  writeln(x);
}

def foo(type t, x: t ...?k) {
  writeln(x);
}

foo(int, 1..5, 1..5);
foo(int, 1..5 by 2, 1..5);
