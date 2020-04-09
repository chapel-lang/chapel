class MyClass {
  var x: int;
}

proc test() {
  
  // Can we make an array of owned?
  var A:[1..3] owned MyClass?;

  A[1].retain(new unmanaged MyClass(1));

  writeln(A[1]!.x);
}

test();
