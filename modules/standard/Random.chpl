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

   This module defines an abstraction for a stream of pseudorandom numbers,
   :class:`~Random.RandomStreamInterface`, supporting methods to get the next random
   number in the stream (:proc:`~Random.RandomStreamInterface.getNext`), to
   fast-forward to a specific value in the stream
   (:proc:`~Random.RandomStreamInterface.skipToNth` and
   :proc:`~Random.RandomStreamInterface.getNth`), or to fill an array with random
   numbers in parallel (:proc:`~Random.RandomStream.fillRandom`).  The module
   also provides a standalone convenience function, :proc:`fillRandom` that can
   be used to fill an array with random numbers in parallel without manually
   creating a :class:`RandomStream` object.

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

   3. The :class:`RandomStreamInterface` is included here only
   for documentation and does not help with compilation in any way.
   In the future, we hope to turn it into an interface.

*/
module Random {

  /*
     Seed generation for pseudorandom number generation

  */
  module RandomSupport {

    /*
       An instance of :record:`SeedGenerators` that provides a
       convenient means of generating seeds when the user does not wish to
       specify one manually.
    */
    const SeedGenerator: SeedGenerators;


    /*
      Provides methods to help generate seeds when the user doesn't want
      to create one.  It currently only supports one such method, but the
      intention is to add more over time.

      .. note::
              Once Chapel supports static class methods,
              :const:`SeedGenerator` and :record:`SeedGenerators` should
              be combined into a single record type with static methods).

    */

    record SeedGenerators {
      /*
        Generate a seed based on the current time in microseconds as
        reported by :proc:`Time.getCurrentTime`. Each RNG should
        adjust a given seed to meet any requirements.
      */
      proc currentTime: int(64) {
        use Time;
        const seed = getCurrentTime(unit=TimeUnits.microseconds):int(64);
        return seed;

      }
    }


  } // close module RandomSupport


  /*
     NAS Parallel Benchmark RNG

     The pseudorandom number generator (PRNG) implemented by
     this module uses the algorithm from the NAS Parallel Benchmarks
     (NPB, available at: http://www.nas.nasa.gov/publications/npb.html),
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


     Here is a list of currently open issues (TODOs) for this module:

     1. This module is currently restricted to generating `real(64)`,
     `imag(64)`, and `complex(128)` complex values using a single PRNG
     algorithm.  As noted above, we would like to extend this support to
     include other algorithms and primitive types over time.


  */
  module NPBRandom {

    use RandomSupport;

    /*
      Models a stream of pseudorandom numbers.  See the module-level
      notes for :mod:`NPBRandom` for details on the PRNG used.
    */
    class RandomStream {
      /*
        Specifies the type of value generated by the NPBRandomStream.
        Currently, only `real(64)`, `imag(64)`, and `complex(128)` are
        supported.
      */
      type eltType = real(64);

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

        :arg eltType: The element type to be generated.  Defaults to `real(64)`.
        :type eltType: type
      */
      proc RandomStream(seed: int(64) = SeedGenerator.currentTime,
                        param parSafe: bool = true,
                        type eltType = real(64)) {
        // NPB seed fixup
        const oddseed = if seed % 2 == 0 then seed + 1 else seed;
        this.seed = oddseed % (1:int(64) << 46);

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
        if eltType == complex then n = n*2 - 1;
        NPBRandomStreamPrivate_count = n;
        NPBRandomStreamPrivate_cursor = randlc_skipto(seed, n);
      }

      /*
        Returns the next value in the random stream.

        :returns: The next value in the random stream as type :type:`eltType`.
       */
      proc getNext(): eltType {
        if parSafe then
          NPBRandomStreamPrivate_lock$ = true;
        const result = NPBRandomStreamPrivate_getNext_noLock();
        if parSafe then
          NPBRandomStreamPrivate_lock$;
        return result;
      }

      /*
        Advances/rewinds the stream to the `n`-th value in the sequence.

        :arg n: The position in the stream to skip to.  Must be non-negative.
        :type n: integral
       */

