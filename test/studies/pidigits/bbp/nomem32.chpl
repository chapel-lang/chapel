// nomem32.chpl
//
// Compute hex digits of Pi using the Bailey, Borwein Plouffe formula.
//
// This version attempts to minimize storage requirements (NO MEMory),
// instead recalculating every remainder from scratch in each iteration.
// It uses a single-precision integer size of 32 bits, and double-precision of 64 bits,
// so it can compute only the first 4 million digits of Pi or so.
//
//
use Time;

config param perfTest = false;

type I1 = int; // Handy typedef so we can substitute int(64) later.
type I2 = int(64); // A double-length integer

param bitsPerHex = 4;
config param blkSz = 7;
param hex = 2**bitsPerHex;
param base = hex ** blkSz;

config const start : I1 = 1;	// The starting (fractional) digit.
	// Digits are number 0, 1, 2, where 0 is the units position
	// Digit 1 is the hex digit in just to the right of the hexadecimal point, etc.
config const count : I1 = 	// How many digits to compute and print.
  if perfTest then 10000 else 1000;

// We add an extra two blocks as guard digits 
// (because the alignedHigh bound of the range can be as small as end+blkSz+1).
const end = start + count - 1;

config const debug = 0;

var digitDom : domain(1) = {start..end+2*blkSz};

// The array of digits which we are computing.
// Each digit consumes only 4 bits.
var digits: [digitDom] int(8);

proc main
{
  var t: Timer;
  if perfTest then t.start();

  for n in digitDom.dim(0) by blkSz align 0 do
  {
    var nd = next_block(n);
    if debug > 0 then {write("= ");print_digits(n, nd);writeln();}
    update_digits(n, nd);
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

// n is the index of the current digit (and the last fraction).
proc next_block(n: I1) : I2
  return + reduce([c in 1..4] next_block(n, offset[c]) * coeff[c]);

// n is the last index of the block.
proc next_block(n: I1, offset) : I2
{
  var result:I2 = 0;
  for f in 0..n do
  {
    var i = n-f;
    var (block, offs) = (i / blkSz, i % blkSz);
    var d = 8 * f + offset;		// Divisor
    var frac = frac_place(hex**offs, d, block, base);
    if debug>1 then {write("+ ");print_digits(n, frac); writeln((block, offs));}
    result += frac;
  }
  return result;
}

// Returns place p in the base b representation of num/denom.
proc frac_place(num:I1, denom:I1, place, base) : I1
{
  if place == 0 then return num / denom;
  return (base:I2 * mul_exp_mod(num % denom, base, place-1, denom) / denom):I1;
}

// Returns result * (base^exp) % mod.
proc mul_exp_mod(in result:I1, base : I1, in exp : I1, mod : I1) : I1
{
  var factor = base;
  while (exp > 0 && result != 0)
  {
    // Use double-length arithmetic for the updates.
    if (exp & 1) then
      result = (result : I2 * factor % mod) : I1;
    factor = (factor : I2 * factor % mod) : I1;
    exp >>= 1;
  }
  return result;
}

proc update_digits(in d, in nd: I2)
{
  while (nd != 0 && d >= start)
  {
    nd += digits[d];
    var digit:int(8);
    (nd, digit) = (nd / hex, (nd % hex):int(8));
    if digit < 0 then (nd, digit) = (nd-1, (digit+hex):int(8));	// Borrow if necessary.
    digits[d] = digit;
    d -= 1;
  }
}

proc print_digits(in d, in nd: I2)
{
  var tmp: [0..13] int(8);
  var i = 0;
  while (nd != 0 && d >= start)
  {
    var digit:int(8);
    (nd, digit) = (nd / hex, (nd % hex):int(8));
    if nd == 0 && digit == -1 then
    {
       tmp[i] = 17; i += 1; d -= 1; break;
    }
    if digit < 0 then (nd, digit) = (nd-1, (digit+hex):int(8));	// Borrow if necessary.
    tmp[i] = digit;
    i += 1; d -= 1;
  }
  while (d >= start)
  {
    write(" "); d-= 1;
  }
  while (i > 0)
  {
    i -= 1;
    write_digit(tmp[i]);
  }
  write("\t");
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
    when 17 do write("!");
    otherwise write(d);
  }
}

