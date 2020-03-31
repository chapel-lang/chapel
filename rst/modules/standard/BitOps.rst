.. default-domain:: chpl

.. module:: BitOps
   :synopsis: Bitwise operations implemented using C intrinsics when possible.

BitOps
======
**Usage**

.. code-block:: chapel

   use BitOps;


Bitwise operations implemented using C intrinsics when possible.


.. function:: proc clz(x: integral)

   
   Count leading zeros in `x`.
   
   :returns: the number of 0 bits before the most significant 1 bit in `x`
   :rtype: `x.type`
   

.. function:: proc ctz(x: integral)

   
   Count trailing zeros in `x`.
   
   :returns: the number of 0 bits after the least significant 1 bit in `x`
   :rtype: `x.type`
   

.. function:: proc popcount(x: integral)

   
   Find the population count of `x`.
   
   :returns: the number of 1 bits set in `x` as `x.type`
   :rtype: `x.type`
   

.. function:: proc parity(x: integral)

   
   Find the parity of `x`.
   
   :returns: * 0 -- when an even number of bits are set in `x`
             * 1 -- when an odd number of bits are set in `x`
   :rtype: `x.type`
   

.. function:: proc rotl(x: integral, n: integral)

   
   Rotate `x` left.
   
   :arg x: integral of size `bits`
   :arg n: rotation amount, must be less than `bits`
   
   :returns: `x` rotated left by `n` bits
   :rtype: `x.type`
   

.. function:: proc rotr(x: integral, n: integral)

   
   Rotate `x` right.
   
   :arg x: integral of size `bits`
   :arg n: rotation amount, must be less than `bits`
   
   :returns: `x` rotated right by `n` bits
   :rtype: `x.type`
   

