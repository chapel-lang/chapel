record foo {
  var bar: int;
}

var tup: (int, foo) = noinit;
tup(2).bar = 2;
tup(1) = 5;
writeln(tup);