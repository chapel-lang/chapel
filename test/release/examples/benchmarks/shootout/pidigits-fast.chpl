/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org

   contributed by Thomas Van Doren, Michael Noakes, and Brad Chamberlain
   derived from the Chapel version by Tom Hildebrandt et al. and the
     GNU C version by Mr Ledrug
*/

use BigInteger;

config const n = 50;         // Compute n digits of pi, 50 by default

proc main() {
  param digitsPerLine = 10;

  // Generate n digits, printing them in groups of digitsPerLine
  for (d, i) in gen_digits(n) {
    write(d);
    if i % digitsPerLine == 0 then
      writeln("\t:", i);
  }

  // Pad out any trailing digits for the final line
  if n % digitsPerLine then
    writeln(" " * (digitsPerLine - n % digitsPerLine), "\t:", n);
}


iter gen_digits(numDigits) {
  var numer, denom: bigint = 1,
      accum, tmp1, tmp2: bigint;

  var i, k = 1;
  while i <= numDigits {
    next_term(k);
    k += 1;
    if numer <= accum {
      const d = extract_digit(3);
      if d == extract_digit(4) {
        yield(d, i);
        eliminate_digit(d);
        i += 1;
      }
    }
  }

  proc next_term(k) {
    const k2 = 2 * k + 1;

    accum.addmul(numer, 2);
    accum *= k2;
    denom *= k2;
    numer *= k;
  }

  proc extract_digit(nth) {
    tmp1.mul(numer, nth);
    tmp2.add(tmp1,accum);
    tmp1.div_q(tmp2, denom);

    return tmp1: int;
  }

  proc eliminate_digit(d) {
    accum.submul(denom, d);
    accum *= 10;
    numer *= 10;
  }
}
