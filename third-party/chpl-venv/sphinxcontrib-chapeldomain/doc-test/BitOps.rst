.. generated with `chpldoc --docs-text-only modules/standard/BitOps.chpl` and
   then modified to include rst directives...

.. default-domain:: chpl

Module: BitOps
==============

.. module:: BitOps
    :synopsis: Bit Manipulation Functions
 
.. function:: inline proc clz(x: uint(?bits))

    count leading zeros

    :arg x: unsigned integer of size `bits`; `bits` can be: 8, 16, 32, 64

    :returns: the number of 0 bits before the most significant 1 bit in `x` as
              `x.type`

    :type x: uint(?bits)
    :rtype: uint(bits)


.. function:: inline proc clz(x: int(?bits))
      
    count leading zeros

    :arg x: integer of size `bits`; `bits` can be: 8, 16, 32, 64
    :type x: int(?bits)

    :returns: the number of 0 bits before the most significant 1 bit in `x` as
              `x.type`

.. function:: inline proc ctz(x)
      
    count trailing zeros

    :arg x: unsigned integer of size `bits`; `bits` can be: 8, 16, 32, 64

    :returns: the number of 0 bits after the least significant 1 bit in `x` as
              `x.type`

    :type x: uint(?bits)
   

.. function:: inline proc ctz(x)
      
    count trailing zeros

    :arg x: integer of size `bits`; `bits` can be: 8, 16, 32, 64

    :returns: the number of 0 bits after the least significant 1 bit in `x` as
              `x.type`
   
    :type x: int(?bits)


.. function:: inline proc popcount(x)
      
    population count

    :arg x: unsigned integer of size `bits`; `bits` can be: 8, 16, 32, 64

    :returns: the number of 1 bits set in `x` as `x.type`

    :type: uint(?bits)
    :rtype: uint(bits)
   

.. function:: inline proc popcount(x)
      
    population count

    :arg x: integer of size `bits`; `bits` can be: 8, 16, 32, 64

    :returns: the number of 1 bits set in `x` as `x.type`

    :type x: int(?bits)
    :rtype: int(bits)
   

.. function:: bitMatMultOr(x, y)

    :type x: uint(64)
    :type y: uint(64)
    :rtype: uint(64)

.. function:: inline proc bitRotLeft(x, shift)

.. function:: inline proc bitRotRight(x: uint(64)(64), shift)
