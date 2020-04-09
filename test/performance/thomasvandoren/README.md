2D matrix multiply performance studies
======================================

While working on the RBC benchmark, I noticed that the way I wanted to write a
matrix multiply was much slower than a more traditional (read non-parallel,
non-productive) implementation. These performance tests evaluate several
implementations, with the hope of highlighting the specific features that are
responsible for the performance differences.
