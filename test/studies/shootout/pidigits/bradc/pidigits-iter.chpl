/* The Computer Language Benchmarks Game
   http://shootout.alioth.debian.org/

   contributed by Tom Hildebrandt and Brad Chamberlain
   initial Chapel version derived from GNU C version (Bonzini,
   Bartlett, Mellor).
*/

use GMP;

config const n = 50;	// Compute 50 digits of Pi, by default

proc main() {
  // Produce the number of digits of Pi specified in n.
  pidigits();
}


proc pidigits() {
  for (d,i) in zip(gen_digits(n), 1..) {
    write(d);
    if i % 10 == 0 then
      writeln("\t:",i);
  }
}


iter gen_digits(numDigits) {
  var numer, accum, denom, tmp1, tmp2: mpz_t;

  // TODO: would be nice to support overloads of assignment to
  // support these as initializations:
  //
  //   numer = 1;
  //   accum = 0;
  //   denom = 1;
  //
  mpz_init(tmp1);
  mpz_init(tmp2);
  mpz_init_set_ui(numer, 1);
  mpz_init_set_ui(accum, 0);
  mpz_init_set_ui(denom, 1);

  var k: uint;
  for i in 1..numDigits {
    do {
      do {
        k += 1;
        const y2 = 2 * k + 1;

        // Compute the next term
        mpz_mul_2exp(tmp1, numer, 1:c_ulong);
        mpz_add(accum, accum, tmp1);		// accum <- accum + numer * 2
        mpz_mul_ui(accum, accum, y2);		// accum <- accum * (2k+1)
        mpz_mul_ui(numer, numer, k);		// numer <- numer * k
        mpz_mul_ui(denom, denom, y2);		// denom <- denom * (2k+1)

        // Extract the digit if it's ready
      } while (mpz_cmp(numer, accum) > 0);

      // Compute (numer * 3 + accum)
      mpz_mul_2exp(tmp1, numer, 1);
      mpz_add(tmp1, tmp1, numer);
      mpz_add(tmp1, tmp1, accum);
      
      // tmp1 <- tmp1 / denom; tmp2 <- tmp1 % denom
      mpz_fdiv_qr(tmp1, tmp2, tmp1, denom);

      // Now, if (numer * 3 + accum) % denom + numer
      // == (numer * 4 + accum) % denom
      mpz_add(tmp2, tmp2, numer);
    } while (mpz_cmp(tmp2, denom) >= 0);

    // compute and yield the digit
    const d = mpz_get_ui(tmp1);
    yield d;

    // eliminate digit d
    mpz_submul_ui(accum, denom, d);
    mpz_mul_ui(accum, accum, 10);
    mpz_mul_ui(numer, numer, 10);
  }
}
