enum color { red, white, blue };

var c: color;

c = color.red;

proc foo(x: int(64), y: int(64)) {
  writeln("In 64-bit foo!");
  return false;
}

proc foo(x: color, y: color) {
  writeln("In enum foo!");
  return true;
}

if (foo(c, color.white)) {
  writeln("We got the right one!");
}

