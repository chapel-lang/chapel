import SetTest;

class T {
  var value = 0;
}

type t = borrowed T?;

SetTest.testSet(t);
