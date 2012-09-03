class C { type myCtype; }
type CT = C(string);
writeln(typeToString(C(int(32)).myCtype));
writeln(typeToString(CT));

class R { type myRtype; }
type RT = R(real);
writeln(typeToString(R(int(64)).myRtype));
writeln(typeToString(RT));

proc p(type t, x, type q, y) {
  writeln(typeToString(t.myCtype));
  writeln(typeToString(x.type.myCtype));
  writeln(typeToString(q.myRtype));
  writeln(typeToString(y.type.myRtype));
}

var x: C(complex), y: R(imag);
p(C(imag), x, R(complex), y);
