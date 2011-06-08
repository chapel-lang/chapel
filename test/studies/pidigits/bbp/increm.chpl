// bbp.chpl -- Compute hex digits of Pi using the Bailey, Borwein Plouffe formula.
//
// This is an implementation which uses only integer arithmetic.
// This version computes a starting point from scratch and then proceeds incrementally.
// The memory cost is proportional to the last digit computed, while the cost to produce
// the next digit is proportional to the index of the digit itself.
//

use Time;

config param perfTest = false;

type I1 = int; // Handy typedef so we can substitute int(64) later.
type I2 = int(64);  // A double-length integer

config const start : I1 = 1;	// The starting (fractional) digit.
	// Digits are number 0, 1, 2, where 0 is the units position
	// Digit 1 is the hex digit in just to the right of the hexadecimal point, etc.
config const count : I1 = 	// How many digits to compute and print.
  if perfTest then 10000 else 1000;

const end = start + count - 1;

// Theoretically, we need 3 + lg(10000) or ~17 bits of guard digits, 
// plus 4 times the number of Fs (for carries) or zeroes (for borrows) 
// that happen to lie to the left of those.
// We can use probabilistic arguments, or we can just run the experiment 
// (setting logSpill to "true") and feed the result back in.
// Up to 10000, the max spill was 6, and we add one for safety.
const extend = end + 7;	// We need 7 guard digits at 10000.

// The array of digits which we are computing.
// Each digit consumes only 4 bits.
var digits: [start..extend] int(8);

// At the beginning of the pass for digit d,
// contains the remainders to be used in calculating the current digit.
// The size of a remainder is bounded by the size of the divisor.
// For digit d, that is 8 * d + coeff[c] -- about 3 bits more than lg(d).
var remainders: [0..extend, 1..4] I1;

proc main
{
  var t: Timer;
  if perfTest then t.start();

  start_digit(start);
  for d in start .. extend do
  {
    var nd = next_digit(d);
    update_digits(d, nd);
  }

  if perfTest then
  {
    t.stop();
    writeln("Elapsed time = ", t.elapsed(TimeUnits.seconds));
  }

  write_out();
}

const coeff: [1..4] int = (4, -2, -1, -1);
const offset: [1..4] int = (1, 4, 5, 6);

proc start_digit(s: I1)
{
  // s -- The digit number of the starting digit.
  // Compute the contribution of each fraction to the starting digit s.
  // We actually just set up the remainders for the digit preceding s.
  // Then, when we run the normal incremental algorithm, we get the
  // starting digit we want.

  // f is the fraction index.
  // c is the coefficient index
  for c in 1..4 do
    forall f in 0..s-1 do
      remainders[f,c] = mod_exp(16, s-1-f, 8 * f + offset[c]);
}

proc next_digit(n: I1) : int
{
  return + reduce([c in 1..4] next_digit(n, c) * coeff[c]);
}

proc next_digit(n: I1, c: int)
{
  var result = 0;
  for f in 0..n-1 do
  {
    var dd = remainders[f,c] * 16;	// Dividend
    var d = 8 * f + offset[c];		// Divisor
    var (q, r) = (dd / d, dd % d);
    remainders[f, c] = r;
    result += q;
  }
  remainders[n, c] = 1;
  return result;
}

////////////////////////////////////////////////////////////////////////
// Compute (b**e) mod m.
//
// Uses the equivalence:
// (b**e) mod m ==
// $\left( \Prod{i=0}{lg e} \left[ b^{e[i] 2^i} mod m \right] \right) mod m$,
// where e[i] is the bit i in the exponent e.
//
proc mod_exp(base : int(8), in exp : I1, mod : I1)
{
  var result : I1 = 1;
  var factor : I1 = base;
  while (exp>0 && result != 0)
  {
    // Use double-length arithmetic for the updates.
    if (exp & 1) then
      result = (result : I2 * factor % mod) : I1;
    factor = (factor : I2 * factor % mod) : I1;
    exp >>= 1;
  }
  return result;
}

// This lets us compute how many digits to allow for carries/borrows
// out of our next-digit
config param logSpill = false;
var maxSpill = 0;

proc update_digits(in d, in nd: int)
{
  var spillCount = 0;
  while (nd != 0 && d >= start)
  {
    if logSpill then spillCount += 1;

    nd += digits[d];
    var digit:int(8);
    (nd, digit) = (nd / 16, (nd % 16):int(8));
    if digit < 0 then (nd, digit) = (nd-1, (digit+16):int(8));	// Borrow if necessary.
    digits[d] = digit;
    d -= 1;
  }
  if logSpill then
    if spillCount > maxSpill then
      maxSpill = spillCount;
}

proc write_out()
{
  if start == 1 then write ("3.");
  for d in start..end do
  {
    write_digit(digits[d]);
    if d % 10 == 0 then write("\n  ");
  }
  writeln();
  if logSpill then writeln("Max spill = ", maxSpill);
}

proc write_digit(d:int(8))
{
  select d
  {
    when 10 do write("A");
    when 11 do write("B");
    when 12 do write("C");
    when 13 do write("D");
    when 14 do write("E");
    when 15 do write("F");
    otherwise write(d);
  }
}

