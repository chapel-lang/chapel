enum color { purple, yellow };

var c$: sync color = color.purple;

proc foo(x: int(64)) {
  writeln("In 64-bit foo()");
  return false;
}

proc foo(x: color) {
  writeln("In color foo()");
  return true;
}

if (foo(c$.readFE())) then
  writeln("correct");
else
  writeln("incorrect");

var c2$: sync color = color.purple;

if (c2$.readFE() == color.yellow) then
  writeln("c2$ was yellow!");
else
  writeln("c2$ was purple!");
