Sparse Matrix-Matrix Multiplication Sketches
============================================

All code in this directory is a work-in-progress and is being made
available as I work on it for the sake of providing insight and
feedback.

Main Modules
------------

* locSpsMatMatMult.chpl: This is my start at a purely local sparse
  matrix-matrix multiplication.

* SpsMatMatUtil.chpl: This is a utility module used by the above,
  which contains most of the key code; going forward, the intent is to
  have spsMatMatMult.chpl will use it as well.

  TODOs:
  - [ ] use multicore parallelism
  - [ ] consider alternative approaches to accumulating nonzeroes:
        * Engin's createIndexBuffer() helper
        * index -> value map?
        * task-private accumulators
  - [ ] look at tuning operations not on critical path (e.g., matrix
        output, setup, ...)

* spsMatMatMult.chpl: This is my start at a Block-distributed
  SUMMA-style sparse matrix-matrix multiplication algorithm.
  TODOs:
  - [x] work on reducing communication costs / maximizing locality
  - [ ] make better use of local algorithm above rather than
        replicating logic
  - [ ] stitch per-node results back together into a global sparse
        array
  - [ ] relax requirement to only the first perfect square number
        of locales
  - [ ] look into further reducing comm counts; are any O(nnz)?


Helper Modules
--------------

* LayoutCSUtil.chpl: In working on this, I found myself wanting
  2D-specific / CSR-/CSC-specific helper routines from the CSC/R
  layout, so started implementing them in this helper module; we
  could/should consider merging these back into the main LayoutCS.chpl
  file.

* SparseBlockDistUtil.chpl: Similar, but for the sparse Block
  distribution.


Other Files
-----------

* *.good, *.compopts, *.execeopts *.numlocales *.notest: Files
  supporting the Chapel test system so that I can easily validate that
  things are still working as I make changes.
  