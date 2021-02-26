use OrderedSet;
use OsetTest;



type T = testClass;

proc <(lhs: borrowed T?, rhs: borrowed T?) {
  if lhs == nil && rhs == nil then return false;
  if lhs == nil then return true;
  if rhs == nil then return false;
  return lhs!.dummy < rhs!.dummy;
}

var s = new orderedSet(borrowed T?, false, defaultComparator);

var a = new T();
var b: borrowed T? = a.borrow();
s.add(b);
assert(s.size == 1);

s.remove(b);
assert(s.size == 0);
