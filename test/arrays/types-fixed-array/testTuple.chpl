import ArrayTest;
class T {
  var value = 0;
}

type t = (shared T, shared T);

ArrayTest.testArray(t);
