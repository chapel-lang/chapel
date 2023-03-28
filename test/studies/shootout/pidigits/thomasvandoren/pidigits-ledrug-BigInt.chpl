/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org

   contributed by Tom Hildebrandt, Brad Chamberlain, Lydia Duncan,
   and Thomas Van Doren
   derived from the GNU C version by Bonzini, Bartlett, and Mellor
*/

use BigInteger;

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

  var digit: uint(64);
  var k:     uint(64);

  for i in 1..numDigits {
    do {
      do {
        k += 1;
        const y2 = 2 * k + 1;

        // Compute the next term.
        accum.addmul(numer, 2);
        accum.mul(accum, y2);
        denom.mul(denom, y2);
        numer.mul(numer, k);

        // Continue looping until the digit is ready.
      } while numer.cmp(accum) > 0; // numer > accum

      digit = extractDigit(3);
    } while digit != extractDigit(4); // 3rd digit != 4th digit

    // Once 3rd and 4th digit differ, yield it.
    yield digit;

    // Eliminate digit.
    accum.submul(denom, digit);
    accum.mul(accum, 10);
    numer.mul(numer, 10);
  }

  // Helper function to extract the nth digit.
  proc extractDigit(nth: uint) {
    tmp1.mul(numer, nth);
    tmp2.add(tmp1, accum);
    tmp1.divQ(tmp2, denom);

    return tmp1 : uint;
  }
}
