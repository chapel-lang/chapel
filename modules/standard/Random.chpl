/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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
    Support for pseudorandom number generation.

    This module defines a :record:`randomStream` type (based upon the PCG
    algorithm) that can be used to generate pseudorandom numbers in parallel.
    The ``randomStream`` type can be used to generate individual random numbers
    via :proc:`randomStream.getNext` or to fill an array with random numbers via
    :proc:`randomStream.fill`. There are also several other methods available
    for manipulating arrays, parallel iteration, or adjusting the stream's
    position.

    This module contains a few top-level procedures that can be used for
    manipulating arrays:

    * :proc:`fillRandom` fills an array with random numbers in parallel
    * :proc:`shuffle` randomly re-arranges the elements of an array
    * :proc:`permute` creates a random permutation of an array's domain

    Seed Generation
    ---------------

    The :record:`randomStream` type can be initialized with a seed value. Any
    two ``randomStream``'s initialized with the same seed value will produce
    identical sequences of random numbers.

    When not provided explicitly, a seed value will be generated in an
    implementation specific manner which is designed to minimize the chance
    that two distinct ``randomStream``'s will have the same seed.

    Prior to Chapel 1.33, seed values could be generated with the now
    deprecated ``RandomSupport.SeedGenerator`` type. For a non-deprecated
    replacement, see :proc:`~NPBRandom.oddTimeSeed`. Note that the
    ``NPBRandom`` module is not stable.


    Note About Deprecations and Future Work
    ---------------------------------------

    Before Chapel 1.33, this module was unstable. It defined an abstract
    random stream interface (:class:`RandomStreamInterface`) and two
    implementations of that interface (based on the PCG and NPB algorithms).

    As of Chapel 1.33, this module is partially stable and defines a single
    :record:`randomStream` type that is based on the PCG algorithm. The
    NPB algorithm is still available via the :mod:`NPBRandom` package module.
    The ``RandomStreamInterface`` is now deprecated.

    Various symbols used to switch between the two algorithms have also been
    deprecated. These include:

    * :proc:`createRandomStream`
    * :type:`RNG`
    * :param:`defaultRNG`
    * :type:`RandomStream`
    * overloads of the top-level methods that accept an ``algorithm`` argument

    In a future release, we intend to use Chapel's interface features to
    define one or more interfaces for random streams. At that point, the
    :record:`randomStream` type will be an implementation of the interface(s)
    for generating a seedable stream of random numbers.

*/
module Random {

  public use RandomSupport;
  public use PCGRandom;
  private use IO;
  private use Math;

  public import NPBRandom;

  @chpldoc.nodoc
  record _RNG {
    proc type PCG param do return 1;
    proc type NPB param do return 2;
  }

  /* Select between different supported RNG algorithms.
     See :mod:`PCGRandom` and :mod:`NPBRandom` for details on
     these algorithms.
   */
  @deprecated("'RNG' is deprecated; please use the PCG algorithm via the :record:`randomStream` type or the NPB algorithm via the :mod:`NPBRandom` package module")
  type RNG = _RNG;

  @chpldoc.nodoc
  param _defaultRNG = _RNG.PCG;

  /* The default RNG. The current default is PCG - see :mod:`PCGRandom`. */
  @deprecated("'defaultRNG' is deprecated; please use the PCG algorithm via the :record:`randomStream` type or the NPB algorithm via the :mod:`NPBRandom` package module")
  param defaultRNG = _defaultRNG;

  @deprecated("the RandomStream class is deprecated; please use the :record:`randomStream` record instead")
  type RandomStream = if _defaultRNG == _RNG.PCG then PCGRandomStreamInternal(?)
                                                 else NPBRandom.NPBRandomStream(?);


  private proc isNumericOrBoolType(type t) param do
    return isNumericType(t) || isBoolType(t);

  private proc is1DRectangularDomain(d) param do
    return d.isRectangular() && d.rank == 1;


  private proc randomishSeed(): int {
    import Time, IO, CTypes;
    extern proc chpl_task_getId(): chpl_taskID_t;
    extern proc getpid(): CTypes.c_int;

    const sWho = chpl_task_getId().hash():int,
          sWhat = (getpid():int).hash():int,
          sWhen = Time.timeSinceEpoch().totalSeconds().hash():int,
          sWhere = here.hash():int;

    var randomBits: int = 0;
    // TODO: separate the "`/dev/urandom` doesn't exist" error handling from the
    //        readBits error handling
    try {
      IO.openReader("/dev/urandom", region=0..<8, locking=false).readBits(randomBits, 64);
    } catch {
      // may not be able to open /dev/urandom, ignore this step
    }

    return sWho ^ sWhat ^ sWhen ^ sWhere ^ randomBits;
  }

  pragma "last resort"
  @deprecated("The overload of `fillRandom` that accepts an 'algorithm' argument is deprecated; please remove the 'algorithm' argument")
  proc fillRandom(ref arr: [], seed: int(64)=_SeedGenerator.oddCurrentTime, param algorithm=_defaultRNG)
    where isNumericOrBoolType(arr.eltType)
  {
    var randNums = _createRandomStream(seed=seed,
                                       eltType=arr.eltType,
                                       parSafe=false,
                                       algorithm=algorithm);
    randNums.fillRandom(arr);
  }

  /*
    Fill a rectangular array of numeric values with pseudorandom values in
    parallel using a new :record:`randomStream`. The first `arr.size` values
    from the stream will be assigned to the array's elements in row-major
    order. The parallelization strategy is determined by the array.

    :arg arr: An array of numeric values
    :arg seed: The seed to use to create the ``randomStream``

  */
  proc fillRandom(ref arr: [] ?t, seed: int)
    where isNumericOrBoolType(t) && arr.isRectangular()
  {
    var rs = new randomStream(t, seed);
    rs.fill(arr);
  }

  /*
    Fill a rectangular array of numeric values with pseudorandom values in
    parallel using a new :record:`randomStream`. The first `arr.size` values
    from the stream will be assigned to the array's elements in row-major
    order. The parallelization strategy is determined by the array.

    .. note:: a seed will be generated in an implementation specific manner
              that depends on the current time.

    :arg arr: An array of numeric values
  */
  proc fillRandom(ref arr: [] ?t)
    where isNumericOrBoolType(t) && arr.isRectangular()
  {
    var rs = new randomStream(t);
    rs.fill(arr);
  }

  @chpldoc.nodoc
  proc fillRandom(ref arr: [], seed: int) {
    compilerError("'fillRandom' does not support non-rectangular arrays");
  }

  @chpldoc.nodoc
  proc fillRandom(ref arr: []) {
    compilerError("'fillRandom' does not support non-rectangular arrays");
  }

  pragma "last resort"
  @chpldoc.nodoc
  @deprecated("The overload of `fillRandom` that accepts an 'algorithm' argument is deprecated; please remove the 'algorithm' argument")
  proc fillRandom(ref arr: [], seed: int(64), param algorithm=_defaultRNG) {
    compilerError("Random.fillRandom is only defined for rectangular numeric arrays");
  }

  /*
    Fill a rectangular array of numeric values with pseudorandom values in
    the range [``min``, ``max``] (inclusive) in parallel using a new
    :record:`randomStream`. The first `arr.size` values from the stream
    will be assigned to the array's elements in row-major order. The
    parallelization strategy is determined by the array.

    :arg arr: An array of numeric values
    :arg min: The (inclusive) lower bound for the random values
    :arg max: The (inclusive) upper bound for the random values
    :arg seed: The seed to use to create the ``randomStream``
  */
  proc fillRandom(ref arr: [] ?t, min: t, max: t, seed: int)
    where isNumericOrBoolType(t) && arr.isRectangular()
  {
    var rs = new randomStream(t, seed);
    rs.fill(arr, min, max);
  }

  /*
    Fill a rectangular array of numeric values with pseudorandom values in
    the range [``min``, ``max``] (inclusive) in parallel using a new
    :record:`randomStream`. The first `arr.size` values from the stream
    will be assigned to the array's elements in row-major order. The
    parallelization strategy is determined by the array.

    .. note:: a seed will be generated in an implementation specific manner
              that depends on the current time.

    :arg arr: An array of numeric values
    :arg min: The (inclusive) lower bound for the random values
    :arg max: The (inclusive) upper bound for the random values
  */
  proc fillRandom(ref arr: [] ?t, min: t, max: t)
    where isNumericOrBoolType(t) && arr.isRectangular()
  {
    var rs = new randomStream(t);
    rs.fill(arr, min, max);
  }

  @chpldoc.nodoc
  proc fillRandom(ref arr: [], min, max, seed: int) {
    compilerError("'fillRandom' does not support non-rectangular arrays");
  }

  @chpldoc.nodoc
  proc fillRandom(ref arr: [], min, max) {
    compilerError("'fillRandom' does not support non-rectangular arrays");
  }

