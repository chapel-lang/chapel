/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org

   contributed by Tom Hildebrandt, Brad Chamberlain, Lydia Duncan
   derived from the GNU C version by Bonzini, Bartlett, and Mellor
*/

use BigInteger;

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
    for leftover..digitsPerLine do
      write(" ");
    writeln("\t:", n);
  }
}


iter gen_digits(numDigits) {
  var numer, accum, denom, tmp1, tmp2: Bigint;
  //
  // TODO: would be nice to support overloads of assignment to
  // support these as initializations:
  //
  numer = 1;
  accum = 0;
  denom = 1;

  var k = 0;
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

        tmp1 = numer * 2;
        accum += tmp1;
        accum *= y2;
        numer *= k;
        denom *= y2;

        //
        // Continue looping until the digit is ready
        //
      } while (numer > accum);

      //
      // Compute (numer * 3 + accum)
      //

      tmp1 = numer * 2;
      tmp1 += numer;
      tmp1 += accum;

      //      
      // tmp1 = tmp1 / denom; tmp2 = tmp1 % denom
      //
      tmp2 = tmp1 % denom;
      tmp1 = tmp1 / denom;

      //
      // Now, if (numer * 3 + accum) % denom + numer
      // == (numer * 4 + accum) % denom
      //
      tmp2 += numer;
    } while (tmp2 >= denom);

    //
    // compute and yield the digit, d
    //

    //
    // TODO: replace with cast
    //
    const d = tmp1.get_ui();
    yield d;

    //
    // eliminate digit d
    //
    accum = accum - denom * d;
    accum *= 10;
    numer *= 10;
  }
}
