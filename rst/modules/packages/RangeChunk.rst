.. default-domain:: chpl

.. module:: RangeChunk
   :synopsis: The ``RangeChunk`` module assists with dividing a bounded ``range`` of any ``idxType``

RangeChunk
==========
**Usage**

.. code-block:: chapel

   use RangeChunk;


The ``RangeChunk`` module assists with dividing a bounded ``range`` of any ``idxType``
and stride into ``numChunks``. Chunks are 0-based, with the ``0`` index chunk including
``range.low`` and the ``numChunks - 1`` index chunk including ``range.high``.

Chunks are accessible in several ways:

  * as a range, through an iterator
  * as a range, through a query
  * as a tuple of 0-based orders into the range, through an iterator
  * as a tuple of 0-based orders into the range, through a query

Given that it will be uncommon for the length of a given ``range`` to be divisible by
``numChunks``, there are three different remainder policies available, expressed
by the enum ``RemElems``.

.. enum:: enum RemElems { Thru, Pack, Mod }

   
   ``RemElems`` specifies the distribution of remainder elements:
     
     - **Thru**: default policy; remainder elements will be distributed throughout
       ``numChunks`` chunks
     - **Pack**: chunks at the front will receive ``ceil(range.size / numChunks)``
       elements, then one chunk will receive what is left over; the actual number of chunks
       may be less than ``numChunks``
     - **Mod**: in ``numChunks`` chunks, every chunk that has an index less than
       ``range.size % numChunks`` will receive a remainder element
   


.. iterfunction:: iter chunks(r: range(?RT, bounded, ?S), numChunks: integral, remPol: RemElems = Thru): range(RT, bounded, S)

   
   Iterates through chunks ``0`` to ``numChunks - 1`` of range ``r``, emitting each
   as a range. The remainders will be distributed according to ``remPol``.
   

.. function:: proc chunk(r: range(?RT, bounded, ?S), numChunks: integral, idx: integral, remPol: RemElems = Thru): range(RT, bounded, S)

   
   Returns the ``idx`` chunk of range ``r`` as a range. The remainders will be
   distributed according to ``remPol``.
   

.. iterfunction:: iter chunksOrder(r: range(?RT, bounded, ?), numChunks: integral, remPol: RemElems = Thru): 2*(RT)

   
   Iterates through chunks ``0`` to ``numChunks - 1`` of range ``r``, emitting each
   as a 0-based order tuple. The remainders will be distributed according to ``remPol``.
   

.. function:: proc chunkOrder(r: range(?RT, bounded, ?), numChunks: integral, idx: integral, remPol: RemElems = Thru): 2*(RT)

   
   Returns the ``idx`` chunk of range ``r`` as a 0-based order tuple. The remainders
   will be distributed according to ``remPol``.
   

