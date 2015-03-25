/* The Computer Language Benchmarks Game
   http://shootout.alioth.debian.org/

   contributed by Tom Hildebrandt, Brad Chamberlain, Lydia Duncan
   derived from the GNU C version by Bonzini, Bartlett, and Mellor
*/

use GMP;

config const n = 50;                // Compute n digits of Pi, 50 by default

var numer, accum, denom, tmp1, tmp2: mpz_t;
//
// TODO: would be nice to support initialization rules for external types
// and then to implement overloads to support these cases more directly
//
mpz_init_set_ui(numer, 1);                 // numer = 1
mpz_init_set_ui(accum, 0);                 // accum = 0
mpz_init_set_ui(denom, 1);                 // denum = 1
mpz_init(tmp1);                            // init tmp1
mpz_init(tmp2);                            // init tmp2


proc main() {
  var k: c_ulong;
  // This needs to be a c_ulong for portability to 32 bit systems.
  // TODO: make our GMP methods accept uints, ints, etc. of various sizes so
  // that the user doesn't need to know C types?

  //
  // extract and print n digits
  //
  for i in 1..n {
    const d = next_digit(k);
    write(d);
    eliminate_digit(d);

    if i % 10 == 0 then
      writeln("\t:", i);
  }

  //
  // Free memory associated with multi-precision valuesx
  //
  mpz_clear(tmp2);
  mpz_clear(tmp1);
  mpz_clear(denom);
  mpz_clear(accum);
  mpz_clear(numer);
}


proc next_digit(inout k) {
  do {
    do {
      k += 1;
      const y2 = 2 * k + 1;

      mpz_mul_2exp(tmp1, numer, 1);        // tmp1  = numer * 2
      mpz_add(accum, accum, tmp1);         // accum += numer * 2
      mpz_mul_ui(accum, accum, y2);        // accum *= (2k+1)
      mpz_mul_ui(numer, numer, k);         // numer *= k
      mpz_mul_ui(denom, denom, y2);        // denom *= (2k+1)
    } while (mpz_cmp(numer, accum) > 0);   // while numer > accum

    //
    // Compute (numer * 3 + accum)
    //
    mpz_mul_2exp(tmp1, numer, 1);          // tmp1 = numer * 2
    mpz_add(tmp1, tmp1, numer);            // tmp1 += numer
    mpz_add(tmp1, tmp1, accum);            // tmp1 += accum

    //
    // tmp1 = tmp1 / denom; tmp2 = tmp1 % denom
    //
    mpz_fdiv_qr(tmp1, tmp2, tmp1, denom);

    //
    // Now, if (numer * 3 + accum) % denom + numer
    // == (numer * 4 + accum) % denom
    //
    mpz_add(tmp2, tmp2, numer);
  } while (mpz_cmp(tmp2, denom) >= 0);     // while tmp2 >= denom

  return mpz_get_ui(tmp1);                 // return least significant digit
}


proc eliminate_digit(d) {
  mpz_submul_ui(accum, denom, d);           // accum = (accum - denom) * d
  mpz_mul_ui(accum, accum, 10);             // accum *= 10
  mpz_mul_ui(numer, numer, 10);             // numer *= 10
}
