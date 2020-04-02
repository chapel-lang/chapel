import TupleTest;
class T {
  var value = 0;
}

type t = borrowed T?;

TupleTest.test(t);
