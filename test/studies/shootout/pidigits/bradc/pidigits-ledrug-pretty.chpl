/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org

   contributed by Tom Hildebrandt, Brad Chamberlain, Lydia Duncan
   derived from the GNU C version by Ledrug
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
  var tmp1, tmp2, acc, den, num: Bigint;

  acc = 0;
  den = 1;
  num = 1;

  var d, k = 0;

  for i in 1..numDigits {
    do {
      do {
        k += 1;
        const k2 = 2 * k + 1;

        //
        // Compute the next term
        //
        acc += num * 2;
        acc *= k2;
        den *= k2;
        num *= k;

        //
        // Continue looping until the digit is ready
        //
      } while (num > acc);

      d = extract_digit(3);
    } while (d != extract_digit(4));       // while the 3rd digit, d != the 4th

    yield d;                               // once it differs, yield it

    //
    // eliminate digit d
    //
    acc = acc - den * d;
    acc *= 10;
    num *= 10;
  }

  //
  // Helper function to extract the nth digit
  //
  proc extract_digit(nth) {
    tmp1 = num * nth;
    tmp2 = tmp1 + acc;
    tmp1 = tmp2 / den;
    
    return tmp1.get_si();
  }
}


