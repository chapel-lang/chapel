class C {
  type t, tt, ttt = int, tttt = real;
  var x: (t, tt, ttt, tttt);
}

var c = new borrowed C(imag, complex, int, real);

writeln(c);
writeln(c.type:string);
