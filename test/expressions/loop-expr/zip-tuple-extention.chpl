// See also:
//  test/functions/ferguson/ref-pair/iterating-over-arrays.chpl

proc foo(x) {
  for e in zip((...x)) do
    writeln(e);

  var a = for e in zip((...x)) do e;
  writeln(a);
}

foo((1..3,));
foo((1..3, 4..6));
foo((1..3, 4..6, 7..9));
