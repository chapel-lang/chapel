pragma "safe"
module l10 {

class MyClass {
  var x:int;
}
record R {
  pragma "owned"
  var c:MyClass;
  proc init(c:MyClass) {
    this.c = c;
  }
  proc borrow() {
    return c;
  }
}


proc returnBorrow(const ref arg:R):MyClass {
  return arg.borrow();
}

proc returnReturnBorrow() {
  var rr = new R(new MyClass(1));

  return returnBorrow(rr);
}

proc test() {
  var x = returnReturnBorrow();
  writeln(x);
}

test();
}
