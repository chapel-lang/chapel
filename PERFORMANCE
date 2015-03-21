========================
Chapel Performance Notes
========================

Though Chapel has been designed to ultimately yield high performance,
our focus to date has predominantly been on implementing its features
correctly and providing user-supported control of features like array
implementations, loop schedules, and architectural descriptions.  To
that end, the current compiler is lacking several key optimizations
and therefore is often not competitive with hand-coded C, Fortran,
MPI, and the like.  We are currently working on closing this gap.


Use the --fast flag!
--------------------
Once your program is correct and you are ready to do a performance
study, make sure to compile with the --fast flag.  This is a compiler
meta-flag that turns off several execution-time correctness checks
(bounds checks, NULL pointer checks, etc.) and turns on C-level
optimizations.  See the 'chpl' man page for details.


How is Chapel performance today?
--------------------------------
To characterize Chapel performance, generally speaking...

* single-locale (CHPL_COMM=none | --local) compilations perform better
  than multi-locale (CHPL_COMM!=none | --no-local) compilations;

* 1D loops/arrays perform better than multidimensional cases;

* codes with structured communication (e.g., stencils) tend not to
  perform competitively with hand-coded computations, whereas
  embarrassingly parallel and unstructured communications tend to be
  more competitive.  The reason for this is that Chapel communications
  currently tend to be very fine-grain and demand-driven unless array
  assignments are used to move chunks of data between locales.


Experimental flags for improving performance
--------------------------------------------
Our current implementation supports the following config param-based
flags, which are intended to provide a preview of performance
improvements that we are working on delivering automatically in
upcoming releases.  Both are available for use "at your own risk" in
that they are not guaranteed to maintain program correctness (detailed
after the flag's description).

* chpl -sassertNoSlicing ...

  At present, indexing into a Chapel array tends to require an extra
  multiply compared to C/Fortran, in order to support Chapel's rich
  array semantics.  More specifically, Chapel's support for striding,
  reindexing, and rank-change of arrays requires a multiplication to
  index into an array's innermost dimension in the general case; but
  we pay this cost for every array.  In contrast, C and Fortran do not
  require such multiplications.  For memory-bound programs, this
  multiplication is rarely noticeable, but for programs that are
  well-tuned for the memory hierarchy, this extra multiplication can
  have a significant performance impact.

  Work is currently underway to automatically distinguish between
  arrays that require this multiplication and those that do not in
  order to remove the overhead in the (common) cases where it is
  unnecessary.  In the meantime, one can request that this
  multiplication never be used for a given Chapel program by compiling
  with this flag.  The flag should be safe for any program that does
  not reindex a strided array or perform rank changes on an array to
  remove the innermost dimension.

* chpl -snoRefCount ...

  At present, Chapel reference counts arrays, domains, and domain maps
  in a manner that is far too conservative.  This can add unnecessary
  overhead, particularly when passing such variables between functions.  

  This flag turns off such reference counting, but also results in
  leaking all arrays, domains, and domain maps.  For programs that
  only use global arrays, domains, and domain maps, this is unlikely
  to be an issue, but for programs with local arrays, domains, and
  domain maps, the resulting memory leaks may prevent the program from
  running correctly.

  We are currently evaluating changes to the implementation and
  language definition that would reduce (or eliminate) the amount of
  reference counting required by Chapel programs without introducing
  these memory leaks.  Once these changes are complete, this flag will
  be retired.


Tracking Chapel Performance
---------------------------
We are currently working to improve Chapel performance with each
release and are making significant strides.  To track our progress
over time, refer to:

        http://chapel.sourceforge.net/perf/

From this page, you can track performance tests, either on a nightly
or release-over-release basis.  Interested users are encouraged to
submit their own performance tests back to the project for tracking on
this page.
