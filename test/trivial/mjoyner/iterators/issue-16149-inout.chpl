iter myiter(inout x: int) {
  x += 500;
  for i in 1..2 {
    x += 1;
    yield i;
  }
  x += 500;
}
var y: int = 0;
for j in myiter(y) {
  writeln("in loop, j=", j, " y=", y);
}
writeln("after loop, y=", y);
