use Set;

class C1 { var x = 0; }
class C2 : C1 { var y = 0; }

proc test1() {
  var s1: set(shared C1);
  for i in 0..3 do s1.add(new shared C1(i));
  var s2: set(shared C2) = s1;
}
test1();

proc test2() {
  var s1: set(int);
  for i in 0..3 do s1.add(i);
  var s2: set(real) = s1;
}
test2();

