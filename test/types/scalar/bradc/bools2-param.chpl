param b  : bool     = true;
param b8 : bool(8)  = true;
param b16: bool(16) = true;
param b32: bool(32) = true;
param b64: bool(64) = true;

proc testBoolCombo(param x, param y) {
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
  writeln();
}

testBoolCombo(b,b);
testBoolCombo(b,b8);
testBoolCombo(b,b16);
testBoolCombo(b,b32);
testBoolCombo(b,b64);

testBoolCombo(b8,b);
testBoolCombo(b8,b8);
testBoolCombo(b8,b16);
testBoolCombo(b8,b32);
testBoolCombo(b8,b64);

testBoolCombo(b16,b);
testBoolCombo(b16,b8);
testBoolCombo(b16,b16);
testBoolCombo(b16,b32);
testBoolCombo(b16,b64);

testBoolCombo(b32,b);
testBoolCombo(b32,b8);
testBoolCombo(b32,b16);
testBoolCombo(b32,b32);
testBoolCombo(b32,b64);

testBoolCombo(b64,b);
testBoolCombo(b64,b8);
testBoolCombo(b64,b16);
testBoolCombo(b64,b32);
testBoolCombo(b64,b64);

