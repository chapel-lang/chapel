config var u: uint = 1;

proc FoldMeOut() {
  writeln("This should get folded out");
}

if (u >= 0) then
  writeln("Whew!");
else
  FoldMeOut();

if (u < 0) then
  FoldMeOut();
else
  writeln("Whew!");

if (0 <= u) then
  writeln("Whew!");
else
  FoldMeOut();

if (0 > u) then
  FoldMeOut();
else
  writeln("Whew!");

