config const n = 100;

// Increase the number of trials to try and catch any sporadic data races
// with the forall loops in the internal module.
config const numTrials = 1000;

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
for 1..numTrials {
  var q = a + b;

  var aad = a.domain;
  var aa : [aad] int = a;
  aa |= b;

  for i in 1..n {
    assert(q.domain.member(i));
    assert(aa.domain.member(i));
    if i % 2 == 0 {
      assert(q[i] == 2);
      assert(aa[i] == 2);
    }
    else {
      assert(q[i] == 1);
      assert(aa[i] == 1);
    }
  }
}

// a and b are disjoint, so r == a
for 1..numTrials {
  var r = a - b;

  var aad = a.domain;
  var aa : [aad] int = a;
  aa -= b;

  for i in r.domain {
    assert(ad.member(i));
    assert(r[i] == a[i]);
    assert(aa[i] == a[i]);
  }
  assert(aa.domain == r.domain);
}

for 1..numTrials {
  var s = a ^ b;

  var aad = a.domain;
  var aa : [aad] int = a;
  aa ^= b;

  for i in s.domain {
    assert(q.domain.member(i));
    assert(s[i] == q[i]);
    assert(aa[i] == q[i]);
  }
  assert(aa.domain == s.domain);
}

// all indices should be less than n/2
for 1..numTrials {
  var t = a & c;

  var aad = a.domain;
  var aa : [aad] int = a;
  aa &= c;

  for i in t.domain do assert(i <= n/2);
  assert(aa.domain == t.domain);
}

for 1..numTrials {
  var u = b ^ c;

  var bbd = b.domain;
  var bb : [bbd] int = b;
  bb ^= c;
  for i in u.domain do
    if i < n/2 then assert(i % 2 == 1);
  assert(bb.domain == u.domain);
}
