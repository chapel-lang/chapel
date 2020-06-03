use Map;

config const n = 100;

// Increase the number of trials to try and catch any sporadic data races
// with the forall loops in the internal module.
config const numTrials = 1000;

//var ad, bd, cd : domain(int);
var a : map(int, int, false);
var b : map(int, int, false);
var c : map(int, int, false);
for i in 1..n by 2 {
  a.add(i, 1);
  b.add(i + 1, 2);
}
for i in 1..n/2 do c.add(i, 3);

// contains all in 1..n
var q = a + b;
for 1..numTrials {
  var q = a + b;

  var aa = a;
  aa |= b;

  for i in 1..n {
    assert(q.contains(i));
    assert(aa.contains(i));
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

  var aa = a;
  aa -= b;

  for i in r {
    assert(a.contains(i));
    assert(r[i] == a[i]);
    assert(aa[i] == a[i]);
  }
  assert(aa == r);
}

for 1..numTrials {
  var s = a ^ b;

  var aa = a;
  aa ^= b;

  for i in s {
    assert(q.contains(i));
    assert(s[i] == q[i]);
    assert(aa[i] == q[i]);
  }
  assert(aa == s);
}

// all indices should be less than n/2
for 1..numTrials {
  var t = a & c;

  var aa = a;
  aa &= c;

  for i in t do assert(i <= n/2);
  assert(aa == t);
}

for 1..numTrials {
  var u = b ^ c;

  var bb = b;
  bb ^= c;
  for i in u do
    if i < n/2 then assert(i % 2 == 1);
  assert(bb == u);
}
