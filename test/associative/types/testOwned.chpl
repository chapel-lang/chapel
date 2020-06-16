import AssocTest;
class T {
  var value = 0;
}

type t = owned T;

AssocTest.testAssoc(t);
