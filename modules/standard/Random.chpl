//
// Random Module
//
// This standard module currently contains a single random number
// generator based on the one used in the NPB benchmarks.  It's a
// linear congruential generator with:
//
//   m = 2^46 (the modulus)
//   a = 1220703125.0 (the multiplier)
//   c = 0 (the increment)
//
// Open Issues
//
// 1. We issue a warning if the seed is set to an even integer. Is
// this really an issue and why? Our default is to set it based on the
// current time of day, incrementing this value by one if it is even.
//
// 2. We would like to support general serial and parallel iterators
// on the RandomStream class, but the update to the shared cursor and
// count is not possible with our current parallel iterators because
// there is no way to mark its completion.  See Mar 2010 code review
// notes.
//
// 3. The fillRandom function currently only fills arrays of 64-bit
// real or 128-bit complex values.  When we have a general iterator
// above, the user will be able to coerce the values returned by
// RandomStream to any type.  Is it a good idea to also allow this
// class to be parameterized on a primitive type so that it could
// return values of that primitive type?  We would have to implement
// other algorithms to support other types.
//

def getRandomStreamClockSeed() {
  use Time;
  const seed = getCurrentTime(unit=TimeUnits.microseconds):int(64);
  return if seed % 2 == 0 then seed+1 else seed;
}

def fillRandom(x:[], seed: int(64) = getRandomStreamClockSeed()) {
  if x.eltType != complex && x.eltType != real then
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
    if seed % 2 == 0 then
      writeln("Warning: RandomStream seed is not an odd number.");
    this.seed = seed;
    RandomStreamPrivate_cursor = seed;
    RandomStreamPrivate_count = 1;
  }

  def getNext(parSafe = this.parSafe) {
    if parSafe then
      RandomStreamPrivate_lock$ = true;
    RandomStreamPrivate_count += 1;
    const result = RandomPrivate_randlc(RandomStreamPrivate_cursor);
    if parSafe then
      RandomStreamPrivate_lock$;
    return result;
  }

  def skipToNth(n: integral, parSafe = this.parSafe) {
    if (n <= 0) then
      halt("RandomStream.skipToNth(n) called with non-positive 'n' value", n);
    if parSafe then
      RandomStreamPrivate_lock$ = true;
    RandomStreamPrivate_count = n;
    RandomPrivate_randlc_skipto(RandomStreamPrivate_cursor, seed, n);
    if parSafe then
      RandomStreamPrivate_lock$;
  }

  def getNth(n: integral, parSafe = this.parSafe) {
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

  def fillRandom(X: [], parSafe = this.parSafe)
        where X.eltType == complex || X.eltType == real {
    param cplxMultiplier = if X.eltType == complex then 2 else 1;
    if parSafe then
      RandomStreamPrivate_lock$ = true;
    const startCount = RandomStreamPrivate_count;
    RandomStreamPrivate_count += cplxMultiplier * X.numElements;
    skipToNth(RandomStreamPrivate_count, parSafe=false);
    if parSafe then
      RandomStreamPrivate_lock$;

    forall (x, r) in (X, RandomStreamPrivate_iterate(startCount, X)) do
      x = r:X.eltType;
  }

  ///////////////////////////////////////////////////////////// CLASS PRIVATE //
  //
  // It is the intention that everything in this class declared below
  // this line is private to this class, but Chapel does not yet
  // support such enforcements.
  //

  var RandomStreamPrivate_lock$: sync bool;
  var RandomStreamPrivate_cursor: real;
  var RandomStreamPrivate_count: int(64);

  def RandomStreamPrivate_iterate(startAt: int(64), A: []) {
    param cplxMultiplier = if A.eltType == complex then 2 else 1;
    var cursor: real;
    RandomPrivate_randlc_skipto(cursor, seed, startAt);
    for i in 1..A.numElements do
      yield RandomPrivate_randlc_tuple(cplxMultiplier, cursor);
  }

  def RandomStreamPrivate_iterate(param tag: iterator,
                                  follower,
                                  startAt: int(64),
                                  A: []) where tag == iterator.follower {
    param cplxMultiplier = if A.eltType == complex then 2 else 1;
    const D = computeZeroBasedDomain(A.domain);
    const innerRange = follower(D.rank);
    var cursor: real;
    for outer in RandomPrivate_outer(follower) {
      var myStart = startAt;
      if D.rank > 1 then
        myStart += cplxMultiplier * D.indexOrder(((...outer), innerRange.low));
      else
        myStart += cplxMultiplier * D.indexOrder(innerRange.low);
      if !innerRange.stridable {
        RandomPrivate_randlc_skipto(cursor, seed, myStart);
        for i in innerRange do
          yield RandomPrivate_randlc_tuple(cplxMultiplier, cursor);
      } else {
        for i in innerRange {
          RandomPrivate_randlc_skipto(cursor, seed, myStart + i * cplxMultiplier);
          yield RandomPrivate_randlc_tuple(cplxMultiplier, cursor);
        }
      }
    }
  }
}

////////////////////////////////////////////////////////////// MODULE PRIVATE //
//
// It is the intention that everything declared below this line is
// private to this module, but Chapel does not yet support such
// enforcements.
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
def RandomPrivate_randlc_skipto(out cursor: real, seed: real, in n: integral) {
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
