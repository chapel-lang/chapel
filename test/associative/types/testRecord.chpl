import AssocTest;

record R { var value = 1; }
type t = R;

AssocTest.testAssoc(t);
