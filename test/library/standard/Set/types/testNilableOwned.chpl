import SetTest;

class T {
  var value = 0;
}

type t = owned T?;

SetTest.testSet(t);