      proc skipToNth(n: integral) {
        if n <= 0 then
          halt("NPBRandomStream.skipToNth(n) called with non-positive 'n' value", n);
        if parSafe then
          NPBRandomStreamPrivate_lock$ = true;
        NPBRandomStreamPrivate_skipToNth_noLock(n);
        if parSafe then
          NPBRandomStreamPrivate_lock$;
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
          halt("NPBRandomStream.getNth(n) called with non-positive 'n' value", n);
        if parSafe then
          NPBRandomStreamPrivate_lock$ = true;
        NPBRandomStreamPrivate_skipToNth_noLock(n);
        const result = NPBRandomStreamPrivate_getNext_noLock();
        if parSafe then
          NPBRandomStreamPrivate_lock$;
        return result;
      }

      /*
        Fill the argument array with pseudorandom values.  This method is
        identical to the standalone :proc:`fillRandom` procedure,
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
        compilerError("NPBRandomStream(eltType=", typeToString(eltType), ") can only be used to fill arrays of ", typeToString(eltType));
      }
        
      pragma "no doc"
      proc iterate(D: domain, type resultType=real) {
        if parSafe then
          NPBRandomStreamPrivate_lock$ = true;
        const start = NPBRandomStreamPrivate_count;
        NPBRandomStreamPrivate_count += D.numIndices.safeCast(int(64));
        NPBRandomStreamPrivate_skipToNth_noLock(NPBRandomStreamPrivate_count);
        if parSafe then
          NPBRandomStreamPrivate_lock$;
        return NPBRandomPrivate_iterate(resultType, D, seed, start);
      }

      pragma "no doc"
      proc writeThis(f: Writer) {
        f <~> "NPBRandomStream(eltType=";
        f <~> typeToString(eltType);
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
      var NPBRandomStreamPrivate_lock$: sync bool;
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
    private iter outer(ranges, param dim: int = 1) {
      if dim + 1 == ranges.size {
        for i in ranges(dim) do
          yield (i,);
      } else if dim + 1 < ranges.size {
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
      for block in D._value.these(tag=iterKind.leader) do
        yield block;
    }

    pragma "no doc"
    iter NPBRandomPrivate_iterate(type resultType, D: domain, seed: int(64),
                 start: int(64), param tag: iterKind, followThis)
          where tag == iterKind.follower {
      param multiplier = if resultType == complex then 2 else 1;
      const ZD = computeZeroBasedDomain(D);
      const innerRange = followThis(ZD.rank);
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



  /*
     Permuted Linear Congurential Random Number Generator

     This module provides PCG random number generation routines.
     See http://www.pcg-random.org/
     and the paper, "PCG: A Family of Simple Fast Space-Efficient Statistically
     Good Algorithms for Random Number Generation" by M.E. O'Neill.
  */
  module PCGRandom {

    use RandomSupport;

    proc isSupportedNumericType(type t) param
      return isNumericType(t) || isBoolType(t);

    // How many generators do we need for this type?
    private
    proc numGenerators(type t) param {
      if isBoolType(t) then return 1;
      else return (numBits(t)+31) / 32;
    }

    private
    proc defaultMax(type t) param
    {
      if isRealType(t) then return 1.0:t;
      if isComplexType(t) then return 1.0 + 1.0i;
      else return max(t);
    }


    /*
      Models a stream of pseudorandom numbers.  See the module-level
      notes for :mod:`PCGRandom` for details on the PRNG used.
    */
    class PCGRandomStream {
      /*
        Specifies the type of value generated by the PCGRandomStream.
        Currently, only numeric types are supported.
      */
      type eltType; // = real(64);

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
        The seed value for the PRNG.
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

        :arg eltType: The element type to be generated.  Defaults to `real(64)`.
        :type eltType: type
      */
      proc PCGRandomStream(seed: int(64) = SeedGenerator.currentTime,
                        param parSafe: bool = true,
                        type eltType = real(64) ) {
        this.seed = seed;
        for param i in 1..numGenerators(eltType) {
          param inc = pcg_getvalid_inc(i);
          PCGRandomStreamPrivate_rngs[i].srandom(seed:uint(64), inc);
        }
        PCGRandomStreamPrivate_count = 1;
      }

