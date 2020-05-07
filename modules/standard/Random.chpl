/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

   This module defines an abstraction for a stream of pseudorandom numbers,
   :class:`~RandomStreamInterface`. Use :proc:`createRandomStream` to
   create such an stream. Each stream supports methods to get the next
   random number in the stream (:proc:`~RandomStreamInterface.getNext`),
   to fast-forward to a specific value in the stream
   (:proc:`~RandomStreamInterface.skipToNth` and
   :proc:`~RandomStreamInterface.getNth`), to iterate over random values
   possibly in parallel (:proc:`~RandomStreamInterface.iterate`), or to
   fill an array with random numbers in parallel
   (:proc:`~RandomStreamInterface.fillRandom`).

   The module also provides several standalone convenience functions that can be
   used without manually creating a :class:`RandomStreamInterface` object.

     * :proc:`fillRandom` fills an array with random numbers in parallel
     * :proc:`shuffle` randomly re-arranges the elements of an array
     * :proc:`permutation` creates a random permutation and stores it in an
       array.

   In these and other methods, generated integer values are uniformly
   distributed from `min(T)` to `max(T)`, where `T` is the integral type and the
   boundaries are included. Generated floating point values are uniformly
   distributed in [0.0, 1.0] with the caveat that it currently depends on the
   RNG whether the boundary values 0.0 and 1.0 can be produced.

   Use :proc:`createRandomStream` or the constructor for a specific RNG
   implementation to get a RandomStream. See the documentation for
   each RNG implementation for more information:

     * :mod:`PCGRandom`
     * :mod:`NPBRandom`

   .. note::

       The RandomStream API (:class:`RandomStreamInterface`) is expected to
       change.

*/
module Random {

  public use RandomSupport;
  public use NPBRandom;
  public use PCGRandom;
  import HaltWrappers;
  import Set;



  /* Select between different supported RNG algorithms.
     See :mod:`PCGRandom` and :mod:`NPBRandom` for details on
     these algorithms.
   */
  enum RNG {
    PCG = 1,
    NPB = 2
  }

  /* The default RNG. The current default is PCG - see :mod:`PCGRandom`. */
  param defaultRNG = RNG.PCG;

  type RandomStream = if defaultRNG == RNG.PCG then PCGRandomStream
                                               else NPBRandomStream;

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

    Fill an array of numeric elements with pseudorandom values in parallel using
    a new stream implementing :class:`RandomStreamInterface` created
    specifically for this call.  The first `arr.size` values from the stream
    will be assigned to the array's elements in row-major order. The
    parallelization strategy is determined by the array.

    .. note::

      :mod:`NPBRandom` only supports `real(64)`, `imag(64)`, and `complex(128)`
      numeric types. :mod:`PCGRandom` supports all primitive numeric types.

    :arg arr: The array to be filled, where T is a primitive numeric type
    :type arr: `[] T`

    :arg seed: The seed to use for the PRNG.  Defaults to
     `oddCurrentTime` from :type:`RandomSupport.SeedGenerator`.
    :type seed: `int(64)`

    :arg algorithm: A param indicating which algorithm to use. Defaults to :param:`defaultRNG`.
    :type algorithm: :type:`RNG`
  */
  proc fillRandom(arr: [], seed: int(64) = SeedGenerator.oddCurrentTime, param
      algorithm=defaultRNG)
    where isSupportedNumericType(arr.eltType) {
    var randNums = createRandomStream(seed=seed,
                                      eltType=arr.eltType,
                                      parSafe=false,
                                      algorithm=algorithm);
    randNums.fillRandom(arr);
  }

  pragma "no doc"
  proc fillRandom(arr: [], seed: int(64) = SeedGenerator.oddCurrentTime, param
      algorithm=defaultRNG) {
    compilerError("Random.fillRandom is only defined for numeric arrays");
  }

  /* Shuffle the elements of an array into a random order.

     :arg arr: a 1-D non-strided array
     :arg seed: the seed to use when shuffling. Defaults to
      `oddCurrentTime` from :type:`RandomSupport.SeedGenerator`.
     :arg algorithm: A param indicating which algorithm to use. Defaults to PCG.
     :type algorithm: :type:`RNG`
   */
  proc shuffle(arr: [], seed: int(64) = SeedGenerator.oddCurrentTime, param algorithm=RNG.PCG) {
    var randNums = createRandomStream(seed=seed,
                                      eltType=arr.domain.idxType,
                                      parSafe=false,
                                      algorithm=algorithm);
    randNums.shuffle(arr);
  }


  /* Produce a random permutation, storing it in a 1-D array.
     The resulting array will include each value from low..high
     exactly once, where low and high refer to the array's domain.

     :arg arr: a 1-D non-strided array
     :arg seed: the seed to use when creating the permutation. Defaults to
      `oddCurrentTime` from :type:`RandomSupport.SeedGenerator`.
     :arg algorithm: A param indicating which algorithm to use. Defaults to PCG.
     :type algorithm: :type:`RNG`
   */
  proc permutation(arr: [], seed: int(64) = SeedGenerator.oddCurrentTime, param algorithm=RNG.PCG) {
    var randNums = createRandomStream(seed=seed,
                                      eltType=arr.eltType,
                                      parSafe=false,
                                      algorithm=algorithm);
    randNums.permutation(arr);
  }

  pragma "no doc"
  proc makeRandomStream(type eltType,
                        seed: int(64) = SeedGenerator.oddCurrentTime,
                        param parSafe: bool = true,
                        param algorithm = defaultRNG) {
    compilerWarning("makeRandomStream is deprecated - " +
                    "please use createRandomStream instead");
    return createRandomStream(eltType, seed, parSafe, algorithm);
  }

  /*
    Constructs a new stream of random numbers using the specified seed
    and parallel safety.  Ensures that the seed value meets the PRNG's
    constraints.

    .. note::

      The :mod:`NPBRandom` RNG will halt if provided an even seed.
      :mod:`PCGRandom` has no restrictions on the provided seed value.

    :arg eltType: The element type to be generated.
    :type eltType: `type`

    :arg seed: The seed to use for the PRNG.  Defaults to `oddCurrentTime` from
     :type:`RandomSupport.SeedGenerator`.
    :type seed: `int(64)`

    :arg parSafe: The parallel safety setting.  Defaults to `true`.
    :type parSafe: `bool`

    :arg algorithm: A param indicating which algorithm to use. Defaults to :param:`defaultRNG`.
    :type algorithm: :type:`RNG`

    :returns: an owned RandomStream
  */
  proc createRandomStream(type eltType,
                          seed: int(64) = SeedGenerator.oddCurrentTime,
                          param parSafe: bool = true,
                          param algorithm = defaultRNG) {
    if algorithm == RNG.PCG then
      return new owned PCGRandomStream(seed=seed,
                                       parSafe=parSafe,
                                       eltType=eltType);
    else if algorithm == RNG.NPB then
      return new owned NPBRandomStream(seed=seed,
                                       parSafe=parSafe,
                                       eltType=eltType);
    else
      compilerError("Unknown random number generator");
  }


  pragma "no doc"
  /* Actual implementation of choice() */
  proc _choice(stream, X: domain, size: ?sizeType, replace: bool, prob: ?probType)
    throws
  {

    if X.rank != 1 {
      compilerError('choice() argument x must be 1 dimensional');
    }
    if X.size < 1 {
      throw new owned IllegalArgumentError('choice() x.size must be greater than 0');
    }

    // Check types of optional void args
    if !isNothingType(probType) {
      if !isArrayType(probType) then
        compilerError('choice() prob must be an array');
      if !(isIntegralType(prob.eltType) || isRealType(prob.eltType)) then
        compilerError('choice() prob.eltType must be real or integral');
      if prob.rank != 1 {
        compilerError('choice() prob array must be 1 dimensional');
      }
      if prob.size != X.size {
        throw new owned IllegalArgumentError('choice() x.size must be equal to prob.size');
      }
    }

    if !isNothingType(sizeType) {
      if isIntegralType(sizeType) {
        if size <= 0 then
          throw new owned IllegalArgumentError('choice() size must be greater than 0');
        if !replace && size > X.size then
          throw new owned IllegalArgumentError('choice() size must be smaller than x.size when replace=false');
      } else if isDomainType(sizeType) {
        if size.size <= 0 then
          throw new owned IllegalArgumentError('choice() size domain can not be empty');
        if !replace && size.size > X.size then
          throw new owned IllegalArgumentError('choice() size must be smaller than x.size when replace=false');
      } else {
        compilerError('choice() size must be integral or domain');
      }
    }

    if isNothingType(probType) {
      return _choiceUniform(stream, X, size, replace);
    } else {
      return _choiceProbabilities(stream, X, size, replace, prob);
    }
  }

  pragma "no doc"
  /* _choice branch for uniform distribution */
  proc _choiceUniform(stream, X: domain, size: ?sizeType, replace: bool) throws
  {

    const low = X.alignedLow,
          stride = abs(X.stride);

    if isNothingType(sizeType) {
      // Return 1 sample
      var randVal = stream.getNext(resultType=int, 0, X.size-1);
      var randIdx = X.dim(0).orderToIndex(randVal);
      return randIdx;
    } else {
      // Return numElements samples

      // Compute numElements for tuple case
      var m = 1;
      if isDomainType(sizeType) then m = size.size;

      var numElements = if isDomainType(sizeType) then m
                        else if isIntegralType(sizeType) then size:int
                        else compilerError('choice() size type must be integral or tuple of ranges');

      // Return N samples
      var samples: [0..<numElements] int;

      if replace {
        for sample in samples {
          var randVal = stream.getNext(resultType=int, 0, X.size-1);
          var randIdx = X.dim(0).orderToIndex(randVal);
          sample = randIdx;
        }
      } else {
        if numElements < log2(X.size) {
          var indices: Set.set(int);
          var i: int = 0;
          while i < numElements {
            var randVal = stream.getNext(resultType=int, 0, X.size-1);
            if !indices.contains(randVal) {
              var randIdx = X.dim(0).orderToIndex(randVal);
              samples[i] = randIdx;
              indices.add(randVal);
              i += 1;
            }
          }
        } else {
          var indices: [X] int = X;
          shuffle(indices);
          for i in samples.domain {
            samples[i] = (indices[X.dim(0).orderToIndex(i)]);
          }
        }
      }
      if isIntegralType(sizeType) {
        return samples;
      } else if isDomainType(sizeType) {
        return reshape(samples, size);
      }
    }
  }

