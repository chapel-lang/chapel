var x = 1.0;
var y = 5.0;
while (x < y) {
  writeln("(1) x = ", x);
  var x = y + 10.0;
  writeln("(2) x = ", x);
  y = y - 1.0;
}
