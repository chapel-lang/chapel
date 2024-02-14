class C { }

proc test1() {
  var myUnmanaged = new unmanaged C();

  // not expecting a warning in this case b/c
  // the RHS isn't a 'new' expression
  var noError: borrowed C = myUnmanaged;

  delete myUnmanaged;
}
test1();
proc test1n() {
  var myUnmanaged = new unmanaged C();

  // not expecting a warning in this case b/c
  // the RHS isn't a 'new' expression
  var noError: borrowed C? = myUnmanaged;

  delete myUnmanaged;
}
test1n();
proc test1nn() {
  var myUnmanaged = new unmanaged C?();

  // not expecting a warning in this case b/c
  // the RHS isn't a 'new' expression
  var noError: borrowed C? = myUnmanaged;

  delete myUnmanaged;
}
test1nn();

proc test2() {
  // warn
  var x: borrowed C = new unmanaged C();

  delete (x: unmanaged);
}
test2();
proc test2n() {
  // warn
  var x: borrowed C? = new unmanaged C();

  delete (x: unmanaged);
}
test2n();
proc test2nn() {
  // warn
  var x: borrowed C? = new unmanaged C?();

  delete (x: unmanaged);
}
test2nn();

proc test3() {
  // warn
  var x: borrowed C;
  x = new unmanaged C();

  delete (x: unmanaged);
}
test3();
proc test3n() {
  // warn
  var x: borrowed C?;
  x = new unmanaged C();

  delete (x: unmanaged);
}
test3n();
proc test3nn() {
  // warn
  var x: borrowed C?;
  x = new unmanaged C?();

  delete (x: unmanaged);
}
test3nn();

proc test4() {
  var myUnmanaged = new unmanaged C();
  var myBorrowed = myUnmanaged.borrow();

  var x: borrowed C = myBorrowed;
  // warn
  x = new unmanaged C();

  delete (x: unmanaged);
  delete myUnmanaged;
}
test4();
proc test4n() {
  var myUnmanaged = new unmanaged C();
  var myBorrowed = myUnmanaged.borrow();

  var x: borrowed C? = myBorrowed;
  // warn
  x = new unmanaged C();

  delete (x: unmanaged);
  delete myUnmanaged;
}
test4n();
proc test4nn() {
  var myUnmanaged = new unmanaged C();
  var myBorrowed = myUnmanaged.borrow();

  var x: borrowed C? = myBorrowed;
  // warn
  x = new unmanaged C?();

  delete (x: unmanaged);
  delete myUnmanaged;
}
test4nn();
