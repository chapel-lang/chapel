use Map;

class C1 { var x = 0; }
class C2 : C1 { var y = 0; }

proc test1() {
  var m1: map(shared C1, int);
  for i in 0..3 do m1.add(new shared C1(i), i);
  var m2: map(shared C2, int) = m1;
}
test1();

proc test2() {
  var m1: map(int, shared C1);
  for i in 0..3 do m1.add(i, new shared C1(i));
  var m2: map(int, shared C2) = m1;
}
test2();

proc test3() {
  var m1: map(int, int);
  for i in 0..3 do m1.add(i, i);
  var m2: map(real, real) = m1;
}
test3();

proc test4() {
  var m1: map(shared C2, int);
  for i in 0..3 do m1.add(new shared C2(i), i);
  var m2: map(shared C1, int) = m1;
}
test4();

proc test5() {
  var m1: map(int, shared C2);
  for i in 0..3 do m1.add(i, new shared C2(i));
  var m2: map(shared C2, int) = m1;
}
test5();

