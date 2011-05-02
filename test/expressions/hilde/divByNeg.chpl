// Test that division by a negative number is well-defined.

// Move through the four quadrants.

var a = 11;
var b = 3;
writeDivMod(a, b);

a = -11;
writeDivMod(a, b);

b = -3;
writeDivMod(a, b);

a = 11;
writeDivMod(a, b);


proc writeDivMod(a, b)
{
  var c = a / b;
  var d = a % b;

  if c * b + d != a then
    writeln("div and mod not consistent.");

  writeln(a, " div ", b, " = ", c);
  writeln(a, " mod ", b, " = ", d);
}

