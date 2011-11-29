proc foo(x: range(int, BoundedRangeType.boundedLow, ?stridable) ... ?k) {
  writeln("bounded low ranges: ", x);
}

proc foo(x: range(int, BoundedRangeType.boundedHigh, ?stridable) ... ?k) {
  writeln("bounded high ranges: ", x);
}

proc foo(x: range(?) ... ?k) {
  writeln("mixed bounded type ranges: ", x);
}

foo(1.., 1.. by 2);
foo(..4, ..4 by 2);
foo(1.., ..4, 1.. by 2, ..4 by 2);
foo(.., 1.., ..4, 1..4, 1..4 by 2);
