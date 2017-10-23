
config const n = 100;

record R {
  var x : range;
}

proc <(x:R, y:R) {
  ref a = x.x;
  ref b = y.x;

  if a.length == 0 then
    return a.length < b.length;

  if a.first != b.first then
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

const one = new R(1..10);
const zero = new R(0..#10);
const empty = new R(1..0);

var recs : [1..0] R;
recs.push_back(one);
recs.push_back(zero);
recs.push_back(empty);

for r in recs {
  D.add(r);
}

for 1..n {
  for r in recs {
    assert(D.member(r));
  }
}

{
  var c = 0;
  for r in recs {
    A[r] = c;
    c += 1;
  }
}

for d in D.sorted() do
  writeln(d, " => ", A[d]);
