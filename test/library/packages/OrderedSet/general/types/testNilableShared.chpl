import OsetTypeTest;
use OsetTest;

type T = shared testClass?;

operator <(lhs: T, rhs: T) {
  if lhs == nil && rhs == nil then return false;
  if lhs == nil then return true;
  if rhs == nil then return false;
  return lhs!.dummy < rhs!.dummy;
}

OsetTypeTest.testSet(T);
