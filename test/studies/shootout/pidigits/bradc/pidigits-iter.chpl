/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org

   contributed by Tom Hildebrandt, Brad Chamberlain, and Lydia Duncan
   derived from the GNU C version by Bonzini, Bartlett, and Mellor
*/

use GMP;

config const n = 50;         // Compute n digits of pi, 50 by default

proc main() {
  param digitsPerLine = 10;

  //
  // Generate n digits, printing them in groups of digitsPerLine
  //
  for (d,i) in zip(gen_digits(n), 1..) {
    write(d);
    if i % digitsPerLine == 0 then
      writeln("\t:", i);
  }

  //
  // Pad out any trailing digits for the final line
  //
  const leftover = n%digitsPerLine;
  if (leftover) {
    for leftover..digitsPerLine-1 do
      write(" ");
    writeln("\t:", n);
  }
}


iter gen_digits(numDigits) {
  use SysCTypes;

  var numer, accum, denom, tmp1, tmp2: mpz_t;

  mpz_init_set_ui(numer, 1);               // numer = 1
  mpz_init_set_ui(accum, 0);               // accum = 0
  mpz_init_set_ui(denom, 1);               // denom = 1
  mpz_init(tmp1);                          // init tmp1
  mpz_init(tmp2);                          // init tmp2

  var k: c_ulong;
  for i in 1..numDigits {
    do {
      do {
        k += 1;
        const y2 = 2 * k + 1;

        //
        // Compute the next term
        //
        mpz_mul_2exp(tmp1, numer, 1);      // tmp1  = numer * 2
        mpz_add(accum, accum, tmp1);	   // accum += numer * 2
        mpz_mul_ui(accum, accum, y2);	   // accum *= (2k+1)
        mpz_mul_ui(numer, numer, k);       // numer *= k
        mpz_mul_ui(denom, denom, y2);      // denom *= (2k+1)

        //
        // Continue looping until the digit is ready
        //
      } while (mpz_cmp(numer, accum) > 0); // while numer > accum

      //
      // Compute (numer * 3 + accum)
      //
      mpz_mul_2exp(tmp1, numer, 1);        // tmp1 = numer * 2
      mpz_add(tmp1, tmp1, numer);          // tmp1 += numer
      mpz_add(tmp1, tmp1, accum);          // tmp1 += accum

      //      
      // tmp1 = tmp1 / denom; tmp2 = tmp1 % denom
      //
      mpz_fdiv_qr(tmp1, tmp2, tmp1, denom);

      //
      // Now, if (numer * 3 + accum) % denom + numer
      // == (numer * 4 + accum) % denom
      //
      mpz_add(tmp2, tmp2, numer);
    } while (mpz_cmp(tmp2, denom) >= 0);   // while tmp2 >= denom

    //
    // compute and yield the digit, d
    //
    const d = mpz_get_ui(tmp1);            // get least significant digit
    yield d;

    //
    // eliminate digit d
    //
    mpz_submul_ui(accum, denom, d);        // accum = (accum - denom) * d
    mpz_mul_ui(accum, accum, 10);          // accum *= 10
    mpz_mul_ui(numer, numer, 10);          // numer *= 10
  }

  // Clean up when done.
  mpz_clear(numer);
  mpz_clear(accum);
  mpz_clear(denom);
  mpz_clear(tmp1);
  mpz_clear(tmp2);
}
