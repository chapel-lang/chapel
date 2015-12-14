var A = (new R(), new R(), new R());

for a in zip((...A)) {
  writeln("a is: ", a);
}

//
// This record intentionally supports no iterators
//
record R {
  var x: int;
}
