function foo(s : seq of string) {
  writeln("foo called on a sequence of strings: ", s);
}

function foo(s : seq of integer) {
  writeln("foo called on a sequence of integers: ", s);
}

foo((/"one", "two", "three"/));
foo((/1, 2, 3/));
