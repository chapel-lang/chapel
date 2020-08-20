use OrderedSet;
use OsetTest;



class C { var x: int = 0; }
proc <(x: shared C?, y: shared C?) {
  if x == nil && y == nil then return 1;
  else if x == nil then return 1;
  else if y == nil then return 0;
  else return x!.x < y!.x;
}

const hi = 128;

proc test() {
  var arr: [0..#hi] shared C?;
  var s = new orderedSet(shared C?, false, defaultComparator);

  for i in 0..#hi do arr[i] = new shared C(i);
  for x in arr do s.add(x);
  s.clear();
  for x in arr do s.add(x);
  for x in arr do s.remove(x);
  assert(s.isEmpty());
  for i in 0..#hi do s.add(new shared C(i));
}
test();