      pragma "no doc"
      proc PCGRandomStreamPrivate_getNext_noLock() {
        PCGRandomStreamPrivate_count += 1;
        return randlc(eltType, PCGRandomStreamPrivate_rngs);
      }
      pragma "no doc"
      proc PCGRandomStreamPrivate_getNext_noLock(min:eltType, max:eltType) {
        PCGRandomStreamPrivate_count += 1;
        return randlc(eltType, PCGRandomStreamPrivate_rngs, min, max);
      }


      pragma "no doc"
      proc PCGRandomStreamPrivate_skipToNth_noLock(in n: integral) {
        PCGRandomStreamPrivate_count = n;
        PCGRandomStreamPrivate_rngs = randlc_skipto(eltType, seed, n);
      }

      /*
        Returns the next value in the random stream.

        Reals are between 0 and 1; integers are in the range of the integer.
        :returns: The next value in the random stream as type :type:`eltType`.
       */
      proc getNext(): eltType {
        if parSafe then
          PCGRandomStreamPrivate_lock$ = true;
        const result = PCGRandomStreamPrivate_getNext_noLock();
        if parSafe then
          PCGRandomStreamPrivate_lock$;
        return result;
      }
      /*
        Return the next random value but within a particular range.
        Returns a number between min and max (inclusive).
       */
      proc getNext(min: eltType, max:eltType): eltType {
        if parSafe then
          PCGRandomStreamPrivate_lock$ = true;
        const result = PCGRandomStreamPrivate_getNext_noLock(min,max);
        if parSafe then
          PCGRandomStreamPrivate_lock$;
        return result;
      }


      /*
        Advances/rewinds the stream to the `n`-th value in the sequence.

        :arg n: The position in the stream to skip to.  Must be non-negative.
        :type n: integral
       */

      proc skipToNth(n: integral) {
        if n <= 0 then
          halt("PCGRandomStream.skipToNth(n) called with non-positive 'n' value", n);
        if parSafe then
          PCGRandomStreamPrivate_lock$ = true;
        PCGRandomStreamPrivate_skipToNth_noLock(n);
        if parSafe then
          PCGRandomStreamPrivate_lock$;
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
          halt("PCGRandomStream.getNth(n) called with non-positive 'n' value", n);
        if parSafe then
          PCGRandomStreamPrivate_lock$ = true;
        PCGRandomStreamPrivate_skipToNth_noLock(n);
        const result = PCGRandomStreamPrivate_getNext_noLock();
        if parSafe then
          PCGRandomStreamPrivate_lock$;
        return result;
      }

      /*
        Fill the argument array with pseudorandom values.  This method is
        identical to the standalone :proc:`fillRandom` procedure,
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

      /* Randomly shuffle a 1-D non-strided array.
         */
      proc shuffle(arr: [] eltType) {
        var low = arr.domain.dim(1).low;
        var high = arr.domain.dim(1).high;

        if arr.domain.rank != 1 then
          compilerError("Shuffle requires 1-D array");
        //if arr.domain.type.strideable then
        //  compilerError("Shuffle requires non-strideable 1-D array");

        for j in low+1..high by -1 {
          var k = randlc(arr.domain.idxType, PCGRandomStreamPrivate_rngs,
                         low, j);
          arr[k] <=> arr[j];
        }
      }
      /* Produce a random permutation.
         */
      proc permutation(arr: [] eltType) {
        var low = arr.domain.dim(1).low;
        var high = arr.domain.dim(1).high;

        if arr.domain.rank != 1 then
          compilerError("Permutation requires 1-D array");
        //if arr.domain.dim(1).strideable then
        //  compilerError("Permutation requires non-strideable 1-D array");

        for i in low..high {
          var j = randlc(arr.domain.idxType, PCGRandomStreamPrivate_rngs,
                         low, i);
          arr[i] = arr[j];
          arr[j] = i;
        }
      }


      pragma "no doc"
      proc fillRandom(arr: []) {
        compilerError("PCGRandomStream(eltType=", typeToString(eltType), ") can only be used to fill arrays of ", typeToString(eltType));
      }
        
