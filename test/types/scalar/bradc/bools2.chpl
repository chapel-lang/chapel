var b  : bool     = true;

proc testBoolCombo(x, y) {
  writeln("!x = ", !x);
  writeln("x && y = ", x && y);
  writeln("x || y = ", x || y);
  writeln("x ^ y = ", x ^ y);
  writeln("x | y = ", x | y);
  writeln("x & y = ", x & y);
  writeln("x + y = ", x + y);
  writeln("numBits(x + y) = ", numBits((x+y).type));
  if (x) then
    writeln("x is true");
  else
    writeln("x is false");
}

testBoolCombo(b,b);
