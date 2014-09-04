const n = 1000;

var a, b, c : domain(int);
for i in 1..n by 2 {
  a.add(i);
  b.add(i + 1);
}
for i in 1..n/2 do c.add(i);

// contains all in 1..n
var q = a + b;
for i in 1..n do
  assert(q.member(i));

// a and b are disjoint, so r == a
var r = a - b;
assert(r == a);

// a and b are disjoint, so a&b has no elements
var s = a & b;
assert(s.size == 0);

// c contains all elements <= n/2, t should have all elements above n/2
var t = a - c;
for i in t do assert(i > n/2);

// if an element is below n/2, it should be odd
var u = b ^ c;
for i in u do
  assert(i > n/2 || (i <= n/2 && i%2 != 0));
