import AssocTest;

class T {
  var value = 0;
}

type t = borrowed T;

AssocTest.testAssoc(t);
