# Submitted Chapel Computer Language Benchmarks Game Codes

Developer Note:

 *This directory is intended to reflect the code submitted to the
  benchmarks game website. If tests in this directory start failing,
  DO NOT update the tests. Instead, give them a .notest or .future
  file to disable them causing testing noise.*


This directory contains 
[Chapel versions](https://benchmarksgame-team.pages.debian.net/benchmarksgame/measurements/chapel.html)
that represent implementations of benchmark programs for the 
[Computer Language Benchmarks Game](https://benchmarksgame-team.pages.debian.net/benchmarksgame/index.html)
that have been submitted across time to the website. For Chapel versions that aren't in sync you can see the 
[Legacy Benchmarks](##legacy-benchmarks) section.

* [binarytrees.chpl #3](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/binarytrees-chapel-3.html)
    * Allocates and deallocates many, many binary trees.
* [fannkuchredux.chpl #2](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/fannkuchredux-chapel-2.html)
    * Performs many operations on small arrays.
* [fasta.chpl #3](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/fasta-chapel-3.html) |
  [fasta.chpl #5](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/fasta-chapel-5.html)
    * Generates and writes random DNA sequences.
* [knucleotide.chpl #3](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/knucleotide-chapel-3.html)
    * Count the frequencies of specific nucleotide sequences.
* [mandelbrot.chpl](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/mandelbrot-chapel-1.html) |
  [mandelbrot.chpl #3](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/mandelbrot-chapel-3.html)
    * Plots the mandelbrot set [-1.5-i,0.5+i] on a bitmap.
* [nbody.chpl #2](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/nbody-chapel-2.html)
    * Performs an n-body simulation of the Jovian planets.
* [pidigits.chpl #2](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/pidigits-chapel-2.html) |
  [pidigits.chpl #3](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/pidigits-chapel-3.html) |
  [pidigits.chpl #4](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/pidigits-chapel-4.html)
    * Computes digits of pi using GMP, if available.
* [regex-redux.chpl #2](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/regexredux-chapel-2.html) |
  [regex-redux.chpl #3](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/regexredux-chapel-3.html)
    * Performs DNA matching
* [revcomp.chpl #2](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/revcomp-chapel-2.html) |
  [revcomp.chpl #3](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/revcomp-chapel-3.html)
    * Calculate the strand to bind with a given DNA strand.
* [spectralnorm.chpl](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/spectralnorm-chapel-1.html)
    * Calculates the spectral norm of an infinite matrix.

## Legacy Benchmarks

This is an archived list for the Chapel versions that aren't synced with the website.

* meteor.chpl
    * Performs a parallel search for all solutions to a puzzle.
* meteor-fast.chpl
    * A less readable, but much faster version of meteor.
* threadring.chpl
    * Passes a token between a large number of threads.

For the latest versions of these benchmarks
and more information on their implementations, see:
[$CHPL_HOME/test/release/examples/benchmarks/shootout/](https://github.com/chapel-lang/chapel/tree/master/test/release/examples/benchmarks/shootout).
