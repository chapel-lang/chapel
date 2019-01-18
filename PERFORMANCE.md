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