      pragma "no doc"
      proc iterate(D: domain, type resultType=real) {
        if parSafe then
          PCGRandomStreamPrivate_lock$ = true;
        const start = PCGRandomStreamPrivate_count;
        PCGRandomStreamPrivate_count += D.numIndices.safeCast(int(64));
        PCGRandomStreamPrivate_skipToNth_noLock(PCGRandomStreamPrivate_count);
        if parSafe then
          PCGRandomStreamPrivate_lock$;
        return PCGRandomPrivate_iterate(resultType, D, seed, start);
      }

      pragma "no doc"
      proc writeThis(f) {
        f <~> "PCGRandomStream(eltType=";
        f <~> typeToString(eltType);
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
      var PCGRandomStreamPrivate_lock$: sync bool;
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

    // Translated from PCG-C-basic-0.9
    // Keeping the same function names in order to simplify maintenance
    // These functions correspond to pcg32_random_r in that version.

    // each RNG has a state and an inc value.
    // the inc value does not change as the RNG iterates, but it must always
    // be odd.
    // the state changes and can be any value.


    param PCG_DEFAULT_MULTIPLIER_8 = 141;
    param PCG_DEFAULT_MULTIPLIER_16 = 12829;
    param PCG_DEFAULT_MULTIPLIER_32 = 747796405;
    param PCG_DEFAULT_MULTIPLIER_64 = 6364136223846793005;

    inline
    proc pcg_setseq_8_step_r(ref state:uint(8), inc:uint(8))
    {
      state = state * PCG_DEFAULT_MULTIPLIER_8 + inc;
    }

    inline
    proc pcg_setseq_16_step_r(ref state:uint(16), inc:uint(16))
    {
      state = state * PCG_DEFAULT_MULTIPLIER_16 + inc;
    }


    inline
    proc pcg_setseq_32_step_r(ref state:uint(32), inc:uint(32))
    {
      state = state * PCG_DEFAULT_MULTIPLIER_32 + inc;
    }


    inline
    proc pcg_setseq_64_step_r(ref state:uint(64), inc:uint(64))
    {
      state = state * PCG_DEFAULT_MULTIPLIER_64 + inc;
    }

    inline
    proc pcg_rotr_32(value:uint(32), rot:uint(32)):uint(32)
    {
      // having trouble using BitOps...
      extern proc chpl_bitops_rotr_32(x: uint(32), n: uint(32)) : uint(32);

      var ret = chpl_bitops_rotr_32(value, rot);
      //writef("\n rotr %i of %xu is %xu\n", rot, value, ret);
      return ret;
    }

    inline
    proc pcg_output_xsh_rr_64_32(state:uint(64)):uint(32)
    {
      const xorshifted:uint(32) = (((state >> 18) ^ state) >> 27):uint(32);
      const rot:uint(32) = (state >> 59):uint(32);
      const ret = pcg_rotr_32(xorshifted, rot);
      // writef("\n returning r %xu\n", ret);
      return ret;
    }


    inline
    proc pcg_output_rxs_m_xs_8_8(state:uint(8)):uint(8)
    {
      const word:uint(8) = ((state >> ((state >> 6) + 2)) ^ state) * 217;
      return (word >> 6) ^ word;
    }

    inline
    proc pcg_output_rxs_m_xs_16_16(state:uint(16)):uint(16)
    {
      const word:uint(16) = ((state >> ((state >> 13) + 3)) ^ state) * 62169;
      return (word >> 11) ^ word;
    }

    inline
    proc pcg_output_rxs_m_xs_32_32(state:uint(32)):uint(32)
    {
      const word:uint(32) = ((state >> ((state >> 28) + 4)) ^ state) * 277803737;
      return (word >> 22) ^ word;
    }

    inline
    proc pcg_output_rxs_m_xs_64_64(state:uint(64)):uint(64)
    {
      const word:uint(64) = ((state >> ((state >> 59) + 5)) ^ state)
                              * 12605985483714917081;
      return (word >> 43) ^ word;
    }


    record pcg_setseq_64_xsh_rr_32_rng {
      var state:uint(64);

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

      inline proc random(inc:uint(64)):uint(32)
      {
        // this is pcg_setseq_64_xsh_rr_32_random_r
        const oldstate:uint(64) = state;
        //writef("\n state=%xu inc=%xu\n", state, inc);
        pcg_setseq_64_step_r(state, inc);
        return pcg_output_xsh_rr_64_32(oldstate);
      }

      // unlike the PCG version, this version advances the RNG only
      // one iteration - if more iterations are necessary, it computes
      // a different stream each time a new bounded random number is needed.
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

        // First, try getting a random number that is within the bounds.
        r = random(inc);
        if r >= threshold then
          return r % bound;

        // not the same as in PCG:
        // If we didn't get the answer we wanted, seed a new
        // RNG starting with state and ask for a different stream.
        // we ask for stream 100, but that value is arbitrary. It's
        // nice if it's different from the values used elsewhere (so
        // that things stay independently random)
        tmpinc = pcg_getvalid_inc(100);
        tmprng.srandom(state, tmpinc);

         // This comment is from pcg32_boundedrand_r:
         // Uniformity guarantees that this loop will terminate.  In practice, it
         // should usually terminate quickly; on average (assuming all bounds are
         // equally likely), 82.25% of the time, we can expect it to require just
         // one iteration.  In the worst case, someone passes a bound of 2^31 + 1
         // (i.e., 2147483649), which invalidates almost 50% of the range.  In 
         // practice, bounds are typically small and only a tiny amount of the range
         // is eliminated.

        while true {
          // Imagine that we call srand/rand to get each number, while
          // varying 'inc'.
          r = tmprng.random(tmpinc);
          if r >= threshold then
            return r % bound;
        }

        // never reached.
        return 0;
      }


      inline
      proc advance(inc:uint(64), delta:uint(64))
      {
        // this is pcg_setseq_64_advance_r
        state = pcg_advance_lcg(64, state, delta, PCG_DEFAULT_MULTIPLIER_64, inc);
      }
    }

