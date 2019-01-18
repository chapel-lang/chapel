pragma "safe"
module l10 {

class MyClass {
  var x:int;
}
record Rinit {
  pragma "owned"
  var c:unmanaged MyClass;
  proc init(c:unmanaged MyClass) {
    this.c = c;
  }
  proc borrow() {
    return _to_borrowed(c);
  }
}


proc returnBorrowRinit(const ref arg:Rinit):borrowed MyClass {
  return arg.borrow();
}

proc badReturnReturnBorrowRinit() {
  var rr = new Rinit(new unmanaged MyClass(1));

  return returnBorrowRinit(rr);
}

proc test1() {
  var x = badReturnReturnBorrowRinit();
  writeln(x);
}

test1();

record R {
  pragma "owned"
  var c:unmanaged MyClass;
  proc borrow() {
    return _to_borrowed(c);
  }
}

proc returnBorrow(const ref arg:R):borrowed MyClass {
  return arg.borrow();
}

proc badReturnReturnBorrow() {
  var rr = new R(new unmanaged MyClass(1));

  return returnBorrow(rr);
}

proc test2() {
  var x = badReturnReturnBorrow();
  writeln(x);
}

test2();


proc badReturnInnerBorrow() {
  var rr = new R(new unmanaged MyClass(1));
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
