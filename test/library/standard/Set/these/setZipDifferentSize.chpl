use Set;

proc test() {
  var s1: set(int);
  for i in 1..8 do s1.add(i);

  var s2 = s1;
  s2.add(9);

  forall (x, y) in zip(s1, s2) do
    var tmp = x + y;

  return;
}
test();