    record pcg_setseq_64_rxs_m_xs_64_rng {
      var state:uint(64);

      inline proc srandom(seed:uint(64), inc:uint(64))
      {
        // this is pcg_setseq_64_srandom_r

        state = inc + seed;
        pcg_setseq_64_step_r(state, inc);
      }

      inline proc random(inc:uint(64)):uint(64)
      {
        // this is pcg_setseq_64_rxs_m_xs_64_random_r
        const oldstate:uint(64) = state;
        pcg_setseq_64_step_r(state, inc);
        return pcg_output_rxs_m_xs_64_64(oldstate);
      }

      inline
      proc advance(inc:uint(64), delta:uint(64))
      {
        // this is pcg_setseq_64_advance_r
        state = pcg_advance_lcg(64, state, delta, PCG_DEFAULT_MULTIPLIER_64, inc);
      }
    }

    record pcg_setseq_32_rxs_m_xs_32_rng {
      var state:uint(32);

      inline proc srandom(seed:uint(32), inc:uint(32))
      {
        // this is pcg_setseq_32_srandom_r

        state = inc + seed;
        pcg_setseq_32_step_r(state, inc);
      }

      inline proc random(inc:uint(32)):uint(32)
      {
        // this is pcg_setseq_32_rxs_m_xs_32_random_r
        const oldstate:uint(32) = state;
        pcg_setseq_32_step_r(state, inc);
        return pcg_output_rxs_m_xs_32_32(oldstate);
      }

      inline
      proc advance(inc:uint(32), delta:uint(32))
      {
        // this is pcg_setseq_32_advance_r
        state = pcg_advance_lcg(32, state, delta, PCG_DEFAULT_MULTIPLIER_32, inc);
      }
    }

    record pcg_setseq_16_rxs_m_xs_16_rng {
      var state:uint(16);

      inline proc srandom(seed:uint(16), inc:uint(16))
      {
        // this is pcg_setseq_16_srandom_r

        state = inc + seed;
        pcg_setseq_16_step_r(state, inc);
      }

