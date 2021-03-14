iter myiter(out x: int) {
  x = 500;
  for i in 1..2 {
    x = 500 + i;
    yield i;
  }
  x = 1000;
}
var y: int = 0;
for j in myiter(y) {
  writeln("in loop, j=", j, " y=", y);
}
writeln("after loop, y=", y);
