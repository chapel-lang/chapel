class pair {
  var x: integer;
  var y: float;
}

function main() {
  var a: pair;
  a = pair();
  a.x = 10;
  a.y = 2.3;
  writeln("a is: ", a.x, " ", a.y);
}
