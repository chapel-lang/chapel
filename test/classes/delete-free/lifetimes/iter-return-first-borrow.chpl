pragma "safe"
module TestReturningIteratorYield {

  class MyClass {
    var x:int;
  }

  iter myiter(arg1, arg2, arg3) {
    yield arg1;
    yield arg2;
    yield arg3;
  }

  proc getfirstborrow(arg0, arg1, arg2, arg3) {
    for i in myiter(arg1, arg2, arg3) {
      return i;
    }
    return arg0;
  }
  proc test() {
    var ownA0 = new owned MyClass(0);
    var ownA1 = new owned MyClass(1);
    var ownA2 = new owned MyClass(2);
    var ownA3 = new owned MyClass(3);
    var a0 = ownA0.borrow();
    var a1 = ownA1.borrow();
    var a2 = ownA2.borrow();
    var a3 = ownA3.borrow();

    var x = getfirstborrow(a0, a1, a2, a3);
    writeln(x);
  }
  test();
}
