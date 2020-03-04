pragma "safe"
module TestReturningIteratorYield {

  class MyClass {
    var x:int;
  }

  proc getfirstborrow() {
    var A:[0..3] borrowed MyClass;
    var c0 = new MyClass(0);
    var c1 = new MyClass(1);
    var c2 = new MyClass(2);
    var c3 = new MyClass(3);

    A[0] = c0.borrow();
    A[1] = c1.borrow();
    A[2] = c2.borrow();
    A[3] = c3.borrow();

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
