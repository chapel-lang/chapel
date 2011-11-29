proc f((x, y) = (3, 4)) {
  writeln(x + y);
}

var t = tuple(1, 2);
f(t);
f();
