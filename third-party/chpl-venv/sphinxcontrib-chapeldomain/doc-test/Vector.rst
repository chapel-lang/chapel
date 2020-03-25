.. generated with `chpldoc --docs-text-only modules/standard/Containers.chpl`
   and then modified to be rst

.. default-domain:: chpl

Module: VectorFake
==================

.. module:: VectorFake
    :synopsis: Container classes! Currently, just Vector

.. class:: Vector

    .. FIXME: should ~ links display as hole value or just leaf value??? (thomasvandoren, 2015-01-22)

    `FIXME: <only copyright comment in file>` blah blah
    :chpl:proc:`~BitOps.clz`... :chpl:attr:`~MyModule.ChplVector.elements`
    blah blah :chpl:meth:`~MyModule.ChplBigNum.fromInt`. See :chpl:mod:`BitOps`.

    .. attribute:: type eltType
    .. attribute:: var capacity: int(64)
    .. attribute:: var lastIdx: int(64)
    .. attribute:: var dom: domain(1)
    .. attribute:: var elements: [.(this, "dom")] .(this, "eltType")
    .. attribute:: param MIN_SIZE: uint

    .. method:: Vector(type eltType, cap: _unknown, offset: _unknown)

    .. method:: push(val: .(this, "eltType"))

    .. method:: low

    .. method:: high

    .. method:: pop()

    .. method:: top ref

    .. method:: this(idx) ref

    .. itermethod:: these() ref

        Iterate over elements in vector.

        :ytype: eltType
        :yields: Reference to element in vector.

    .. method:: size

    .. method:: empty
