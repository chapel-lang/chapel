use IO, Math;

var z = nan;
writeln(z);

{ // test NaN real and imag components
  var c = z + z*1.0i;
  writeln(c);

  var s = c: string;
  writeln(s);

  var b = c: bytes;
  writeln(b);

  var c2 = s: complex;
  writeln(c2);

  var c3 = b: complex;
  writeln(c3);

  var c4 = stdin.read(complex);
  writeln(c4);
}

{ // Test NaN real, non-NaN imag
  var c = z + 1.2i;
  writeln(c);

  var s = c: string;
  writeln(s);

  var b = c: bytes;
  writeln(b);

  var c2 = s: complex;
  writeln(c2);

  var c3 = b: complex;
  writeln(c3);

  var c4 = stdin.read(complex);
  writeln(c4);
}

{ // Test non-NaN real, NaN imag
  var c = 3.4 + z*1.0i;
  writeln(c);

  var s = c: string;
  writeln(s);

  var b = c: bytes;
  writeln(b);

  var c2 = s: complex;
  writeln(c2);

  var c3 = b: complex;
  writeln(c3);

  var c4 = stdin.read(complex);
  writeln(c4);
}
