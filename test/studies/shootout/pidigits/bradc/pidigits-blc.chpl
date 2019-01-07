/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org

   contributed by Brad Chamberlain
   derived from the Julia version by Jarret Revels
*/

use BigInteger;

config const n = 50;      // Compute n digits of pi, 50 by default

var i, k, ns = 0,
    k1 = 1,
    num, den = 1: bigint,
    a, t, u: bigint;

while true {
  k += 1;
  t = num * 2;
  num *= k;
  a += t;
  k1 += 2;
  a *= k1;
  den *= k1;

  if a >= num {
    t.div_qr(u, 3*num + a, den);
    u += num;
    if den > u {
      ns = 10*ns + t:int;
      i += 1;
      if i >= n then
        break;
      if i % 10 == 0 {
        writef("%010i\t:%i\n", ns, i);
        ns = 0;
      }
      a -= den*t;
      a *= 10;
      num *= 10;
    }
  }
}
const s = "%0*i".format((i%10), ns);
writef("%-10s\t:%i\n", s, i);
