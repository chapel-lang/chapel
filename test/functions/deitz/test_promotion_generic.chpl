def foo(i: int) {
  writeln(i);
}

def foo(s) {
  writeln(s);
}

var s: seq of int = (/1, 2, 3, 4/);

foo(s);
