/*
 * Copyright 2004-2015 Cray Inc.
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

/*
   Support for pseudorandom number generation

   This module defines an abstraction for a stream of pseudorandom
   numbers, :class:`RandomStream`.  It also provides a
   convenience function, :proc:`fillRandom` that can be used to
   fill an array with random numbers in parallel.

   The current pseudorandom number generator (PRNG) implemented by
   this module uses the algorithm from the NAS Parallel Benchmarks
   (NPB, available at: http://www.nas.nasa.gov/publications/npb.html).
   The longer-term intention is to add knobs to select between a menu
   of PRNG algorithms, such as the Mersenne twister.

   Paraphrasing the comments from the NPB reference implementation:

     This generator returns uniform pseudorandom real values in the
     range (0, 1) by using the linear congruential generator

       x_{k+1} = a x_k  (mod 2**46)

     where 0 < x_k < 2**46 and 0 < a < 2**46.  This scheme generates
     2**44 numbers before repeating.  The generated values are
     normalized to be between 0 and 1, i.e., 2**(-46) * x_k.

     This generator should produce the same results on any computer
     with at least 48 mantissa bits for `real(64)` data.


   Here is a list of currently open issues (TODOs) for this module:

   1. This module is currently restricted to generating `real(64)`,
   `imag(64)`, and `complex(128)` complex values.  We would like to
   extend this support to include other primitive types as well.

   2. We plan to support general serial and parallel iterators on the
   RandomStream class; however, providing the full suite of iterators
   is not possible with our current parallel iterator framework.
   Specifically, if :class:`RandomStream` is a follower in a
   zippered iteration context, there is no way for it to update the
   total number of random numbers generated in a safe/sane/coordinated
   way.  We are exploring a revised leader-follower iterator framework
   that would support this idiom (and other cursor-based ones).

   3. If no seed is provided by the user, one is chosen based on the
   current time in microseconds, allowing for some degree of
   pseudorandomness in seed selection.  The intent of
   :record:`SeedGenerators` is to provide a menu of other options
   for initializing the random stream seed, but only one option is
   implemented at present.

   4. As noted above, we plan to add support for additional PRNG
   algorithms over time.

*/
module Random {

  /* 
     Note for developers on "private" symbols:
     
     It is the intent that once Chapel supports a notion of 'private'
     symbols, everything prefixed with RandomPrivate will be made
     private to this module and everything prefixed with
     'RandomStreamPrivate_' will be made private to the RandomStream
     class.
  */

