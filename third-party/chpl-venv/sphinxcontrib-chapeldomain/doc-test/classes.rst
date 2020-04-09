Classy?
=======

Let's hope so...

Chapel classes
--------------

.. default-domain:: chpl

.. module:: MyModule
    :synopsis: some random module...

.. class:: ChplVector

    See also :chpl:record:`ChplBigNum`. The :chpl:iter:`these` iterator can be
    used to iterate over the elements of the vector.

    .. attribute:: type eltType

        Generic type for Vector elements.

    .. type:: age = int(64)

        Type alias for tracking age.

    .. attribute:: capacity

        Integer capacity of vector. See also... random
        :chpl:attr:`ChplBigNum.myAttr`

    .. attribute:: dom

        One dimensional domain for array elements. See also
        :chpl:attr:`ChplVector.elements`...

    .. attribute:: elements

        One dimensional array that stores elements of vector. See also
        :chpl:attr:`ChplVector.dom`.

    .. attribute:: var lock$: sync bool

        Instance lock that ensures certain operations are serialized.

    .. method:: proc type currentTime: int(64)


        Generate a seed based on the current time in microseconds as
        reported by :proc:`Time.getCurrentTime`. This seed is not
        suitable for the NPB RNG since that requires an odd seed.


    .. method:: ChplVector(type eltType, cap=4, offset=0)

        Initialize new generic ChplVector of type eltType. See
        :chpl:meth:`ChplBigNum.readWriteThis`...

        :arg type eltType: generic type for new vector
        :arg int cap: capacity of vector
        :arg int offset: vector offset

    .. method:: readWriteThis(f)

        Implement generic reader/writer methods for ChplVector. blah blah
        :chpl:meth:`ChplBigNum.fromInt`. to construct a ChplVector, see
        :chpl:meth:`ChplVector.ChplVector`.

        :arg Reader,Writer f: reader or writer
        :returns: nothing
        :rtype: nil

    .. itermethod:: these() ref

        Iterate over elements in vector.

        :ytype: eltType
        :yields: Reference to element in vector.

.. record:: ChplBigNum

    See also :chpl:class:`ChplVector`...

    .. attribute:: myAttr

        whatever, some random attr for fun!

    .. method:: fromInt(value: int): ChplBigNum

        Initialize new ChplBigNum from integer. random
        :chpl:meth:`ChplVector.readWriteThis`...

        :arg int value: integer to intialize ChplBigNum with.
        :returns: new instance of ChplBigNum
        :rtype: ChplBigNum

    .. method:: inline proc read()

        Inline the reads!

        :rtype: bool
        :returns: True if read was successful.


Python classes
--------------


.. py:module:: PyContainers
    :synopsis: Container classes! Currently, just Vector.

.. py:class:: PyVector

    .. py:attribute:: eltType

        Generic type of Vector.

    .. py:attribute:: capacity

        Should be type ``int``.

    .. py:attribute:: lastIdx

        Should be type ``int``

    .. py:attribute:: dom

        Should be type ``domain(1)``

    .. py:attribute:: elements

        Should be of type ``[dom] eltType``

    .. py:method:: Vector(type eltType, cap=4, offset=0)

        Intialize new instance with given args.

        :arg type eltType: generic type for Vector elements
        :arg int cap: Capacity for vector.
        :arg int offset: Vector offset.

    .. py:method:: push(_mt: _MT, this: Vector, val: .(this, "eltType"))

    .. py:method:: low(_mt: _MT, this: Vector)

    .. py:method:: high(_mt: _MT, this: Vector)

    .. py:method:: pop(_mt: _MT, this: Vector)

    .. py:method:: top(_mt: _MT, this: Vector) ref

    .. py:method:: this(_mt: _MT, this: Vector, idx) ref

    .. py:method:: these(_mt: _MT, this: Vector) ref

    .. py:method:: size(_mt: _MT, this: Vector)

    .. py:method:: empty(_mt: _MT, this: Vector)
