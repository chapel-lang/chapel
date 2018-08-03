pragma "safe"
module TestReturningIteratorYield {

  class MyClass {
    var x:int;
  }

  proc getfirstborrow() {
    var A:[0..3] borrowed MyClass;
    A[0] = new borrowed MyClass(0);
    A[1] = new borrowed MyClass(1);
    A[2] = new borrowed MyClass(2);
    A[3] = new borrowed MyClass(3);

    for x in A {
      return x;
    }
    return A[0];
  }

  proc test() {
    var x = getfirstborrow();
    writeln(x);
  }
  test();
}
