use samples;
// one defined in samples as new R(1)

var A:[1..4] R;

proc f() {
  ref B = A;
  return B;
}

proc g(x) {
  x[2] = one;
}

g(f());
writeln(A[2].x);

// Does this program output 0 or 1?
//  
// In other words, Does f() return an array alias referring to A's elements,
// or to a copy?

