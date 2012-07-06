enum color { purple, yellow };

var c$: sync color = color.purple;

proc foo(x: int(64)) {
  writeln("In 64-bit foo()");
  return true;
}

proc foo(x: int(32)) {
  writeln("In 32-bit foo()");
  return false;
}

if (foo(c$)) then
  writeln("correct");
else
  writeln("incorrect");

var c2$: sync color = color.purple;

if (c2$ == color.yellow) then
  writeln("c2$ was yellow!");
else
  writeln("c2$ was purple!");
