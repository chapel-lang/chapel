import SparseTest;

class T {
  var value = 0;
}

type t = borrowed T;

SparseTest.testSparse(t);