  /* Internal TODOs for developers:

     - should RandomStream be parameterized by the type to return?
       it seems odd that currently getNext() and getNth() return
       reals always...
  */

/* 
   An instance of :record:`SeedGenerators` that provides a
   convenient means of generating seeds when the user does not wish to
   specify one manually.
*/
const SeedGenerator: SeedGenerators;


// CHPLDOC FIXME: Respect ordering of things in the file

/*
  Provides methods to help generate seeds when the user doesn't want
  to create one.  It currently only supports one such method, but the
  intention is to add more over time.

  (Note: once Chapel supports static class methods,
  :const:`SeedGenerator` and :record:`SeedGenerators` should
  be combined into a single record type with static methods).
*/

record SeedGenerators {
  /*
    Generate a seed based on the current time in microseconds as
    reported by :proc:`Time.getCurrentTime`, ensuring that it
    meets the PRNG's requirements.
  */
  proc currentTime: int(64) {
    use Time;
    const seed: int(64) = getCurrentTime(unit=TimeUnits.microseconds):int(64);
    return (if seed % 2 == 0 then seed + 1 else seed) % (1:int(64) << 46);
  }
};


// CHPLDOC FEEDBACK: If easy, I'd suggest either deprecating the 
// :arg <type> <name>: form or else switching the order to 
// :arg <name> <type>: as it's an easy trap to simply type the same
// argument as in the function's signature.


// CHPLDOC FIXME: if the first line below is shifted one character to the
// left, it ends up being rendered like a method

// CHPLDOC FIXME: if the first line below is shifted one character to the
// right, it ends up causing a warning (promoted to error) in the .rst
// file.  It'd be preferable to have it declare the issue in terms of
// the .chpl line numbers.
//

/*
  Fill an array of `real(64)`, `imag(64)`, or `complex(128)` elements
  with pseudorandom values in parallel using a new
  :class:`RandomStream` created specifically for this call.  The
  first `arr.size` values from the stream will be assigned to the
  array's elements in row-major order for `real` and `imag` elements.
  For `complex` elements, consecutive pairs of random numbers are
  assigned to the real and imaginary components, respectively.  The
  parallelization strategy is determined by the array.

  :arg arr: The array to be filled, where T is real(64), imag(64), or complex(128).
  :type arr: [] T

  :arg seed: The seed to use for the PRNG.  Defaults to :proc:`SeedGenerator.currentTime <SeedGenerators.currentTime>`.
  :type seed: int(64)
*/

proc fillRandom(arr: [], seed: int(64) = SeedGenerator.currentTime)
  where (arr.eltType == real || arr.eltType == imag || arr.eltType == complex) {
  var randNums = new RandomStream(seed, parSafe=false);
  randNums.fillRandom(arr);
  delete randNums;
}

pragma "no doc"
proc fillRandom(arr: [], seed: int(64) = SeedGenerator.currentTime) {
  compilerError("Random.fillRandom is only defined for real(64), imag(64), and complex(128) arrays");
}

/*
  Models a stream of pseudorandom numbers.  See the module-level
  notes for :mod:`Random` for details on the PRNG used.
*/
class RandomStream {
  /*
    Indicates whether or not the RandomStream needs to be
    parallel-safe by default.  If multiple tasks interact with it in
    an uncoordinated fashion, this must be set to `true`.  If it will
    only be called from a single task, or if only one task will call
    into it at a time, setting to `false` will reduce overhead related
    to ensuring mutual exclusion.
  */
  param parSafe: bool = true;
  /*
    The seed value for the PRNG.  It must be an odd integer in the
    range (1, 2**46).
  */
  const seed: int(64);


  /*
    Constructs a new stream of random numbers using the specified seed
    and parallel safety.  Ensures that the seed value meets the PRNG's
    constraints.

    :arg seed: The seed to use for the PRNG.  Defaults to :proc:`SeedGenerator.currentTime <SeedGenerators.currentTime>`..
    :type seed: int(64)

    :arg parSafe: The parallel safety setting.  Defaults to `true`.
    :type parSafe: bool
  */
  proc RandomStream(seed: int(64) = SeedGenerator.currentTime,
                   param parSafe: bool = true) {
    if seed % 2 == 0 || seed < 1 || seed > 1:int(64)<<46 then
      halt("RandomStream seed must be an odd integer between 0 and 2**46");
    RandomStreamPrivate_init(seed);
  }

  pragma "no doc"
  proc RandomStreamPrivate_getNext_noLock() {
    RandomStreamPrivate_count += 1;
    return RandomPrivate_randlc(RandomStreamPrivate_cursor);
  }

  /*
    Returns the next value in the random stream as a `real(64)`

    :returns: The next value in the random stream as a `real(64)`.
   */
  proc getNext(): real(64) {
    if parSafe then
      RandomStreamPrivate_lock$ = true;
    const result = RandomStreamPrivate_getNext_noLock();
    if parSafe then
      RandomStreamPrivate_lock$;
    return result;
  }

  pragma "no doc"
  proc RandomStreamPrivate_skipToNth_noLock(n: integral) {
    RandomStreamPrivate_count = n;
    RandomStreamPrivate_cursor = RandomPrivate_randlc_skipto(seed, n);
  }


  /*
    Advances/rewinds the stream to the `n`-th value in the sequence.

    :arg n: The position in the stream to skip to.  Must be non-negative.
    :type n: integral
   */