  /* Shuffle the elements of a rectangular array into a random order.

     :arg arr: a rectangular 1-D non-strided array
     :arg seed: the seed to use when shuffling. Defaults to
      `oddCurrentTime` from :type:`RandomSupport.SeedGenerator`.
     :arg algorithm: A param indicating which algorithm to use. Defaults to PCG.
     :type algorithm: :type:`RNG`
   */
  pragma "last resort"
  @deprecated("The overload of 'shuffle' that accepts an 'algorithm' argument is deprecated; please remove the 'algorithm' argument")
  proc shuffle(ref arr: [], seed: int(64) = _SeedGenerator.oddCurrentTime, param algorithm=_RNG.PCG) {
    if(algorithm==_RNG.NPB) then
      compilerError("Cannot use NPB Random number generator for array shuffling");

    var randNums = _createRandomStream(seed=seed,
                                      eltType=arr.domain.idxType,
                                      parSafe=false,
                                      algorithm=algorithm);
    randNums.shuffle(arr);
  }

  /*
    Use a new :record:`randomStream` to shuffle an array in place.

    :arg arr: A non-strided default rectangular 1D array
    :arg seed: The seed to initialize a ``randomStream`` with
  */
  proc shuffle(ref arr: [?d], seed: int) where is1DRectangularDomain(d) {
    var rs = new randomStream(d.idxType, seed);
    rs.shuffle(arr);
  }

  /*
    Use a new :record:`randomStream` to shuffle an array in place.

    .. note:: a seed will be generated in an implementation specific manner
              that depends on the current time.

    :arg arr: A non-strided default rectangular 1D array
  */
  proc shuffle(ref arr: [?d]) where is1DRectangularDomain(d) {
    var rs = new randomStream(d.idxType);
    rs.shuffle(arr);
  }

  @chpldoc.nodoc
  proc shuffle(ref arr: [], seed: int) {
    compilerError("'shuffle' only supports 1D rectangular arrays");
  }

  @chpldoc.nodoc
  proc shuffle(ref arr: []) {
    compilerError("'shuffle' only supports 1D rectangular arrays");
  }

  /*
    Produce a random permutation of an array's elements

    :arg arr: A 1D rectangular array
    :arg seed: The seed to use when creating the ``randomStream``

    :return: A new array containing each of the values from ``arr`` in a
              pseudo-random order.
  */
  proc permute(const ref arr: [?d] ?t, seed: int): [] t
    where is1DRectangularDomain(d)
  {
    var rs = new randomStream(d.idxType, seed);
    return rs.permute(arr);
  }

  /*
    Produce a random permutation of an array's elements

    :arg arr: A 1D rectangular array

    :return: A new array containing each of the values from ``arr`` in a
              pseudo-random order.
  */
  proc permute(const ref arr: [?d] ?t): [] t
    where is1DRectangularDomain(d)
  {
    var rs = new randomStream(d.idxType);
    return rs.permute(arr);
  }

  /*
    Produce a random permutation of the indices in a domain.

    :arg d: A 1D rectangular domain
    :arg seed: The seed to use when creating the ``randomStream``

    :return: An array containing each of the indices from ``d`` in a
              pseudo-random order.
  */
  proc permute(d: domain(?), seed: int): [] d.idxType
    where is1DRectangularDomain(d)
  {
    var rs = new randomStream(d.idxType, seed);
    return rs.permute(d);
  }

  /*
    Produce a random permutation of the indices in a domain.

    :arg d: A 1D rectangular domain

    :return: An array containing each of the indices from ``d`` in a
              pseudo-random order.
  */
  proc permute(d: domain(?)): [] d.idxType
    where is1DRectangularDomain(d)
  {
    var rs = new randomStream(d.idxType);
    return rs.permute(d);
  }

  /*
    Produce a random permutation of the values in a range.

    :arg r: A fully bounded range
    :arg seed: The seed to use when creating the ``randomStream``

    :return: An array containing each of the values from ``r`` in a
              pseudo-random order.
  */
  proc permute(r: range(bounds=boundKind.both, ?), seed: int): [] r.idxType {
    var rs = new randomStream(r.idxType, seed);
    return rs.permute(r);
  }
  /*
    Produce a random permutation of the values in a range.

    :arg r: A fully bounded range

    :return: An array containing each of the values from ``r`` in a
              pseudo-random order.
  */
  proc permute(r: range(bounds=boundKind.both, ?)): [] r.idxType {
    var rs = new randomStream(r.idxType);
    return rs.permute(r);
  }

  /* Produce a random permutation, storing it in a 1-D array.
     The resulting array will include each value from low..high
     exactly once, where low and high refer to the array's domain.

     :arg arr: a rectangular 1-D non-strided array
     :arg seed: the seed to use when creating the permutation. Defaults to
      `oddCurrentTime` from :type:`RandomSupport.SeedGenerator`.
     :arg algorithm: A param indicating which algorithm to use. Defaults to PCG.
     :type algorithm: :type:`RNG`
   */
  pragma "last resort"
  @deprecated("The overload of 'permutation' that accepts an 'algorithm' argument is deprecated; please remove the 'algorithm' argument")
  proc permutation(ref arr: [], seed: int(64) = _SeedGenerator.oddCurrentTime, param algorithm=_RNG.PCG) {
    if(algorithm==_RNG.NPB) then
      compilerError("Cannot use NPB Random number generator for array permutation");

    var randNums = createRandomStream(seed=seed,
                                      eltType=arr.eltType,
                                      parSafe=false,
                                      algorithm=algorithm);
    randNums.permutation(arr);
  }

  /*
    Use a new :record:`randomStream` to produce a random permutation of an
    array's domain. The values ``d.dim(0).low..d.dim(0).high`` will appear
    exactly once in the array in a pseudo-random order.

    :arg arr: The array to store the permutation in
    :arg seed: The seed to use when creating the ``randomStream``
  */
  @deprecated("'permutation(arr, seed)' is deprecated; please use 'arr = permutation(arr.domain, seed)' instead")
  proc permutation(ref arr: [?d] ?t, seed: int)
    where isCoercible(d.idxType, t) && is1DRectangularDomain(d) do
  {
    var rs = new randomStream(d.idxType, seed);
    rs.permutation(arr);
  }

  /*
    Use a new :record:`randomStream` to produce a random permutation of an
    array's domain. The values ``d.dim(0).low..d.dim(0).high`` will appear
    exactly once in the array in a pseudo-random order.

    .. note:: a seed will be generated in an implementation specific manner
              that depends on the current time.

    :arg arr: The array to store the permutation in
  */
  @deprecated("'permutation(arr)' is deprecated; please use 'arr = permutation(arr.domain)' instead")
  proc permutation(ref arr: [?d] ?t)
    where isCoercible(d.idxType, t) && is1DRectangularDomain(d) do
  {
    var rs = new randomStream(d.idxType);
    rs.permutation(arr);
  }

  @chpldoc.nodoc
  proc permutation(ref arr: [], seed: int) {
    compilerError("'permutation' only supports 1D rectangular arrays whose domain idxType is compatible with eltType");
  }

  @chpldoc.nodoc
  proc permutation(ref arr: []) {
        compilerError("'permutation' only supports 1D rectangular arrays whose domain idxType is compatible with eltType");
  }

  /*
    Choose a random element from an array.

    :arg arr: The 1D rectangular array to choose from

    :return: A random element from the array
  */
  proc choose(const ref arr: [?d] ?t): t
    where is1DRectangularDomain(d)
  {
    var rs = new randomStream(d.idxType);
    return rs.choose(arr);
  }

  /*
    Choose a random element from an array.

    :arg arr: The 1D rectangular array to choose from
    :arg seed: The seed to use when creating the ``randomStream``

    :return: A random element from the array
  */
  proc choose(const ref arr: [?d] ?t, seed: int): t
    where is1DRectangularDomain(d)
  {
    var rs = new randomStream(d.idxType, seed);
    return rs.choose(arr);
  }

  /*
    Choose a random index from a domain.

    :arg d: The 1D rectangular domain to choose from

    :return: A random index from the domain
  */
  proc choose(d: domain(?)): d.idxType {
    var rs = new randomStream(d.idxType);
    return rs.choose(d);
  }

  /*
    Choose a random index from a domain.

    :arg d: The 1D rectangular domain to choose from
    :arg seed: The seed to use when creating the ``randomStream``

    :return: A random index from the domain
  */
  proc choose(d: domain(?), seed: int): d.idxType {
    var rs = new randomStream(d.idxType, seed);
    return rs.choose(d);
  }

  /*
    Choose a random value from a range.

    :arg r: A fully bounded range to choose from

    :return: A random value from the range
  */
  proc choose(r: range(bounds=boundKind.both, ?)): r.idxType {
    var rs = new randomStream(r.idxType);
    return rs.choose({r});
  }

  /*
    Choose a random value from a range.

    :arg r: A fully bounded range to choose from
    :arg seed: The seed to use when creating the ``randomStream``

    :return: A random value from the range
  */
  proc choose(r: range(bounds=boundKind.both, ?), seed: int): r.idxType {
    var rs = new randomStream(r.idxType, seed);
    return rs.choose({r});
  }

  /*
    Randomly sample ``n`` elements from an array.

    :arg arr: The 1D rectangular array to sample from
    :arg n: The number of elements to sample
    :arg withReplacement: Whether or not to sample with replacement

    :return: A zero-based array of ``n`` random elements sampled from the array

    :throws IllegalArgumentError: If ``n < 1`` or if ``n > arr.size`` and
                                  ``withReplacement=false``. If ``arr`` is
                                  empty.
  */
  proc sample(const ref arr: [?d] ?t, n: int, withReplacement=false): [] t throws
    where is1DRectangularDomain(d)
  {
    var rs = new randomStream(d.idxType);
    return rs.sample(arr, n, withReplacement);
  }

