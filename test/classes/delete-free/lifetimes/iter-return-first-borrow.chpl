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
    var a0 = new borrowed MyClass(0);
    var a1 = new borrowed MyClass(1);
    var a2 = new borrowed MyClass(2);
    var a3 = new borrowed MyClass(3);

    var x = getfirstborrow(a0, a1, a2, a3);
    writeln(x);
  }
  test();
}
