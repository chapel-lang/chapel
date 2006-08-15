def foo(s : seq of string) {
  writeln("foo called on a sequence of strings: ", s);
}

def foo(s : seq of int) {
  writeln("foo called on a sequence of ints: ", s);
}

foo((/"one", "two", "three"/));
foo((/1, 2, 3/));
