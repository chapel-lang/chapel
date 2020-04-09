use LinkedLists;

proc foo(i: int) {
  writeln(i);
}

proc foo(s) {
  writeln(s);
}

var s: LinkedList(int) = makeList(1, 2, 3, 4);

foo(s);

s.destroy();
