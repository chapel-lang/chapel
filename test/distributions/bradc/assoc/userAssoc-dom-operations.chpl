use HashedDist;

config const verbose = false;

var newDist = new dmap(new Hashed(idxType=real));

var D: domain(real) dmapped newDist;

D += 1.1;
D += 2.2;
D += 3.3;
D += 3.5;
D += 4.4;
D += 5.5;
D += 6.6;
D += 7.7;
D += 8.8;


D -= 3.5;

writeln("D is:");
for d in D.sorted() {
  writeln(d);
}

assert(D.contains(1.1));
assert(!D.contains(3.5));
assert(!D.contains(100.0));

var A: [D] string;

A(1.1) = "one point one";
A(2.2) = "two point two";
A(3.3) = "three point three";
A(4.4) = "four point four";
A(5.5) = "five point five";
A(6.6) = "six point six";
A(7.7) = "seven point seven";
A(8.8) = "eight point eight";

writeln("A is:");
for d in D.sorted() {
  writeln(d, ": ", A[d]);
  if verbose then
    writeln(A[d], " on locale ", A[d].locale);
}
writeln();

D += 9.9;
writeln("after adding 9.9 to D, A is:");
for d in D.sorted() {
  writeln(d, ": ", A[d]);
  if verbose then
    writeln(A[d], " on locale ", A[d].locale);
}

writeln();

D.clear();

assert(!D.contains(9.9));
writeln("after D.clear, A is:");
for d in D.sorted() {
  writeln(A[d]);
  if verbose then
    writeln(A[d], " on locale ", A[d].locale);
}

