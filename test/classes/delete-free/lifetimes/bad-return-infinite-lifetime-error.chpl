pragma "safe"
module TestInfiniteLifetime {

  class MyClass {
    var x:int;
  }

  pragma "fn returns infinite lifetime"
  proc f(const ref arg: borrowed MyClass) const ref {
    return arg; // this should be an error
  }

  proc test() {
    var ownX = new owned MyClass(1);
    var x = ownX.borrow();
    var y = f(x);
  }
  test();
}
