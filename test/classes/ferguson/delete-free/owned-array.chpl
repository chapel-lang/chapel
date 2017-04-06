use OwnedObject;

class MyClass {
  var x: int;
}

proc test() {
  
  // Can we make an array of Owned?
  var A:[1..3] Owned(MyClass);

  A[1].retain(new MyClass(1));

  writeln(A[1].x);
}

test();
