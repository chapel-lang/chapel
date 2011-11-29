proc foo(r: real) {
  writeln("coercion incorrectly made");
}

param im: imag = 1.1i;
foo(im);

