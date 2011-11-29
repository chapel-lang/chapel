use List;

proc foo(s : list(string)) {
  writeln("foo called on a sequence of strings: ", s);
}

proc foo(s : list(int)) {
  writeln("foo called on a sequence of ints: ", s);
}

foo(makeList("one", "two", "three"));
foo(makeList(1, 2, 3));
