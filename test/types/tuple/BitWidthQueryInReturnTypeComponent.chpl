proc baz((r, s) : (real(?w), imag(?_w))) {
  if w != _w then compilerError("real and imag argument sizes must match!");
  var c: complex(2*w) = (r, s): complex;
  writeln(c.type:string);
}

proc main() {
  baz((1.0, 8.0:imag));
}
