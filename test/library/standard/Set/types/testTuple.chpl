import SetTest;

class T {
  var value = 0;
}

type t = (shared T, shared T);

SetTest.testSet(t);
