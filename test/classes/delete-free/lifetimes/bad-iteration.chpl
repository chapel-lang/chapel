class C { var x:int; }

iter yieldLoopTmpBorrow() {
  for i in 1..10 {
    var tmp = new owned C(i);
    yield tmp.borrow();
  }
}
proc badIteration1() {
  var b: borrowed C?;
  for x in yieldLoopTmpBorrow() {
    b = x; // expecting error
  }
  var v = b!.x; // uh-oh, use after free
  writeln(v);
}
badIteration1();

iter yieldIteratorTmpBorrow() {
  var tmp = new owned C();
  for i in 1..10 {
    yield tmp.borrow();
  }
}
proc badIteration2() {
  var b: borrowed C?;
  for x in yieldIteratorTmpBorrow() {
    b = x; // expecting error
  }
  var v = b!.x; // uh-oh, use after free
  writeln(v);
}
badIteration2();

var global = new owned C();
// this iterator yields borrows from globals
iter yieldGlobalBorrows() {
  for i in 1..10 {
    yield global.borrow();
  }
}
proc maybeOkGlobalBorrow() {
  var b: borrowed C?;
  for x in yieldGlobalBorrows() {
    b = x; // expecting error
  }
  var v = b!.x;
  writeln(v);
}
maybeOkGlobalBorrow();

iter yieldRefX() ref {
  var x = 0;
  for i in 1..10 {
    yield x;
  }
  writeln(x);
}
proc okIterX() {
  var b: borrowed C?;
  var first = true;
  for x in yieldRefX() {
    x = 2;
  }
}
okIterX();
