// the warning emitted for throwing a borrow
// should be changed to an error after 1.19.

proc test() throws {
  var x = new borrowed Error();
  throw x;
}

try! test();