      inline proc random(inc:uint(16)):uint(16)
      {
        // this is pcg_setseq_16_rxs_m_xs_16_random_r
        const oldstate:uint(16) = state;
        pcg_setseq_16_step_r(state, inc);
        return pcg_output_rxs_m_xs_16_16(oldstate);
      }

      inline
      proc advance(inc:uint(16), delta:uint(16))
      {
        // this is pcg_setseq_16_advance_r
        state = pcg_advance_lcg(16, state, delta, PCG_DEFAULT_MULTIPLIER_16, inc);
      }
    }

    record pcg_setseq_8_rxs_m_xs_8_rng {
      var state:uint(8);

      inline proc srandom(seed:uint(8), inc:uint(8))
      {
        // this is pcg_setseq_8_srandom_r

        state = inc + seed;
        pcg_setseq_8_step_r(state, inc);
      }

      inline proc random(inc:uint(8)):uint(8)
      {
        // this is pcg_setseq_8_rxs_m_xs_8_random_r
        const oldstate:uint(8) = state;
        pcg_setseq_8_step_r(state, inc);
        return pcg_output_rxs_m_xs_8_8(oldstate);
      }

      inline
      proc advance(inc:uint(8), delta:uint(8))
      {
        // this is pcg_setseq_8_advance_r
        state = pcg_advance_lcg(8, state, delta, PCG_DEFAULT_MULTIPLIER_8, inc);
      }
    }


    proc uint_with_bits(param nbits) type {
      if nbits <= 8 then return uint(8);
      else if nbits <= 16 then return uint(16);
      else if nbits <= 32 then return uint(32);
      else return uint(64);
    }

    // zero all but the bottom nbits of x
    inline proc normalize(nbits:int, x:uint) {
      var oldx = x;
      var shiftamt = 64 - nbits;
      oldx <<= shiftamt;
      return oldx >> shiftamt;
    }


    proc pcg_output_rxs_m_xs_N_mine(nbits:int, state_in)
    {
      // no sense it trying to get randomness for really small examples.
      if nbits < 8 then return state_in;

      if nbits == 8 then return pcg_output_rxs_m_xs_8_8(state_in:uint(8));
      if nbits == 16 then return pcg_output_rxs_m_xs_16_16(state_in:uint(16));
      if nbits == 32 then return pcg_output_rxs_m_xs_32_32(state_in:uint(32));
      if nbits == 64 then return pcg_output_rxs_m_xs_64_64(state_in:uint(64));

      {
        var shiftamt = nbits - 4;
        var state = state_in:uint;
        var word:uint = ((state >> ((state >> shiftamt) + 2)) ^ state) * 217;
        word = normalize(nbits, word); 
        return (word >> shiftamt) ^ word;
      }
    }


    record pcg_setseq_N_rxs_m_xs_N_rng {
      const nbits;
      var state:uint;

      // zero all but the bottom nbits of state
      inline proc mask_state() {
        state = normalize(nbits, state);
      }

      inline proc srandom(seed:uint, inc:uint)
      {
        state = inc + seed;
        if nbits <= 8 {
          var tmpstate = state:uint(8);
          pcg_setseq_8_step_r(tmpstate, inc:uint(8));
          state = tmpstate;
        } else if nbits <= 16 {
          var tmpstate = state:uint(16);
          pcg_setseq_16_step_r(tmpstate, inc:uint(16));
          state = tmpstate;
        } else if nbits <= 32 {
          var tmpstate = state:uint(32);
          pcg_setseq_32_step_r(tmpstate, inc:uint(32));
          state = tmpstate;
        } else if nbits <= 64 {
          pcg_setseq_64_step_r(state, inc);
        }
        mask_state();
      }

