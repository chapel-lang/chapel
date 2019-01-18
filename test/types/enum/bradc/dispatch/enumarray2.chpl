enum color { red, white, blue };

record R {
  var c: color;
}

var c: [0..1] R;

c[0].c = color.red;
c[1].c = color.blue;

proc foo(x: int(64), y: int(64)) {
  writeln("In 64-bit foo!");
  return false;
}

proc foo(x: color, y: color) {
  writeln("In enum foo!");
  return true;
}

if (foo(c[0].c, color.white)) {
  writeln("Got correct version!");
}
