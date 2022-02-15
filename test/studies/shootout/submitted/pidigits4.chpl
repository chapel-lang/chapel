/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Tom Hildebrandt, Brad Chamberlain, and Lydia Duncan
   derived from the GNU C version by Mr Ledrug
*/

use GMP, SysCTypes;

config const n = 50;         // Compute n digits of pi, 50 by default

proc main() {
  param digitsPerLine = 10;

  // Generate n digits, printing them in groups of digitsPerLine
  for (d, i) in genDigits(n) {
    write(d);
    if i % digitsPerLine == 0 then
      writeln("\t:", i);
  }

  // Pad out any trailing digits for the final line
  if n % digitsPerLine then
    writeln(" " * (digitsPerLine - n % digitsPerLine), "\t:", n);
}


iter genDigits(numDigits) {
  var numer, accum, denom, tmp1, tmp2: mpz_t;

  mpz_init(tmp1);                          // init tmp1
  mpz_init(tmp2);                          // init tmp2
  mpz_init_set_ui(accum, 0);               // accum = 0
  mpz_init_set_ui(denom, 1);               // denom = 1
  mpz_init_set_ui(numer, 1);               // numer = 1

  var d, k: c_ulong;
  for i in 1..numDigits {
    do {
      do {
        k += 1;
        nextTerm(k);

        // Continue looping until the digit is ready
      } while (mpz_cmp(numer, accum) > 0);  // while numer > accum

      d = extractDigit(3);
    } while (d != extractDigit(4));       // while the 3rd digit != the 4th

    yield (d,i);              // once it differs, yield it and its position

    eliminateDigit(d);
  }

  // Helper function to compute the next term
  proc nextTerm(k) {
    const k2 = 2 * k + 1;

    mpz_addmul_ui(accum, numer, 2);    // accum += numer * 2
    mpz_mul_ui(accum, accum, k2);      // accum *= (2k+1)
    mpz_mul_ui(denom, denom, k2);      // denom *= (2k+1)
    mpz_mul_ui(numer, numer, k);       // numer *= k
  }

  // Helper function to extract the nth digit
  proc extractDigit(nth: c_ulong) {
    mpz_mul_ui(tmp1, numer, nth);            // tmp1 = numer * nth
    mpz_add(tmp2, tmp1, accum);              // tmp2 = tmp1 + accum
    mpz_tdiv_q(tmp1, tmp2, denom);           // tmp1 = tmp2 / denom
    
    return mpz_get_ui(tmp1);                 // convert tmp1 to a c_uint
  }

  // Helper function to eliminate digit d
  proc eliminateDigit(d) {
    mpz_submul_ui(accum, denom, d);        // accum = (accum - denom) * d
    mpz_mul_ui(accum, accum, 10);          // accum *= 10
    mpz_mul_ui(numer, numer, 10);          // numer *= 10
  }

  // Clean up when done
  mpz_clear(numer);
  mpz_clear(denom);
  mpz_clear(accum);
  mpz_clear(tmp2);
  mpz_clear(tmp1);
}