  /*
    Randomly sample ``n`` elements from an array.

    :arg arr: The 1D rectangular array to sample from
    :arg n: The number of elements to sample
    :arg withReplacement: Whether or not to sample with replacement
    :arg seed: The seed to use when creating the ``randomStream``

    :return: A zero-based array of ``n`` random elements sampled from the array

    :throws IllegalArgumentError: If ``n < 1`` or if ``n > arr.size`` and
                                  ``withReplacement=false``. If ``arr`` is
                                  empty.
  */
  proc sample(const ref arr: [?d] ?t, n: int, withReplacement=false, seed: int): [] t throws
    where is1DRectangularDomain(d)
  {
    var rs = new randomStream(d.idxType, seed);
    return rs.sample(arr, n, withReplacement);
  }

  /*
    Randomly sample ``n`` indices from a domain.

    :arg d: The 1D rectangular domain to sample from
    :arg n: The number of indices to sample
    :arg withReplacement: Whether or not to sample with replacement

    :return: A zero-based array of ``n`` random indices sampled from the domain

    :throws IllegalArgumentError: If ``n < 1`` or if ``n > arr.size`` and
                                  ``withReplacement=false``. If ``d`` is
                                  empty.
  */
  proc sample(d: domain(?), n: int, withReplacement=false): [] d.idxType throws
    where is1DRectangularDomain(d)
  {
    var rs = new randomStream(d.idxType);
    return rs.sample(d, n, withReplacement);
  }

  /*
    Randomly sample ``n`` indices from a domain.

    :arg d: The 1D rectangular domain to sample from
    :arg n: The number of indices to sample
    :arg withReplacement: Whether or not to sample with replacement
    :arg seed: The seed to use when creating the ``randomStream``

    :return: A zero-based array of ``n`` random indices sampled from the domain

    :throws IllegalArgumentError: If ``n < 1`` or if ``n > arr.size`` and
                                  ``withReplacement=false``. If ``d`` is
                                  empty.
  */
  proc sample(d: domain(?), n: int, withReplacement=false, seed: int): [] d.idxType throws
    where is1DRectangularDomain(d)
  {
    var rs = new randomStream(d.idxType, seed);
    return rs.sample(d, n, withReplacement);
  }

  /*
    Randomly sample ``n`` values from a range.

    :arg r: A fully bounded range to sample from
    :arg n: The number of values to sample
    :arg withReplacement: Whether or not to sample with replacement

    :return: A zero-based array of ``n`` random values sampled from the range

    :throws IllegalArgumentError: If ``n < 1`` or if ``n > arr.size`` and
                                  ``withReplacement=false``. If ``r`` is
                                  empty.
  */
  proc sample(r: range(bounds=boundKind.both, ?), n: int, withReplacement=false): [] r.idxType throws {
    var rs = new randomStream(r.idxType);
    return rs.sample(r, n, withReplacement);
  }

  /*
    Randomly sample ``n`` values from a range.

    :arg r: A fully bounded range to sample from
    :arg n: The number of values to sample
    :arg withReplacement: Whether or not to sample with replacement
    :arg seed: The seed to use when creating the ``randomStream``

    :return: A zero-based array of ``n`` random values sampled from the range

    :throws IllegalArgumentError: If ``n < 1`` or if ``n > arr.size`` and
                                  ``withReplacement=false``. If ``r`` is
                                  empty.
  */
  proc sample(r: range(bounds=boundKind.both, ?), n: int, withReplacement=false, seed: int): [] r.idxType throws {
    var rs = new randomStream(r.idxType, seed);
    return rs.sample(r, n, withReplacement);
  }

  /*
    A :record:`randomStream` represents a stream of pseudorandom numbers of a
    particular type. Numeric and bool types are supported.

    Conceptually it can be thought of as an indexed sequence of numbers ranging
    from 0 to infinity. Each index in the sequence corresponds to a random
    number of the specified type. This allows for the generation of random
    numbers in parallel, where each task involved in the parallel iteration can
    request random numbers within a particular range and traverse that range of
    the sequence independently of other tasks (see :proc:`randomStream.iterate`
    (*unstable*)).

    Although parallel iteration is supported via the ``iterate`` method, the
    type itself is not thread-safe. In particular, it is not safe to call
    methods such as ``getNext`` or ``fill`` on a ``randomStream`` from multiple
    tasks concurrently.

    Several high-level methods are provided to generate random numbers or
    to manipulate arrays using random numbers:

        * :proc:`randomStream.fill` to fill an array with random numbers
        * :proc:`randomStream.shuffle` to randomly re-arrange the elements of an
          array
        * :proc:`randomStream.permute` to create a random permutation of
          an arrays domain, and store it in the array
        * :proc:`randomStream.choice` to randomly sample from an array or
          range (*unstable*)

    Note that these methods have top-level counterparts that will internally
    create a ``randomStream`` and then call the corresponding method on it —
    convenient for one-off uses. To generate many random numbers, it is
    generally more efficient to create a ``randomStream`` and call the relevant
    method on it repeatedly.

    An individual random number can be requested using :proc:`randomStream.getNext`
    which will advance the stream to the next position and return the value at
    that position. The position of the stream can also be manipulated using:

      * :proc:`randomStream.skipToNth` to skip to a particular position in the
        stream (*unstable*)
      * :proc:`randomStream.getNth` to skip to a particular position in the
        stream and return the value at that position (*unstable*)

    A ``randomStream`` can be initialized with a user-provided seed value. Two
    ``randomStream`` instances initialized with the same seed will produce
    identical sequences of random numbers. When not provided explicitly, a seed
    value will be generated in an implementation specific manner designed to
    minimize the chance that two distinct instances of the ``randomStream``
    will have the same seed.

    When copied, the ``randomStream``'s seed, state, and position in the stream
    will also be copied. This means that the copy and original will produce the
    same sequence of random numbers without affecting each others state.

    .. note:: **Implementation Details:**

      This stream is implemented using the PCG random number generator algorithm.
      See http://www.pcg-random.org/ and the paper, `PCG: A Family
      of Simple Fast Space-Efficient Statistically Good Algorithms for Random
      Number Generation` by M.E. O'Neill.

      This record builds upon the :record:`~PCGRandomLib.pcg_setseq_64_xsh_rr_32_rng`
      PCG RNG which has 64 bits of state and 32 bits of output.

      While the PCG RNG used here is believed to have good statistical
      properties, it is not suitable for generating key material for encryption
      since the output of this RNG may be predictable.
      Additionally, if statistical properties of the random numbers are very
      important, another strategy may be required.

      We have good confidence that the random numbers generated by this record
      match the C PCG reference implementation and have specifically verified
      equal output given the same seed. However, this implementation differs
      from the C PCG reference implementation in how it produces random integers
      within particular bounds (with :proc:`randomStream.getNext` using ``min``
      and ``max`` arguments). In addition, this implementation directly supports
      the generation of random ``real`` values, unlike the C PCG implementation.

      Smaller numbers, such as ``uint(8)`` or ``uint(16)``, are generated from
      the high-order bits of the 32-bit output.

      To generate larger numbers, several 32-bit-output RNGs are composed
      together. Each of these 32-bit RNGs has a different sequence constant and
      so will be independent and uncorrelated. For example, to generate 128-bit
      complex numbers, this RNG will use four 32-bit PCG RNGs with different
      sequence constants. One impact of this approach is that this implementation
      will only generate 2**64 different complex numbers with a given seed
      (for example).

      This record also supports generating integers within particular bounds.
      When that is required, it uses a strategy different from the PCG
      reference implementation to support efficient parallel iteration. In
      particular, when more than 1 random value is required as part of
      generating a value in a range, conceptually it uses more composed
      RNGs (as with the 32x2 strategy). Each new value beyond the first that
      is computed will be computed with a different RNG. This strategy is meant
      to avoid statistical bias. While we have tested this strategy to our
      satisfaction, it has not been subject to rigorous analysis and may
      have undesirable statistical properties.

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
  record randomStream: writeSerializable {
    /*
      Specifies the type of value generated by the random stream. Currently,
      numeric and bool types are supported.
    */
    type eltType;

    /*
      The seed value for the PCG random number generator.
    */
    const seed: int;

    @chpldoc.nodoc
    var pcg: PCGImpl(eltType);

    /*
      Create a new ``randomStream`` using the specified seed.
    */
    proc init(type eltType, seed: int) where isNumericOrBoolType(eltType) {
      this.eltType = eltType;
      this.seed = seed;
      this.pcg = new PCGImpl(eltType, seed);
    }

    /*
      Create a new ``randomStream``.

      A seed value will be generated in an implementation specific manner
      designed to minimize the chance that two distinct invocations of this
      initializer will produce the same seed.
    */
    proc init(type eltType) where isNumericOrBoolType(eltType) {
      this.eltType = eltType;
      this.seed = randomishSeed();
      this.pcg = new PCGImpl(eltType, this.seed);
    }

    @chpldoc.nodoc
    proc init(type eltType, seed: int) {
      this.eltType = eltType;
      compilerError("'randomStream' only supports numeric or bool types");
    }

    @chpldoc.nodoc
    proc init(type eltType) {
      this.eltType = eltType;
      compilerError("'randomStream' only supports numeric or bool types");
    }

