use OwnedObject;

class MyClass {
  var x: int;
}

var D = {1..3};
var A: [D] Owned(MyClass);

var c = new Owned(new MyClass(1));
A.push_back(c);

writeln(A[4].x);
