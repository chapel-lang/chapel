class X {
  var x: int;
}

proc =(lhs: X, rhs: X) {
  lhs.x = rhs.x;
}

var myX = new X(42);
var myX2 = new X(33);

myX = myX2;

writeln(myX, myX2);
