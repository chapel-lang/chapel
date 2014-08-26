/*
 * Copyright 2004-2014 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
// on the RandomStream class, but this is not possible with our
// current parallel iterator framework.
//
// 2. The random number generation functionality in this module is
// currently restricted to 64-bit real, 64-bit imag, and 128-bit
// complex values.  This should be extended to other primitive types
// for which this would make sense.  Coercions are insufficient.
//
// 3. Can the multiplier 'arand' be moved into the RandomStream class
// so that it can be changed by a user of this class.
//
// 4. By default, the random stream seed is initialized based on the
// current time in microseconds, allowing for some degree of
// randomness.  The intent of the SeedGenerator enumerated type is to
// provide a menu of options for initializing the random stream seed,
// but only one option is implemented to date.
//
// Note on Private
//
// It is the intent that once Chapel supports the notion of 'private',
// everything prefixed with RandomPrivate_ will be made private to
// this module and everything prefixed with RandomStreamPrivate_ will
// be made private to the RandomStream class.
//

proc fillRandom(x:[], seed: int(64)) {
  if x.eltType != complex && x.eltType != real && x.eltType != imag then
    compilerError("Random.fillRandom is only defined for real(64), imag(64), and complex(128) arrays");
  var randNums = new RandomStream(seed, parSafe=false);
  randNums.fillRandom(x);
  delete randNums;
}

proc fillRandom(x:[]) {
  if x.eltType != complex && x.eltType != real && x.eltType != imag then
    compilerError("Random.fillRandom is only defined for real(64), imag(64), and complex(128) arrays");
  var randNums = new RandomStream(parSafe=false);
  randNums.fillRandom(x);
  delete randNums;
}

record SeedGenerators {
  proc currentTime {
    use Time;
    const seed: int(64) = getCurrentTime(unit=TimeUnits.microseconds):int(64);
    return (if seed % 2 == 0 then seed + 1 else seed) % (1:int(64) << 46);
  }
};

const SeedGenerator: SeedGenerators;

class RandomStream {
  param parSafe: bool = true;
  const seed: int(64);

  proc RandomStream(seed: int(64) = SeedGenerator.currentTime,
                   param parSafe: bool = true) {
    if seed % 2 == 0 || seed < 1 || seed > 1:int(64)<<46 then
      halt("RandomStream seed must be an odd integer between 0 and 2**46");
    RandomStreamPrivate_init(seed);
  }

  proc getNext(param parSafe = this.parSafe) {
    if parSafe then
      RandomStreamPrivate_lock$ = true;
    RandomStreamPrivate_count += 1;
    const result = RandomPrivate_randlc(RandomStreamPrivate_cursor);
    if parSafe then
      RandomStreamPrivate_lock$;
    return result;
  }

  proc skipToNth(n: integral, param parSafe = this.parSafe) {
    if n <= 0 then
      halt("RandomStream.skipToNth(n) called with non-positive 'n' value", n);
    if parSafe then
      RandomStreamPrivate_lock$ = true;
    RandomStreamPrivate_count = n;
    RandomStreamPrivate_cursor = RandomPrivate_randlc_skipto(seed, n);
    if parSafe then
      RandomStreamPrivate_lock$;
  }

  proc getNth(n: integral, param parSafe = this.parSafe) {
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

  proc fillRandom(X: [], param parSafe = this.parSafe) {
    if X.eltType != complex && X.eltType != real && X.eltType != imag then
      compilerError("RandomStream.fillRandom is only defined for real(64), imag(64), and complex(128) arrays");
    forall (x, r) in zip(X, iterate(X.domain, X.eltType, parSafe)) do
      x = r;
  }

  proc iterate(D: domain, type resultType=real, param parSafe = this.parSafe) {
    if resultType != complex && resultType != real && resultType != imag then
      compilerError("RandomStream.iterate is only defined for real(64), imag(64), and complex(128) result types");
    param cplxMultiplier = if resultType == complex then 2 else 1;
    if parSafe then
      RandomStreamPrivate_lock$ = true;
    const start = RandomStreamPrivate_count;
    // NOTE: Not bothering to check to see if D.numIndices can fit into int(64)
    RandomStreamPrivate_count += cplxMultiplier * D.numIndices:int(64);
    skipToNth(RandomStreamPrivate_count, parSafe=false);
    if parSafe then
      RandomStreamPrivate_lock$;
    return RandomPrivate_iterate(resultType, D, seed, start);
  }

  proc writeThis(f: Writer) {
    f <~> "RandomStream(parSafe=";
    f <~> parSafe;
    f <~> ", seed = ";
    f <~> seed;
    f <~> ")";
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

  proc RandomStreamPrivate_init(seed: int(64)) {
    this.seed = seed;
    RandomStreamPrivate_cursor = seed;
    RandomStreamPrivate_count = 1;
  }    
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
proc RandomPrivate_randlc(inout x: real, a: real = RandomPrivate_arand) {
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

// Wrapper that takes a result type (two calls for complex types)
proc RandomPrivate_randlc(type resultType, inout x: real) {
  if resultType == complex then
    return (RandomPrivate_randlc(x), RandomPrivate_randlc(x)):complex;
  else
    return RandomPrivate_randlc(x):resultType;
}

//
// Return a value for the cursor so that the next call to randlc will
// return the same value as the nth call to randlc
//
proc RandomPrivate_randlc_skipto(seed: int(64), in n: integral): real {
  var cursor = seed:real;
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
  return cursor;
}

//
// iterate over outer ranges in tuple of ranges
//
iter RandomPrivate_outer(ranges, param dim: int = 1) {
  if dim + 1 == ranges.size {
    for i in ranges(dim) do
      yield (i,);
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
iter RandomPrivate_iterate(type resultType, D: domain, seed: int(64),
                          start: int(64)) {
  var cursor = RandomPrivate_randlc_skipto(seed, start);
  for i in D do
    yield RandomPrivate_randlc(resultType, cursor);
}

iter RandomPrivate_iterate(type resultType, D: domain, seed: int(64),
                          start: int(64), param tag: iterKind)
      where tag == iterKind.leader {
  for block in D._value.these(tag=iterKind.leader) do
    yield block;
}

iter RandomPrivate_iterate(type resultType, D: domain, seed: int(64),
                          start: int(64), param tag: iterKind, followThis)
      where tag == iterKind.follower {
  param multiplier = if resultType == complex then 2 else 1;
  const ZD = computeZeroBasedDomain(D);
  const innerRange = followThis(ZD.rank);
  var cursor: real;
  for outer in RandomPrivate_outer(followThis) {
    var myStart = start;
    // NOTE: Not bothering to check to see if this can fit into int(64)
    if ZD.rank > 1 then
      myStart += multiplier * ZD.indexOrder(((...outer), innerRange.low)):int(64);
    else
      myStart += multiplier * ZD.indexOrder(innerRange.low):int(64);
    if !innerRange.stridable {
      cursor = RandomPrivate_randlc_skipto(seed, myStart);
      for i in innerRange do
        yield RandomPrivate_randlc(resultType, cursor);
    } else {
      // NOTE: Not bothering to check to see if this can fit into int(64)
      myStart -= innerRange.low:int(64);
      for i in innerRange {
        cursor = RandomPrivate_randlc_skipto(seed, myStart + i:int(64) * multiplier);
        yield RandomPrivate_randlc(resultType, cursor);
      }
    }
  }
}
