use HashedDist;

config const verbose = false;

record MyMapper {
  proc this(ind, targetLocs: [] locale) {
    const numlocs = targetLocs.domain.size;
    const indAsInt = ind: int;
    return indAsInt % numlocs;
  }
}

var myMapper = new MyMapper();
var newDist = new dmap(new unmanaged Hashed(idxType=real, mapper=myMapper));

var D: domain(real) dmapped newDist;

D += 1.3;
D += 2.4;
D += 3.5;

writeln("D is:");
for d in D.sorted() {
  writeln(d);
}

var A: [D] string;

A(1.3) = "one point three";
A(2.4) = "two point four";
A(3.5) = "three point five";

writeln("A is:");
for d in D.sorted() {
  writeln(A[d]);
  if verbose then
    writeln(A[d], " on locale ", A[d].locale);
  assert(A[d].locale == Locales[myMapper(d, Locales)]);
}

writeln("Checking d locales standalone");
// standalone iterator
forall d in D {
  assert(here == Locales[myMapper(d, Locales)]);
  if verbose then writeln(d);
}
writeln("Checking d locales leader/follower");
// leader/follower iterator
forall (d1, d2) in zip(D, D) {
  assert(d1 == d2);
  assert(here == Locales[myMapper(d1, Locales)]);
  if verbose then writeln(d1, " ", d2);
}
writeln("Checking d/array locales leader/follower");
// leader/follower array iterator
forall (d, a) in zip(D, A) {
  if verbose then writeln(d, " ", a, " on ", here.id);
  assert(here == Locales[myMapper(d, Locales)]);
  assert(here == a.locale);
}
writeln("Checking array/d locales leader/follower");
// leader/follower array iterator
forall (a, d) in zip(A, D) {
  if verbose then writeln(a, " ", d, " on ", here.id);
  assert(here == Locales[myMapper(d, Locales)]);
  assert(here == a.locale);
}
