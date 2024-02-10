proc printComponentWidths(type T: (int(?), int(?))) {
  var (a, b): T;
  printComponentWidths((a, b));
}

proc printComponentWidths((a, b) : (int(?w1), int(?w2))) {
  writeln('w1: ', w1, ', w2: ', w2);
}

proc testPrintWidths() {
  writeln('Printing tuple component bitwidth queries...');
  printComponentWidths((int(8), int(8)));
  printComponentWidths((int(8), int(16)));
  printComponentWidths((int(8), int(32)));
  printComponentWidths((int(8), int(64)));

  printComponentWidths((int(16), int(8)));
  printComponentWidths((int(16), int(16)));
  printComponentWidths((int(16), int(32)));
  printComponentWidths((int(16), int(64)));

  printComponentWidths((int(32), int(8)));
  printComponentWidths((int(32), int(16)));
  printComponentWidths((int(32), int(32)));
  printComponentWidths((int(32), int(64)));

  printComponentWidths((int(64), int(8)));
  printComponentWidths((int(64), int(16)));
  printComponentWidths((int(64), int(32)));
  printComponentWidths((int(64), int(64)));
  writeln('-----');
}

// Original reproducer:
proc baz((r, s) : (real(?w), imag(?_w))) {
  if w != _w then compilerError("real and imag argument sizes must match!");
  var c: complex(2*w) = (r, s): complex;
  writeln(c.type:string);
}

proc testReproducer() {
  baz((1.0, 8.0:imag));
}

proc main() {
  testPrintWidths();
  testReproducer();
}
