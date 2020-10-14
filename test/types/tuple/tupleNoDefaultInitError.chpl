class C { var x: int = 0; }
class D { var x: int = 0; }

proc test() {
  var tup1: (int, owned C);
  var tup2: (shared C, owned D, int);
  return;
}
test();

