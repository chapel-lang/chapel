private use List;

class C { var x: int = 0; }

var L: list(owned C);
L.append(new owned C(1));
writeln(L[0].x);

proc test() {
  var LL: list(owned C);
  var cc = new owned C(1);
  LL.append(cc);
  writeln(LL[0].x);
}
test();
