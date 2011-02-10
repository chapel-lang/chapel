proc foo(x, y) {
  writeln(x);
  writeln(y);
}

var i : int = 2;
var s : string = "sugar";

foo(i, i);
foo(i, s);
foo(s, i);
foo(s, s);
