class MyClass {
  var x: int;
}

var D = {1..3};
var A: [D] owned MyClass;

var c = new owned MyClass(1);
A.push_back(c);

writeln(A[4].x);
