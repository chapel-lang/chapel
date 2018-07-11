enum color {red=7, orange=6, yellow=5, green=4, blue=3, indigo=2, violet=1};

// test non-param overloads
proc test(c1: color, c2: color) {
  writeln(c1, " <  ", c2, ": ", c1 < c2);
  writeln(c1, " <= ", c2, ": ", c1 <= c2);
  writeln(c1, " >= ", c2, ": ", c1 >= c2);
  writeln(c1, " >  ", c2, ": ", c1 > c2);
  writeln(c1, " == ", c2, ": ", c1 == c2);
  writeln(c1, " != ", c2, ": ", c1 != c2);
  writeln();
}

// test param overloads
proc test(param c1: color, param c2: color, param relation: int) {
  if (c1 < c2 && relation != -1) then
     compilerError("Shouldn't get here (A)");
  if (c1 <= c2 && relation == 1) then
     compilerError("Shouldn't get here (B)");
  if (c1 >= c2 && relation == -1) then
     compilerError("Shouldn't get here (C)");
  if (c1 > c2 && relation != 1) then
     compilerError("Shouldn't get here (D)");
  if (c1 == c2 && relation != 0) then
     compilerError("Shouldn't get here (E)");
  if (c1 != c2 && relation == 0) then
     compilerError("Shouldn't get here (F)");
  writeln("Param tests pass");
  test(c1, c2);
}

test(color.yellow, color.yellow, 0);
test(color.yellow, color.green, -1);
test(color.yellow, color.red, 1);
