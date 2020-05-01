/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org

   contributed by Tom Hildebrandt, Brad Chamberlain, Lydia Duncan
   derived from the GNU C version by Bonzini, Bartlett, and Mellor
*/

use GMP, GMPops;

config const n = 50;                // Compute n digits of Pi, 50 by default

proc main() {
  param digitsPerLine = 10;

  for (d,i) in zip(gen_digits(n), 1..) {
    write(d);
    if i % digitsPerLine == 0 then
      writeln("\t:",i);
  }

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
  //
  // TODO: would be nice to support overloads of assignment to
  // support these as initializations:
  //
  mpz_init_set_ui(numer, 1);               // numer = 1
  mpz_init_set_ui(accum, 0);               // accum = 0
  mpz_init_set_ui(denom, 1);               // denom = 1
  mpz_init(tmp1);                          // init tmp1
  mpz_init(tmp2);                          // init tmp2

  var k: c_ulong;
  // This needs to be a c_ulong for portability to 32 bit systems.
  // TODO: make our GMP methods accept uints, ints, etc. of various sizes so
  // that the user doesn't need to know C types?
  for i in 1..numDigits {
    do {
      do {
        k += 1;
        const y2 = 2 * k + 1;

        //
        // Compute the next term
        //

        //
        // TODO: This one is difficult to do with operator overloads
        // without using an additional mpz_tmp
        //
        mpz_mul_2exp(tmp1, numer, 1);      // tmp1  = numer * 2
        accum += tmp1;
        accum *= y2;
        numer *= k;
        denom *= y2;

        //
        // Continue looping until the digit is ready
        //

        //
        // TODO: Can't overload > because it's ambiguous with tuple's > (?)
        //
        // } while (numer > accum);
      } while (mpz_cmp(numer, accum) > 0); // while numer > accum

      //
      // Compute (numer * 3 + accum)
      //

      //
      // TODO: This one is difficult to do with operator overloads
      // without using an additional mpz_tmp
      //
      mpz_mul_2exp(tmp1, numer, 1);        // tmp1 = numer * 2
      tmp1 += numer;
      tmp1 += accum;

      //      
      // tmp1 = tmp1 / denom; tmp2 = tmp1 % denom
      //
      mpz_fdiv_qr(tmp1, tmp2, tmp1, denom);

      //
      // Now, if (numer * 3 + accum) % denom + numer
      // == (numer * 4 + accum) % denom
      //
      tmp2 += numer;

      //
      // TODO: See previous loop
      //
      //    } while (tmp2 >= denom);
    } while (mpz_cmp(tmp2, denom) >= 0);   // while tmp2 >= denom

    //
    // compute and yield the digit, d
    //

    //
    // TODO: replace with cast
    //
    const d = mpz_get_ui(tmp1);            // get least significant digit
    yield d;

    //
    // eliminate digit d
    //
    mpz_submul_ui(accum, denom, d);        // accum = (accum - denom) * d
    accum *= 10;
    numer *= 10;
  }

  //
  // Free memory associated with multi-precision values
  //
  mpz_clear(tmp2);
  mpz_clear(tmp1);
  mpz_clear(denom);
  mpz_clear(accum);
  mpz_clear(numer);
}
