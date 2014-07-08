// Ensure sync variables are passable by reference.

var aaa: sync int; // -> empty

proc test1(ref bbb) {
  bbb = 1; // -> full
}

proc test2(ref ccc: sync int) {
  writeln(ccc); // -> empty
}

proc test2(ref ddd: single int) {
  writeln(ddd); // -> empty
}

test1(aaa); // -> full
test2(aaa); // -> empty

var eee: single int;
test1(eee);
test2(eee);
