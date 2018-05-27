enum color {red, orange, yellow, green, blue, indigo, violet};

proc test(c1: color, c2: color) {
  writeln(c1, " <  ", c2, ": ", c1 < c2);
  writeln(c1, " <= ", c2, ": ", c1 <= c2);
  writeln(c1, " >= ", c2, ": ", c1 >= c2);
  writeln(c1, " >  ", c2, ": ", c1 > c2);
  writeln(c1, " == ", c2, ": ", c1 == c2);
  writeln(c1, " != ", c2, ": ", c1 != c2);
  writeln();
}

test(color.yellow, color.yellow);
test(color.yellow, color.green);
test(color.yellow, color.red);
