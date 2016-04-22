use List;

proc foo(s : list(string)) {
  writeln("foo called on a sequence of strings: ", s);
}

proc foo(s : list(int)) {
  writeln("foo called on a sequence of ints: ", s);
}

var l1 = makeList("one", "two", "three");
var l2 = makeList(1, 2, 3);

foo(l1);
foo(l2);

l2.destroy();
l1.destroy();

