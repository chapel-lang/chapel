import OsetTypeTest;
use OsetTest;

type t = unmanaged testClass?;

proc <(lhs: t, rhs: t) {
  if lhs == nil && rhs == nil then return false;
  if lhs == nil then return true;
  if rhs == nil then return false;
  return lhs!.dummy < rhs!.dummy;
}

OsetTypeTest.testSet(t);
