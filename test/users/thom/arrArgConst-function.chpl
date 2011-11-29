proc writeArr(const X: [] real) {
  writeln(X);
}

var A: [1..3] real = (1.0, 2.0, 3.0);
var B: [1..3] real = (3.0, 2.0, 1.0);

proc addEm(X, Y) {
  return X+Y;
}

writeArr(addEm(A,B));
