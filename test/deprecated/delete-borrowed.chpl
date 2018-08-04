// The warning emitted by the compiler for this test
// should be an error after 1.18.
// At that point, this test can be removed in favor of
//   test/classes/delete-free/delete-borrowed.chpl

class MyClass {
  var field:int;
}

var x = new borrowed MyClass(1);
delete x;
