Sparse Matrix-Matrix Multiplication Sketches
============================================

All code in this directory is a work-in-progress and is being made
available as I work on it for the sake of providing insight and
feedback.

Main Modules
------------

* driver.chpl: This is a driver for my matrix-matrix multiplication
  routines to test that they work.  It's controlled by:

  - -sdistributed=true/false (at compile-time) to say whether to use
    Block-distributed arrays vs. local ones

  - --n to say the per-dimension problem size (square matrices only,
    currently)

  - --printMatrices : to say whether or not matrices should be printed

  - --skipDense : to say whether or not to skip the (expensive)
    computation of the dense multiplication


* MatMatMult.chpl: This is where the matrix-matrix multiplication
  routines themselves are implemented.  It can be used with

  - --countComms : to count communications for the algorithms

  - --printTimings : to print timings for the algorithms

  TODOs:
  - [x] work on reducing communication costs / maximizing locality
  - [x] make better use of local algorithm above rather than
        replicating logic
  - [x] stitch per-node results back together into a global sparse
        array
  - [x] introduce nicer interface for querying local blocks
  - [x] introduce nicer interface for stitching results together
  - [ ] relax requirement to only the first perfect square number
        of locales
  - [ ] look into further reducing comm counts; are any O(nnz)?


Helper Modules
--------------

* SpsMatUtil.chpl: This is a utility module used by the above,
  which contains routines to generate sparse matrices, print them, as
  well as purely local/trivial sparse and dense matrix multiplication
  routines.

  TODOs:
  - [x] use multicore parallelism for sparse mat-mat mult
  - [ ] consider alternative approaches to accumulating nonzeroes:
        - [ ] Engin's createIndexBuffer() helper
        - [x] index -> value map?
        - [ ] task-private accumulators
  - [ ] look at tuning operations not on critical path (e.g., matrix
        output, setup, ...)
        - [ ] assigning sparse domains may be easily optimizable
  - [ ] look at alternatives to reduction in order to reduce deep
        copies of maps and to support tree-based combining


Other Files
-----------

* *.good, *.compopts, *.execeopts *.numlocales *.notest: Files
  supporting the Chapel test system so that I can easily validate that
  things are still working as I make changes.


General TODOs
-------------

- [ ] currently I make a lot of assumptions that I should check in the
      code, like:
      - [ ] which matrices are CSC vs. CSR
      - [ ] that matrices are square
      - [ ] that target locale grids are the same
      - [ ] parent domains are always dense
      - etc.

