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

  var digit, k: uint(64);

  for i in 1..numDigits {
    do {
      do {
        k += 1;
        const y2 = 2 * k + 1;

        // Compute the next term.
        accum.addmul_ui(numer, 2);
        accum.mul_ui(accum, y2);
        denom.mul_ui(denom, y2);
        numer.mul_ui(numer, k);

        // Continue looping until the digit is ready.
      } while numer.cmp(accum) > 0; // numer > accum

      digit = extractDigit(3);
    } while digit != extractDigit(4); // 3rd digit != 4th digit

    // Once 3rd and 4th digit differ, yield it.
    yield digit;

    // Eliminate digit.
    accum.submul_ui(denom, digit);
    accum.mul_ui(accum, 10);
    numer.mul_ui(numer, 10);
  }

  // Helper function to extract the nth digit.
  proc extractDigit(nth: uint) {
    tmp1.mul_ui(numer, safe_cast(c_ulong, nth));
    tmp2.add(tmp1, accum);
    tmp1.div_q(Round.ZERO, tmp2, denom);

    return tmp1.get_ui();
  }

  // Free the memory associated with these.
  delete numer;
  delete accum;
  delete denom;
  delete tmp1;
  delete tmp2;
}
