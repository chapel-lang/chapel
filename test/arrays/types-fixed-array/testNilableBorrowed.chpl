import ArrayTest;
class T {
  var value = 0;
}

type t = borrowed T?;

ArrayTest.testArray(t);
