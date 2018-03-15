pragma "safe"
module arrborrow {

class MyClass {
  var x:int;
}

record R {
  pragma "owned"
  var owned:MyClass;

  proc readOwned() {
    return owned;
  }
}

proc R.deinit() {
  delete owned;
}

proc badReturnBorrowLocalArrayElement() {
  var A:[1..10] R;
  A[1] = new R(new MyClass(1));

  return A[1].readOwned();
  // A and its elements are destroyed here
}

proc badReturnBorrowLocalArrayElement2() {
  var A:[1..10] R;
  A[1] = new R(new MyClass(1));

  var ret:MyClass;
  
  ret = A[1].readOwned();
  return ret;
  // A and its elements are destroyed here
}


proc badReturnBorrowLocalArrayIteration() {
  var ret:MyClass;
  var A:[1..10] R;

  for i in 1..10 {
    A[i] = new R(new MyClass(i));
  }

  for elt in A {
    ret = elt.readOwned();
  }
  return ret;
  // A and its elements are destroyed here
}

proc badReturnBorrowLocalArrayIteration2() {
  var ret:MyClass;
  var A:[1..10] R;

  for i in 1..10 {
    A[i] = new R(new MyClass(i));
  }

  for i in 1..10 {
    ret = A[i].readOwned();
  }
  return ret;
  // A and its elements are destroyed here
}


proc test() {
  var v1 = badReturnBorrowLocalArrayElement();
  var v2 = badReturnBorrowLocalArrayElement2();
  var v3 = badReturnBorrowLocalArrayIteration();
  var v4 = badReturnBorrowLocalArrayIteration2();

  writeln(v1);
  writeln(v2);
  writeln(v3);
  writeln(v4);
}

test();
}
