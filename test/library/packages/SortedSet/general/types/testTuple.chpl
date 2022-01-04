import OsetTypeTest;
use OsetTest;

type T = testClass;

type t = (shared T, shared T);

OsetTypeTest.testSet(t);
