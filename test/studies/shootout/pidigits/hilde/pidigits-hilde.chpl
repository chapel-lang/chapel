/* The Computer Language Benchmarks Game
  http://shootout.alioth.debian.org/

  contributed by Paolo Bonzini & Sean Bartlett
  modified by Michael Mellor
*/
// Translated to Chapel by Tom Hildebrandt (hilde@cray.com)


extern proc printf(s:c_string, args ...);
extern proc putchar(c:int);

use GMP;
use Time;

config const n: uint = 50;	// Compute 50 digits of Pi, usually.
config param perfTest = false;

var numer, accum, denom, tmp1, tmp2: mpz_t;

proc main
{
  var t: Timer;

  if perfTest then t.start();
  // Produce the number of digits of Pi specified in n.
  pidigits;
  if perfTest then t.stop();

  if perfTest then
    writeln("Elapsed time = ", t.elapsed(TimeUnits.seconds));
}

proc pidigits
{
  var k, i, m: uint;
  var d: int;

  mpz_init(tmp1);
  mpz_init(tmp2);
  mpz_init_set_ui(numer, 1);
  mpz_init_set_ui(accum, 0);
  mpz_init_set_ui(denom, 1);

  // extract and print digits until done.
  while (true)
  {
    // Get the next valid digit and print it.
    do {
      k += 1;
      next_term(k);
      d = extract_digit;
    } while (d == -1);

    putchar(d:int + 48);

    // Do some format control.
    i += 1;
    m = i % 10;
    if m == 0 then writeln("\t:", i);
    if i >= n then break;
    eliminate_digit(d:uint);
  }

  mpz_clear(denom);
  mpz_clear(accum);
  mpz_clear(numer);
  mpz_clear(tmp2);
  mpz_clear(tmp1);
}

proc next_term(k:uint)
{
  var y2_tmp:uint = 2 * k + 1,
    y2 = y2_tmp.safeCast(c_ulong),
    ck = k.safeCast(c_ulong);

  mpz_mul_2exp(tmp1, numer, 1:c_ulong);
  mpz_add(accum, accum, tmp1);		// accum <- accum + numer * 2
  mpz_mul_ui(accum, accum, y2);		// accum <- accum * (2k+1)
  mpz_mul_ui(numer, numer, ck);		// numer <- numer * k
  mpz_mul_ui(denom, denom, y2);		// denom <- denom * (2k+1)
}

proc extract_digit : int
{
  if mpz_cmp(numer, accum) > 0 then return -1;	// No digits yet.

  // Compute (numer * 3 + accum)
  mpz_mul_2exp(tmp1, numer, 1);
  mpz_add(tmp1, tmp1, numer);
  mpz_add(tmp1, tmp1, accum);

  // tmp1 <- tmp1 / denom; tmp2 <- tmp1 % denom
  mpz_fdiv_qr(tmp1, tmp2, tmp1, denom);

  // Now, if (numer * 3 + accum) % denom + numer
  // == (numer * 4 + accum) % denom
  mpz_add(tmp2, tmp2, numer);
  if mpz_cmp(tmp2, denom) >= 0 then return -1;

  return mpz_get_ui(tmp1) : int;
}

proc eliminate_digit(d: uint)
{
  mpz_submul_ui(accum, denom, d.safeCast(c_ulong));
  mpz_mul_ui(accum, accum, 10);
  mpz_mul_ui(numer, numer, 10);
}
