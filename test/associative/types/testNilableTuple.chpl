import AssocTest;
class T {
  var value = 0;
}

type t = (shared T?, shared T?);

AssocTest.testAssoc(t);
