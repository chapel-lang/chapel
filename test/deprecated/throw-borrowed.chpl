// the warning emitted for throwing a borrow
// should be changed to an error after 1.19.

// the test throw-new-undecorated.chpl already
// covers this case and should be updated once
// it is changed to an error.

proc test() throws {
  throw new Error();
}

try! test();