      inline proc random(inc:uint):uint
      {
        if nbits <= 8 {
          var tmpstate = state:uint(8);
          var oldstate = tmpstate;
          pcg_setseq_8_step_r(tmpstate, inc:uint(8));
          state = tmpstate;
          mask_state();
          return normalize(nbits, pcg_output_rxs_m_xs_N_mine(nbits, oldstate));
        } else if nbits <= 16 {
          var tmpstate = state:uint(16);
          var oldstate = tmpstate;
          pcg_setseq_16_step_r(tmpstate, inc:uint(16));
          state = tmpstate;
          mask_state();
          return normalize(nbits, pcg_output_rxs_m_xs_N_mine(nbits, oldstate));
        } else if nbits <= 32 {
          var tmpstate = state:uint(32);
          var oldstate = tmpstate;
          pcg_setseq_32_step_r(tmpstate, inc:uint(32));
          state = tmpstate;
          mask_state();
          return normalize(nbits, pcg_output_rxs_m_xs_N_mine(nbits, oldstate));
        } else if nbits <= 64 {
          var oldstate = state;
          pcg_setseq_64_step_r(state, inc);
          mask_state();
          return normalize(nbits, pcg_output_rxs_m_xs_N_mine(nbits, oldstate));
        }
        return 0;
      }

      inline
      proc advance(inc:uint, delta:uint)
      {
        if nbits <= 8 {
          state = pcg_advance_lcg(8, state:uint(8), delta:uint(8),
                                  PCG_DEFAULT_MULTIPLIER_8, inc:uint(8));
          mask_state();
        } else if nbits <= 16 {
          state = pcg_advance_lcg(16, state:uint(16), delta:uint(16),
                                  PCG_DEFAULT_MULTIPLIER_16, inc:uint(16));
          mask_state();
        } else if nbits <= 32 {
          state = pcg_advance_lcg(32, state:uint(32), delta:uint(32),
                                  PCG_DEFAULT_MULTIPLIER_32, inc:uint(32));
          mask_state();
        } else if nbits <= 64 {
          state = pcg_advance_lcg(64, state, delta, PCG_DEFAULT_MULTIPLIER_64, inc);
          mask_state();
        }
      }
    }

    // the 'inc' field in the PCG RNG must be odd
    // these functions arrange for that to be the case given any input.
    inline
    proc pcg_getvalid_inc(initseq:uint(64)):uint(64) return (initseq<<1) | 1;
    inline
    proc pcg_getvalid_inc(param initseq:uint(64)) param return (initseq<<1) | 1;


    // pcg_advance_lcg_8/16/32/64
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

    // returns a random number in [0, 1)
    // where the number is a multiple of 2**-64
    private inline
    proc randToReal64(x: uint(64)):real(64)
    {
      return ldexp(x, -64);
    }
    // returns a random number in [min, max)
    // where the number is a multiple of 2**-64
    private inline
    proc randToReal64(x: uint(64), min:real(64), max:real(64)):real(64)
    {
      var normalized = ldexp(x, -64);
      return (max-min)*normalized + min;
    }

    // returns a random number in [0, 1)
    // where the number is a multiple of 2**-64
    private inline
    proc randToReal32(x: uint(32))
    {
      return ldexp(x, -32);
    }

    // returns a random number in [min, max)
    // where the number is a multiple of 2**-64
    private inline
    proc randToReal32(x: uint(32), min:real(32), max:real(32)):real(32)
    {
      var normalized = ldexp(x, -32);
      return (max-min)*normalized + min;
    }


    // These would form the RNG interface.
    private inline
    proc rand32_1(ref states):uint(32) {
      return states[1].random(pcg_getvalid_inc(1));
    }
    private inline
    proc rand32_2(ref states):uint(32) {
      return states[2].random(pcg_getvalid_inc(2));
    }
    // returns x with 0 <= x <= bound
    private inline
    proc boundedrand32_1(ref states, bound:uint(32)):uint(32) {
      // just get 32 random bits if bound+1 is not representable.
      if bound == max(uint(32)) then return rand32_1(states);
      else return states[1].bounded_random(pcg_getvalid_inc(1), bound + 1);
    }
    // returns x with 0 <= x <= bound
    private inline
    proc boundedrand32_2(ref states, bound:uint(32)):uint(32) {
      // just get 32 random bits if bound+1 is not representable.
      if bound == max(uint(32)) then return rand32_2(states);
      else return states[2].bounded_random(pcg_getvalid_inc(2), bound + 1);
    }

