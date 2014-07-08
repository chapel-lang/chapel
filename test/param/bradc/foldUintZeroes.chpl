config var u: uint = 1;

if (u >= 0) then
  writeln("Whew!");
else
  writeln("This should get folded out");

if (u < 0) then
  writeln("This should get folded out");
else
  writeln("Whew!");

if (0 <= u) then
  writeln("Whew!");
else
  writeln("This should get folded out");

if (0 > u) then
  writeln("This should get folded out");
else
  writeln("Whew!");

