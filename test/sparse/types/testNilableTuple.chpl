import SparseTest;
class T {
  var value = 0;
}

type t = (shared T?, shared T?);

SparseTest.testSparse(t);