  pragma "no doc"
  /* _choice branch for distribution defined by probabilities array */
  proc _choiceProbabilities(stream, X:domain, size:?sizeType, replace, prob:?probType) throws
  {
    import Search;
    import Sort;
    
    if prob.size != X.size {
      throw new owned IllegalArgumentError('choice() x.size must be equal to prob.size');
    }
    
    if prob.size == 0 then
      throw new owned IllegalArgumentError('choice() prob array cannot be empty');

    const low = X.alignedLow,
          stride = abs(X.stride);
    ref P = prob.reindex(0..<X.size);

    // Construct cumulative sum array
    var cumulativeArr = (+ scan P): real;

    if !Sort.isSorted(cumulativeArr) then
      throw new owned IllegalArgumentError("choice() prob array cannot contain negative values");

    // Confirm the array has at least one value > 0
    if cumulativeArr[P.domain.last] <= 0 then
      throw new owned IllegalArgumentError('choice() prob array requires a value greater than 0');

    // Normalize cumulative sum array
    var total = cumulativeArr[P.domain.last];
    cumulativeArr /= total;

    // Begin sampling
    if isNothingType(sizeType) {
      // Return 1 sample
      var randNum = stream.getNext(resultType=real);
      var (found, idx) = Search.binarySearch(cumulativeArr, randNum);
      return X.dim(0).orderToIndex(idx);
    } else {
      // Return numElements samples

      // Compute numElements for tuple case
      var m = 1;
      if isDomainType(sizeType) then m = size.size;

      var numElements = if isDomainType(sizeType) then m
                        else if isIntegralType(sizeType) then size:int
                        else compilerError('choice() size type must be integral or tuple of ranges');

      // Return N samples
      var samples: [0..<numElements] int;

      if replace {
        for sample in samples {
          var randNum = stream.getNext(resultType=real);
          var (found, idx) = Search.binarySearch(cumulativeArr, randNum);
          sample = X.dim(0).orderToIndex(idx);
        }
      } else {
        var indicesChosen: domain(int);
        var i = 0;
        while indicesChosen.size < samples.size {

          // Recalculate normalized cumulativeArr
          if indicesChosen.size > 0 {
            cumulativeArr = (+ scan P): real;
            total = cumulativeArr[P.domain.last];
            cumulativeArr /= total;
          }

          var remainingSamples = samples.size - indicesChosen.size;
          for randNum in stream.iterate({1..(samples.size - indicesChosen.size)}, resultType=real) {
            // A potential optimization: Generate rand nums ahead of time
            // and do a multi-target binary search to find all of their positions
            var (found, indexChosen) = Search.binarySearch(cumulativeArr, randNum);
            if !indicesChosen.contains(indexChosen) {
              indicesChosen += indexChosen;
              samples[i] = X.dim(0).orderToIndex(indexChosen);;
              i += 1;
            }
            P[indexChosen] = 0;
          }
        }
      }
      if isIntegralType(sizeType) {
        return samples;
      } else if isDomainType(sizeType) {
        return reshape(samples, size);
      }
    }
  }

  /*

    Models a stream of pseudorandom numbers.  This class is defined for
    documentation purposes and should not be instantiated. See
    :mod:`PCGRandom` and :mod:`NPBRandom` for RNGs that can be
    instantiated. To create a random stream, use :proc:`createRandomStream`.

    .. note::

      This RandomStreamInterface is expected to change`.

    .. note::

      At present, different implementations of this interface can vary in
      whether or not they can generate 0.0 and/or 1.0.  (e.g. They can be
      generated by :mod:`PCGRandom` but not by :mod:`NPBRandom`).

    .. note::

      We plan to support general serial and parallel iterator methods on
      :class:`RandomStreamInterface`; however, providing the full suite of
      iterators is not possible with our current parallel iterator framework.
      Specifically, if :class:`RandomStreamInterface` is a follower in a
      zippered iteration context, there is no way for it to update the total
      number of random numbers generated in a safe/sane/coordinated way.  We are
      exploring a revised leader-follower iterator framework that would support
      this idiom (and other cursor-based ones).  With Chapel's recent support
      for standalone parallel iterators, one could define a standalone parallel
      iterator for :class:`RandomStreamInterface`, but this effort has not yet
      been taken on.

    .. note::

     The :class:`RandomStreamInterface` is included here only for documentation
     and does not help with compilation in any way.  In the future, we hope to
     turn it into an interface.

  */
  class RandomStreamInterface {
    /*
      Specifies the type of value generated by the RandomStream.
      Not all RandomStream implementations support all types.
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
      The seed value for the PRNG.  There may be constraints upon
      legal values depending on the specific RNG.
    */
    const seed: int(64);

    /*
      Returns the next value in the random stream.

      :returns: The next value in the random stream as type :type:`eltType`.
     */
    proc getNext(): eltType {
      compilerError("RandomStreamInterface.getNext called");
      var x:eltType;
      return x;
    }

    /*
      Advances/rewinds the stream to the `n`-th value in the sequence.
      The first value corresponds to n=0.  n must be >= 0, otherwise an
      IllegalArgumentError is thrown.

      :arg n: The position in the stream to skip to.  Must be >= 0.
      :type n: `integral`

      :throws IllegalArgumentError: When called with negative `n` value.
     */
    proc skipToNth(n: integral) throws {
      compilerError("RandomStreamInterface.skipToNth called");
    }

    /*
      Advance/rewind the stream to the `n`-th value and return it
      (advancing the stream by one).  n must be >= 0, otherwise an
      IllegalArgumentError is thrown.  This is equivalent to
      :proc:`skipToNth()` followed by :proc:`getNext()`.

      :arg n: The position in the stream to skip to.  Must be >= 0.
      :type n: `integral`

      :returns: The `n`-th value in the random stream as type :type:`eltType`.
      :throws IllegalArgumentError: When called with negative `n` value.
     */
    proc getNth(n: integral): eltType throws {
      compilerError("RandomStreamInterface.getNth called");
    }

    /*
      Fill the argument array with pseudorandom values.  This method is
      identical to the standalone :proc:`fillRandom` procedure,
      except that it consumes random values from the
      :class:`RandomStreamInterface` object on which it's invoked rather
      than creating a new stream for the purpose of the call.

      :arg arr: The array to be filled
      :type arr: [] :type:`eltType`
     */
    proc fillRandom(arr: [] eltType) {
      compilerError("RandomStreamInterface.fillRandom called");
    }

    pragma "no doc"
    proc fillRandom(arr: []) {
      compilerError("RandomStreamInterface.fillRandom called");
    }

    /*
     Returns a random sample from a given 1-D array, ``x``.

     :arg x: a 1-D array with values that will be sampled from.
     :arg size: An optional integral value specifying the number of elements to
                choose, or a domain specifying the dimensions of the
                sampled array to be filled, otherwise a single element will be
                chosen.
     :arg replace: an optional ``bool`` specifying whether or not to sample with
                   replacement, i.e. elements will only be chosen up to one
                   time when ``replace=false``.
     :arg prob: an optional 1-D array that contains probabilities of choosing
                each element of ``x``, otherwise elements will be chosen over
                a uniform distribution. ``prob`` must have integral or real
                element type, with no negative values and at least one non-zero
                value. The size must be equal to that of ``x.domain``.

     :return: An element chosen from ``x`` if ``size == 1``, or an array of
              element chosen from ``x`` if ``size > 1`` or ``size`` is a
              domain.

     :throws IllegalArgumentError: if ``x.size == 0``,
                                   if ``x.size != prob.size``,
                                   if ``prob`` contains a negative value,
                                   if ``prob`` has no non-zero values,
                                   if ``size < 1 || size.size < 1``,
                                   if ``replace=false`` and ``size > x.size || size.size > x.size``
     */
     proc choice(x: [], size:?sizeType=none, replace=true, prob:?probType=none) throws
     {
       compilerError("RandomStreamInterface.choice called");
     }

    /*
     Returns a random sample from a given bounded range, ``x``.

     :arg x: a bounded range with values that will be sampled from.
     :arg size: An optional integral value specifying the number of elements to
                choose, or a domain specifying the dimensions of the
                sampled array to be filled, otherwise a single element will be
                chosen.
     :arg replace: an optional ``bool`` specifying whether or not to sample with
                   replacement, i.e. elements will only be chosen up to one
                   time when ``replace=false``.
     :arg prob: an optional 1-D array that contains probabilities of choosing
                each element of ``x``, otherwise elements will be chosen over
                a uniform distribution. ``prob`` must have integral or real
                element type, with no negative values and at least one non-zero
                value. The size must be equal to that of ``x``.

     :return: An element chosen from ``x`` if ``size == 1``, or an array of
              element chosen from ``x`` if ``size > 1`` or ``size`` is a
              domain.

     :throws IllegalArgumentError: if ``x.size == 0``,
                                   if ``x.size != prob.size``,
                                   if ``prob`` contains a negative value,
                                   if ``prob`` has no non-zero values,
                                   if ``size < 1 || size.size < 1``,
                                   if ``replace=false`` and ``size > x.size || size.size > x.size``.
                                   if ``isBoundedRange(x) == false``
     */
     proc choice(x: range(stridable=?), size:?sizeType=none, replace=true, prob:?probType=none) throws
     {
       compilerError("RandomStreamInterface.choice called");
     }

    /*
     Returns a random sample from a given 1-D domain, ``x``.

     :arg x: a 1-D dom with values that will be sampled from.
     :arg size: An optional integral value specifying the number of elements to
                choose, or a domain specifying the dimensions of the
                sampled array to be filled, otherwise a single element will be
                chosen.
     :arg replace: an optional ``bool`` specifying whether or not to sample with
                   replacement, i.e. elements will only be chosen up to one
                   time when ``replace=false``.
     :arg prob: an optional 1-D array that contains probabilities of choosing
                each element of ``x``, otherwise elements will be chosen over
                a uniform distribution. ``prob`` must have integral or real
                element type, with no negative values and at least one non-zero
                value. The size must be equal to that of ``x``.

     :return: An element chosen from ``x`` if ``size == 1``, or an array of
              element chosen from ``x`` if ``size > 1`` or ``size`` is a
              domain.

     :throws IllegalArgumentError: if ``x.size == 0``,
                                   if ``x.size != prob.size``,
                                   if ``prob`` contains a negative value,
                                   if ``prob`` has no non-zero values,
                                   if ``size < 1 || size.size < 1``,
                                   if ``replace=false`` and ``size > x.size || size.size > x.size``.
     */
     proc choice(x: domain, size:?sizeType=none, replace=true, prob:?probType=none) throws
     {
       compilerError("RandomStreamInterface.choice called");
     }


    /*

       Returns an iterable expression for generating `D.size` random
       numbers. The RNG state will be immediately advanced by `D.size`
       before the iterable expression yields any values.

       The returned iterable expression is useful in parallel contexts,
       including standalone and zippered iteration. The domain will determine
       the parallelization strategy.

       :arg D: a domain
       :arg resultType: the type of number to yield
       :return: an iterable expression yielding random `resultType` values

     */
    proc iterate(D: domain, type resultType=eltType) {
      compilerError("RandomStreamInterface.iterate called");
    }

    pragma "no doc"
    proc writeThis(f) throws {
      f <~> "RandomStreamInterface(eltType=";
      f <~> eltType:string;
      f <~> ", parSafe=";
      f <~> parSafe;
      f <~> ", seed=";
      f <~> seed;
      f <~> ")";
    }
  }

  // An apparent bug prevents this from working.
  //type RandomStream = PCGRandomStream;


  /*
     Seed generation for pseudorandom number generation


     .. note::

       For many of the functions in :mod:`Random`, if no seed is provided by the
       user, one is chosen based on the current time in microseconds, allowing
       for some degree of variability in seed selection.  The intent of
       :record:`SeedGenerator` is to provide a menu of other options for
       initializing the random stream seed, but few options are implemented
       at present. In particular, it would be useful to allow multiple tasks to
       create different seeds at the same time, or to allow different
       functions running at the same time to produce different seeds.

  */
  module RandomSupport {

    /*
      Provides methods to help generate seeds when the user doesn't want
      to create one.  It currently supports two type methods. Both start
      with the current time.
    */
    record SeedGenerator {
      /*
        Generate a seed based on the current time in microseconds as
        reported by :proc:`Time.getCurrentTime`. This seed is not
        suitable for the NPB RNG since that requires an odd seed.
      */
      proc type currentTime: int(64) {
        use Time;
        const seed = getCurrentTime(unit=TimeUnits.microseconds):int(64);
        return seed;

      }
      /*
        Generate an odd seed based on the current time in microseconds as
        reported by :proc:`Time.getCurrentTime`. This seed is suitable
        for the NPB RNG.
      */
      proc type oddCurrentTime: int(64) {
        use Time;
        const seed = getCurrentTime(unit=TimeUnits.microseconds):int(64);
        const oddseed = if seed % 2 == 0 then seed + 1 else seed;
        return oddseed;
      }
    }


  } // close module RandomSupport




