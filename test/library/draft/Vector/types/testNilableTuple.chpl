import VectorTest;

class T {
  var value = 0;
}

type t = (shared T?, shared T?);

VectorTest.testVector(t);
