use BlockDist;

var dom = blockDist.createDomain(0..10);
var a: [dom] int;
var b: [dom] int;
var c = blockDist.createArray(0..10, int);

// as we can not statically say that the iterators are aligned, we never see
// `a[i]` as local. So we'll aggregate the second assignment, though we
// shouldn't.
forall (elem,i) in zip(c, a.domain) with (ref a) {
  elem = 5;
  a[i] = 1;
}

writeln(a);
writeln(c);
writeln();
