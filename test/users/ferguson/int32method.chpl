proc (int(32)).mysquare(): int(32) {
  return this * this;
}

var x : int(32) = 5;
var y = x.mysquare();

writeln(y);