    private inline
    proc rand64_1(ref states):uint(64) {
      var ret:uint(64) = 0;
      ret |= states[1].random(pcg_getvalid_inc(1));
      ret <<= 32;
      ret |= states[2].random(pcg_getvalid_inc(2));
      return ret;
    }
    private inline
    proc rand64_2(ref states):uint(64) {
      var ret:uint(64) = 0;
      ret |= states[3].random(pcg_getvalid_inc(3));
      ret <<= 32;
      ret |= states[4].random(pcg_getvalid_inc(4));
      return ret;
    }

    // Returns an unsigned integer x with 0 <= x <= bound
    private proc boundedrand64_1(ref states, bound:uint):uint
    {
      if bound > max(uint(32)):uint {
        var toprand = 0:uint;
        var botrand = 0:uint;
        
        // compute the bounded number in two calls to a 32-bit RNG
        toprand = boundedrand32_1(states, (bound >> 32):uint(32));
        botrand = boundedrand32_2(states, (bound & max(uint(32))):uint(32));
        return (toprand << 32) | botrand;
      } else {
        return boundedrand32_1(states, bound:uint(32));
      }
    }


    // Wrapper that takes a result type
    private inline
    proc randlc(type resultType, ref states) {
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
    private inline
    proc randlc(type resultType, ref states, min, max) {
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
        return (boundedrand64_1(states, (max-min):uint(64)) + min:uint(64)):resultType;
      } else if resultType == uint(32) || resultType == int(32) {
        return (boundedrand32_1(states, (max-min):uint(32)) + min:uint(32)):resultType;
      } else if(resultType == uint(16) ||
                resultType == int(16)) {
        return (boundedrand32_1(states, (max-min):uint(32)) + min:uint(32)):resultType;
      } else if(resultType == uint(8) ||
                resultType == int(8)) {
        return (boundedrand32_1(states, (max-min):uint(32)) + min:uint(32)):resultType;
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

      for param i in 1..states.size {
        param inc = pcg_getvalid_inc(i);
        states[i].srandom(seed:uint(64), inc);
        states[i].advance(inc, (n - 1):uint(64));
      }
      return states;
    }

    //
    // iterate over outer ranges in tuple of ranges
    //
    private iter outer(ranges, param dim: int = 1) {
      if dim + 1 == ranges.size {
        for i in ranges(dim) do
          yield (i,);
      } else if dim + 1 < ranges.size {
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
      for block in D._value.these(tag=iterKind.leader) do
        yield block;
    }

    pragma "no doc"
    iter PCGRandomPrivate_iterate(type resultType, D: domain, seed: int(64),
                 start: int(64), param tag: iterKind, followThis)
          where tag == iterKind.follower {
      param multiplier = if resultType == complex then 2 else 1;
      const ZD = computeZeroBasedDomain(D);
      const innerRange = followThis(ZD.rank);
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


  use RandomSupport;
  use NPBRandom;
  use PCGRandom;


  /* Select between different supported RNG algorithms.
   */
  enum RNG {
    PCG = 1,
    NPB = 2
  }

  param defaultRNG = RNG.NPB;

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
  proc fillRandom(arr: [], seed: int(64) = SeedGenerator.currentTime, param
      algorithm=defaultRNG)
    where isSupportedNumericType(arr.eltType) {
    var randNums = makeRandomStream(seed, eltType=arr.eltType, parSafe=false, algorithm=algorithm);
    randNums.fillRandom(arr);
    delete randNums;
  }

  pragma "no doc"
  proc fillRandom(arr: [], seed: int(64) = SeedGenerator.currentTime, param
      algorithm=defaultRNG) {
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

    ///////////////////////////////////////////////////////// CLASS PRIVATE //
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
                        param algorithm = defaultRNG) {
    if algorithm == RNG.PCG then
      return new PCGRandomStream(seed=seed, parSafe=parSafe, eltType=eltType);
    else if algorithm == RNG.NPB then
      return new /*NPB*/RandomStream(seed=seed, parSafe=parSafe, eltType=eltType);
    else
      compilerError("Unknown random number generator");
  }

  // An apparent bug prevents this from working.
  //type RandomStream = PCGRandomStream;

} // close module Random
