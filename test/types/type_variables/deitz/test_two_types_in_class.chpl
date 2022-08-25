class C {
  type t, tt, ttt = int, tttt = real;
  var x: (t, tt, ttt, tttt);
}

var c = (new owned C(imag, complex, int, real)).borrow();

writeln(c);
writeln(c.type:string);
