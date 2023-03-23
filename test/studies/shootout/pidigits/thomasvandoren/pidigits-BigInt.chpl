/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org

   contributed by Tom Hildebrandt, Brad Chamberlain, Lydia Duncan,
   and Thomas Van Doren
   derived from the GNU C version by Bonzini, Bartlett, and Mellor
*/

use BigInteger;
import round; // avoid conflict with Math.round

// Compute n digits of Pi, 10 000 by default to match benchmark expectation.
config const n = 10000;

proc main() {
  param digitsPerLine = 10;

  for (digit, i) in zip(genDigits(n), 1..) {
    write(digit);
    if i % digitsPerLine == 0 then
      writeln("\t:", i);
  }

  const leftover = n%digitsPerLine;
  if (leftover) {
    for leftover..digitsPerLine do
      write(" ");
    writeln("\t:", n);
  }
}

iter genDigits(numDigits) {
  var numer = new bigint(1);
  var denom = new bigint(1);

  var accum = new bigint(0);

  var tmp1  = new bigint();
  var tmp2  = new bigint();

  var k: uint(64);

  for i in 1..numDigits {
    do {
      do {
        k += 1;
        const y2 = 2 * k + 1;

        // Compute the next term.
        mul(tmp1, numer, 2);
        add(accum, accum, tmp1);
        mul(accum, accum, y2);
        mul(numer, numer, k);
        mul(denom, denom, y2);

        // Continue looping until the digit is ready.
      } while numer.cmp(accum) > 0; // numer > accum

      // Compute: numer * 3 + accum
      mul(tmp1, numer, 3);
      add(tmp1, tmp1, accum);

      // tmp1 = tmp1 / denom; tmp2 = tmp1 % denom
      // tmp1 gets quotient, tmp2 gets remainder
      divQR(tmp1, tmp2, tmp1, denom, round.down);

      // Now, if:
      //   (numer * 3 + accum) % denom + numer == (numer * 4 + accum) + numer
      add(tmp2, tmp2, numer);
    } while tmp2.cmp(denom) >= 0; // tmp2 >= denom

    // Compute and yield the digit.
    const digit = tmp1 : uint;

    yield digit;

    // Eliminate digit.
    submul(accum, denom, digit);
    mul(accum, accum, 10);
    mul(numer, numer, 10);
  }
}