    /*
      Fill the array with pseudorandom values sampled from this stream.

      :arg arr: The rectangular array to be filled
    */
    proc ref fill(ref arr: []) where arr.isRectangular() do
      this.pcg.fillRandom(arr);

    /*
      Fill the array with pseudorandom values within a particular range. Each
      array element is set to a number in [``min``, ``max``] (inclusive)
      sampled from this stream.

      :arg arr: The rectangular array to be filled
      :arg min: The minimum value to sample
      :arg max: The maximum value to sample
    */
    proc ref fill(ref arr: [] ?t, min: t, max: t) where arr.isRectangular() do
      this.pcg.fillRandom(arr, min, max);

    @deprecated("'randomStream.fillRandom' is deprecated; please use 'fill' instead")
    proc ref fillRandom(ref arr: []) do
      this.fill(arr);

    @deprecated("'randomStream.fillRandom' is deprecated; please use 'fill' instead")
    proc ref fillRandom(ref arr: [], min, max) do
      this.fill(arr, min, max);

    /*
      Randomly rearrange a 1D array using values from this random stream.

      :arg arr: The array to shuffle. Its domain's ``idxType`` should be
                coercible to this stream's :type:`eltType`.
    */
    proc ref shuffle(ref arr: [?d]) where is1DRectangularDomain(d) && isCoercible(this.eltType, d.idxType)
      do this.pcg.shuffle(arr);

    /*
      Produce a random permutation of an array's elements

      :arg arr: A 1D rectangular array whose domain's ``idxType``
                must be coercible from this stream's ``eltType``.

      :return: A new array containing each of the values from ``arr`` in a
               pseudo-random order.
    */
    proc ref permute(const ref arr: [?d] ?t): [] t
      where is1DRectangularDomain(d) && isCoercible(this.eltType, d.idxType)
    {
      const dp = this.pcg.domPermutation(d);
      var res: [d] t;
      forall (i, ip) in zip(d, dp) do res[i] = arr[ip];
      return res;
    }

    /*
      Produce a random permutation of the indices in a domain.

      :arg d: A 1D rectangular domain whose ``idxType`` must be coercible
                from this stream's ``eltType``.

      :return: An array containing each of the indices from ``d`` in a
               pseudo-random order.
    */
    proc ref permute(d: domain(?)): [] d.idxType
      where is1DRectangularDomain(d) && isCoercible(this.eltType, d.idxType)
        do return this.pcg.domPermutation(d);

    /*
      Produce a random permutation of the values in a range.

      :arg r: A fully bounded range whose ``idxType`` must be coercible from
              this stream's ``eltType``.

      :return: An array containing each of the values from ``r`` in a
               pseudo-random order.
    */
    proc ref permute(r: range(bounds=boundKind.both, ?)): [] r.idxType
      where isCoercible(this.eltType, r.idxType)
        do return this.pcg.domPermutation({r});

    /*
      Produce a random permutation of an array's domain. The values
      ``d.dim(0).low..d.dim(0).high`` will appear exactly once in the array in
      a pseudo-random order.

      :arg arr: The array to store the permutation in
    */
    @deprecated("'randomStream.permutation(arr)' is deprecated; please use 'arr = randomStream.permutation(arr.domain)' instead")
    proc ref permutation(ref arr: [?d] ?t)
      where isCoercible(this.eltType, d.idxType) && isCoercible(d.idxType, t) && is1DRectangularDomain(d) do
        this.pcg.permutation(arr);

    /*
      Choose a random element from an array.

      :arg arr: The 1D rectangular array to choose from. Its domain's ``idxType``
                should be coercible from this stream's ``eltType``.

      :return: A random element from the array
    */
    proc ref choose(const ref arr: [?d] ?t): t
      where is1DRectangularDomain(d) && isCoercible(this.eltType, d.idxType)
    {
      return arr[this.choose(d)];
    }

    /*
      Choose a random index from a domain.

      :arg d: The 1D rectangular domain to choose from. Its ``idxType`` should
              be coercible from this stream's ``eltType``.

      :return: A random index from the domain
    */
    proc ref choose(d: domain(?)): d.idxType
      where is1DRectangularDomain(d) && isCoercible(this.eltType, d.idxType)
    {
      return d.orderToIndex(this.next(0:this.eltType, (d.size-1):this.eltType));
    }

    /*
      Choose a random value from a range.

      :arg r: The fully bounded range to choose from. Its ``idxType`` should
              be coercible from this stream's ``eltType``.

      :return: A random value from the range
    */
    proc ref choose(r: range(bounds=boundKind.both, ?)): r.idxType
      where isCoercible(this.eltType, r.idxType)
    {
      return this.choose({r});
    }

    /*
      Sample ``n`` random elements from an array.

      :arg arr: The 1D rectangular array to sample from. Its domain's ``idxType``
                should be coercible from this stream's ``eltType``.
      :arg n: The number of elements to sample
      :arg withReplacement: Whether or not to sample with replacement

      :return: A zero-based array of ``n`` random elements sampled from the array

      :throws IllegalArgumentError: If ``n < 1`` or if ``n > arr.size`` and
                                    ``withReplacement=false``. If ``arr`` is
                                    empty.
    */
    proc ref sample(const ref arr: [?d] ?t, n: int, withReplacement=false): [] t throws
      where is1DRectangularDomain(d) && isCoercible(this.eltType, d.idxType)
    {
      if d.size < 1 then
        throw new IllegalArgumentError("Cannot sample from an empty array");

      if n < 1 || (n > d.size && !withReplacement) then
        throw new IllegalArgumentError("Number of samples must be >= 1 and <= arr.size when withReplacement=false");

      const ds = this.sample(d, n, withReplacement);
      var res: [0..<n] t;
      forall (i, di) in zip(res.domain, ds) do res[i] = arr[di];
      return res;
    }

    /*
      Sample ``n`` random indices from a domain.

      :arg d: The 1D rectangular domain to sample from. Its ``idxType`` should
              be coercible from this stream's ``eltType``.
      :arg n: The number of indices to sample
      :arg withReplacement: Whether or not to sample with replacement

      :return: A zero-based array of ``n`` random indices sampled from the domain

      :throws IllegalArgumentError: If ``n < 1`` or if ``n > arr.size`` and
                                    ``withReplacement=false``. If ``d`` is
                                    empty.
    */
    proc ref sample(d: domain, n: int, withReplacement=false): [] d.idxType throws
      where is1DRectangularDomain(d) && isCoercible(this.eltType, d.idxType)
    {
      if d.size < 1 then
        throw new IllegalArgumentError("Cannot sample from an empty domain");

      if n < 1 || (n > d.size && !withReplacement) then
        throw new IllegalArgumentError("Number of samples must be >= 1 and <= d.size when withReplacement=false");

      // create an array of n indices
      const dOut = {0..<n};
      var samples: [dOut] d.idxType;

      if withReplacement {
        for s in samples do s = this.choose(d);
      } else {
        if n < log2(d.size) {
          // for a sufficiently small number of indices,
          // use a set to fill 'samples' with n unique values
          var indices: domain(int, parSafe=false),
              i = 0;
          while i < n {
            const sample = this.choose(d);
            if !indices.contains(sample) {
              samples[dOut.orderToIndex(i)] = sample;
              indices.add(sample);
              i += 1;
            }
          }
        } else {
          // otherwise, shuffle an array of all
          // indices and take the first n of them
          var indices = [idx in d] idx;
          this.shuffle(indices);
          forall i in dOut
            do samples[i] = indices[d.orderToIndex(i)];
        }
      }

      return samples;
    }

    /*
      Sample ``n`` random values from a range.

      :arg r: The fully bounded range to sample from. Its ``idxType`` should
              be coercible from this stream's ``eltType``.
      :arg n: The number of values to sample
      :arg withReplacement: Whether or not to sample with replacement

      :return: A zero-based array of ``n`` random values sampled from the range

      :throws IllegalArgumentError: If ``n < 1`` or if ``n > arr.size`` and
                                    ``withReplacement=false``. If ``r`` is
                                    empty.
    */
    proc ref sample(r: range(bounds=boundKind.both, ?), n: int, withReplacement=false): [] r.idxType throws
      where isCoercible(this.eltType, r.idxType)
    {
      if r.size < 1 then
        throw new IllegalArgumentError("Cannot sample from an empty range");

      if n < 1 || (n > r.size && !withReplacement) then
        throw new IllegalArgumentError("Number of samples must be >= 1 and <= r.size when withReplacement=false");

      return this.sample({r}, n, withReplacement);
    }

