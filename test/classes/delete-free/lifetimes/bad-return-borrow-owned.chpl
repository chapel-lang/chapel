pragma "safe"
module l10 {

class MyClass {
  var x:int;
}
record Rinit {
  pragma "owned"
  var c:MyClass;
  proc init(c:MyClass) {
    this.c = c;
  }
  proc borrow() {
    return c;
  }
}


proc returnBorrowRinit(const ref arg:Rinit):MyClass {
  return arg.borrow();
}

proc badReturnReturnBorrowRinit() {
  var rr = new Rinit(new MyClass(1));

  return returnBorrowRinit(rr);
}

proc test1() {
  var x = badReturnReturnBorrowRinit();
  writeln(x);
}

test1();

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

proc test2() {
  var x = badReturnReturnBorrow();
  writeln(x);
}

test2();


proc badReturnInnerBorrow() {
  var rr = new R(new MyClass(1));
  proc innerNoArgs() {
    return rr.borrow();
  }
  return innerNoArgs();
}

proc test3() {
  var x = badReturnInnerBorrow();
  writeln(x);
}

test3();

}
