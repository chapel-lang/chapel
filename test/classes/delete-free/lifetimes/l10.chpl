pragma "safe"
module l10 {

class MyClass {
  var x:int;
}
record R {
  pragma "unsafe"
  var c:MyClass;
}


proc returnBorrow(const ref arg:R):MyClass {
  return arg.c;
}

proc returnReturnBorrow() {
  pragma "unsafe"
  var rr = new R(new MyClass(1));

  return returnBorrow(rr);
}

proc test() {
  var x = returnReturnBorrow();
  writeln(x);
}

test();
}
