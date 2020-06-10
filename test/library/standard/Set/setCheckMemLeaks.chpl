import Set.set;

class C { var x: int = 0; }
const hi = 128;

proc test() {
  var arr: [0..#hi] shared C?;
  var s: set(shared C?);

  for i in 0..#hi do arr[i] = new shared C(i);
  for x in arr do s.add(x);
  s.clear();
  for x in arr do s.add(x);
  for x in arr do s.remove(x);
  assert(s.isEmpty());
  for i in 0..#hi do s.add(new shared C(i));
}
test();

