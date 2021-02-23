use PrivateDist;

class C { var x = 0; }

proc test1() {
  var a: [PrivateSpace] shared C;
  writeln(a);
}
test1();

