proc int.mysquare(): int {
  return this * this;
}

var x : int = 5;
var y = x.mysquare();

writeln(y);

