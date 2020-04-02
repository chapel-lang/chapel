import SparseTest;

record R { var value = 1; }
type t = R;

SparseTest.testSparse(t);
