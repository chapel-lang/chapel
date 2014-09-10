config const n = 20;

var ad, bd, cd : domain(int);
var a : [ad] int;
var b : [bd] int;
var c : [cd] int;
for i in 1..n by 2 {
  ad.add(i);
  bd.add(i + 1);
}
for i in 1..n/2 do cd.add(i);
a = 1;
b = 2;
c = 3;

// contains all in 1..n
var q = a + b;
for i in 1..n {
  assert(q.domain.member(i));
  if i % 2 == 0 then assert(q[i] == 2);
  else assert(q[i] == 1);
}

// a and b are disjoint, so r == a
var r = a - b;
for i in r.domain {
  assert(ad.member(i));
  assert(r[i] == a[i]);
}

var s = a ^ b;
for i in s.domain {
  assert(q.domain.member(i));
  assert(s[i] == q[i]);
}

// all indices should be less than n/2
var t = a & c;
for i in t.domain do assert(i <= n/2);

var u = b ^ c;
for i in u.domain do
  if i < n/2 then assert(i % 2 == 1);