  /*
     Permuted Linear Congruential Random Number Generator

     This module provides PCG random number generation routines.
     See http://www.pcg-random.org/
     and the paper, `PCG: A Family of Simple Fast Space-Efficient Statistically
     Good Algorithms for Random Number Generation` by M.E. O'Neill.

     It also includes some Chapel-specific features, such as generating real,
     imag, and complex numbers; and generating numbers in a range in parallel.
     These features are not available in the reference implementations of PCG.

     The related module :mod:`PCGRandomLib` provides a lower-level interface to
     many PCG functions.

     .. note::

       The interface provided by this module is expected to change.

  */
  module PCGRandom {

    use RandomSupport;
    public use PCGRandomLib;
    private use ChapelLocks;

    // How many generators do we need for this type?
    private
    proc numGenerators(type t) param {
      if isBoolType(t) then return 1;
      else return (numBits(t)+31) / 32;
    }

    /*

      Models a stream of pseudorandom numbers generated by the PCG random number
      generator.  See http://www.pcg-random.org/ and the paper, `PCG: A Family
      of Simple Fast Space-Efficient Statistically Good Algorithms for Random
      Number Generation` by M.E. O'Neill.

      This class builds upon the :record:`~PCGRandomLib.pcg_setseq_64_xsh_rr_32_rng` PCG RNG
      which has 64 bits of state and 32 bits of output.

      While the PCG RNG used here is believed to have good statistical
      properties, it is not suitable for generating key material for encryption
      since the output of this RNG may be predictable.
      Additionally, if statistical properties of the random numbers are very
      important, another strategy may be required.

      We have good confidence that the random numbers generated by this class
      match the C PCG reference implementation and have specifically verified
      equal output given the same seed. However, this implementation differs
      from the C PCG reference implementation in how it produces random integers
      within particular bounds (with :proc:`PCGRandomStream.getNext` using `min`
      and `max` arguments). In addition, this implementation directly supports
      the generation of random `real` values, unlike the C PCG implementation.

      Smaller numbers, such as `uint(8)` or `uint(16)`, are generated from
      the high-order bits of the 32-bit output.

      To generate larger numbers, several 32-bit-output RNGs are ganged
      together.  This strategy is recommended by the author of PCG (and
      demonstrated in the file `pcg32x2-demo.c`. Each of these 32-bit RNGs has a
      different sequence constant and so will be independent and uncorrelated.
      For example, to generate 128-bit complex numbers, this RNG will use
      4 ganged 32-bit PCG RNGs with different sequence constants. One impact of
      this approach is that this implementation will only generate 2**64
      different complex numbers with a given seed (for example).

      This class also supports generating integers within particular bounds.
      When that is required, this class uses a strategy different from the PCG
      reference implementation in order to work better in a parallel setting. In
      particular, when more than 1 random value is required as part of
      generating a value in a range, conceptually it uses more ganged-together
      RNGs (as with the 32x2 strategy). Each new value beyond the first that
      is computed will be computed with a different ganged-together RNG.
      This strategy is meant to avoid statistical bias. While we have tested
      this strategy to our satisfaction, it has not been subject to rigorous
      analysis and may have undesirable statistical properties.

      When generating a real, imaginary, or complex number, this implementation
      uses the strategy of generating a 64-bit unsigned integer and then
      multiplying it by 2.0**-64 in order to convert it to a floating point
      number. While this does construct a uniform distribution on rounded
      floating point values, it leaves out many possible real values (for
      example, 2**-128). We believe that this strategy has reasonable
      statistical properties. One side effect of this strategy is that the real
      number 1.0 can be generated because of rounding. The real number 0.0 can
      be generated because PCG can produce the value 0 as a random integer.


      We have tested this implementation with TestU01 (available at
      http://simul.iro.umontreal.ca/testu01/tu01.html ).  We measured our
      implementation with TestU01 1.2.3 and the Crush suite, which consists of
      144 statistical tests. The results were:

       * no failures for generating uniform reals
       * 1 failure for generating 32-bit values (which is also true for the
         reference version of PCG with the same configuration)
       * 0 failures for generating 64-bit values (which we provided to TestU01
         as 2 different 32-bit values since it only accepts 32 bits at a time)
       * 0 failures for generating bounded integers (which we provided to
         TestU01 by requesting values in [0..,2**31+2**30+1) until we
         had two values < 2**31, removing the top 0 bit, and then combining
         the top 16 bits into the value provided to TestU01).

    */
    class PCGRandomStream {
      /*
        Specifies the type of value generated by the PCGRandomStream.
        All numeric types are supported: `int`, `uint`, `real`, `imag`,
        `complex`, and `bool` types of all sizes.
      */
      type eltType;

      /*
        The seed value for the PRNG.
      */
      const seed: int(64);

      /*
        Indicates whether or not the PCGRandomStream needs to be
        parallel-safe by default.  If multiple tasks interact with it in
        an uncoordinated fashion, this must be set to `true`.  If it will
        only be called from a single task, or if only one task will call
        into it at a time, setting to `false` will reduce overhead related
        to ensuring mutual exclusion.
      */
      param parSafe: bool = true;

      /*
        Creates a new stream of random numbers using the specified seed
        and parallel safety.

        :arg eltType: The element type to be generated.
        :type eltType: `type`

        :arg seed: The seed to use for the PRNG.  Defaults to
          `currentTime` from :type:`RandomSupport.SeedGenerator`.
          Can be any int(64) value.
        :type seed: `int(64)`

        :arg parSafe: The parallel safety setting.  Defaults to `true`.
        :type parSafe: `bool`

      */
      proc init(type eltType,
                seed: int(64) = SeedGenerator.currentTime,
                param parSafe: bool = true) {
        this.eltType = eltType;
        this.seed = seed;
        this.parSafe = parSafe;
        this.complete();
        for param i in 0..numGenerators(eltType)-1 {
          param inc = pcg_getvalid_inc(i+1);
          PCGRandomStreamPrivate_rngs[i].srandom(seed:uint(64), inc);
        }
        PCGRandomStreamPrivate_count = 1;
      }

      pragma "no doc"
      proc PCGRandomStreamPrivate_getNext_noLock(type resultType) {
        PCGRandomStreamPrivate_count += 1;
        return randlc(resultType, PCGRandomStreamPrivate_rngs);
      }
      pragma "no doc"
      proc PCGRandomStreamPrivate_getNext_noLock(type resultType,
                                                 min:resultType,
                                                 max:resultType) {

        // If the resultType is a type that fits into
        PCGRandomStreamPrivate_count += 1;
        return randlc_bounded(resultType, PCGRandomStreamPrivate_rngs,
                              seed, PCGRandomStreamPrivate_count-1, min, max);
      }


      pragma "no doc"
      proc PCGRandomStreamPrivate_skipToNth_noLock(in n: integral) {
        PCGRandomStreamPrivate_count = n+1;
        PCGRandomStreamPrivate_rngs = randlc_skipto(eltType, seed, n+1);
      }

      /*
        Returns the next value in the random stream.

        Generated reals are in [0,1] - both 0.0 and 1.0 are possible values.
        Imaginary numbers are analogously in [0i, 1i]. Complex numbers will
        consist of a generated real and imaginary part, so 0.0+0.0i and 1.0+1.0i
        are possible.

        Generated integers cover the full value range of the integer.

        :arg resultType: the type of the result. Defaults to :type:`eltType`.
          `resultType` must be the same or a smaller size number.
        :returns: The next value in the random stream as type `resultType`.
       */
      proc getNext(type resultType=eltType): resultType {
        _lock();
        const result = PCGRandomStreamPrivate_getNext_noLock(resultType);
        _unlock();
        return result;
      }
      /*
        Return the next random value but within a particular range.
        Returns a number in [`min`, `max`] (inclusive). Halts if checks are enabled and ``min > max``.

        .. note::

           For integers, this class uses a strategy for generating a value
           in a particular range that has not been subject to rigorous
           study and may have statistical problems.

           For real numbers, this class generates a random value in [max, min]
           by computing a random value in [0,1] and scaling and shifting that
           value. Note that not all possible floating point values in
           the interval [`min`, `max`] can be constructed in this way.

       */
      proc getNext(min: eltType, max:eltType): eltType {
        _lock();
        if boundsChecking && min > max then
          HaltWrappers.boundsCheckHalt("Cannot generate random numbers within empty range: [" + min:string + ", " + max:string +  "]");

        const result = PCGRandomStreamPrivate_getNext_noLock(eltType,min,max);
        _unlock();
        return result;
      }

      /*
        As with getNext(min, max) but allows specifying the result type.
       */
      proc getNext(type resultType,
                   min: resultType, max:resultType): resultType {
        _lock();
        if boundsChecking && min > max then
          HaltWrappers.boundsCheckHalt("Cannot generate random numbers within empty range: [" + min:string + ", " + max:string + "]");

        const result = PCGRandomStreamPrivate_getNext_noLock(resultType,min,max);
        _unlock();
        return result;
      }

      /*
        Advances/rewinds the stream to the `n`-th value in the sequence.
        The first value corresponds to n=0.  n must be >= 0, otherwise an
        IllegalArgumentError is thrown.

        :arg n: The position in the stream to skip to.  Must be >= 0.
        :type n: `integral`

        :throws IllegalArgumentError: When called with negative `n` value.
       */
      proc skipToNth(n: integral) throws {
        if n < 0 then
          throw new owned IllegalArgumentError("PCGRandomStream.skipToNth(n) called with negative 'n' value " + n:string);
        _lock();
        PCGRandomStreamPrivate_skipToNth_noLock(n);
        _unlock();
      }

      /*
        Advance/rewind the stream to the `n`-th value and return it
        (advancing the stream by one).  n must be >= 0, otherwise an
        IllegalArgumentError is thrown.  This is equivalent to
        :proc:`skipToNth()` followed by :proc:`getNext()`.

        :arg n: The position in the stream to skip to.  Must be >= 0.
        :type n: `integral`

        :returns: The `n`-th value in the random stream as type :type:`eltType`.
        :throws IllegalArgumentError: When called with negative `n` value.
       */
      proc getNth(n: integral): eltType throws {
        if (n < 0) then
          throw new owned IllegalArgumentError("PCGRandomStream.getNth(n) called with negative 'n' value " + n:string);
        _lock();
        PCGRandomStreamPrivate_skipToNth_noLock(n);
        const result = PCGRandomStreamPrivate_getNext_noLock(eltType);
        _unlock();
        return result;
      }

      /*
        Fill the argument array with pseudorandom values.  This method is
        identical to the standalone :proc:`~Random.fillRandom` procedure,
        except that it consumes random values from the
        :class:`PCGRandomStream` object on which it's invoked rather
        than creating a new stream for the purpose of the call.

        :arg arr: The array to be filled
        :type arr: [] :type:`eltType`
      */
      proc fillRandom(arr: [] eltType) {
        forall (x, r) in zip(arr, iterate(arr.domain, arr.eltType)) do
          x = r;
      }

      /*
     Returns a random sample from a given 1-D array, ``x``.

     :arg x: a 1-D array with values that will be sampled from.
     :arg size: An optional integral value specifying the number of elements to
                choose, or a domain specifying the dimensions of the
                sampled array to be filled, otherwise a single element will be
                chosen.
     :arg replace: an optional ``bool`` specifying whether or not to sample with
                   replacement, i.e. elements will only be chosen up to one
                   time when ``replace=false``.
     :arg prob: an optional 1-D array that contains probabilities of choosing
                each element of ``x``, otherwise elements will be chosen over
                a uniform distribution. ``prob`` must have integral or real
                element type, with no negative values and at least one non-zero
                value. The size must be equal to that of ``x.domain``.

     :return: An element chosen from ``x`` if ``size == 1``, or an array of
              element chosen from ``x`` if ``size > 1`` or ``size`` is a
              domain.

     :throws IllegalArgumentError: if ``x.size == 0``,
                                   if ``x.size != prob.size``,
                                   if ``prob`` contains a negative value,
                                   if ``prob`` has no non-zero values,
                                   if ``size < 1 || size.size < 1``,
                                   if ``replace=false`` and ``size > x.size || size.size > x.size``
     */
      proc choice(x: [?dom], size:?sizeType=none, replace=true, prob:?probType=none)
        throws
      {
        var idx = _choice(this, dom, size=size, replace=replace, prob=prob);
        return x[idx];
      }

