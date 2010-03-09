//
// Random Module
//
// This standard module contains a random number generator based on
// the one used in the NPB benchmarks.  Tailoring the NPB comments to
// this code, we can say the following:
//
//   This generator returns uniform pseudorandom real values in the
//   range (0, 1) by using the linear congruential generator
//
//     x_{k+1} = a x_k  (mod 2**46)
//
//   where 0 < x_k < 2**46 and 0 < a < 2**46.  This scheme generates
//   2**44 numbers before repeating.  The seed value must be an odd
//   64-bit integer in the range (1, 2^46).  The generated values are
//   normalized to be between 0 and 1, i.e., 2**(-46) * x_k.
//
//   This generator should produce the same results on any computer
//   with at least 48 mantissa bits for real(64) data.
//
// Open Issues
//
// 1. We would like to support general serial and parallel iterators
// on the RandomStream class, but the update to the shared cursor and
// count is not possible with our current parallel iterators because
// there is no way to mark its completion.  See Mar 2010 code review
// notes.
//
// 2. The fillRandom function currently only fills arrays of 64-bit
// real, 64-bit imag, or 128-bit complex values.  When we have a
// general iterator above, the user will be able to coerce the values
// returned by RandomStream to any type.  Is it a good idea to also
// allow this class to be parameterized on a primitive type so that it
// could return values of that primitive type?  We would have to
// implement other algorithms to support other types.
//
// Note on Private
//
// It is the intent that once Chapel supports the notion of 'private',
// everything prefixed with RandomPrivate_ will be made private to
// this module and everything prefixed with RandomStreamPrivate_ will
// be made private to the RandomStream class.
// 

def getRandomStreamClockSeed() {
  use Time;
  const seed = getCurrentTime(unit=TimeUnits.microseconds):int(64);
  return (if seed % 2 == 0 then seed+1 else seed) % (1:int(64)<<46);
}

def fillRandom(x:[], seed: int(64) = getRandomStreamClockSeed()) {
  if x.eltType != complex && x.eltType != real && x.eltType != imag then
    compilerError("Random.fillRandom is only defined for real/complex arrays");
  var randNums = new RandomStream(seed, parSafe=false);
  randNums.fillRandom(x);
  delete randNums;
}

class RandomStream {
  param parSafe: bool = true;
  const seed: int(64);

  def RandomStream(seed: int(64) = getRandomStreamClockSeed(),
                   param parSafe: bool = true) {
    if seed % 2 == 0 || seed < 1 || seed > 1:int(64)<<46 then
      halt("RandomStream seed must be an odd integer between 0 and 2**46");
    this.seed = seed;
    RandomStreamPrivate_cursor = seed;
    RandomStreamPrivate_count = 1;
  }

  def getNext(param parSafe = this.parSafe) {
    if parSafe then
      RandomStreamPrivate_lock$ = true;
    RandomStreamPrivate_count += 1;
    const result = RandomPrivate_randlc(RandomStreamPrivate_cursor);
    if parSafe then
      RandomStreamPrivate_lock$;
    return result;
  }

  def skipToNth(n: integral, param parSafe = this.parSafe) {
    if n <= 0 then
      halt("RandomStream.skipToNth(n) called with non-positive 'n' value", n);
    if parSafe then
      RandomStreamPrivate_lock$ = true;
    RandomStreamPrivate_count = n;
    RandomPrivate_randlc_skipto(RandomStreamPrivate_cursor, seed, n);
    if parSafe then
      RandomStreamPrivate_lock$;
  }

  def getNth(n: integral, param parSafe = this.parSafe) {
    if (n <= 0) then 
      halt("RandomStream.getNth(n) called with non-positive 'n' value", n);
    if parSafe then
      RandomStreamPrivate_lock$ = true;
    skipToNth(n, parSafe=false);
    const result = getNext(parSafe=false);
    if parSafe then
      RandomStreamPrivate_lock$;
    return result;
  }

  def fillRandom(X: [], param parSafe = this.parSafe) {
    if X.eltType != complex && X.eltType != real && X.eltType != imag then
      compilerError("RandomStream.fillRandom called on array with unsupported element type");
    forall (x, r) in (X, iterate(X.domain, X.eltType, parSafe)) do
      x = r;
  }

  def iterate(D: domain, type resultType=real, param parSafe = this.parSafe) {
    if resultType != complex && resultType != real && resultType != imag then
      compilerError("unsupported resultType for RandomStream.iterate");
    param cplxMultiplier = if resultType == complex then 2 else 1;
    if parSafe then
      RandomStreamPrivate_lock$ = true;
    const start = RandomStreamPrivate_count;
    RandomStreamPrivate_count += cplxMultiplier * D.numIndices;
    skipToNth(RandomStreamPrivate_count, parSafe=false);
    if parSafe then
      RandomStreamPrivate_lock$;
    return RandomPrivate_iterate(resultType, D, seed, start);
  }

