pragma "safe"
module l11 {

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


proc returnBorrow(const ref arg:R):MyClass {
  return arg.borrow();
}

proc badReturnReturnBorrow() {
  var rr = new R(new MyClass(1));

  return returnBorrow(rr);
}

proc test() {
  var x = badReturnReturnBorrow();
  writeln(x);
}

test();
}