    /*
      Return a random sample from a given 1D array, ``x``.

      :arg x: The array to take a sample from.
      :arg size: An optional integral value specifying the number of elements to
                choose, or a domain specifying the dimensions of the
                sampled array to be filled, otherwise a single element will be
                chosen.
      :arg replace: an optional ``bool`` specifying whether or not to sample with
                   replacement, i.e. elements will only be chosen up to one
                   time when ``replace=false``.
      :arg prob: an optional 1D array that contains probabilities of choosing
                each element of ``x``, otherwise elements will be chosen over
                a uniform distribution. ``prob`` must have integral or real
                element type, with no negative values and at least one non-zero
                value. The size must be equal to that of ``x.domain``.

      :return: An element chosen from ``x`` if ``size == 1``, or an array of
              elements chosen from ``x`` if ``size > 1`` or ``size`` is a
              domain.

      :throws IllegalArgumentError: if ``x.size == 0``,
                                   if ``x.size != prob.size``,
                                   if ``prob`` contains a negative value,
                                   if ``prob`` has no non-zero values,
                                   if ``size < 1 || size.size < 1``,
                                   if ``replace=false`` and ``size > x.size || size.size > x.size``
    */
    @deprecated("randomStream.choice is deprecated; please use :proc:`~randomStream.choose` or :proc:`~randomStream.sample` instead")
    proc ref choice(const x: [?d], size:?sizeType=none, replace=true, prob:?probType=none) throws
      where is1DRectangularDomain(d) && isCoercible(this.eltType, d.idxType)
    {
      const idx = _choice(this, d, size, replace, prob);
      return x[idx];
    }

    /*
      Return a random sample from a given bounded range, ``x``.

      :arg x: a bounded range with values that will be sampled from.
      :arg size: An optional integral value specifying the number of elements to
                choose, or a domain specifying the dimensions of the
                sampled array to be filled, otherwise a single element will be
                chosen.
      :arg replace: an optional ``bool`` specifying whether or not to sample with
                    replacement, i.e. elements will only be chosen up to one
                    time when ``replace=false``.
      :arg prob: an optional 1D array that contains probabilities of choosing
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
                                    if ``replace=false`` and ``size > x.size || size.size > x.size``
    */
    @deprecated("randomStream.choice is deprecated; please use :proc:`~randomStream.choose` or :proc:`~randomStream.sample` instead")
    proc ref choice(x: range(?), size:?sizeType=none, replace=true, prob:?probType=none) throws
      where isCoercible(this.eltType, x.idxType)
        do return _choice(this, {x}, size, replace, prob);

    /*
      Return a random sample from a given 1D domain, ``x``.

      :arg x: a 1D domain with values that will be sampled from.
      :arg size: An optional integral value specifying the number of elements to
                choose, or a domain specifying the dimensions of the
                sampled array to be filled, otherwise a single element will be
                chosen.
      :arg replace: an optional ``bool`` specifying whether or not to sample with
                    replacement, i.e. elements will only be chosen up to one
                    time when ``replace=false``.
      :arg prob: an optional 1D array that contains probabilities of choosing
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
    @deprecated("randomStream.choice is deprecated; please use :proc:`~randomStream.choose` or :proc:`~randomStream.sample` instead")
    proc ref choice(x: domain, size:?sizeType=none, replace=true, prob:?probType=none) throws
      where is1DRectangularDomain(x) && isCoercible(this.eltType, x.idxType)
        do return _choice(this, x, size, replace, prob);

    /*
      Get the next value in the random stream and advance its position by one.
    */
    proc ref next(): eltType do
      return this.pcg.getNext();

    /*
      Get the next random value from the stream within a given range. Returns
      a number in [``min``, ``max``] (inclusive).

      This method will halt if checks are enabled and ``min > max``.

      .. note::

        For integers, this type uses a strategy for generating a value
        in a particular range that has not been subject to rigorous
        study and may have statistical problems.

        For real numbers, this type generates a random value in [max, min]
        by computing a random value in [0,1] and scaling and shifting that
        value. Note that not all possible floating point values in
        the interval [`min`, `max`] can be constructed in this way.

      :arg min: The minimum value to sample
      :arg max: The maximum value to sample
    */
    proc ref next(min: eltType, max: eltType): eltType do
      return this.pcg.getNext(min, max);

    /*
      Return an iterable object yielding values from the random stream.

      For example, a rectangular array ``A`` could be filled with random values using:

      .. code-block:: chapel

        var rs = new randomStream(int),
            A: [1..1000] int;

        forall (a, r) in zip(A, rs.next(A.domain)) do
          a = r;

      *Note that :proc:`randomStream.fill` also serves the same purpose.*

      :arg d: domain associated with the iteration. ``d.size`` values will be yielded
              by the iterator. When ``d`` is the first argument in a zippered iteration,
              its parallelization strategy will be used.
    */
    pragma "fn returns iterator"
    proc ref next(d: domain) do
      return this.pcg.iterate(d, eltType);

    pragma "fn returns iterator"
    @chpldoc.nodoc
    proc next(d: domain, param tag: iterKind)
      where tag == iterKind.leader
        do return this.pcg.iterate(d, eltType, tag);

    /*
      Return an iterable object yielding values from the random stream within
      a given range.

      :arg d: domain associated with the iteration. ``d.size`` values will be yielded
              by the iterator. When ``d`` is the first argument in a zippered iteration,
              its parallelization strategy will be used.
      :arg min: The minimum value to sample
      :arg max: The maximum value to sample
    */
    pragma "fn returns iterator"
    proc ref next(d: domain, min: eltType, max: eltType) do
      return this.pcg.iterate(d, eltType, min, max);

    pragma "fn returns iterator"
    @chpldoc.nodoc
    proc next(d: domain, min: eltType, max: eltType, param tag: iterKind)
      where tag == iterKind.leader
        do return this.pcg.iterate(d, eltType, min, max, tag);

    @deprecated("randomStream.getNext is deprecated; please use :proc:`next` instead")
    proc ref getNext(): eltType do
      return this.pcg.getNext();

    @deprecated("randomStream.getNext is deprecated; please use :proc:`next` instead")
    proc ref getNext(min: eltType, max: eltType): eltType do
      return this.pcg.getNext(min, max);

    /*
      Advance or rewind the random stream to the ``n``-th position in the
      pseudorandom sequence (where ``n=0`` is the starting position)

      This method will halt for negative arguments if checks are enabled.

      :arg n: The position to skip to
    */
    proc ref skipTo(n: int) {
      if boundsChecking then
        if n < 0 then halt("cannot skip to a negative position: " + n:string + " in the random stream");

      try! this.pcg.skipToNth(n);
    }

    @deprecated("skipToNth is deprecated; please use :proc:`skipTo` instead")
    proc ref skipToNth(n: integral) throws do
      this.pcg.skipToNth(n);

    /*
      Advance or rewind the random stream to the ``n``-th position in the
      pseudorandom sequence and return the value. This is equivalent to
      :proc:`skipToNth` followed by :proc:`getNext`.

      :arg n: The position to skip to and retrieve (must be greater than zero)

      :throws IllegalArgumentError: If ``n`` is negative
    */
    @deprecated("getNth is deprecated; please use :proc:`skipTo` followed by :proc:`next` instead")
    proc ref getNth(n: integral): eltType throws do
      return this.pcg.getNth(n);


    pragma "fn returns iterator"
    @deprecated("iterate is deprecated; please use :proc:`next` instead")
    proc ref iterate(D: domain) do
      return this.pcg.iterate(D, eltType);

    pragma "fn returns iterator"
    @deprecated("iterate is deprecated; please use :proc:`next` instead")
    proc ref iterate(D: domain, min: eltType, max: eltType) do
      return this.pcg.iterate(D, eltType, min, max);

    pragma "fn returns iterator"
    @chpldoc.nodoc
    @deprecated("iterate is deprecated; please use :proc:`next` instead")
    proc iterate(D: domain, param tag: iterKind)
      where tag == iterKind.leader
        do return this.pcg.iterate(D, eltType, tag);

    pragma "fn returns iterator"
    @chpldoc.nodoc
    @deprecated("iterate is deprecated; please use :proc:`next` instead")
    proc iterate(D: domain, min: eltType, max: eltType, param tag: iterKind)
      where tag == iterKind.leader
        do return this.pcg.iterate(D, eltType, min, max, tag);

    /*
      serialize the ``randomStream`` as a record with two fields: ``eltType`` and ``seed``.
    */
    proc serialize(writer, ref serializer) throws {
      var ser = serializer.startRecord(writer, "randomStream", 2);
      ser.writeField("eltType", this.eltType:string);
      ser.writeField("seed", this.seed);
      ser.endRecord();
    }
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
  @deprecated("'createRandomStream' is deprecated; please use 'new randomStream' instead")
  proc createRandomStream(type eltType,
                          seed: int(64) = _SeedGenerator.oddCurrentTime,
                          param parSafe: bool = true,
                          param algorithm = _defaultRNG) do
    return _createRandomStream(eltType, seed, parSafe, algorithm);

  @chpldoc.nodoc
  proc _createRandomStream(type eltType,
                          seed: int(64) = _SeedGenerator.oddCurrentTime,
                          param parSafe: bool = true,
                          param algorithm = _defaultRNG) {
    if algorithm == _RNG.PCG then
      return new owned PCGRandomStreamInternal(seed=seed,
                                       parSafe=parSafe,
                                       eltType=eltType);
    else if algorithm == _RNG.NPB then
      return new owned NPBRandom.NPBRandomStream(seed=seed,
                                       parSafe=parSafe,
                                       eltType=eltType);
    else
      compilerError("Unknown random number generator");
  }


