.. default-domain:: chpl

.. module:: Random
   :synopsis: Support for pseudorandom number generation

Random
======
**Usage**

.. code-block:: chapel

   use Random;

**Submodules**

.. toctree::
   :maxdepth: 1
   :glob:

   Random/*


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


.. enum:: enum RNG { PCG = 1, NPB = 2 }

   Select between different supported RNG algorithms.
   See :mod:`PCGRandom` and :mod:`NPBRandom` for details on
   these algorithms.
   


.. data:: param defaultRNG = RNG.PCG

   The default RNG. The current default is PCG - see :mod:`PCGRandom`. 

.. type:: type RandomStream = AppendExpr.01

.. function:: proc fillRandom(arr: [], seed: int(64) = SeedGenerator.oddCurrentTime, param algorithm = defaultRNG)

   
   
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
   

.. function:: proc shuffle(arr: [], seed: int(64) = SeedGenerator.oddCurrentTime, param algorithm = RNG.PCG)

   Shuffle the elements of an array into a random order.
   
   :arg arr: a 1-D non-strided array
   :arg seed: the seed to use when shuffling. Defaults to
    `oddCurrentTime` from :type:`RandomSupport.SeedGenerator`.
   :arg algorithm: A param indicating which algorithm to use. Defaults to PCG.
   :type algorithm: :type:`RNG`
   

.. function:: proc permutation(arr: [], seed: int(64) = SeedGenerator.oddCurrentTime, param algorithm = RNG.PCG)

   Produce a random permutation, storing it in a 1-D array.
   The resulting array will include each value from low..high
   exactly once, where low and high refer to the array's domain.
   
   :arg arr: a 1-D non-strided array
   :arg seed: the seed to use when creating the permutation. Defaults to
    `oddCurrentTime` from :type:`RandomSupport.SeedGenerator`.
   :arg algorithm: A param indicating which algorithm to use. Defaults to PCG.
   :type algorithm: :type:`RNG`
   

.. function:: proc createRandomStream(type eltType, seed: int(64) = SeedGenerator.oddCurrentTime, param parSafe: bool = true, param algorithm = defaultRNG)

   
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
   

.. class:: RandomStreamInterface

   
   
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
   
   


   .. attribute:: type eltType = real(64)

      
      Specifies the type of value generated by the RandomStream.
      Not all RandomStream implementations support all types.
      

   .. attribute:: param parSafe: bool = true

      
      Indicates whether or not the RandomStream needs to be
      parallel-safe by default.  If multiple tasks interact with it in
      an uncoordinated fashion, this must be set to `true`.  If it will
      only be called from a single task, or if only one task will call
      into it at a time, setting to `false` will reduce overhead related
      to ensuring mutual exclusion.
      

   .. attribute:: const seed: int(64)

      
      The seed value for the PRNG.  There may be constraints upon
      legal values depending on the specific RNG.
      

   .. method:: proc getNext(): eltType

      
      Returns the next value in the random stream.
      
      :returns: The next value in the random stream as type :type:`eltType`.
      

   .. method:: proc skipToNth(n: integral) throws

      
      Advances/rewinds the stream to the `n`-th value in the sequence.
      The first value is with n=1.  n must be > 0, otherwise an
      IllegalArgumentError is thrown.
      
      :arg n: The position in the stream to skip to.  Must be > 0.
      :type n: `integral`
      
      :throws IllegalArgumentError: When called with non-positive `n` value.
      

   .. method:: proc getNth(n: integral): eltType throws

      
      Advance/rewind the stream to the `n`-th value and return it
      (advancing the stream by one).  n must be > 0, otherwise an
      IllegalArgumentError is thrown.  This is equivalent to
      :proc:`skipToNth()` followed by :proc:`getNext()`.
      
      :arg n: The position in the stream to skip to.  Must be > 0.
      :type n: `integral`
      
      :returns: The `n`-th value in the random stream as type :type:`eltType`.
      :throws IllegalArgumentError: When called with non-positive `n` value.
      

   .. method:: proc fillRandom(arr: [] eltType)

      
      Fill the argument array with pseudorandom values.  This method is
      identical to the standalone :proc:`fillRandom` procedure,
      except that it consumes random values from the
      :class:`RandomStreamInterface` object on which it's invoked rather
      than creating a new stream for the purpose of the call.
      
      :arg arr: The array to be filled
      :type arr: [] :type:`eltType`
      

   .. method:: proc choice(arr: [], size: ?sizeType = none, replace = true, prob: ?probType = none) throws

      
      Returns a random sample from a given 1-D array, ``arr``.
      
      :arg arr: a 1-D array with values that will be sampled from.
      :arg size: An optional integral value specifying the number of elements to
                 choose, or a domain specifying the dimensions of the
                 sampled array to be filled, otherwise a single element will be
                 chosen.
      :arg replace: an optional ``bool`` specifying whether or not to sample with
                    replacement, i.e. elements will only be chosen up to one
                    time when ``replace=false``.
      :arg prob: an optional 1-D array that contains probabilities of choosing
                 each element of ``arr``, otherwise elements will be chosen over
                 a uniform distribution. ``prob`` must have integral or real
                 element type, with no negative values and at least one non-zero
                 value. The domain must be equal to that of ``arr.domain``.
      
      :return: An element chosen from ``arr`` is ``size == 1``, or an array of
               element chosen from ``arr`` if ``size > 1`` or ``size`` is a
               domain.
      
      :throws IllegalArgumentError: if ``arr.size == 0``,
                                    if ``arr`` contains a negative value,
                                    if ``arr`` has no non-zero values.,
                                    if ``arr.domain != prob.domain``,
                                    if ``size < 1 || size.size < 1``,
                                    if ``replace=false`` and ``size > arr.size || size.size > arr.size``
      

   .. method:: proc iterate(D: domain, type resultType = eltType)

      
      
      Returns an iterable expression for generating `D.size` random
      numbers. The RNG state will be immediately advanced by `D.size`
      before the iterable expression yields any values.
      
      The returned iterable expression is useful in parallel contexts,
      including standalone and zippered iteration. The domain will determine
      the parallelization strategy.
      
      :arg D: a domain
      :arg resultType: the type of number to yield
      :return: an iterable expression yielding random `resultType` values
      
      

