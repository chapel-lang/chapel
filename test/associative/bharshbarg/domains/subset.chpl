config const n = 1000;

var all : domain(int) = 1..n;

assert(all.isSubset(all));
assert(all.isSuper(all));

{
  var sub : domain(int) = 1..n by 5;
  assert(sub.isSubset(all));
  assert(all.isSuper(sub));
}

{
  var sub : domain(int) = 1..n;
  sub += n..#10;
  assert(!sub.isSubset(all));
  assert(sub.isSuper(all));
}

{
  var sub : domain(int) = 1..n/2;
  assert(sub.isSubset(all));
  assert(all.isSuper(sub));
  assert(!sub.isSuper(all));
}
