class C {
  var x: int;
  var y: (real, imag);
  var z: [1..4] complex;
}

var c = new C();

var s: string;

s.write(s, c);

writeln(s);

delete c;
