import TupleTest;
class T {
  var value = 0;
}

type t = (shared T?, shared T?);

TupleTest.test(t);
