import ListTest;

class T {
  var value = 0;
}

type t = borrowed T?;

ListTest.testList(t);
