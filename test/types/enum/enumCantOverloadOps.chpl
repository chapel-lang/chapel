enum color {red, green, blue};

proc =(ref lhs: color, rhs: color) {
  lhs = (rhs:int + 1): color;
}

proc ==(lhs: color, rhs: color) {
  return lhs != rhs;
}

proc !=(lhs: color, rhs: color) {
  return lhs == color.red;
}

var c = color.red;
var c2: color;
c2 = c;
if (c2 == c) then
  writeln("The same");
if (c2 != c) then
  writeln("different");
