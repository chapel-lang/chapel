/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Brad Chamberlain
   derived from the Julia version by Jarret Revels
*/

use BigInteger;

config const n = 50;      // Compute n digits of pi, 50 by default

var i, ns, x = 0,
    num, den = 1: bigint,
    a, t, u: bigint;

for k in (1..) {
  t = num * 2;
  num *= k;
  a += t;
  const k1 = 2*k + 1;
  a *= k1;
  den *= k1;

  if a >= num {
    t.div_qr(u, 3*num + a, den);
    u += num;
    if den > u {
      ns = 10*ns + t:int;
      i += 1;
      x = i % 10;
      if x == 0 {
        writef("%010i\t:%i\n", ns, i);
        ns = 0;
      }
      if i == n {
        break;
      }
      a -= den*t;
      a *= 10;
      num *= 10;
    }
  }
}
if x != 0 then
  writef("%0*i%s\t:%i\n", x, ns, " "*(10-x), i);
