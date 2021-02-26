import VectorTest;

class T {
  var value = 0;
}

type t = borrowed T;

VectorTest.testVector(t);