      /*
     Returns a random sample from a given bounded range, ``x``.

     :arg x: a bounded range with values that will be sampled from.
     :arg size: An optional integral value specifying the number of elements to
                choose, or a domain specifying the dimensions of the
                sampled array to be filled, otherwise a single element will be
                chosen.
     :arg replace: an optional ``bool`` specifying whether or not to sample with
                   replacement, i.e. elements will only be chosen up to one
                   time when ``replace=false``.
     :arg prob: an optional 1-D array that contains probabilities of choosing
                each element of ``x``, otherwise elements will be chosen over
                a uniform distribution. ``prob`` must have integral or real
                element type, with no negative values and at least one non-zero
                value. The size must be equal to that of ``x``.

     :return: An element chosen from ``x`` if ``size == 1``, or an array of
              element chosen from ``x`` if ``size > 1`` or ``size`` is a
              domain.

     :throws IllegalArgumentError: if ``x.size == 0``,
                                   if ``x.size != prob.size``,
                                   if ``prob`` contains a negative value,
                                   if ``prob`` has no non-zero values,
                                   if ``size < 1 || size.size < 1``,
                                   if ``replace=false`` and ``size > x.size || size.size > x.size``.
                                   if ``isBoundedRange(x) == false``
     */
      proc choice(x: range(stridable=?), size:?sizeType=none, replace=true, prob:?probType=none)
        throws
      { 
        var dom: domain(1,stridable=true);

        if !isBoundedRange(x) then
          throw new owned IllegalArgumentError('input range must be bounded');
        else
          dom = {x};
        return _choice(this, dom, size=size, replace=replace, prob=prob);
      }

      /*
     Returns a random sample from a given 1-D domain, ``x``.

     :arg x: a 1-D dom with values that will be sampled from.
     :arg size: An optional integral value specifying the number of elements to
                choose, or a domain specifying the dimensions of the
                sampled array to be filled, otherwise a single element will be
                chosen.
     :arg replace: an optional ``bool`` specifying whether or not to sample with
                   replacement, i.e. elements will only be chosen up to one
                   time when ``replace=false``.
     :arg prob: an optional 1-D array that contains probabilities of choosing
                each element of ``x``, otherwise elements will be chosen over
                a uniform distribution. ``prob`` must have integral or real
                element type, with no negative values and at least one non-zero
                value. The size must be equal to that of ``x``.

     :return: An element chosen from ``x`` if ``size == 1``, or an array of
              element chosen from ``x`` if ``size > 1`` or ``size`` is a
              domain.

     :throws IllegalArgumentError: if ``x.size == 0``,
                                   if ``x.size != prob.size``,
                                   if ``prob`` contains a negative value,
                                   if ``prob`` has no non-zero values,
                                   if ``size < 1 || size.size < 1``,
                                   if ``replace=false`` and ``size > x.size || size.size > x.size``.
     */
      proc choice(x: domain, size:?sizeType=none, replace=true, prob:?probType=none)
        throws
      {
        return _choice(this, x, size=size, replace=replace, prob=prob);
      }

      /* Randomly shuffle a 1-D array. */
      proc shuffle(arr: [?D] ?eltType ) {

        if D.rank != 1 then
          compilerError("Shuffle requires 1-D array");

        const low = D.alignedLow,
              stride = abs(D.stride);

        _lock();

        // Fisher-Yates shuffle
        for i in 0..#D.size by -1 {
          var k = randlc_bounded(D.idxType,
                                 PCGRandomStreamPrivate_rngs,
                                 seed, PCGRandomStreamPrivate_count,
                                 0, i);

          var j = i;

          // Strided case
          if stride > 1 {
            k *= stride;
            j *= stride;
          }

          // Alignment offsets
          k += low;
          j += low;

          arr[k] <=> arr[j];
        }

        PCGRandomStreamPrivate_count += D.size;

        _unlock();
      }

      /* Produce a random permutation, storing it in a 1-D array.
         The resulting array will include each value from low..high
         exactly once, where low and high refer to the array's domain.
         */
      proc permutation(arr: [] eltType) {
        var low = arr.domain.dim(0).low;
        var high = arr.domain.dim(0).high;

        if arr.domain.rank != 1 then
          compilerError("Permutation requires 1-D array");
        //if arr.domain.dim(0).stridable then
        //  compilerError("Permutation requires non-stridable 1-D array");

        _lock();

        for i in low..high {
          var j = randlc_bounded(arr.domain.idxType,
                                 PCGRandomStreamPrivate_rngs,
                                 seed, PCGRandomStreamPrivate_count,
                                 low, i);
          arr[i] = arr[j];
          arr[j] = i;
        }

        PCGRandomStreamPrivate_count += high-low;

        _unlock();
      }


      pragma "no doc"
      proc fillRandom(arr: []) {
        compilerError("PCGRandomStream(eltType=", eltType:string,
                      ") can only be used to fill arrays of ", eltType:string);
      }

      /*

         Returns an iterable expression for generating `D.size` random
         numbers. The RNG state will be immediately advanced by `D.size`
         before the iterable expression yields any values.

         The returned iterable expression is useful in parallel contexts,
         including standalone and zippered iteration. The domain will determine
         the parallelization strategy.

         :arg D: a domain
         :arg resultType: the type of number to yield
         :return: an iterable expression yielding random `resultType` values

       */
      pragma "fn returns iterator"
      proc iterate(D: domain, type resultType=eltType) {
        _lock();
        const start = PCGRandomStreamPrivate_count;
        PCGRandomStreamPrivate_count += D.size.safeCast(int(64));
        PCGRandomStreamPrivate_skipToNth_noLock(PCGRandomStreamPrivate_count-1);
        _unlock();
        return PCGRandomPrivate_iterate(resultType, D, seed, start);
      }

      // Forward the leader iterator as well.
      pragma "no doc"
      pragma "fn returns iterator"
      proc iterate(D: domain, type resultType=eltType, param tag)
        where tag == iterKind.leader
      {
        // Note that proc iterate() for the serial case (i.e. the one above)
        // is going to be invoked as well, so we should not be taking
        // any actions here other than the forwarding.
        const start = PCGRandomStreamPrivate_count;
        return PCGRandomPrivate_iterate(resultType, D, seed, start, tag);
      }

      pragma "no doc"
      override proc writeThis(f) throws {
        f <~> "PCGRandomStream(eltType=";
        f <~> eltType:string;
        f <~> ", parSafe=";
        f <~> parSafe;
        f <~> ", seed=";
        f <~> seed;
        f <~> ")";
      }

      ///////////////////////////////////////////////////////// CLASS PRIVATE //
      //
      // It is the intent that once Chapel supports the notion of
      // 'private', everything in this class declared below this line will
      // be made private to this class.
      //


      pragma "no doc"
      var _l: if parSafe then chpl_LocalSpinlock else nothing;
      pragma "no doc"
      inline proc _lock() {
        if parSafe then _l.lock();
      }
      pragma "no doc"
      inline proc _unlock() {
        if parSafe then _l.unlock();
      }
      // up to 4 RNGs
      pragma "no doc"
      var PCGRandomStreamPrivate_rngs: numGenerators(eltType) * pcg_setseq_64_xsh_rr_32_rng;
      pragma "no doc"
      var PCGRandomStreamPrivate_count: int(64) = 1;
    }


    ////////////////////////////////////////////////////////// MODULE PRIVATE //
    //
    // It is the intent that once Chapel supports the notion of 'private',
    // everything declared below this line will be made private to this
    // module.
    //


    // returns a random number in [0, 1]
    // where the number is a multiple of 2**-64
    private inline
    proc randToReal64(x: uint(64)):real(64)
    {
      return ldexp(x:real(64), -64);
    }
    // returns a random number in [min, max]
    // by scaling a multiple of 2**-64 by (max-min)
    private inline
    proc randToReal64(x: uint(64), min:real(64), max:real(64)):real(64)
    {
      var normalized = randToReal64(x);
      return (max-min)*normalized + min;
    }

    // returns a random number in [0, 1]
    // where the number is a rounded multiple of 2**-24
    private inline
    proc randToReal32(x: uint(32))
    {
      return ldexp(x:real(32), -32);
    }

    // returns a random number in [min, max)
    // where the number is a multiple of 2**-24
    private inline
    proc randToReal32(x: uint(32), min:real(32), max:real(32)):real(32)
    {
      var normalized = randToReal32(x);
      return (max-min)*normalized + min;
    }


    // These would form the RNG interface.
    private inline
    proc rand32_1(ref states):uint(32) {
      return states[0].random(pcg_getvalid_inc(1));
    }
    private inline
    proc rand32_2(ref states):uint(32) {
      return states[1].random(pcg_getvalid_inc(2));
    }
    // returns x with 0 <= x <= bound
    // count is 1-based
    private inline
    proc boundedrand32_1(ref states, seed:int(64), count:int(64),
                         bound:uint(32)):uint(32) {
      // just get 32 random bits if bound+1 is not representable.
      if bound == max(uint(32)) then return rand32_1(states);
      else return states[0].bounded_random_vary_inc(
          pcg_getvalid_inc(1), bound + 1,
          seed:uint(64), (count - 1):uint(64),
          101, 4);
    }
    // returns x with 0 <= x <= bound
    // count is 1-based
    private inline
    proc boundedrand32_2(ref states, seed:int(64), count:int(64),
                         bound:uint(32)):uint(32) {
      // just get 32 random bits if bound+1 is not representable.
      if bound == max(uint(32)) then return rand32_2(states);
      else return states[1].bounded_random_vary_inc(
          pcg_getvalid_inc(2), bound + 1,
          seed:uint(64), (count - 1):uint(64),
          102, 4);
    }

    private inline
    proc rand64_1(ref states):uint(64) {
      var ret:uint(64) = 0;
      ret |= states[0].random(pcg_getvalid_inc(1));
      ret <<= 32;
      ret |= states[1].random(pcg_getvalid_inc(2));
      return ret;
    }
    private inline
    proc rand64_2(ref states):uint(64) {
      var ret:uint(64) = 0;
      ret |= states[2].random(pcg_getvalid_inc(3));
      ret <<= 32;
      ret |= states[3].random(pcg_getvalid_inc(4));
      return ret;
    }

    // Returns an unsigned integer x with 0 <= x <= bound
    // count is 1-based
    private proc boundedrand64_1(ref states, seed:int(64), count:int(64),
                                 bound:uint):uint
    {
      if bound > max(uint(32)):uint {
        var toprand = 0:uint;
        var botrand = 0:uint;

        // compute the bounded number in two calls to a 32-bit RNG
        toprand = boundedrand32_1(states, seed, count, (bound >> 32):uint(32));
        botrand = boundedrand32_2(states, seed, count, (bound & max(uint(32))):uint(32));
        return (toprand << 32) | botrand;
      } else {
        // Generate a # with RNG 1 but ignore it, to keep the
        // stepping consistent.
        rand32_1(states);
        return boundedrand32_2(states, seed, count, bound:uint(32));
      }
    }

