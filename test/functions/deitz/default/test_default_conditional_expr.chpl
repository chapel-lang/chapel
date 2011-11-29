proc f(x, y = if x > 2 then 3 else 4) {
  writeln(x, y);
}

f(1);
f(3);
f(5, 1);
