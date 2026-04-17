proc foo(x: range(int, boundKind.low, ?stridable) ... ?k) {
  writeln("bounded low ranges: ", x);
}

proc foo(x: range(int, boundKind.high, ?stridable) ... ?k) {
  writeln("bounded high ranges: ", x);
}

proc foo(x: range(?) ... ?k) {
  writeln("mixed bounded type ranges: ", x);
}

// can't call overload 1 because one range is one-strided and the other isn't
foo(1.., 1.. by 2);

// can't call overload 2 because one range is one-strided and the other isn't
foo(..4, ..4 by 2);

// can't call overload 1 or 2 because of mixed boundedness
foo(1.., ..4, 1.. by 2, ..4 by 2);
foo(.., 1.., ..4, 1..4, 1..4 by 2);
