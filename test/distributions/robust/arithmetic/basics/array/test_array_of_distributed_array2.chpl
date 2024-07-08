use driver_domains;

var A1DAA1D: [Dom1D] [1..3] [Dom1D] real;

for e in A1DAA1D {
  for i in 1..3 {
    e(i) = i+next();
  }
}

for i in 1..97 do
  writeln(A1DAA1D(i)(i%3+1)(i+3));
