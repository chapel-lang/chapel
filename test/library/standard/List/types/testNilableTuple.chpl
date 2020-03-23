import ListTest;

class T {
  var value = 0;
}

type t = (shared T?, shared T?);

ListTest.testList(t);
