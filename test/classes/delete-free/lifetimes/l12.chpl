pragma "safe"
module l12 {

class MyClass {
  var x:int;
}
record R {
  pragma "owned"
  var c:MyClass;
  proc borrow() {
    return c;
  }
}

proc test() {
  var rr = new R(new MyClass(1));
  proc innerNoArgs() {
    return rr.borrow();
  }
  return innerNoArgs();
}

test();
}
