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

  var d, k = 0;
  for i in 1..numDigits {
    do {
      do {
        k += 1;
        next_term(k);

        //
        // Continue looping until the digit is ready
        //
      } while (numer > accum);

      d = extract_digit(3);
    } while (d != extract_digit(4));       // while the 3rd digit != the 4th

    yield (d, i);                          // once it differs, yield it

    eliminate_digit(d);
  }

  proc next_term(k) {
    const k2 = 2 * k + 1;

    accum += numer * 2;
    accum *= k2;
    denom *= k2;
    numer *= k;
  }

  proc extract_digit(nth) {
    tmp1 = numer * nth;
    tmp2 = tmp1 + accum;
    tmp1 = tmp2 / denom;

    return tmp1: int;
  }

  proc eliminate_digit(d) {
    accum -= denom * d;
    accum *= 10;
    numer *= 10;
  }
}
