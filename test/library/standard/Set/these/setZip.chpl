use Set;

proc test() {
  var s1: set(int);
  for i in 1..8 do s1.add(i);
  var s2 = s1;

  var check1: atomic int;
  var check2: atomic int;

  check1.write(0);
  check2.write(0);

  forall (x, y) in zip(s1, s2) with (ref check1, ref check2) {
    check1.add(x);
    check2.add(y);
  }

  writeln(check1);
  writeln(check2);

  var a = check1.read();
  var b = check2.read();

  assert(a == b);

  return;
}
test();

