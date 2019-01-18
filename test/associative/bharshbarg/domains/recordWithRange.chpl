
config const n = 100;

record R {
  var x : range;
}

proc <(x:R, y:R) {
  ref a = x.x;
  ref b = y.x;

  if a.length == 0 && b.length != 0 then
    return true;
  else if a.length != 0 && b.length == 0 then
    return false;
  else if a.first != b.first then
    return a.first < b.first;
  else if a.stride != b.stride then
    return a.stride < b.stride;
  else if a.alignment != b.alignment then
    return a.alignment < b.alignment;
  else if a.last != b.last then
    return a.last < b.last;
  else
    return false;
}

var D : domain(R);
var A : [D] int;

var recs : [1..0] R;
recs.push_back(new R(1..10));
recs.push_back(new R(0..#10));
recs.push_back(new R(1..0));
recs.push_back(new R(0..-1));

for r in recs {
  D.add(r);
}
assert(D.size == recs.size);

// Verify that each record is present in the domain.
for 1..n {
  for r in recs {
    assert(D.contains(r));
  }
}

{
  var c = 0;
  for r in recs {
    A[r] = c;
    c += 1;
  }

  // Ensure that we didn't somehow implicitly add new indices through 'A[r] ='
  assert(D.size == recs.size);
}

for d in D.sorted() do
  writeln(d, " => ", A[d]);
