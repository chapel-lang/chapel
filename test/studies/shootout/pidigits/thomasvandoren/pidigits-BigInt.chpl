/* The Computer Language Benchmarks Game
   http://shootout.alioth.debian.org/

   contributed by Tom Hildebrandt, Brad Chamberlain, Lydia Duncan,
   and Thomas Van Doren
   derived from the GNU C version by Bonzini, Bartlett, and Mellor
*/

use GMP;

// Compute n digits of Pi, 10 000 by default to match benchmark expectation.
config const n = 10000;

proc main() {
  for (digit, i) in zip(genDigits(n), 1..) {
    write(digit);
    if i % 10 == 0 then
      writeln("\t:", i);
  }
}

iter genDigits(numDigits) {
  var numer = new BigInt(1),
    accum = new BigInt(0),
    denom = new BigInt(1),
    tmp1 = new BigInt(),
    tmp2 = new BigInt();

  var k: uint(64);

  for i in 1..numDigits {
    do {
      do {
        k += 1;
        const y2 = 2 * k + 1;

        // Compute the next term.
        tmp1.mul_ui(numer, 2);
        accum.add(accum, tmp1);
        accum.mul_ui(accum, y2);
        numer.mul_ui(numer, k);
        denom.mul_ui(denom, y2);

        // Continue looping until the digit is ready.
      } while numer.cmp(accum) > 0; // numer > accum

      // Compute: numer * 3 + accum
      tmp1.mul_ui(numer, 3);
      tmp1.add(tmp1, accum);

      // tmp1 = tmp1 / denom; tmp2 = tmp1 % denom
      // tmp1 gets quotient, tmp2 gets remainder
      tmp1.div_qr(Round.DOWN, tmp2, tmp1, denom);

      // Now, if:
      //   (numer * 3 + accum) % denom + numer == (numer * 4 + accum) + numer
      tmp2.add(tmp2, numer);
    } while tmp2.cmp(denom) >= 0; // tmp2 >= denom

    // Compute and yield the digit.
    const digit = tmp1.get_ui();
    yield digit;

    // Eliminate digit.
    accum.submul_ui(denom, digit);
    accum.mul_ui(accum, 10);
    numer.mul_ui(numer, 10);
  }

  // Free the memory associated with these.
  delete numer;
  delete accum;
  delete denom;
  delete tmp1;
  delete tmp2;
}
