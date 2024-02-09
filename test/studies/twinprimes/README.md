This directory contains files related to a quick port I did of Jabari
Zakiya's 'twinprimes_ssoz' computation from C++ and Crystal to Chapel.

* `twinprimes_ssoz.chpl`: This is the original port, done primarily as
  a quick transliteration to check correctness; opportunities for
  performance and style improvements may exist.

  - some TODOs:
    - remove 'threadscnt' atomic
    - consider changing 'for' loops to 'foreach'?
    - consider replacing manual reduction with 'reduce' expression
    - I used 64-bit values exclusively where the C++ version used 32 & 64 — OK?
    - Should we use ranges more rather than explicit start/end pairs?
    - Use more nesting of iterators and procedures to avoid long arg lists?

* `twinprimes_ssoz_dm_rr.chpl`: This is a variation of the original
  port that deals parallel tasks out to locales (compute nodes) in a
  round-robin manner.  Apart from some communication getting tasks set
  up and torn down, no communication is required once the tasks are
  running.
  
  - some TODOs:
    - consider using a distributed domain to deal out the work (may reduce
      task startup communication or have benefits w.r.t. parallel overheads)

* `*.good`: These are files containing expected output at various
  problem sizes for use in testing

* `*.compopts`: Used for correctness testing
* `*.execopts`: "

* `*.perf*`: Used for performance testing

* `*.numlocales`: Used for multi-locale testing