  @chpldoc.nodoc
  /* Actual implementation of choice() */
  proc _choice(ref stream, X: domain, size: ?sizeType, replace: bool, prob: ?probType)
    throws
  {
    if X.rank != 1 {
      compilerError('choice() argument x must be 1 dimensional');
    }
    if X.sizeAs(X.idxType) < 1 {
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
      if prob.size != X.sizeAs(X.idxType) {
        throw new owned IllegalArgumentError('choice() x.size must be equal to prob.size');
      }
    }

    if !isNothingType(sizeType) {
      if isIntegralType(sizeType) {
        if size <= 0 then
          throw new owned IllegalArgumentError('choice() size must be greater than 0');
        if !replace && size > X.sizeAs(X.idxType) then
          throw new owned IllegalArgumentError('choice() size must be smaller than x.size when replace=false');
      } else if isDomainType(sizeType) {
        if size.size <= 0 then
          throw new owned IllegalArgumentError('choice() size domain can not be empty');
        if !replace && size.size > X.sizeAs(X.idxType) then
          throw new owned IllegalArgumentError('choice() size must be smaller than x.size when replace=false');
      } else {
        compilerError('choice() size must be integral or domain');
      }
    }

    if isNothingType(probType) {
      return _choiceUniform(stream, X, size, replace);
    } else {
      var realStream = new randomStream(real, seed=stream.seed);
      return _choiceProbabilities(realStream, X, size, replace, prob);
    }
  }

