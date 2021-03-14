proc foo(type t, x: range(t, BoundedRangeType.bounded, ?stridable) ... ?k) {
  writeln(x);
}

proc foo(type t, x: t ...?k) {
  writeln(x);
}

foo(int, 1..5, 1..5);
foo(int, 1..5 by 2, 1..5 by 2);
