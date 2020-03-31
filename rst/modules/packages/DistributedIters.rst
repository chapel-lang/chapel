.. default-domain:: chpl

.. module:: DistributedIters
   :synopsis: This module contains iterators that can be used to distribute a `forall`

DistributedIters
================
**Usage**

.. code-block:: chapel

   use DistributedIters;


This module contains iterators that can be used to distribute a `forall`
loop for a range or domain by dynamically splitting iterations between
locales.

..
  Part of a 2017 Cray summer intern project by Sean I. Geronimo Anderson
  (github.com/s-geronimoanderson) as mentored by Ben Harshbarger
  (github.com/benharsh).

.. data:: config param debugDistributedIters: bool = false

   
   Toggle debugging output.

.. data:: config param timeDistributedIters: bool = false

   
   Toggle per-locale performance timing and output.

.. data:: config const infoDistributedIters: bool = false

   
   Toggle invocation information output.

.. iterfunction:: iter distributedDynamic(c, chunkSize: int = 1, numTasks: int = 0, parDim: int = 1, localeChunkSize: int = 0, coordinated: bool = false, workerLocales = Locales)

.. iterfunction:: iter distributedGuided(c, numTasks: int = 0, parDim: int = 1, minChunkSize: int = 1, coordinated: bool = false, workerLocales = Locales)

