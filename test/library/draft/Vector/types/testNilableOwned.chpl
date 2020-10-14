import VectorTest;

class T {
  var value = 0;
}

type t = owned T?;

VectorTest.testVector(t);
