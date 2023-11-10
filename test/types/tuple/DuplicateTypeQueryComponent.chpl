use Math;

// OK! Triggers redefinition error.
proc foo(tup : (int(?w), int(?w))) { writeln(w); }

// Should also trigger redefinition error.
proc foo((x, y): (real(?w), real(?w))) {
  writeln(w);
}

// error: unable to resolve type
//inline proc cmplx((x , y)) where isRealType(x.type) && isImagType(y.type)
//{
//    return x * exp(y);
//}

// edit from BLC from the notes below:  Here's another case that doesn't work, but should:
//
// error: w and _w undefined in routine:
// proc foo((r, s) : (real(?w), imag(?_w))) {
//   if w != _w then compilerError("real and imag argument sizes must match!");
//   var c: complex(2*w) = (r, s): complex;
//  return c;
// }

proc main() {
  foo((0, 8));
  foo((0.0, 8.0));
}
