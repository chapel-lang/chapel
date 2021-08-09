module A {
  record R {
    var x : int;
  }
}
var X = new A.R(5);
var Y = new A.R(10);
X = Y;
writeln(X);
writeln(Y);
