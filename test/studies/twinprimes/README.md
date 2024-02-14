Twin Primes SSOZ Port
=====================

This directory contains files related to a quick port I did of Jabari
Zakiya's 'twinprimes_ssoz' computation from C++ and Crystal to Chapel.

* `TPUtil.chpl`: A utility module containing the routines used to set
  up the main arguments, tables, etc.  Arguably 'twins_sieve()' and
  'nextp_init()' could also be moved here (or into another common
  module) since they do not change between versions, but since they
  feel more like they're part of the main algorithm, I left them
  as-is.

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

* `twinprimes_ssoz-forall.chpl`: This is a shared-memory variation on
  `twinprimes_ssoz.chpl that uses a 'forall' loop rather than a
  'coforall' loop.  In my simple studies, the performance difference
  between the two was negligible and the complexity of this one was
  slightly higher.  However, as thread counts vary, the performance
  characteristics could change. And this would be a good foundation
  for a distributed-memory version based on distributed domains.

* 'twinprimes_ssoz_dm_forall.chpl: This is a distributed-memory
  variant of the previous code, designed to reduce the number of
  cross-locale copies relative to `twinprimes_ssoz_dm_rr.chpl`.

* `*.good`: These are files containing expected output at various
  problem sizes for use in testing

* `*.compopts`: Used for correctness testing
* `*.execopts`: "

* `*.perf*`: Used for performance testing

* `*.numlocales`: Used for multi-locale testing