    private
    proc checkSufficientBitsAndAdvanceOthers(type resultType, ref states) {
      // Note - this error could be eliminated if we used
      // the same strategy as bounded_rand_vary_inc and
      // just computed the RNGs at the later incs
      param numGenForResultType = numGenerators(resultType);
      param numGen = states.size;
      if numGenForResultType > numGen then
        compilerError("PCGRandomStream cannot produce " +
                      resultType:string +
                      " (requiring " +
                      (32*numGenForResultType):string +
                      " bits) from a stream configured for " +
                      (32*numGen):string +
                      " bits of output");

      // Step each RNG that is not involved in the output.
      for i in numGenForResultType+1..numGen {
        states[i-1].random(pcg_getvalid_inc(i:uint));
      }
    }


    // Wrapper that takes a result type
    private inline
    proc randlc(type resultType, ref states) {

      checkSufficientBitsAndAdvanceOthers(resultType, states);

      if resultType == complex(128) {
        return (randToReal64(rand64_1(states)),
                randToReal64(rand64_2(states))):complex(128);
      } else if resultType == complex(64) {
        return (randToReal32(rand32_1(states)),
                randToReal32(rand32_2(states))):complex(64);
      } else if resultType == imag(64) {
        return _r2i(randToReal64(rand64_1(states)));
      } else if resultType == imag(32) {
        return _r2i(randToReal32(rand32_1(states)));
      } else if resultType == real(64) {
        return randToReal64(rand64_1(states));
      } else if resultType == real(32) {
        return randToReal32(rand32_1(states));
      } else if resultType == uint(64) || resultType == int(64) {
        return rand64_1(states):resultType;
      } else if resultType == uint(32) || resultType == int(32) {
        return rand32_1(states):resultType;
      } else if(resultType == uint(16) ||
                resultType == int(16)) {
        return (rand32_1(states) >> 16):resultType;
      } else if(resultType == uint(8) ||
                resultType == int(8)) {
        return (rand32_1(states) >> 24):resultType;
      } else if isBoolType(resultType) {
        return (rand32_1(states) >> 31) != 0;
      }
    }

    // returns x with min <= x <= max (for integers)
    // and min <= x < max (for real/complex/imag)
    // seed should be the initial seed of the RNG
    // count should be the current count value
    private inline
    proc randlc_bounded(type resultType,
                        ref states, seed:int(64), count:int(64),
                        min, max) {

      checkSufficientBitsAndAdvanceOthers(resultType, states);

      if resultType == complex(128) {
        return (randToReal64(rand64_1(states), min.re, max.re),
                randToReal64(rand64_2(states), min.im, max.im)):complex(128);
      } else if resultType == complex(64) {
        return (randToReal32(rand32_1(states), min.re, max.re),
                randToReal32(rand32_2(states), min.im, max.im)):complex(64);
      } else if resultType == imag(64) {
        return _r2i(randToReal64(rand64_1(states), _i2r(min), _i2r(max)));
      } else if resultType == imag(32) {
        return _r2i(randToReal32(rand32_1(states), _i2r(min), _i2r(max)));
      } else if resultType == real(64) {
        return randToReal64(rand64_1(states), min, max);
      } else if resultType == real(32) {
        return randToReal32(rand32_1(states), min, max);
      } else if resultType == uint(64) || resultType == int(64) {
        return (boundedrand64_1(states, seed, count, (max-min):uint(64)) + min:uint(64)):resultType;
      } else if resultType == uint(32) || resultType == int(32) {
        return (boundedrand32_1(states, seed, count, (max-min):uint(32)) + min:uint(32)):resultType;
      } else if(resultType == uint(16) ||
                resultType == int(16)) {
        return (boundedrand32_1(states, seed, count, (max-min):uint(32)) + min:uint(32)):resultType;
      } else if(resultType == uint(8) ||
                resultType == int(8)) {
        return (boundedrand32_1(states, seed, count, (max-min):uint(32)) + min:uint(32)):resultType;
      } else if isBoolType(resultType) {
        compilerError("bounded rand with boolean type");
        return false;
      }
    }

    //
    // Return a value for the cursor so that the next call to randlc will
    // return the same value as the nth call to randlc
    //
    // resultType is used to compute the size required.
    private proc randlc_skipto(type resultType, seed: int(64), n: integral) {
      var states: numGenerators(resultType) * pcg_setseq_64_xsh_rr_32_rng;

      for param i in 0..states.size-1 {
        param inc = pcg_getvalid_inc(i+1);
        states[i].srandom(seed:uint(64), inc);
        states[i].advance(inc, (n - 1):uint(64));
      }
      return states;
    }

    //
    // iterate over outer ranges in tuple of ranges
    //
    private iter outer(ranges, param dim: int = 0) {
      if dim + 2 == ranges.size {
        for i in ranges(dim) do
          yield (i,);
      } else if dim + 2 < ranges.size {
        for i in ranges(dim) do
          for j in outer(ranges, dim+1) do
            yield (i, (...j));
      } else {
        yield 0; // 1D case is a noop
      }
    }

    //
    // PCGRandomStream iterator implementation
    //
    pragma "no doc"
    iter PCGRandomPrivate_iterate(type resultType, D: domain, seed: int(64),
                               start: int(64)) {
      var cursor = randlc_skipto(resultType, seed, start);
      for i in D do
        yield randlc(resultType, cursor);
    }

    pragma "no doc"
    iter PCGRandomPrivate_iterate(type resultType, D: domain, seed: int(64),
                               start: int(64), param tag: iterKind)
          where tag == iterKind.leader {
      for block in D.these(tag=iterKind.leader) do
        yield block;
    }

