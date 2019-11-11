/* The Computer Language Benchmarks Game
 * http://benchmarksgame.alioth.debian.org/
 *
 * contributed by Kyle Brady
 * based on C implementation by Ledrug Katz
 *
 */

use IO;

config const N: int(32) = 7;

var D: domain(1, int(32), false) = {0..15:int(32)};
var s: [D] int(32);
var t: [D] int(32);
var maxFlips: int(32);
var maxN: int(32);
var odd: bool = false;
var checksum: int(32);

proc flip() : int(32) {
  var i, c, x, y: int(32);

  i = maxN;
  while i {
    t[x] = s[y];
    x += 1;
    y += 1;
    i -= 1;
  }

  i = 1;
  do {
    x = 0;
    y = t[0];
    while x < y {
      c = t[x];
      t[x] = t[y];
      x += 1;
      t[y] = c;
      y -= 1;
    }
    i += 1;
  } while t[t[0]];

  return i;
}

inline proc rotate(n: int(32)) {
  var c = s[0];
  for i in 1..n {
    s[i-1] = s[i];
  }
  s[n] = c;
}

proc tk(n: int(32)) {
  var i, f: int(32);
  var c: [D] int(32);

  while i < n {
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
      f = if s[s[0]] then flip() else 1;
      if f > maxFlips then maxFlips = f;
      checksum += if odd then -f else f;
    }
  }
}

proc main() {
  maxN = N;
  if maxN < 3 || maxN > 15 {
    stderr.writeln("range: must be 3 <= N <= 12");
    exit(1);
  }
  for i in 0..maxN do s[i] = i;
  tk(maxN);
  writef("%i\nPfannkuchen(%i) = %i\n", checksum, maxN, maxFlips);
}