  proc skipToNth(n: integral) {
    if n <= 0 then
      halt("RandomStream.skipToNth(n) called with non-positive 'n' value", n);
    if parSafe then
      RandomStreamPrivate_lock$ = true;
    RandomStreamPrivate_skipToNth_noLock(n);
    if parSafe then
      RandomStreamPrivate_lock$;
  }

  /*
    Advance/rewind the stream to the `n`-th value and return it
    (advancing the strem by one).  This is equivalent to
    :proc:`skipToNth()` followed by :proc:`getNext()`.

    :arg n: The position in the stream to skip to.  Must be non-negative.
    :type n: integral

    :returns: The `n`-th value in the random stream as a `real(64)`.
  */

  proc getNth(n: integral): real(64) {
    if (n <= 0) then 
      halt("RandomStream.getNth(n) called with non-positive 'n' value", n);
    if parSafe then
      RandomStreamPrivate_lock$ = true;
    RandomStreamPrivate_skipToNth_noLock(n);
    const result = RandomStreamPrivate_getNext_noLock();
    if parSafe then
      RandomStreamPrivate_lock$;
    return result;
  }

  /*
    Fill the argument array with pseudorandom values.  This method is
    identical to the standalone :proc:`fillRandom` procedure,
    except that it consumes random values from the
    :class:`RandomStream` object on which it's invoked rather
    than creating a new stream for the purpose of the call.

    :arg arr: The array to be filled, where T is real(64), imag(64), or complex(128).
    :type arr: [] T
  */

  proc fillRandom(arr: []) {
    if arr.eltType != complex && arr.eltType != real && arr.eltType != imag then
      compilerError("RandomStream.fillRandom is only defined for real(64), imag(64), and complex(128) arrays");
    forall (x, r) in zip(arr, iterate(arr.domain, arr.eltType)) do
      x = r;
  }

  pragma "no doc"
  proc iterate(D: domain, type resultType=real) {
    if resultType != complex && resultType != real && resultType != imag then
      compilerError("RandomStream.iterate is only defined for real(64), imag(64), and complex(128) result types");
    param cplxMultiplier = if resultType == complex then 2 else 1;
    if parSafe then
      RandomStreamPrivate_lock$ = true;
    const start = RandomStreamPrivate_count;
    // NOTE: Not bothering to check to see if D.numIndices can fit into int(64)
    RandomStreamPrivate_count += cplxMultiplier * D.numIndices:int(64);
    RandomStreamPrivate_skipToNth_noLock(RandomStreamPrivate_count);
    if parSafe then
      RandomStreamPrivate_lock$;
    return RandomPrivate_iterate(resultType, D, seed, start);
  }

  pragma "no doc"
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

  pragma "no doc"
  var RandomStreamPrivate_lock$: sync bool;
  pragma "no doc"
  var RandomStreamPrivate_cursor: real;
  pragma "no doc"
  var RandomStreamPrivate_count: int(64);

  pragma "no doc"
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
pragma "no doc"
const RandomPrivate_r23   = 0.5**23,
      RandomPrivate_t23   = 2.0**23,
      RandomPrivate_r46   = 0.5**46,
      RandomPrivate_t46   = 2.0**46,
      RandomPrivate_arand = 1220703125.0; // TODO: Is arand something that a
                                          // user might want to set on a
                                          // case-by-case basis?

//
// NPB-defined randlc routine
//
pragma "no doc"
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
pragma "no doc"
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
pragma "no doc"
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
pragma "no doc"
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
pragma "no doc"
iter RandomPrivate_iterate(type resultType, D: domain, seed: int(64),
                          start: int(64)) {
  var cursor = RandomPrivate_randlc_skipto(seed, start);
  for i in D do
    yield RandomPrivate_randlc(resultType, cursor);
}

pragma "no doc"
iter RandomPrivate_iterate(type resultType, D: domain, seed: int(64),
                          start: int(64), param tag: iterKind)
      where tag == iterKind.leader {
  for block in D._value.these(tag=iterKind.leader) do
    yield block;
}

pragma "no doc"
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

} // close module Random
