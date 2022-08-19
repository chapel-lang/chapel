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
    var a0 = (new owned MyClass(0)).borrow();
    var a1 = (new owned MyClass(1)).borrow();
    var a2 = (new owned MyClass(2)).borrow();
    var a3 = (new owned MyClass(3)).borrow();

    var x = getfirstborrow(a0, a1, a2, a3);
    writeln(x);
  }
  test();
}
