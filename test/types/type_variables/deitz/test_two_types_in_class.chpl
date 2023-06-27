class C {
  type t, tt, ttt = int, tttt = real;
  var x: (t, tt, ttt, tttt);
}

var ownC = new owned C(imag, complex, int, real);
var c = ownC.borrow();

writeln(c);
writeln(c.type:string);
