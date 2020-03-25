pragma "safe"
module zzz {



class MyClass {
  var x:int;
}


/* 
   Another question: What if a method on a global variable of record or class
   type sets a field from a borrow?
 */

record R {
  var borrow:borrowed MyClass?;
}
var global:R;

proc bad2(arg:borrowed MyClass) {
  // This is bad because global.borrow has infinite lifetime and
  // arg does not.
  global.borrow = arg;
}

proc test() {
  var myowned = new owned MyClass(1);

  var borrow = myowned.borrow();
  bad2(borrow);
}

test();
writeln(global);


record RBorrowAndOwn {
  var _borrowed:borrowed MyClass;

  pragma "owned"
  var myowned:unmanaged MyClass;

  proc readOwned() {
    return _to_borrowed(myowned);
  }
}

proc RBorrowAndOwn.deinit() {
  delete myowned;
}

proc makeR(borrow:borrowed MyClass) {
  return new RBorrowAndOwn(borrow, new unmanaged MyClass(10*borrow.x));
}

proc badF3() {
  var c = new borrowed MyClass(1);
  var r = makeR(c);
  {
    var r2 = makeR(r._borrowed);
    return r2;
  }
}

proc test2() {
  badF3();
}

test2();
}
