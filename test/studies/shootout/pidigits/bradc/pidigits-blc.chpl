/* The Computer Language Benchmarks Game 
 * https://benchmarksgame-team.pages.debian.net/benchmarksgame/index.html
 *
 * Contributed by Brad Chamberlain and Jatin Garg
 * Derived from James Wright's C and Lew Palm's C++ multi-threaded versions
 */

use BigInteger;

config var n = 27;

var q, r, t: bigint;
var tCalculating, qCalculating, extractCalculating: atomic bool;
var k, doubleK, qMultiplicand, digit: int;

proc main() {
  cobegin {
    tMultiplier();
    qMultiplier();
    extractor();
    computePi();
  }
}

proc tMultiplier() {
  multiplier(t, doubleK, tCalculating);
}

proc qMultiplier() {
  multiplier(q, qMultiplicand, qCalculating);
}

proc multiplier(ref result, ref multiplicand, waitCond) {
  while true {
    waitCond.waitFor(true);

    result *= multiplicand;

    waitCond.write(false);
  }
}

proc extractor() {
  var tmp1, tmp2: bigint;
  
  while true {
    extractCalculating.waitFor(true);

    tmp1.mul(q, 3);
    tmp1 += r;
    tmp2.divQ(tmp1, t);
    digit = tmp2: int;

    extractCalculating.write(false);
  }
}

proc computePi() {
  q = 1;
  t = 1;
  
  var temp1, temp2: bigint;
  var tPrecalculation = false;
  var nDigits = 0;
  
  while nDigits < n {
    var i = 0;
    while i < 10 && nDigits < n {
      if !tPrecalculation {
        k += 1;
        doubleK = 2*k + 1;
        tCalculating.write(true); // Unblock 't *= doubleK' task
      } else {
        tPrecalculation = false;
      }
      qCalculating.waitFor(false); // Wait for 'q *= 10' to finish (if it runs)
      temp1.add(q,q);

      qMultiplicand = k;
      qCalculating.write(true);  // Unblock task for 'q *= qMultiplicand'

      temp1 += r;
      r.mul(temp1,doubleK);

      qCalculating.waitFor(false); // Wait for 'q *= qMultiplicand' to finish
      tCalculating.waitFor(false); // Wait for 't *= doubleK' to finish (if...)
      if q <= r {
        extractCalculating.write(true);  // Unblock extraction task

        temp1.mul(q,4);
        temp1 += r;

        temp2.divQ(temp1, t);
        const digit2 = temp2: int;

        extractCalculating.waitFor(false); // Wait for extraction task

        if digit == digit2 {
          qMultiplicand = 10;
          qCalculating.write(true);  // Unblock task for 'q *= 10'

          write(digit);
          temp1.mul(t, digit);

          k += 1;
          doubleK = 2*k + 1;

          tCalculating.write(true);  // Unblock 't *= doubleK' task
          tPrecalculation = true;

          temp1.sub(r, temp1);
          r.mul(temp1, 10);
          
          i += 1;
          nDigits += 1;
        }
      }
    }
    if i != 10 then
      write(" "*(10-i));
    writeln("\t:", nDigits);
  }
  exit(0);
}
