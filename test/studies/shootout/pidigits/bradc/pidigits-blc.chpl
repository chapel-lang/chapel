/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Brad Chamberlain
   derived from the Julia version by Jarret Revels
*/

use BigInteger;

config const n = 50;      // Compute n digits of pi, 50 by default

var i, ns = 0,
    num, den = 1: bigint,
    a, t, u: bigint;

for (k, k1) in zip(1..,3.. by 2) {
  t = num * 2;
  num *= k;
  a += t;
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
