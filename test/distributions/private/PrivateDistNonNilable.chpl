use PrivateDist;

class C { var x = 0; }

proc test1() {
  var a = [PrivateSpace] new shared C();
  writeln(a);
}
test1();

proc test2() {
  var a = [i in PrivateSpace] new shared C(i);
  writeln(a);
}
test2();

