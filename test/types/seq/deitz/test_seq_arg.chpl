use List;

def foo(s : list of string) {
  writeln("foo called on a sequence of strings: ", s);
}

def foo(s : list of int) {
  writeln("foo called on a sequence of ints: ", s);
}

foo(makeList("one", "two", "three"));
foo(makeList(1, 2, 3));
