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
   numbers, :class:`RandomStreamInterface`, supporting methods to get the next
   random number in the stream (:proc:`.getNext`), to fast-forward to
   a specific value in the stream (:proc:`.skipToNth` and
   :proc:`.getNth`), or to fill an array with random numbers in
   parallel (:proc:`~Random.RandomStream.fillRandom`).  The module
   also provides a standalone convenience function, :proc:`fillRandom`
   that can be used to fill an array with random numbers in parallel
   without manually creating a :class:`RandomStream` object.

   Use :proc:`makeRandomStream` or the constructor for a specific RNG
   implementation to get a RandomStream.

   Here is a list of currently open issues (TODOs) for this module:

   1. We plan to support general serial and parallel iterator methods
   on :class:`RandomStream`; however, providing the full suite of
   iterators is not possible with our current parallel iterator
   framework.  Specifically, if :class:`RandomStream` is a follower in
   a zippered iteration context, there is no way for it to update the
   total number of random numbers generated in a safe/sane/coordinated
   way.  We are exploring a revised leader-follower iterator framework
   that would support this idiom (and other cursor-based ones).  With
   Chapel's recent support for standalone parallel iterators, one
   could define a standalone parallel iterator for
   :class:`RandomStream`, but this effort has not yet been taken on.

   2. If no seed is provided by the user, one is chosen based on the
   current time in microseconds, allowing for some degree of
   pseudorandomness in seed selection.  The intent of
   :record:`SeedGenerators` is to provide a menu of other options
   for initializing the random stream seed, but only one option is
   implemented at present.

*/
module Random {

  use RandomSupport;
  use NPBRandom;
  use PCGRandom;


/* Select between different supported RNG algorithms.
 */
enum RNG {
  PCG = 1,
  NPB = 2
}


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

private
proc isSupportedNumericType(type t) param
  return isNumericType(t) || isBoolType(t);

/*
  Fill an array of `real(64)`, `imag(64)`, or `complex(128)` elements
  with pseudorandom values in parallel using a new
  :class:`RandomStream` created specifically for this call.  The
  first `arr.size` values from the stream will be assigned to the
  array's elements in row-major order for `real` and `imag` elements.
  For `complex` elements, consecutive pairs of random numbers are
  assigned to the real and imaginary components, respectively.  The
  parallelization strategy is determined by the array.

  :arg arr: The array to be filled, where T is `real(64)`, `imag(64)`, or `complex(128)`.
  :type arr: [] T

  :arg seed: The seed to use for the PRNG.  Defaults to :proc:`SeedGenerator.currentTime <SeedGenerators.currentTime>`.
  :type seed: int(64)
*/
proc fillRandom(arr: [], seed: int(64) = SeedGenerator.currentTime, param algorithm=RNG.PCG)
  where isSupportedNumericType(arr.eltType) {
  var randNums = makeRandomStream(seed, eltType=arr.eltType, parSafe=false, algorithm=algorithm);
  randNums.fillRandom(arr);
  delete randNums;
}

pragma "no doc"
proc fillRandom(arr: [], seed: int(64) = SeedGenerator.currentTime, param algorithm=RNG.PCG) {
  compilerError("Random.fillRandom is only defined for numeric arrays");
}

// shuffle might also be a bad name
proc shuffle(arr: [], seed: int(64) = SeedGenerator.currentTime, param algorithm=RNG.PCG) {
  var randNums = makeRandomStream(seed, eltType=arr.eltType, parSafe=false, algorithm=algorithm);
  randNums.shuffle(arr);
  delete randNums;
}

/// permutation is a poor name
proc permutation(arr: [], seed: int(64) = SeedGenerator.currentTime, param algorithm=RNG.PCG) {
  var randNums = makeRandomStream(seed, eltType=arr.eltType, parSafe=false, algorithm=algorithm);
  randNums.permutation(arr);
  delete randNums;
}

/*
  Models a stream of pseudorandom numbers.  This is an abstract
  base class that should not be instantiated. See :class:`PCGRandom` and
  :class:`NPBRandom` for details on the PRNG used. To create a random stream,
  use :proc:`makeRandomStream`.
*/
class RandomStreamInterface {
  /*
    Specifies the type of value generated by the RandomStream.
    Currently, only `real(64)`, `imag(64)`, and `complex(128)` are
    supported.
  */
  type eltType = real(64);

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
    Returns the next value in the random stream.

    :returns: The next value in the random stream as type :type:`eltType`.
   */
  proc getNext(): eltType {
    if parSafe then
      RandomStreamPrivate_lock$ = true;
    const result = RandomStreamPrivate_getNext_noLock();
    if parSafe then
      RandomStreamPrivate_lock$;
    return result;
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
    (advancing the stream by one).  This is equivalent to
    :proc:`skipToNth()` followed by :proc:`getNext()`.

    :arg n: The position in the stream to skip to.  Must be non-negative.
    :type n: integral

    :returns: The `n`-th value in the random stream as type :type:`eltType`.
  */

  proc getNth(n: integral): eltType {
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

    :arg arr: The array to be filled
    :type arr: [] :type:`eltType`
  */
  proc fillRandom(arr: [] eltType) {
    forall (x, r) in zip(arr, iterate(arr.domain, arr.eltType)) do
      x = r;
  }

  pragma "no doc"
  proc fillRandom(arr: []) {
    compilerError("RandomStream(eltType=", typeToString(eltType), ") can only be used to fill arrays of ", typeToString(eltType));
  }
    
  pragma "no doc"
  proc iterate(D: domain, type resultType=real) {
    if parSafe then
      RandomStreamPrivate_lock$ = true;
    const start = RandomStreamPrivate_count;
    RandomStreamPrivate_count += D.numIndices.safeCast(int(64));
    RandomStreamPrivate_skipToNth_noLock(RandomStreamPrivate_count);
    if parSafe then
      RandomStreamPrivate_lock$;
    return RandomPrivate_iterate(resultType, D, seed, start);
  }

  pragma "no doc"
  proc writeThis(f: Writer) {
    f <~> "RandomStream(eltType=";
    f <~> typeToString(eltType);
    f <~> ", parSafe=";
    f <~> parSafe;
    f <~> ", seed=";
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
  var RandomStreamPrivate_cursor: real = seed;
  pragma "no doc"
  var RandomStreamPrivate_count: int(64) = 1;
}

/*
  Constructs a new stream of random numbers using the specified seed
  and parallel safety.  Ensures that the seed value meets the PRNG's
  constraints.

  :arg seed: The seed to use for the PRNG.  Defaults to :proc:`SeedGenerator.currentTime <SeedGenerators.currentTime>`..
  :type seed: int(64)

  :arg parSafe: The parallel safety setting.  Defaults to `true`.
  :type parSafe: bool

  :arg eltType: The element type to be generated.  Defaults to `real(64)`.
  :type eltType: type

  :arg algorithm: which algorithm to use. A param enum RNG element.  Defaults to PCG.
  :type algorithm: RNG
*/
proc makeRandomStream(seed: int(64) = SeedGenerator.currentTime,
                      param parSafe: bool = true,
                      type eltType = real(64),
                      param algorithm: RNG ) {
  if algorithm == RNG.PCG then
    return new PCGRandomStream(seed=seed, parSafe=parSafe, eltType=eltType);
  else if algorithm == RNG.NPB then
    return new NPBRandomStream(seed=seed, parSafe=parSafe, eltType=eltType);
  else
    compilerError("Unknown random number generator");
}

// An apparent bug prevents this from working.
//type RandomStream = PCGRandomStream;

} // close module Random
