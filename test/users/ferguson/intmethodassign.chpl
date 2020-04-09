proc int.mysquare(): int {
  this = this * this;
}

var x : int = 5;
x.mysquare();

writeln(x);

