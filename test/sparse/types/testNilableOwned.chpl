import SparseTest;
class T {
  var value = 0;
}

type t = owned T?;

SparseTest.testSparse(t);