  ///////////////////////////////////////////////////////////// CLASS PRIVATE //
  //
  // It is the intent that once Chapel supports the notion of
  // 'private', everything in this class declared below this line will
  // be made private to this class.
  //

  var RandomStreamPrivate_lock$: sync bool;
  var RandomStreamPrivate_cursor: real;
  var RandomStreamPrivate_count: int(64);
}

////////////////////////////////////////////////////////////// MODULE PRIVATE //
//
// It is the intent that once Chapel supports the notion of 'private',
// everything declared below this line will be made private to this
// module.
//

//
// NPB-defined constants for linear congruential generator
//
const RandomPrivate_r23   = 0.5**23,
      RandomPrivate_t23   = 2.0**23,
      RandomPrivate_r46   = 0.5**46,
      RandomPrivate_t46   = 2.0**46,
      RandomPrivate_arand = 1220703125.0;

//
// NPB-defined randlc routine
//
def RandomPrivate_randlc(inout x: real, a: real = RandomPrivate_arand) {
  var t1 = RandomPrivate_r23 * a;
  const a1 = floor(t1),
    a2 = a - RandomPrivate_t23 * a1;
  t1 = RandomPrivate_r23 * x;
  const x1 = floor(t1),
    x2 = x - RandomPrivate_t23 * x1;
  t1 = a1 * x2 + a2 * x1;
  const t2 = floor(RandomPrivate_r23 * t1),
    z  = t1 - RandomPrivate_t23 * t2,
    t3 = RandomPrivate_t23 * z + a2 * x2,
    t4 = floor(RandomPrivate_r46 * t3),
    x3 = t3 - RandomPrivate_t46 * t4;
  x = x3;
  return RandomPrivate_r46 * x3;
}

//
// Skip to the nth random value and set a cursor.
//
def RandomPrivate_randlc_skipto(out cursor: real, seed: int(64), in n: integral) {
  cursor = seed:real;
  n -= 1;
  var t = RandomPrivate_arand;
  RandomPrivate_arand;
  while (n != 0) {
    const i = n / 2;
    if (2 * i != n) then
      RandomPrivate_randlc(cursor, t);
    if i == 0 then
      break;
    else
      n = i;
    RandomPrivate_randlc(t, t);
    n = i;
  }
}

//
// Return tuple of randlc values
//
def RandomPrivate_randlc_tuple(param size, inout x: real) {
  if size == 1 {
    return RandomPrivate_randlc(x);
  } else {
    var result: size*real;
    for param i in 1..size do
      result(i) = RandomPrivate_randlc(x);
    return result;
  }
}

//
// iterate over outer ranges in tuple of ranges
//
def RandomPrivate_outer(ranges, param dim: int = 1) {
  if dim + 1 == ranges.size {
    for i in ranges(dim) do
      yield tuple(i);
  } else if dim + 1 < ranges.size {
    for i in ranges(dim) do
      for j in RandomPrivate_outer(ranges, dim+1) do
        yield (i, (...j));
  } else {
    yield 0; // 1D case is a noop
  }
}

//
// RandomStream iterator implementation
//
def RandomPrivate_iterate(type resultType, D: domain, seed: int(64),
                          start: int(64)) {
  param cplxMultiplier = if resultType == complex then 2 else 1;
  var cursor: real;
  RandomPrivate_randlc_skipto(cursor, seed, start);
  for i in D do
    yield RandomPrivate_randlc_tuple(cplxMultiplier, cursor):resultType;
}

def RandomPrivate_iterate(type resultType, D: domain, seed: int(64),
                          start: int(64), param tag: iterator)
      where tag == iterator.leader {
  for block in D._value.these(tag=iterator.leader) do
    yield block;
}

def RandomPrivate_iterate(type resultType, D: domain, seed: int(64),
                          start: int(64), param tag: iterator, follower)
      where tag == iterator.follower {
  param cplxMultiplier = if resultType == complex then 2 else 1;
  const ZD = computeZeroBasedDomain(D);
  const innerRange = follower(ZD.rank);
  var cursor: real;
  for outer in RandomPrivate_outer(follower) {
    var myStart = start;
    if ZD.rank > 1 then
      myStart += cplxMultiplier * ZD.indexOrder(((...outer), innerRange.low));
    else
      myStart += cplxMultiplier * ZD.indexOrder(innerRange.low);
    if !innerRange.stridable {
      RandomPrivate_randlc_skipto(cursor, seed, myStart);
      for i in innerRange do
        yield RandomPrivate_randlc_tuple(cplxMultiplier, cursor):resultType;
    } else {
      for i in innerRange {
        RandomPrivate_randlc_skipto(cursor, seed, myStart + i * cplxMultiplier);
        yield RandomPrivate_randlc_tuple(cplxMultiplier, cursor):resultType;
      }
    }
  }
}
