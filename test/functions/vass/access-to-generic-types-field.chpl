class C { type myCtype; }
type CT = C(string);
writeln(C(int(32)).myCtype:string);
writeln(CT:string);

class R { type myRtype; }
type RT = R(real);
writeln(R(int(64)).myRtype:string);
writeln(RT:string);

proc p(type t, x, type q, y) {
  writeln(t.myCtype:string);
  writeln(x.type.myCtype:string);
  writeln(q.myRtype:string);
  writeln(y.type.myRtype:string);
}

var x: borrowed C(complex)?, y: borrowed R(imag)?;
p(C(imag), x, R(complex), y);
