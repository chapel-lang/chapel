private use List;

class C { var x: int = 0; }

var L: list(owned C);
L.append(new owned C(1));
writeln(L[1].x);

proc test() {
  var LL: list(owned C);
  var cc = new owned C(1);
  LL.append(cc);
  writeln(LL[1].x);
}
test();
