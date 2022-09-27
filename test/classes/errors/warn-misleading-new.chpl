class C { }

proc test1() {
  var myOwned = new owned C();
  var myShared = new shared C();

  // not expecting a warning in this case b/c
  // the RHS isn't a 'new' expression
  var noError: borrowed C = myOwned;
  var noErrorS: borrowed C = myShared;
}
test1();
proc test1n() {
  var myOwned = new owned C();
  var myShared = new shared C();

  // not expecting a warning in this case b/c
  // the RHS isn't a 'new' expression
  var noError: borrowed C? = myOwned;
  var noErrorS: borrowed C? = myShared;
}
test1n();
proc test1nn() {
  var myOwned = new owned C?();
  var myShared = new shared C?();

  // not expecting a warning in this case b/c
  // the RHS isn't a 'new' expression
  var noError: borrowed C? = myOwned;
  var noErrorS: borrowed C? = myShared;
}
test1nn();

proc test2() {
  // warn
  var x: borrowed C = new owned C();
  var y: borrowed C = new shared C();
  var z: borrowed C = new C();
}
test2();
proc test2n() {
  // warn
  var x: borrowed C? = new owned C();
  var y: borrowed C? = new shared C();
  var z: borrowed C? = new C();
}
test2n();
proc test2nn() {
  // warn
  var x: borrowed C? = new owned C?();
  var y: borrowed C? = new shared C?();
  var z: borrowed C? = new C?();
}
test2nn();

proc test3() {
  // warn
  var x: borrowed C;
  var y: borrowed C;
  var z: borrowed C;
  x = new owned C();
  y = new shared C();
  z = new C();
}
test3();
proc test3n() {
  // warn
  var x: borrowed C?;
  var y: borrowed C?;
  var z: borrowed C?;
  x = new owned C();
  y = new shared C();
  z = new C();
}
test3n();
proc test3nn() {
  // warn
  var x: borrowed C?;
  var y: borrowed C?;
  var z: borrowed C?;
  x = new owned C?();
  y = new shared C?();
  z = new C?();
}
test3nn();

proc test4() {
  var myOwned = new owned C();
  var myBorrowed = myOwned.borrow();

  var x: borrowed C = myBorrowed;
  var y: borrowed C = myBorrowed;
  var z: borrowed C = myBorrowed;
  // warn
  x = new owned C();
  y = new shared C();
  z = new C();
}
test4();
proc test4n() {
  var myOwned = new owned C();
  var myBorrowed = myOwned.borrow();

  var x: borrowed C? = myBorrowed;
  var y: borrowed C? = myBorrowed;
  var z: borrowed C? = myBorrowed;
  // warn
  x = new owned C();
  y = new shared C();
  z = new C();
}
test4n();
proc test4nn() {
  var myOwned = new owned C();
  var myBorrowed = myOwned.borrow();

  var x: borrowed C? = myBorrowed;
  var y: borrowed C? = myBorrowed;
  var z: borrowed C? = myBorrowed;
  // warn
  x = new owned C?();
  y = new shared C?();
  z = new C?();
}
test4nn();
