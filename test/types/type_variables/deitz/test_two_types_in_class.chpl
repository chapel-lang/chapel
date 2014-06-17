class C {
  type t, tt, ttt = int, tttt = real;
  var x: (t, tt, ttt, tttt);
}

var c = new C(imag, complex, int, real);

writeln(c);
writeln(typeToString(c.type));
