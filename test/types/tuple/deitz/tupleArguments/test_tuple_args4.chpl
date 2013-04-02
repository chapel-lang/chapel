proc f((x, y): (int, int) = (3, 4)) {
  writeln(x + y);
}

var t = (1, 2);
f(t);
f();