  @chpldoc.nodoc
  /* _choice branch for uniform distribution */
  proc _choiceUniform(ref stream, X: domain, size: ?sizeType, replace: bool) throws
  {
    const low = X.low,
          stride = abs(X.stride);

    if isNothingType(sizeType) {
      // Return 1 sample
      var randVal;
      // TODO: removed first branch of this conditional after PCG/NPBRandomStream deprecations
      if __primitive("method call and fn resolves", stream, "next", X.idxType) {
        randVal = stream.next(resultType=X.idxType, 0, X.sizeAs(X.idxType)-1);
      } else {
        randVal = stream.next(0, X.sizeAs(X.idxType)-1);
      }
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
          var randVal;
          // TODO: removed first branch of this conditional after PCG/NPBRandomStream deprecations
          if __primitive("method call and fn resolves", stream, "next", X.idxType) {
            randVal = stream.next(resultType=X.idxType, 0, X.sizeAs(X.idxType)-1);
          } else {
            randVal = stream.next(0, X.sizeAs(X.idxType)-1);
          }
          var randIdx = X.dim(0).orderToIndex(randVal);
          sample = randIdx;
        }
      } else {
        if numElements < log2(X.sizeAs(X.idxType)) {
          var indices: domain(int, parSafe=false);
          var i: int = 0;
          while i < numElements {
            var randVal;
            // TODO: removed first branch of this conditional after PCG/NPBRandomStream deprecations
            if __primitive("method call and fn resolves", stream, "next", X.idxType) {
              randVal = stream.next(resultType=X.idxType, 0, X.sizeAs(X.idxType)-1);
            } else {
              randVal = stream.next(0, X.sizeAs(X.idxType)-1);
            }
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

  @chpldoc.nodoc
  /* _choice branch for distribution defined by probabilities array */
  proc _choiceProbabilities(ref stream, X:domain, size:?sizeType, replace, prob:?probType) throws
  {
    import Search;
    import Sort;

    if prob.size != X.sizeAs(X.idxType) {
      throw new owned IllegalArgumentError('choice() x.size must be equal to prob.size');
    }

    if prob.size == 0 then
      throw new owned IllegalArgumentError('choice() prob array cannot be empty');

    const low = X.low,
          stride = abs(X.stride);
    ref P = prob.reindex(0..<X.sizeAs(X.idxType));

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
      var randNum = stream.next();
      var (found, idx) = Search.binarySearch(cumulativeArr, randNum);
      return X.dim(0).orderToIndex(idx);
    } else {
      // Return numElements samples

      // Compute numElements for tuple case
      var m = 1;
      if isDomainType(sizeType) then m = size.sizeAs(size.idxType);

      var numElements = if isDomainType(sizeType) then m
                        else if isIntegralType(sizeType) then size:int
                        else compilerError('choice() size type must be integral or tuple of ranges');

      // Return N samples
      var samples: [0..<numElements] int;

      if replace {
        for sample in samples {
          var randNum = stream.next();
          var (found, idx) = Search.binarySearch(cumulativeArr, randNum);
          sample = X.dim(0).orderToIndex(idx);
        }
      } else {
        var indicesChosen: domain(int);
        var i = 0;
        while indicesChosen.sizeAs(int) < samples.sizeAs(int) {

          // Recalculate normalized cumulativeArr
          if indicesChosen.sizeAs(int) > 0 {
            cumulativeArr = (+ scan P): real;
            total = cumulativeArr[P.domain.last];
            cumulativeArr /= total;
          }

          var remainingSamples = samples.sizeAs(int) - indicesChosen.sizeAs(int);
          for randNum in stream.next({1..(samples.sizeAs(int) - indicesChosen.sizeAs(int))}) {
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
  @deprecated("'RandomStreamInterface' is deprecated")
  class RandomStreamInterface : writeSerializable {
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
    proc fillRandom(ref arr: [] eltType) {
      compilerError("RandomStreamInterface.fillRandom called");
    }

    @chpldoc.nodoc
    proc fillRandom(ref arr: []) {
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
     proc choice(const x: [], size:?sizeType=none, replace=true, prob:?probType=none) throws
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
     */
     proc choice(x: range(strides=?), size:?sizeType=none, replace=true, prob:?probType=none) throws
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

    @chpldoc.nodoc
    proc writeThis(f) throws {
      f.write("RandomStreamInterface(eltType=", eltType:string);
      f.write(", parSafe=", parSafe);
      f.write(", seed=", seed, ")");
    }

    @chpldoc.nodoc
    proc serialize(writer, ref serializer) throws {
      writeThis(writer);
    }
  }


  /*
     Seed generation for pseudorandom number generation.

     .. warning:

       The ``RandomSupport`` sub-module and its contents are deprecated

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
    @deprecated("'SeedGenerator' is deprecated")
    type SeedGenerator = _SeedGenerator;

    @chpldoc.nodoc
    record _SeedGenerator {
      /*
        Generate a seed based on the current time in microseconds as
        reported by :proc:`Time.timeSinceEpoch`. This seed is not
        suitable for the NPB RNG since that requires an odd seed.
      */
      proc type currentTime: int(64) {
        use Time;
        const seed = (timeSinceEpoch().totalSeconds()*1_000_000):int(64);
        return seed;

      }
      /*
        Generate an odd seed based on the current time in microseconds as
        reported by :proc:`Time.timeSinceEpoch`. This seed is suitable
        for the NPB RNG.
      */
      proc type oddCurrentTime: int(64) {
        use Time;
        const seed = (timeSinceEpoch().totalSeconds()*1_000_000): int;
        const oddseed = if seed % 2 == 0 then seed + 1 else seed;
        return oddseed;
      }
    }
  } // close module RandomSupport

  /*
     Permuted Linear Congruential Random Number Generator.

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
  @unstable("the 'PCGRandom' module is unstable and may be removed in the future")
  module PCGRandom {

    private use Random, IO;
    private use Math only ldExp;
    private use PCGRandomLib;
    use ChapelLocks;

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
      within particular bounds (with ``PCGRandomStream.getNext`` using ``min``
      and ``max`` arguments). In addition, this implementation directly supports
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
    @deprecated("'PCGRandomStream' is deprecated; please use :record:`~Random.randomStream` instead")
    type PCGRandomStream = PCGRandomStreamInternal(?);

    @chpldoc.nodoc
    class PCGRandomStreamInternal : writeSerializable {
      type eltType;
      const seed: int(64);
      param parSafe: bool = true;

      var pcg: PCGImpl(eltType);

      // locking
      var _l: if parSafe then chpl_LocalSpinlock else nothing;
      inline proc _lock() do if parSafe then _l.lock();
      inline proc _unlock() do if parSafe then _l.unlock();

      proc init(type eltType,
                seed: int(64) = _SeedGenerator.currentTime,
                param parSafe: bool = true) {
        this.eltType = eltType;
        this.seed = seed;
        this.parSafe = parSafe;
        init this;
        this.pcg = new PCGImpl(eltType, seed);
      }

      proc getNext(type resultType=eltType): resultType {
        _lock();
        const result = this.pcg.getNext(resultType);
        _unlock();
        return result;
      }

      proc getNext(min: eltType, max:eltType): eltType {
        _lock();
        const result = this.pcg.getNext(min, max);
        _unlock();
        return result;
      }

      proc getNext(type resultType,
                   min: resultType, max:resultType): resultType {
        _lock();
        const result = this.pcg.getNext(resultType, min, max);
        _unlock();
        return result;
      }

      proc skipToNth(n: integral) throws {
        _lock(); defer _unlock();
        this.pcg.skipToNth(n);
      }

      proc getNth(n: integral): eltType throws {
        _lock(); defer _unlock();
        return this.pcg.getNth(n);
      }

      proc fillRandom(ref arr: []) {
        if(!arr.isRectangular()) then
          compilerError("fillRandom does not support non-rectangular arrays");

        // call this.iterate instead of pcg.iterate to use locking
        forall (x, r) in zip(arr, this.iterate(arr.domain, arr.eltType)) do
          x = r;
      }

      proc fillRandom(ref arr: [], min: arr.eltType, max:arr.eltType) {
        if(!arr.isRectangular()) then
          compilerError("fillRandom does not support non-rectangular arrays");

        // call this.iterate instead of pcg.iterate to use locking
        forall (x, r) in zip(arr, this.iterate(arr.domain, arr.eltType,
                                          min, max)) do
          x = r;
      }

      proc choice(const x: [?dom], size:?sizeType=none, replace=true, prob:?probType=none) throws
      {
        _lock(); defer _unlock();
        return this.pcg.choice(x, size=size, replace=replace, prob=prob);
      }

      proc choice(x: range(?), size:?sizeType=none, replace=true, prob:?probType=none) throws
      {
        _lock(); defer _unlock();
        return this.pcg.choice(x, size=size, replace=replace, prob=prob);
      }

      proc choice(x: domain, size:?sizeType=none, replace=true, prob:?probType=none) throws
      {
        _lock(); defer _unlock();
        return this.pcg.choice(x, size=size, replace=replace, prob=prob);
      }

      proc shuffle(ref arr: [?D] ?eltType) {
        _lock();
        this.pcg.shuffle(arr);
        _unlock();
      }

      proc permutation(ref arr: [] eltType) {
        _lock();
        this.pcg.permutation(arr);
        _unlock();
      }

      pragma "fn returns iterator"
      proc iterate(D: domain, type resultType=eltType) {
        _lock();
        const start = this.pcg._iterateStart(D);
        _unlock();
        return this.pcg._iterate(D, resultType, start);
      }

      pragma "fn returns iterator"
      proc iterate(D: domain, type resultType=eltType,
                   min: resultType, max: resultType) {
        _lock();
        const start = this.pcg._iterateStart(D);
        _unlock();
        return this.pcg._iterate(D, resultType, min, max, start);
      }

      // Forward the leader iterator as well.
      pragma "fn returns iterator"
      proc iterate(D: domain, type resultType=eltType, param tag)
        where tag == iterKind.leader
      {
        return this.pcg.iterate(D, resultType, tag);
      }
      pragma "fn returns iterator"
      @chpldoc.nodoc
      proc iterate(D: domain, type resultType=eltType,
                   min: resultType, max: resultType, param tag)
        where tag == iterKind.leader
      {
        return this.pcg.iterate(D, resultType, min, max, tag);
      }

      override proc serialize(writer, ref serializer) throws {
        var ser = serializer.startRecord(writer, "PCGRandomStream", 3);
        ser.writeField("eltType", eltType:string);
        ser.writeField("parSafe", parSafe);
        ser.writeField("seed", seed);
        ser.endRecord();
      }
    }

    @chpldoc.nodoc
    record PCGImpl: writeSerializable {
      type eltType;
      var seed: int(64);

      // state
      var PCGRandomStreamPrivate_rngs: numGenerators(eltType) * pcg_setseq_64_xsh_rr_32_rng;
      var PCGRandomStreamPrivate_count: int(64) = 1;

      proc init(type eltType,
                seed: int(64) = _SeedGenerator.currentTime) {
        this.eltType = eltType;
        this.seed = seed;
        init this;
        for param i in 0..<numGenerators(eltType) {
          param inc = pcg_getvalid_inc(i+1);
          PCGRandomStreamPrivate_rngs[i].srandom(seed:uint(64), inc);
        }
        PCGRandomStreamPrivate_count = 1;
      }

      proc ref PCGRandomStreamPrivate_getNext_noLock(type resultType) {
        PCGRandomStreamPrivate_count += 1;
        return randlc(resultType, PCGRandomStreamPrivate_rngs);
      }
      proc ref PCGRandomStreamPrivate_getNext_noLock(type resultType,
                                                 min:resultType,
                                                 max:resultType) {

        // If the resultType is a type that fits into
        PCGRandomStreamPrivate_count += 1;
        return randlc_bounded(resultType, PCGRandomStreamPrivate_rngs,
                              seed, PCGRandomStreamPrivate_count-1, min, max);
      }

      proc ref PCGRandomStreamPrivate_skipToNth_noLock(in n: integral) {
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
      proc ref getNext(type resultType=eltType): resultType {
        return PCGRandomStreamPrivate_getNext_noLock(resultType);
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
      proc ref getNext(min: eltType, max:eltType): eltType {
        use HaltWrappers;
        if boundsChecking && min > max then
          HaltWrappers.boundsCheckHalt("Cannot generate random numbers within empty range: [" + min:string + ", " + max:string +  "]");

        return PCGRandomStreamPrivate_getNext_noLock(eltType,min,max);
      }

      /*
        As with getNext(min, max) but allows specifying the result type.
       */
      proc ref getNext(type resultType,
                   min: resultType, max:resultType): resultType {
        use HaltWrappers;
        if boundsChecking && min > max then
          HaltWrappers.boundsCheckHalt("Cannot generate random numbers within empty range: [" + min:string + ", " + max:string + "]");

        return PCGRandomStreamPrivate_getNext_noLock(resultType,min,max);
      }

      /*
        Advances/rewinds the stream to the `n`-th value in the sequence.
        The first value corresponds to n=0.  n must be >= 0, otherwise an
        IllegalArgumentError is thrown.

        :arg n: The position in the stream to skip to.  Must be >= 0.
        :type n: `integral`

        :throws IllegalArgumentError: When called with negative `n` value.
       */
      proc ref skipToNth(n: integral) throws {
        if n < 0 then
          throw new owned IllegalArgumentError("PCGRandomStream.skipToNth(n) called with negative 'n' value " + n:string);
        PCGRandomStreamPrivate_skipToNth_noLock(n);
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
      proc ref getNth(n: integral): eltType throws {
        if (n < 0) then
          throw new owned IllegalArgumentError("PCGRandomStream.getNth(n) called with negative 'n' value " + n:string);
        PCGRandomStreamPrivate_skipToNth_noLock(n);
        return PCGRandomStreamPrivate_getNext_noLock(eltType);
      }

      /*
        Fill the argument array with pseudorandom values.  This method is
        identical to the standalone :proc:`~Random.fillRandom` procedure,
        except that it consumes random values from the
        :class:`PCGRandomStream` object on which it's invoked rather
        than creating a new stream for the purpose of the call.

        :arg arr: The array to be filled
        :type arr: `[] T`
      */
      proc ref fillRandom(ref arr: []) {
        if(!arr.isRectangular()) then
          compilerError("fillRandom does not support non-rectangular arrays");

        forall (x, r) in zip(arr, iterate(arr.domain, arr.eltType)) do
          x = r;
      }

      /*
        Fill the argument array with pseudorandom values within a particular
        range. Sets each element to a number in [`min`, `max`] (inclusive).

        See also the caveats in the :proc:`PCGRandomStream.getNext` accepting
        min and max arguments..

        :arg arr: The array to be filled
        :type arr: `[] T`
      */
      proc ref fillRandom(ref arr: [], min: arr.eltType, max:arr.eltType) {
        if(!arr.isRectangular()) then
          compilerError("fillRandom does not support non-rectangular arrays");

        forall (x, r) in zip(arr, iterate(arr.domain, arr.eltType,
                                          min, max)) do
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
      proc ref choice(const x: [?dom], size:?sizeType=none, replace=true, prob:?probType=none)
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
      */
      proc ref choice(x: range(?), size:?sizeType=none, replace=true, prob:?probType=none)
        throws
      {
        if x.bounds != boundKind.both {
          compilerError('input range must be bounded');
        }
        var dom = {x};
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
      proc ref choice(x: domain, size:?sizeType=none, replace=true, prob:?probType=none)
        throws
      {
        return _choice(this, x, size=size, replace=replace, prob=prob);
      }

      /* Randomly shuffle a 1-D array. */
      proc ref shuffle(ref arr: [?D] ) {

        if(!arr.isRectangular()) then
          compilerError("shuffle does not support non-rectangular arrays");

        if D.rank != 1 then
          compilerError("Shuffle requires 1-D array");

        const low = D.low: this.eltType,
              stride = abs(D.stride): this.eltType;

        // Fisher-Yates shuffle
        for i in 0..#D.sizeAs(D.idxType) by -1 {
          var k = PCGRandomStreamPrivate_getNext_noLock(this.eltType, 0:this.eltType, i:this.eltType);
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
      }

      // TODO: can be removed when the deprecated permutation procs are removed
      proc ref permutation(ref arr: [] eltType) {

        if(!arr.isRectangular()) then
          compilerError("permutation does not support non-rectangular arrays");

        var low = arr.domain.dim(0).lowBound;
        var high = arr.domain.dim(0).highBound;

        if arr.domain.rank != 1 then
          compilerError("Permutation requires 1-D array");
        //if arr.domain.dim(0).stridable then
        //  compilerError("Permutation requires non-stridable 1-D array");

        for i in low..high {
          var j = PCGRandomStreamPrivate_getNext_noLock(arr.domain.idxType,
                                                        low, i);
          arr[i] = arr[j];
          arr[j] = i;
        }
      }

      proc ref domPermutation(d: domain): [d] d.idxType
        where is1DRectangularDomain(d)
      {
        const lo = d.lowBound,
              hi = d.highBound;

        var indices: [d] d.idxType,
            count: d.idxType = 0;

        for i in d {
          const j = d.orderToIndex(PCGRandomStreamPrivate_getNext_noLock(d.idxType, 0, count));
          count += 1;
          indices[i] = indices[j];
          indices[j] = i;
        }

        return indices;
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
      proc ref iterate(D: domain, type resultType=eltType) {
        const start = this._iterateStart(D);
        return this._iterate(D, resultType, start);
      }

      // Deprecation Note: iterate is split into two methods here so the
      // 'PCGRandomStreamInternal' class can lock around '_iterateStart'
      // and call '_iterate' separately.
      // When 'PCGRandomStreamInternal' is removed, these two methods
      // can be combined back into a single 'iterate' proc:
      //
      //  pragma "fn returns iterator"
      //  proc ref iterate(D: domain, type resultType=eltType) {
      //    const start = PCGRandomStreamPrivate_count;
      //    PCGRandomStreamPrivate_count += D.sizeAs(int);
      //    PCGRandomStreamPrivate_skipToNth_noLock(PCGRandomStreamPrivate_count-1);
      //    return PCGRandomPrivate_iterate(resultType, d, seed, start);
      //  }
      //
      // The same is true for the bounded version of iterate.

      proc ref _iterateStart(D: domain): int {
        const start = PCGRandomStreamPrivate_count;
        PCGRandomStreamPrivate_count += D.sizeAs(int);
        PCGRandomStreamPrivate_skipToNth_noLock(PCGRandomStreamPrivate_count-1);
        return start;
      }
      pragma "fn returns iterator"
      proc ref _iterate(D: domain, type resultType=eltType, start: int) {
        return PCGRandomPrivate_iterate(resultType, D, seed, start);
      }

      /*

         Returns an iterable expression for generating `D.size` random
         numbers within the range [`min`, `max`] (inclusive).

         See also the caveats in the :proc:`PCGRandomStream.getNext` accepting
         min and max arguments.

         The RNG state will be immediately advanced by `D.size`
         before the iterable expression yields any values.

         The returned iterable expression is useful in parallel contexts,
         including standalone and zippered iteration. The domain will determine
         the parallelization strategy.

         :arg D: a domain
         :arg resultType: the type of number to yield
         :return: an iterable expression yielding random `resultType` values

       */
      pragma "fn returns iterator"
      proc ref iterate(D: domain, type resultType=eltType,
                   min: resultType, max: resultType) {
        const start = this._iterateStart(D);
        return this._iterate(D, resultType, min, max, start);
      }

      pragma "fn returns iterator"
      proc ref _iterate(D: domain, type resultType=eltType,
                   min: resultType, max: resultType, start: int) {
        return PCGRandomPrivate_iterate_bounded(resultType, D, seed, start,
                                                min, max);
      }

      // Forward the leader iterator as well.
      pragma "fn returns iterator"
      @chpldoc.nodoc
      proc iterate(D: domain, type resultType=eltType, param tag)
        where tag == iterKind.leader
      {
        // Note that proc iterate() for the serial case (i.e. the one above)
        // is going to be invoked as well, so we should not be taking
        // any actions here other than the forwarding.
        const start = PCGRandomStreamPrivate_count;
        return PCGRandomPrivate_iterate(resultType, D, seed, start, tag);
      }
      pragma "fn returns iterator"
      @chpldoc.nodoc
      proc iterate(D: domain, type resultType=eltType,
                   min: resultType, max: resultType, param tag)
        where tag == iterKind.leader
      {
        // Note that proc iterate() for the serial case (i.e. the one above)
        // is going to be invoked as well, so we should not be taking
        // any actions here other than the forwarding.
        const start = PCGRandomStreamPrivate_count;
        return PCGRandomPrivate_iterate_bounded(resultType, D, seed, start,
                                                min, max, tag);
      }

      @chpldoc.nodoc
      proc serialize(writer, ref serializer) throws {
        var ser = serializer.startRecord(writer, "PCGRandomStream", 2);
        ser.writeField("eltType", eltType:string);
        ser.writeField("seed", seed);
        ser.endRecord();
      }
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
      return ldExp(x:real(64), -64);
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
      return ldExp(x:real(32), -32);
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
    inline
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
        foreach i in ranges(dim) do
          yield (i,);
      } else if dim + 2 < ranges.size {
        foreach i in ranges(dim) do
          foreach j in outer(ranges, dim+1) do
            yield (i, (...j));
      } else {
        yield 0; // 1D case is a noop
      }
    }

    //
    // PCGRandomStream iterator implementation
    //
    @chpldoc.nodoc
    iter PCGRandomPrivate_iterate(type resultType, D: domain, seed: int(64),
                                  start: int(64)) {
      var cursor = randlc_skipto(resultType, seed, start);
      for i in D do
        yield randlc(resultType, cursor);
    }

    @chpldoc.nodoc
    iter PCGRandomPrivate_iterate(type resultType, D: domain, seed: int(64),
                                  start: int(64), param tag: iterKind)
          where tag == iterKind.leader {
      for block in D.these(tag=iterKind.leader) do
        yield block;
    }

    @chpldoc.nodoc
    iter PCGRandomPrivate_iterate(type resultType, D: domain, seed: int(64),
                                 start: int(64), param tag: iterKind,
                                 followThis)
          where tag == iterKind.follower {
      use DSIUtil;
      param multiplier = 1;
      const ZD = computeZeroBasedDomain(D);
      const innerRange = followThis(ZD.rank-1);
      for outer in outer(followThis) {
        var myStart = start;
        if ZD.rank > 1 then
          myStart += multiplier * ZD.indexOrder(((...outer), innerRange.lowBound)).safeCast(int(64));
        else
          myStart += multiplier * ZD.indexOrder(innerRange.lowBound).safeCast(int(64));
        if innerRange.hasUnitStride() {
          var cursor = randlc_skipto(resultType, seed, myStart);
          for i in innerRange do
            yield randlc(resultType, cursor);
        } else {
          myStart -= innerRange.lowBound.safeCast(int(64));
          for i in innerRange {
            var cursor = randlc_skipto(resultType, seed, myStart + i.safeCast(int(64)) * multiplier);
            yield randlc(resultType, cursor);
          }
        }
      }
    }

    @chpldoc.nodoc
    iter PCGRandomPrivate_iterate_bounded(type resultType, D: domain,
                                          seed: int(64), start: int(64),
                                          min: resultType, max: resultType) {
      var cursor = randlc_skipto(resultType, seed, start);
      var count = start;
      for i in D {
        yield randlc_bounded(resultType, cursor, seed, count, min, max);
        count += 1;
      }
    }

    @chpldoc.nodoc
    iter PCGRandomPrivate_iterate_bounded(type resultType, D: domain,
                                          seed: int(64),
                                          start: int(64),
                                          min: resultType, max: resultType,
                                          param tag: iterKind)
          where tag == iterKind.leader {
      for block in D.these(tag=iterKind.leader) do
        yield block;
    }

    @chpldoc.nodoc
    iter PCGRandomPrivate_iterate_bounded(type resultType, D: domain,
                                          seed: int(64), start: int(64),
                                          min: resultType, max: resultType,
                                          param tag: iterKind, followThis)
          where tag == iterKind.follower {
      use DSIUtil;
      param multiplier = 1;
      const ZD = computeZeroBasedDomain(D);
      const innerRange = followThis(ZD.rank-1);
      for outer in outer(followThis) {
        var myStart = start;
        if ZD.rank > 1 then
          myStart += multiplier * ZD.indexOrder(((...outer), innerRange.lowBound)).safeCast(int(64));
        else
          myStart += multiplier * ZD.indexOrder(innerRange.lowBound).safeCast(int(64));
        if innerRange.hasUnitStride() {
          var cursor = randlc_skipto(resultType, seed, myStart);
          var count = myStart;
          for i in innerRange {
            yield randlc_bounded(resultType, cursor, seed, count, min, max);
            count += 1;
          }
        } else {
          myStart -= innerRange.lowBound.safeCast(int(64));
          for i in innerRange {
            var count = myStart + i.safeCast(int(64)) * multiplier;
            var cursor = randlc_skipto(resultType, seed, count);
            yield randlc_bounded(resultType, cursor, seed, count, min, max);
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
  @unstable("the 'PCGRandomLib' module is unstable and may be removed or moved in the future")
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
      pragma "fn synchronization free"
      pragma "codegen for CPU and GPU"
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
      inline proc ref srandom(seed:uint(64), inc:uint(64))
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
      inline proc ref random(inc:uint(64)):uint(32)
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
      proc ref bounded_random(inc:uint(64), bound:uint(32))
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
      proc ref bounded_random_vary_inc(inc:uint(64), bound:uint(32),
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
      proc ref advance(inc:uint(64), delta:uint(64))
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
      inline proc ref srandom(seed:uint(8), inc:uint(8))
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
      inline proc ref random(inc:uint(8)):uint(8)
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
      @chpldoc.nodoc
      inline
      proc ref mask_state() {
        state = normalize(N, state);
      }

      /* Seed the random number generator.

         :arg seed: The initial internal state.
         :arg inc: The sequence constant
       */
      inline proc ref srandom(seed:uint, inc:uint)
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
      inline proc ref random(inc:uint):uint
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
    proc pcg_getvalid_inc(initseq:uint(64)):uint(64) do return (initseq<<1) | 1;
    @chpldoc.nodoc // documented in the not param version
    inline
    proc pcg_getvalid_inc(param initseq:uint(64)) param do return (initseq<<1) | 1;


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

} // close module Random
