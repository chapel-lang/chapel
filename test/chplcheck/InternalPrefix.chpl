module InternalPrefix {
  proc myprefix_my_proc() {} // bad
  proc myprefix_MyProc() {}  // bad
  proc myprefix_myProc() {}  // ok

  record myprefix_my_rec {} // bad
  record myprefix_MyRec {}  // bad
  record myprefix_myRec {}  // ok

  class myprefix_my_class {} // bad
  class myprefix_MyClass {}  // ok
  class myprefix_myClass {}  // bad
}
