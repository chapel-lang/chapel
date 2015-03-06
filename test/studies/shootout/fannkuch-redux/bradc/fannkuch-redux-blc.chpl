/* The Computer Language Benchmarks Game
 * http://benchmarksgame.alioth.debian.org/
 *
 * contributed by Kyle Brady and Brad Chamberlain
 * based on C implementation by Ledrug Katz
 *
 */

config const N = 7;

assert(N >=3, "N must be in 3..");

const D = {0..N};
var s, t: [D] int;

proc main() {
  for i in D do     // TODO: Could use s = D here
    s[i] = i;
  const (checksum, maxFlips) = tk();
  writeln(checksum);
  writeln("Pfannkuchen(", N, ") = ", maxFlips);
}


proc tk() {
  var checksum, maxFlips = 0;
  var odd = false;
  var c: [D] int;

  var i = 0;
  while i < N {
    rotate(i);
    if c[i] >= i {
      c[i] = 0;
      i += 1;
      continue;
    }

    c[i] += 1;
    i = 1;
    odd = !odd;
    if s[0] {
      const f = if s[s[0]] then flip() else 1;
      if f > maxFlips then maxFlips = f;
      checksum += if odd then -f else f;
    }
  }

  return (checksum, maxFlips);
}


inline proc rotate(n) {
  const c = s[0];
  for i in 1..n {
    s[i-1] = s[i];
  }
  s[n] = c;
}


proc flip() {
  for i in D do           // TODO: Want to use t = s here
    t[i] = s[i];

  for i in (2..) {
    var x = 0;
    var y = t[0];
    while x < y {
      t[x] <=> t[y];
      x += 1;
      y -= 1;
    }
    if t[t[0]] == 0 then return i;
  }
  assert(true);
  return -1;
}
