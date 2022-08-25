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
    var x = (new owned MyClass(1)).borrow();
    var y = f(x);
  }
  test();
}
