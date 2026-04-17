===============================
Debugging Memory-related Issues
===============================

Chapel provides several tools and techniques for debugging memory-related
issues in your programs. For many problems related to memory leaks or overall
memory usage, you can use the built-in memory diagnostics features.
Additionally, Chapel supports using external tools like AddressSanitizer (ASan)
and Valgrind to help identify and resolve memory issues.

-------------------------------------------
Configuration Constants for Tracking Memory
-------------------------------------------

Chapel supports a number of configuration constants related to dynamic
memory allocation for the compiler-generated executable. For full information on
these configuration constants consult :chpl:mod:`MemDiagnostics`.

A brief synopsis of these configuration constants is as follows:

  --memTrack            turn on memory tracking and enable reporting
  --memStats            call ``printMemAllocStats()`` on normal termination
  --memLeaksByType      call ``printMemAllocsByType()`` on normal termination
  --memLeaks            call ``printMemAllocs()`` on normal termination
  --memMax=int          set maximum level of allocatable memory
  --memThreshold=int    set minimum threshold for memory tracking
  --memLog=string       file to contain all memory reporting
  --memLeaksLog=string  if set, append final stats and leaks-by-type here

Using Sanitizers
----------------

Using AddressSanitizer (ASan) with Chapel can help detect many common
memory errors at runtime with minimal overhead. See :ref:`readme-sanitizers`
for details on how to set up and use sanitizers with Chapel.

Using Valgrind
--------------

Valgrind is a powerful tool for detecting memory errors at runtime. See
:ref:`best-practices-valgrind` for details on how to set up and use Valgrind
with Chapel.
