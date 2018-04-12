pragma "safe"
module zzz {

use OwnedObject;

class MyClass {
  var x:int;
}


/*(The question is, if you mutated a ref to a borrow, would  the other borrow
  checking rules catch an error, or would it cause a gap in the checking?)
 */
proc bad1(ref r:MyClass) {

  var owny = new Owned(new MyClass(1));
  r = owny.borrow();
}

/* 
   Another question: What if a method on a global variable of record or class
   type sets a field from a borrow?
 */

record R {
  var borrow:MyClass;
}
var global:R;

proc bad2(arg:MyClass) {
  // This is bad because global.borrow has infinite lifetime and
  // arg does not.
  global.borrow = arg;
}

proc test() {
  var myowned = new Owned(new MyClass(1));

  var borrow = myowned.borrow();
  bad1(borrow);
  bad2(borrow);
}

test();
writeln(global);


record RBorrowAndOwn {
  var borrowed:MyClass;

  pragma "owned"
  var myowned:MyClass;

  proc readOwned() {
    return myowned;
  }
}

proc RBorrowAndOwn.deinit() {
  delete myowned;
}

proc makeR(borrow:MyClass) {
  return new RBorrowAndOwn(borrow, new MyClass(10*borrow.x));
}

proc badF3() {
  var c = new MyClass(1);
  var r = makeR(c);
  {
    var r2 = makeR(r.borrowed);
    return r2;
  }
}

proc test2() {
  badF3();
}

test2();
}
