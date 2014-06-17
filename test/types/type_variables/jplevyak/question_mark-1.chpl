
proc f(a : ?t) {
  var y : t = a * 2;
  y = y + 3;
  writeln(y);
}

f(0);
f(1.0);