    pragma "no doc"
    iter PCGRandomPrivate_iterate(type resultType, D: domain, seed: int(64),
                 start: int(64), param tag: iterKind, followThis)
          where tag == iterKind.follower {
      param multiplier = 1;
      const ZD = computeZeroBasedDomain(D);
      const innerRange = followThis(ZD.rank-1);
      for outer in outer(followThis) {
        var myStart = start;
        if ZD.rank > 1 then
          myStart += multiplier * ZD.indexOrder(((...outer), innerRange.low)).safeCast(int(64));
        else
          myStart += multiplier * ZD.indexOrder(innerRange.low).safeCast(int(64));
        if !innerRange.stridable {
          var cursor = randlc_skipto(resultType, seed, myStart);
          for i in innerRange do
            yield randlc(resultType, cursor);
        } else {
          myStart -= innerRange.low.safeCast(int(64));
          for i in innerRange {
            var cursor = randlc_skipto(resultType, seed, myStart + i.safeCast(int(64)) * multiplier);
            yield randlc(resultType, cursor);
          }
        }
      }
    }


  } // close module PCGRandom

  /*

   Low-level PCG RNG implementation.

   This module includes a number of low-level PCG random functions.
   See http://www.pcg-random.org/
   and the paper, `PCG: A Family of Simple Fast Space-Efficient Statistically
   Good Algorithms for Random Number Generation` by M.E. O'Neill.

   This module provides the following low-level PCG RNGs:

     * :record:`pcg_setseq_64_xsh_rr_32_rng` (the default PCG RNG)
     * :record:`pcg_setseq_64_rxs_m_xs_64_rng`
     * :record:`pcg_setseq_32_rxs_m_xs_32_rng`
     * :record:`pcg_setseq_16_rxs_m_xs_16_rng`
     * :record:`pcg_setseq_8_rxs_m_xs_8_rng`
     * :record:`pcg_setseq_N_rxs_m_xs_N_rng` which is a generalization of the
       above

   These names come from the PCG paper and reference implementations.  The
   first integer is the number of state bits, and the last integer is the
   number of output bits. The other parts describe the permutation function in
   the LCG.

   Conceptually, a PCG RNG consists of three things:

     * the algorithm variant (e.g. pcg_setseq_64_xsh_rr_32_rng)
     * the current state
     * the RNG sequence constant

   Note that the sequence constant must be odd for this generator to
   function correctly. The function :proc:`pcg_getvalid_inc` is available
   to construct an odd number based upon an arbitrary input.

   When using these low-level interfaces, the sequence constant is not
   actually stored inside the RNG. Instead, users of this interface
   must pass the same sequence constant used in `srandom` to each of
   the other calls for that RNG object.

   Besides storing the RNG state in the record, each of these PCG RNGs
   include at least the following methods:

     * `srandom` to initialize the state from a seed
     * `random` to produce the next random value
     * `advance` to skip ahead some number of steps in RNG generation

   */
  module PCGRandomLib {

    // Translated from PCG-C-basic-0.9
    // Keeping the same function names in order to simplify maintenance
    // These functions correspond to pcg32_random_r in that version.

    // each RNG has a state and an inc value.
    // the inc value does not change as the RNG iterates, but it must always
    // be odd.
    // the state changes and can be any value.


    private
    param PCG_DEFAULT_MULTIPLIER_8 = 141;

    private
    param PCG_DEFAULT_MULTIPLIER_16 = 12829;

    private
    param PCG_DEFAULT_MULTIPLIER_32 = 747796405;

    private
    param PCG_DEFAULT_MULTIPLIER_64 = 6364136223846793005;

    private inline
    proc pcg_setseq_8_step_r(ref state:uint(8), inc:uint(8))
    {
      state = state * PCG_DEFAULT_MULTIPLIER_8 + inc;
    }

    private inline
    proc pcg_setseq_16_step_r(ref state:uint(16), inc:uint(16))
    {
      state = state * PCG_DEFAULT_MULTIPLIER_16 + inc;
    }


    private inline
    proc pcg_setseq_32_step_r(ref state:uint(32), inc:uint(32))
    {
      state = state * PCG_DEFAULT_MULTIPLIER_32 + inc;
    }

    private inline
    proc pcg_setseq_64_step_r(ref state:uint(64), inc:uint(64))
    {
      state = state * PCG_DEFAULT_MULTIPLIER_64 + inc;
    }

    private inline
    proc pcg_rotr_32(value:uint(32), rot:uint(32)):uint(32)
    {
      // having trouble using BitOps...
      extern proc chpl_bitops_rotr_32(x: uint(32), n: uint(32)) : uint(32);

      var ret = chpl_bitops_rotr_32(value, rot);
      //writef("\n rotr %i of %xu is %xu\n", rot, value, ret);
      return ret;
    }

    private inline
    proc pcg_output_xsh_rr_64_32(state:uint(64)):uint(32)
    {
      const xorshifted:uint(32) = (((state >> 18) ^ state) >> 27):uint(32);
      const rot:uint(32) = (state >> 59):uint(32);
      const ret = pcg_rotr_32(xorshifted, rot);
      // writef("\n returning r %xu\n", ret);
      return ret;
    }


    private inline
    proc pcg_output_rxs_m_xs_8_8(state:uint(8)):uint(8)
    {
      const word:uint(8) = ((state >> ((state >> 6) + 2)) ^ state) * 217;
      return (word >> 6) ^ word;
    }

    private inline
    proc pcg_output_rxs_m_xs_16_16(state:uint(16)):uint(16)
    {
      const word:uint(16) = ((state >> ((state >> 13) + 3)) ^ state) * 62169;
      return (word >> 11) ^ word;
    }

    private inline
    proc pcg_output_rxs_m_xs_32_32(state:uint(32)):uint(32)
    {
      const word:uint(32) = ((state >> ((state >> 28) + 4)) ^ state) * 277803737;
      return (word >> 22) ^ word;
    }

    private inline
    proc pcg_output_rxs_m_xs_64_64(state:uint(64)):uint(64)
    {
      const word:uint(64) = ((state >> ((state >> 59) + 5)) ^ state)
                              * 12605985483714917081;
      return (word >> 43) ^ word;
    }


    /*
       Low-level PCG random number generation interface (64-bits of state,
       32-bits output).

       This record implements the same RNG as pcg32_random_r does in
       PCG-C-0.94.

       This RNG has 64-bits of internal state and outputs 32-bits at a time.

       This RNG will iterate through all possible 64-bit values of state.
       The sequence constant chooses between 2**63 random sequences that the
       RNG is iterating through.

     */
    record pcg_setseq_64_xsh_rr_32_rng {
      /* The RNG state */
      var state:uint(64);

      /* Seed the random number generator.
         This function corresponds to pcg32_srandom_r.

         :arg seed: The initial internal state.
         :arg inc: The sequence constant.
       */
      inline proc srandom(seed:uint(64), inc:uint(64))
      {
        // this is pcg_setseq_64_srandom_r

        //state = 0;
        //inc = pcg_getvalid_inc(initseq);
        //pcg_setseq_64_step_r(state, inc); // this just sets state = inc.
        //state += initstate;
        state = inc + seed;
        pcg_setseq_64_step_r(state, inc);
      }

      /* Get the next 32-bit random number.
         This function corresponds to pcg32_random_r.

         :arg inc: The sequence constant (same as passed to `srandom`)
         :returns: 32 bits generated by the RNG.
       */
      inline proc random(inc:uint(64)):uint(32)
      {
        // this is pcg_setseq_64_xsh_rr_32_random_r
        const oldstate:uint(64) = state;
        //writef("\n state=%xu inc=%xu\n", state, inc);
        pcg_setseq_64_step_r(state, inc);
        return pcg_output_xsh_rr_64_32(oldstate);
      }

      /* Generate a random number in [0,bound).

         This function corresponds to pcg32_boundedrand_r and can
         call the random-number generator more than once.

         :arg inc: The sequence constant (same as passed to `srandom`)
         :arg bound: The returned value will be < `bound`.
         :returns: a random number in [0,bound).
       */
      inline
      proc bounded_random(inc:uint(64), bound:uint(32))
      {
            // This comment is from pcg32_boundedrand_r:
            // To avoid bias, we need to make the range of the RNG a multiple of
            // bound, which we do by dropping output less than a threshold.
            // A naive scheme to calculate the threshold would be to do
            //
            //     uint32_t threshold = 0x100000000ull % bound;
            //
            // but 64-bit div/mod is slower than 32-bit div/mod (especially on
            // 32-bit platforms).  In essence, we do
            //
            //     uint32_t threshold = (0x100000000ull-bound) % bound;
            //
            // because this version will calculate the same modulus, but the LHS
            // value is less than 2^32.

        const negbound:uint(32) = ( -(bound:int(32)) ):uint(32);
        const threshold:uint(32) = negbound % bound;
        var tmprng:pcg_setseq_64_xsh_rr_32_rng;
        var tmpinc:uint(64);
        var r:uint(32);

        // Keep trying until we get a random number that is within the bounds.
        while true {
          r = random(inc);
          if r >= threshold then
            return r % bound;
        }

        // never reached.
        return 0;
      }


      /* Generate a random number in [0,bound).

         This function corresponds to pcg32_boundedrand_r, but has one
         difference. Because parallel random number generation relies
         upon advancing to a known position, this function only advances
         the RNG state once per call. Where the pcg32_boundedrand_r would
         advance the RNG state multiple times, this function creates
         a new RNGs with the same initial seed but different sequence
         numbers and uses those when more random numbers are needed.
         In this way, this strategy is similar to the strategy
         for generating 64-bit numbers by pairing 32-bit PCG RNGs.

         .. note::

           The `nextint` and `inc_increment` values need to define a sequence of
           increments that is different from other increments used. Otherwise,
           these streams will not be independent.

         .. note::

           This a strategy for generating a value in a particular range that
           has not been subject to rigorous study and may have statistical
           problems. Additionally, its performance could be improved
           in the case that this function is called many times
           in a row by caching the temporary RNGs at their current position.


         :arg inc: The sequence constant (same as passed to `srandom`)
         :arg bound: The returned value will be < `bound`.
         :arg seed: The seed this RNG started with
         :arg skip: How many numbers, before this one, has this RNG generated?
         :arg nextinc: The first increment to pass to pcg_getvalid_inc and that should be unique for this RNG. Defaults to 100.
         :arg inc_increment: Advance nextinc by inc_increment each time a new value is needed.
         :returns: a random number in [0,bound).
       */
      inline
      proc bounded_random_vary_inc(inc:uint(64), bound:uint(32),
                                   seed:uint(64), skip:uint(64),
                                   next_inc:uint(64),
                                   inc_increment:uint(64))
      {
        // unlike the PCG-C version, this version advances the RNG only
        // one iteration - if more iterations are necessary, it computes
        // a paired RNG with different inc each time a new value is needed.

            // This comment is from pcg32_boundedrand_r:
            // To avoid bias, we need to make the range of the RNG a multiple of
            // bound, which we do by dropping output less than a threshold.
            // A naive scheme to calculate the threshold would be to do
            //
            //     uint32_t threshold = 0x100000000ull % bound;
            //
            // but 64-bit div/mod is slower than 32-bit div/mod (especially on
            // 32-bit platforms).  In essence, we do
            //
            //     uint32_t threshold = (0x100000000ull-bound) % bound;
            //
            // because this version will calculate the same modulus, but the LHS
            // value is less than 2^32.

        const negbound:uint(32) = ( -(bound:int(32)) ):uint(32);
        const threshold:uint(32) = negbound % bound;
        var r:uint(32);

        // First, try getting a random number that is within the bounds.
        r = random(inc);

        if r >= threshold then
          return r % bound;

        // not the same as in PCG:
        // If we didn't get the answer we wanted, make a different RNG
        // with the same position and initial seed but with a different
        // inc.

        var newinc:uint(64);
        for newinc in next_inc.. by inc_increment {
          var tmprng:pcg_setseq_64_xsh_rr_32_rng;
          var tmpinc:uint(64);

          tmpinc = pcg_getvalid_inc(newinc);

          // Seed the tmp RNG with the provided seed
          tmprng.srandom(seed, tmpinc);
          // Advance the tmp RNG to the right offset
          tmprng.advance(tmpinc, skip);

          // Get a number from the RNG at that inc.
          r = tmprng.random(tmpinc);

          if r >= threshold then
            return r % bound;
        }

        // never reached.
        return 0;
      }

      /* Advance the RNG. Adjusts the state of the RNG to be the
         same as if `delta` calls were made to `random`.

         This function corresponds to pcg32_advance_r.

         :arg inc: The sequence constant (same as passed to `srandom`)
         :arg delta: The number of steps to jump ahead
       */
      inline
      proc advance(inc:uint(64), delta:uint(64))
      {
        // this is pcg_setseq_64_advance_r
        state = pcg_advance_lcg(64, state, delta, PCG_DEFAULT_MULTIPLIER_64, inc);
      }
    }

    /*
       Low-level PCG random number generation interface (64-bits of state,
       64-bits output).

       This record implements the same RNG as pcg64i_random_r does
       in PCG-C-0.94.

       This RNG has 64-bits of internal state and outputs 64-bits at a time.

       This generator produces each 64-bit value exactly once.

       This generator should be considered insecure since it reveals
       its entire internal state with each output.

     */
    record pcg_setseq_64_rxs_m_xs_64_rng {
      /* The RNG state */
      var state:uint(64);

      /* Seed the random number generator.
         This function corresponds to pcg64i_srandom_r.

         :arg seed: The initial internal state.
         :arg inc: The sequence constant.
       */
      inline proc srandom(seed:uint(64), inc:uint(64))
      {
        // this is pcg_setseq_64_srandom_r

        state = inc + seed;
        pcg_setseq_64_step_r(state, inc);
      }

      /* Get the next 64-bit random number.
         This function corresponds to pcg64i_random_r.

         :arg inc: The sequence constant (same as passed to `srandom`)
         :returns: 64 bits generated by the RNG.
       */
      inline proc random(inc:uint(64)):uint(64)
      {
        // this is pcg_setseq_64_rxs_m_xs_64_random_r
        const oldstate:uint(64) = state;
        pcg_setseq_64_step_r(state, inc);
        return pcg_output_rxs_m_xs_64_64(oldstate);
      }

      /* Advance the RNG. Adjusts the state of the RNG to be the
         same as if `delta` calls were made to `random`.

         This function corresponds to pcg64i_advance_r.

         :arg inc: The sequence constant (same as passed to `srandom`)
         :arg delta: The number of steps to jump ahead
       */
      inline
      proc advance(inc:uint(64), delta:uint(64))
      {
        // this is pcg_setseq_64_advance_r
        state = pcg_advance_lcg(64, state, delta, PCG_DEFAULT_MULTIPLIER_64, inc);
      }
    }

    /*
       Low-level PCG random number generation interface (32-bits of state,
       32-bits output).

       This record implements the same RNG as pcg32i_random_r does
       in PCG-C-0.94.

       This RNG has 32-bits of internal state and outputs 32-bits at a time.

       This generator produces each 32-bit value exactly once.

       This generator should be considered insecure since it reveals
       its entire internal state with each output.

     */
    record pcg_setseq_32_rxs_m_xs_32_rng {
      /* The RNG state */
      var state:uint(32);

      /* Seed the random number generator.
         This function corresponds to pcg32i_srandom_r.

         :arg seed: The initial internal state.
         :arg inc: The sequence constant
       */
      inline proc srandom(seed:uint(32), inc:uint(32))
      {
        // this is pcg_setseq_32_srandom_r

        state = inc + seed;
        pcg_setseq_32_step_r(state, inc);
      }

      /* Get the next 32-bit random number.
         This function corresponds to pcg32i_random_r.

         :arg inc: The sequence constant (same as passed to `srandom`)
         :returns: 32 bits generated by the RNG.
       */
      inline proc random(inc:uint(32)):uint(32)
      {
        // this is pcg_setseq_32_rxs_m_xs_32_random_r
        const oldstate:uint(32) = state;
        pcg_setseq_32_step_r(state, inc);
        return pcg_output_rxs_m_xs_32_32(oldstate);
      }

      /* Advance the RNG. Adjusts the state of the RNG to be the
         same as if `delta` calls were made to `random`.

         This function corresponds to pcg32i_advance_r.

         :arg inc: The sequence constant (same as passed to `srandom`)
         :arg delta: The number of steps to jump ahead
       */
      inline
      proc advance(inc:uint(32), delta:uint(32))
      {
        // this is pcg_setseq_32_advance_r
        state = pcg_advance_lcg(32, state, delta, PCG_DEFAULT_MULTIPLIER_32, inc);
      }
    }

    /*
       Low-level PCG random number generation interface (16-bits of state,
       16-bits output).

       This record implements the same RNG as pcg16i_random_r does
       in PCG-C-0.94.

       This RNG has 16-bits of internal state and outputs 16-bits at a time.

       This generator produces each 16-bit value exactly once.

       This generator should be considered insecure since it reveals
       its entire internal state with each output.

     */
    record pcg_setseq_16_rxs_m_xs_16_rng {
      /* The RNG state */
      var state:uint(16);

      /* Seed the random number generator.
         This function corresponds to pcg16i_srandom_r.

         :arg seed: The initial internal state.
         :arg inc: The sequence constant
       */
      inline proc srandom(seed:uint(16), inc:uint(16))
      {
        // this is pcg_setseq_16_srandom_r

        state = inc + seed;
        pcg_setseq_16_step_r(state, inc);
      }

      /* Get the next 16-bit random number.
         This function corresponds to pcg16i_random_r.

         :arg inc: The sequence constant (same as passed to `srandom`)
         :returns: 16 bits generated by the RNG.
       */
      inline proc random(inc:uint(16)):uint(16)
      {
        // this is pcg_setseq_16_rxs_m_xs_16_random_r
        const oldstate:uint(16) = state;
        pcg_setseq_16_step_r(state, inc);
        return pcg_output_rxs_m_xs_16_16(oldstate);
      }

      /* Advance the RNG. Adjusts the state of the RNG to be the
         same as if `delta` calls were made to `random`.

         This function corresponds to pcg16i_advance_r.

         :arg inc: The sequence constant (same as passed to `srandom`)
         :arg delta: The number of steps to jump ahead
       */
      inline
      proc advance(inc:uint(16), delta:uint(16))
      {
        // this is pcg_setseq_16_advance_r
        state = pcg_advance_lcg(16, state, delta, PCG_DEFAULT_MULTIPLIER_16, inc);
      }
    }

    /*
       Low-level PCG random number generation interface (8-bits of state,
       8-bits output).

       This record implements the same RNG as pcg8i_random_r does
       in PCG-C-0.94.

       This RNG has 8-bits of internal state and outputs 8-bits at a time.

       This generator produces each 8-bit value exactly once.

       This generator should be considered insecure since it reveals
       its entire internal state with each output.

     */
    record pcg_setseq_8_rxs_m_xs_8_rng {
      /* The RNG state */
      var state:uint(8);

      /* Seed the random number generator.
         This function corresponds to pcg16i_srandom_r.

         :arg seed: The initial internal state.
         :arg inc: The sequence constant
       */
      inline proc srandom(seed:uint(8), inc:uint(8))
      {
        // this is pcg_setseq_8_srandom_r

        state = inc + seed;
        pcg_setseq_8_step_r(state, inc);
      }

      /* Get the next 16-bit random number.
         This function corresponds to pcg16i_random_r.

         :arg inc: The sequence constant (same as passed to `srandom`)
         :returns: 16 bits generated by the RNG.
       */
      inline proc random(inc:uint(8)):uint(8)
      {
        // this is pcg_setseq_8_rxs_m_xs_8_random_r
        const oldstate:uint(8) = state;
        pcg_setseq_8_step_r(state, inc);
        return pcg_output_rxs_m_xs_8_8(oldstate);
      }

      /* Advance the RNG. Adjusts the state of the RNG to be the
         same as if `delta` calls were made to `random`.

         This function corresponds to pcg16i_advance_r.

         :arg inc: The sequence constant (same as passed to `srandom`)
         :arg delta: The number of steps to jump ahead
       */
      inline
      proc advance(inc:uint(8), delta:uint(8))
      {
        // this is pcg_setseq_8_advance_r
        state = pcg_advance_lcg(8, state, delta, PCG_DEFAULT_MULTIPLIER_8, inc);
      }
    }


    private
    proc uint_with_bits(param nbits) type {
      if nbits <= 8 then return uint(8);
      else if nbits <= 16 then return uint(16);
      else if nbits <= 32 then return uint(32);
      else return uint(64);
    }

    // zero all but the bottom nbits of x
    private inline
    proc normalize(nbits:int, x:uint) {
      var oldx = x;
      var shiftamt = 64 - nbits;
      oldx <<= shiftamt;
      return oldx >> shiftamt;
    }


    private
    proc pcg_output_rxs_m_xs_N_mine(nbits:int, state_in)
    {
      // no sense it trying to get randomness for really small examples.
      if nbits < 8 then return state_in;

      if nbits == 8 then return pcg_output_rxs_m_xs_8_8(state_in:uint(8));
      if nbits == 16 then return pcg_output_rxs_m_xs_16_16(state_in:uint(16));
      if nbits == 32 then return pcg_output_rxs_m_xs_32_32(state_in:uint(32));
      if nbits == 64 then return pcg_output_rxs_m_xs_64_64(state_in:uint(64));

      // Generic output function, adapted for a different number of bits.
      {
        var shiftamt = nbits - 4;
        var state = state_in:uint;
        var word:uint = ((state >> ((state >> shiftamt) + 2)) ^ state) * 217;
        word = normalize(nbits, word);
        return (word >> shiftamt) ^ word;
      }
    }


    /*
       Low-level PCG random number generation interface for N bits of
       state, N bits output. This generator can be useful for generating
       a permutation since it produces each N-bit output exactly once
       and N is variable.

       This record implements an N-bit random number generator based upon
       :record:`pcg_setseq_64_rxs_m_xs_64_rng`,
       :record:`pcg_setseq_32_rxs_m_xs_32_rng`,
       :record:`pcg_setseq_16_rxs_m_xs_16_rng`,
       :record:`pcg_setseq_8_rxs_m_xs_8_rng`, and a custom generalization of
       these generators. This generator always truncates its internal state to N
       bits.

       This generator should be considered insecure since it reveals its
       entire internal state with each output.  It produces each N-bit value
       exactly once.

     */
    record pcg_setseq_N_rxs_m_xs_N_rng {
      /* the number of bits in state and in each output random number */
      const N;
      /* The RNG state */
      var state:uint;

      // zero all but the bottom N bits of state
      pragma "no doc"
      inline
      proc mask_state() {
        state = normalize(N, state);
      }

      /* Seed the random number generator.

         :arg seed: The initial internal state.
         :arg inc: The sequence constant
       */
      inline proc srandom(seed:uint, inc:uint)
      {
        state = inc + seed;
        if N <= 8 {
          var tmpstate = state:uint(8);
          pcg_setseq_8_step_r(tmpstate, inc:uint(8));
          state = tmpstate;
        } else if N <= 16 {
          var tmpstate = state:uint(16);
          pcg_setseq_16_step_r(tmpstate, inc:uint(16));
          state = tmpstate;
        } else if N <= 32 {
          var tmpstate = state:uint(32);
          pcg_setseq_32_step_r(tmpstate, inc:uint(32));
          state = tmpstate;
        } else if N <= 64 {
          pcg_setseq_64_step_r(state, inc);
        }
        mask_state();
      }

      /* Get the next N-bit random number.

         :arg inc: The sequence constant (same as passed to `srandom`)
         :returns: N bits generated by the RNG.
       */
      inline proc random(inc:uint):uint
      {
        if N <= 8 {
          var tmpstate = state:uint(8);
          var oldstate = tmpstate;
          pcg_setseq_8_step_r(tmpstate, inc:uint(8));
          state = tmpstate;
          mask_state();
          return normalize(N, pcg_output_rxs_m_xs_N_mine(N, oldstate));
        } else if N <= 16 {
          var tmpstate = state:uint(16);
          var oldstate = tmpstate;
          pcg_setseq_16_step_r(tmpstate, inc:uint(16));
          state = tmpstate;
          mask_state();
          return normalize(N, pcg_output_rxs_m_xs_N_mine(N, oldstate));
        } else if N <= 32 {
          var tmpstate = state:uint(32);
          var oldstate = tmpstate;
          pcg_setseq_32_step_r(tmpstate, inc:uint(32));
          state = tmpstate;
          mask_state();
          return normalize(N, pcg_output_rxs_m_xs_N_mine(N, oldstate));
        } else if N <= 64 {
          var oldstate = state;
          pcg_setseq_64_step_r(state, inc);
          mask_state();
          return normalize(N, pcg_output_rxs_m_xs_N_mine(N, oldstate));
        }
        return 0;
      }

      /* Advance the RNG. Adjusts the state of the RNG to be the
         same as if `delta` calls were made to `random`.

         :arg inc: The sequence constant (same as passed to `srandom`)
         :arg delta: The number of steps to jump ahead
       */
      inline
      proc advance(inc:uint, delta:uint)
      {
        if N <= 8 {
          state = pcg_advance_lcg(8, state:uint(8), delta:uint(8),
                                  PCG_DEFAULT_MULTIPLIER_8, inc:uint(8));
          mask_state();
        } else if N <= 16 {
          state = pcg_advance_lcg(16, state:uint(16), delta:uint(16),
                                  PCG_DEFAULT_MULTIPLIER_16, inc:uint(16));
          mask_state();
        } else if N <= 32 {
          state = pcg_advance_lcg(32, state:uint(32), delta:uint(32),
                                  PCG_DEFAULT_MULTIPLIER_32, inc:uint(32));
          mask_state();
        } else if N <= 64 {
          state = pcg_advance_lcg(64, state, delta, PCG_DEFAULT_MULTIPLIER_64, inc);
          mask_state();
        }
      }
    }

    /* The `inc` field in the PCG RNG must be odd.
       This function arranges for that to be the case given any input.
     */
    inline
    proc pcg_getvalid_inc(initseq:uint(64)):uint(64) return (initseq<<1) | 1;
    pragma "no doc" // documented in the not param version
    inline
    proc pcg_getvalid_inc(param initseq:uint(64)) param return (initseq<<1) | 1;


    // pcg_advance_lcg_8/16/32/64
    private
    proc pcg_advance_lcg(param bits,
                         state:uint_with_bits(bits),
                         in delta:uint_with_bits(bits),
                         in cur_mult:uint_with_bits(bits),
                         in cur_plus:uint_with_bits(bits)):uint_with_bits(bits)
    {
      var acc_mult:uint_with_bits(bits) = 1;
      var acc_plus:uint_with_bits(bits) = 0;
      while delta > 0 {
        if (delta & 1) != 0 {
          acc_mult *= cur_mult;
          acc_plus = acc_plus * cur_mult + cur_plus;
        }
        cur_plus = (cur_mult + 1) * cur_plus;
        cur_mult *= cur_mult;
        delta /= 2;
      }
      return acc_mult * state + acc_plus;
    }

  } // end PCGRandomLib

  /*
     NAS Parallel Benchmark RNG

     The pseudorandom number generator (PRNG) implemented by
     this module uses the algorithm from the NAS Parallel Benchmarks
     (NPB, available at: https://www.nas.nasa.gov/publications/npb.html
     which can be used to generate random values of type `real(64)`,
     `imag(64)`, and `complex(128)`.

     Paraphrasing the comments from the NPB reference implementation:

       This generator returns uniform pseudorandom real values in the
       range (0, 1) by using the linear congruential generator

         `x_{k+1} = a x_k  (mod 2**46)`

       where 0 < x_k < 2**46 and 0 < a < 2**46.  This scheme
       generates 2**44 numbers before repeating.  The generated values
       are normalized to be between 0 and 1, i.e., 2**(-46) * x_k.

       This generator should produce the same results on any computer
       with at least 48 mantissa bits for `real(64)` data.

     To generate `complex` elements, consecutive pairs of random numbers are
     assigned to the real and imaginary components, respectively.

     The values generated by this NPB RNG do not include 0.0 and 1.0.

     We have tested this implementation with TestU01 (available at
     http://simul.iro.umontreal.ca/testu01/tu01.html ). In our experiments with
     TestU01 1.2.3 and the Crush suite (which consists of 144 statistical
     tests), this NPB RNG failed 41/144 tests. As a linear congruential
     generator, this RNG has known statistical problems that TestU01
     was able to detect.

     .. note::

       This module is currently restricted to generating `real(64)`,
       `imag(64)`, and `complex(128)` complex values.

  */
  module NPBRandom {

    use RandomSupport;
    private use ChapelLocks;

    /*
      Models a stream of pseudorandom numbers.  See the module-level
      notes for :mod:`NPBRandom` for details on the PRNG used.
    */
    class NPBRandomStream {
      /*
        Specifies the type of value generated by the NPBRandomStream.
        Currently, only `real(64)`, `imag(64)`, and `complex(128)` are
        supported.
      */
      type eltType = real(64);

      /*
        The seed value for the PRNG.  It must be an odd integer in the
        interval [1, 2**46).
      */
      const seed: int(64);

      /*
        Indicates whether or not the NPBRandomStream needs to be
        parallel-safe by default.  If multiple tasks interact with it in
        an uncoordinated fashion, this must be set to `true`.  If it will
        only be called from a single task, or if only one task will call
        into it at a time, setting to `false` will reduce overhead related
        to ensuring mutual exclusion.
      */
      param parSafe: bool = true;


      /*
        Creates a new stream of random numbers using the specified seed
        and parallel safety.

        .. note::

          The NPB generator requires an odd seed value. Constructing
          an NPBRandomStream with an even seed value will cause a call to
          halt(). Only the lower 46 bits of the seed will be used.

        :arg eltType: The element type to be generated.
        :type eltType: `type`

        :arg seed: The seed to use for the PRNG.  Defaults to
          `oddCurrentTime` from :type:`~RandomSupport.SeedGenerator`.
        :type seed: `int(64)`

        :arg parSafe: The parallel safety setting.  Defaults to `true`.
        :type parSafe: `bool`

      */
      proc init(type eltType = real(64),
                seed: int(64) = SeedGenerator.oddCurrentTime,
                param parSafe: bool = true) {
        this.eltType = eltType;

        // The mod operation is written in these steps in order
        // to work around an apparent PGI compiler bug.
        // See test/portability/bigmod.test.c
        var one:uint(64) = 1;
        var two_46:uint(64) = one << 46;
        var two_46_mask:uint(64) = two_46 - 1;
        var useed = seed:uint(64);
        var mod:uint(64);
        if useed % 2 == 0 then
          HaltWrappers.initHalt("NPBRandomStream seed must be an odd integer");
        // Adjust seed to be between 0 and 2**46.
        mod = useed & two_46_mask;
        this.seed = mod:int(64);
        this.parSafe = parSafe;
        this.complete();

        if this.seed % 2 == 0 || this.seed < 1 || this.seed > two_46:int(64) then
          HaltWrappers.initHalt("NPBRandomStream seed must be an odd integer between 0 and 2**46");

        NPBRandomStreamPrivate_cursor = seed;
        NPBRandomStreamPrivate_count = 1;
        if eltType == real || eltType == imag || eltType == complex {
          // OK, supported element type
        } else {
          compilerError("NPBRandomStream only supports eltType=real(64), imag(64), or complex(128)");
        }
      }

      pragma "no doc"
      proc NPBRandomStreamPrivate_getNext_noLock() {
        if (eltType == complex) {
          NPBRandomStreamPrivate_count += 2;
        } else {
          NPBRandomStreamPrivate_count += 1;
        }
        return randlc(eltType, NPBRandomStreamPrivate_cursor);
      }

      pragma "no doc"
      proc NPBRandomStreamPrivate_skipToNth_noLock(in n: integral) {
        n += 1;
        if eltType == complex then n = n*2 - 1;
        NPBRandomStreamPrivate_count = n;
        NPBRandomStreamPrivate_cursor = randlc_skipto(seed, n);
      }

      /*
        Returns the next value in the random stream.

        Real numbers generated by the NPB RNG are in (0,1). It is not
        possible for this particular RNG to generate 0.0 or 1.0.

        :returns: The next value in the random stream as type :type:`eltType`.
       */
      proc getNext(): eltType {
        _lock();
        const result = NPBRandomStreamPrivate_getNext_noLock();
        _unlock();
        return result;
      }

      /*
        Advances/rewinds the stream to the `n`-th value in the sequence.
        The first value corresponds to n=0.  n must be >= 0, otherwise an
        IllegalArgumentError is thrown.

        :arg n: The position in the stream to skip to.  Must be >= 0.
        :type n: `integral`

        :throws IllegalArgumentError: When called with negative `n` value.
       */
      proc skipToNth(n: integral) throws {
        if n < 0 then
          throw new owned IllegalArgumentError("NPBRandomStream.skipToNth(n) called with negative 'n' value " + n:string);
        _lock();
        NPBRandomStreamPrivate_skipToNth_noLock(n);
        _unlock();
      }

      /*
        Advance/rewind the stream to the `n`-th value and return it
        (advancing the stream by one).  n must be >= 0, otherwise an
        IllegalArgumentError is thrown.  This is equivalent to
        :proc:`skipToNth()` followed by :proc:`getNext()`.

        :arg n: The position in the stream to skip to.  Must be >= 0.
        :type n: `integral`

        :returns: The `n`-th value in the random stream as type :type:`eltType`.
        :throws IllegalArgumentError: When called with negative `n` value.
       */
      proc getNth(n: integral): eltType throws {
        if (n < 0) then
          throw new owned IllegalArgumentError("NPBRandomStream.getNth(n) called with negative 'n' value " + n:string);
        _lock(); 
        NPBRandomStreamPrivate_skipToNth_noLock(n);
        const result = NPBRandomStreamPrivate_getNext_noLock();
        _unlock();
        return result;
      }

      /*
        Fill the argument array with pseudorandom values.  This method is
        identical to the standalone :proc:`~Random.fillRandom` procedure,
        except that it consumes random values from the
        :class:`NPBRandomStream` object on which it's invoked rather
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
        compilerError("NPBRandomStream(eltType=", eltType:string,
                      ") can only be used to fill arrays of ", eltType:string);
      }

      pragma "no doc"
      proc choice(x: [], size:?sizeType=none, replace=true, prob:?probType=none)
        throws
      {
        compilerError("NPBRandomStream.choice() is not supported.");
      }

      pragma "no doc"
      proc choice(x: range(stridable=?), size:?sizeType=none, replace=true, prob:?probType=none)
        throws
      {
        compilerError("NPBRandomStream.choice() is not supported.");
      }

      pragma "no doc"
      proc choice(x: domain, size:?sizeType=none, replace=true, prob:?probType=none)
        throws
      {
        compilerError("NPBRandomStream.choice() is not supported.");
      }

      /*

         Returns an iterable expression for generating `D.size` random
         numbers. The RNG state will be immediately advanced by `D.size`
         before the iterable expression yields any values.

         The returned iterable expression is useful in parallel contexts,
         including standalone and zippered iteration. The domain will determine
         the parallelization strategy.

         :arg D: a domain
         :arg resultType: the type of number to yield
         :return: an iterable expression yielding random `resultType` values

       */
      pragma "fn returns iterator"
      proc iterate(D: domain, type resultType=real) {
        _lock();
        const start = NPBRandomStreamPrivate_count;
        NPBRandomStreamPrivate_count += D.size.safeCast(int(64));
        NPBRandomStreamPrivate_skipToNth_noLock(NPBRandomStreamPrivate_count-1);
        _unlock();
        return NPBRandomPrivate_iterate(resultType, D, seed, start);
      }

      // Forward the leader iterator as well.
      pragma "no doc"
      pragma "fn returns iterator"
      proc iterate(D: domain, type resultType=real, param tag)
        where tag == iterKind.leader
      {
        // Note that proc iterate() for the serial case (i.e. the one above)
        // is going to be invoked as well, so we should not be taking
        // any actions here other than the forwarding.
        const start = NPBRandomStreamPrivate_count;
        return NPBRandomPrivate_iterate(resultType, D, seed, start, tag);
      }

      pragma "no doc"
      override proc writeThis(f) throws {
        f <~> "NPBRandomStream(eltType=";
        f <~> eltType:string;
        f <~> ", parSafe=";
        f <~> parSafe;
        f <~> ", seed=";
        f <~> seed;
        f <~> ")";
      }

      ///////////////////////////////////////////////////////// CLASS PRIVATE //
      //
      // It is the intent that once Chapel supports the notion of
      // 'private', everything in this class declared below this line will
      // be made private to this class.
      //

      pragma "no doc"
      var _l: if parSafe then chpl_LocalSpinlock else nothing;
      pragma "no doc"
      inline proc _lock() {
        if parSafe then _l.lock();
      }
      pragma "no doc"
      inline proc _unlock() {
        if parSafe then _l.unlock();
      }
      pragma "no doc"
      var NPBRandomStreamPrivate_cursor: real = seed;
      pragma "no doc"
      var NPBRandomStreamPrivate_count: int(64) = 1;
    }


    ////////////////////////////////////////////////////////// MODULE PRIVATE //
    //
    // It is the intent that once Chapel supports the notion of 'private',
    // everything declared below this line will be made private to this
    // module.
    //

    //
    // NPB-defined constants for linear congruential generator
    //
    private const r23   = 0.5**23,
                  t23   = 2.0**23,
                  r46   = 0.5**46,
                  t46   = 2.0**46,
                  arand = 1220703125.0; // TODO: Is arand something that a
                                        // user might want to set on a
                                        // case-by-case basis?

    //
    // NPB-defined randlc routine
    //
    private proc randlc(inout x: real, a: real = arand) {
      var t1 = r23 * a;
      const a1 = floor(t1),
        a2 = a - t23 * a1;
      t1 = r23 * x;
      const x1 = floor(t1),
        x2 = x - t23 * x1;
      t1 = a1 * x2 + a2 * x1;
      const t2 = floor(r23 * t1),
        z  = t1 - t23 * t2,
        t3 = t23 * z + a2 * x2,
        t4 = floor(r46 * t3),
        x3 = t3 - t46 * t4;
      x = x3;
      return r46 * x3;
    }

    // Wrapper that takes a result type (two calls for complex types)
    private proc randlc(type resultType, inout x: real) {
      if resultType == complex then
        return (randlc(x), randlc(x)):complex;
      else
        if resultType == imag then
          //
          // BLC: I thought that casting real to imag did this automatically?
          //
          return _r2i(randlc(x));
        else
          return randlc(x);
     }

    //
    // Return a value for the cursor so that the next call to randlc will
    // return the same value as the nth call to randlc
    //
    private proc randlc_skipto(seed: int(64), in n: integral): real {
      var cursor = seed:real;
      n -= 1;
      var t = arand;
      arand;
      while (n != 0) {
        const i = n / 2;
        if (2 * i != n) then
          randlc(cursor, t);
        if i == 0 then
          break;
        else
          n = i;
        randlc(t, t);
        n = i;
      }
      return cursor;
    }

    //
    // iterate over outer ranges in tuple of ranges
    //
    private iter outer(ranges, param dim: int = 0) {
      if dim + 2 == ranges.size {
        for i in ranges(dim) do
          yield (i,);
      } else if dim + 2 < ranges.size {
        for i in ranges(dim) do
          for j in outer(ranges, dim+1) do
            yield (i, (...j));
      } else {
        yield 0; // 1D case is a noop
      }
    }

    //
    // RandomStream iterator implementation
    //
    pragma "no doc"
    iter NPBRandomPrivate_iterate(type resultType, D: domain, seed: int(64),
                         start: int(64)) {
      var cursor = randlc_skipto(seed, start);
      for i in D do
        yield randlc(resultType, cursor);
    }

    pragma "no doc"
    iter NPBRandomPrivate_iterate(type resultType, D: domain, seed: int(64),
                         start: int(64), param tag: iterKind)
          where tag == iterKind.leader {
      // forward to the domain D's iterator
      for block in D.these(tag=iterKind.leader) do
        yield block;
    }

    pragma "no doc"
    iter NPBRandomPrivate_iterate(type resultType, D: domain, seed: int(64),
                 start: int(64), param tag: iterKind, followThis)
          where tag == iterKind.follower {
      param multiplier = if resultType == complex then 2 else 1;
      const ZD = computeZeroBasedDomain(D);
      const innerRange = followThis(ZD.rank-1);
      var cursor: real;
      for outer in outer(followThis) {
        var myStart = start;
        if ZD.rank > 1 then
          myStart += multiplier * ZD.indexOrder(((...outer), innerRange.low)).safeCast(int(64));
        else
          myStart += multiplier * ZD.indexOrder(innerRange.low).safeCast(int(64));
        if !innerRange.stridable {
          cursor = randlc_skipto(seed, myStart);
          for i in innerRange do
            yield randlc(resultType, cursor);
        } else {
          myStart -= innerRange.low.safeCast(int(64));
          for i in innerRange {
            cursor = randlc_skipto(seed, myStart + i.safeCast(int(64)) * multiplier);
            yield randlc(resultType, cursor);
          }
        }
      }
    }

  } // close module NPBRandom



} // close module Random
