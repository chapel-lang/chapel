# Submitted Chapel Computer Language Benchmarks Game Codes

Developer Note:

> :warning: *This directory is intended to reflect the versions of the
  Chapel codes that currently live on the CLBG website. Please do not
  update the source code of the tests without also updating the source
  code on the benchmarks site so that the `./clbg-diff.py` script
  indicates that they are in sync.  If the tests start generating
  warnings due to changes in the language or libraries, please try to
  keep them working by updating the `.good` files or suppressing
  warnings, since warnings will not break the official entries, and
  keeping the tests running will permit us to continue gathering
  performance reults.  If that's not possible, give them a .notest or
  .future file to disable them without causing testing noise.  Please
  check with Brad if you have any questions here.*


This directory contains 
[Chapel versions](https://benchmarksgame-team.pages.debian.net/benchmarksgame/measurements/chapel.html)
that represent implementations of benchmark programs for the 
[Computer Language Benchmarks Game](https://benchmarksgame-team.pages.debian.net/benchmarksgame/index.html)
that have been submitted across time to the website. For Chapel versions that aren't in sync you can see the 
[Legacy Benchmarks](##legacy-benchmarks) section.

* [binarytrees3.chpl](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/binarytrees-chapel-3.html)
    * Allocates and deallocates many, many binary trees.
* [fannkuchredux2.chpl](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/fannkuchredux-chapel-2.html)
    * Performs many operations on small arrays.
* [fasta3.chpl](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/fasta-chapel-3.html) |
  [fasta5.chpl](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/fasta-chapel-5.html)
    * Generates and writes random DNA sequences.
* [knucleotide3.chpl](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/knucleotide-chapel-3.html)
    * Count the frequencies of specific nucleotide sequences.
* [mandelbrot.chpl](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/mandelbrot-chapel-1.html) |
  [mandelbrot3.chpl](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/mandelbrot-chapel-3.html)
    * Plots the mandelbrot set [-1.5-i,0.5+i] on a bitmap.
* [nbody2.chpl](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/nbody-chapel-2.html)
    * Performs an n-body simulation of the Jovian planets.
* [pidigits2.chpl](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/pidigits-chapel-2.html) |
  [pidigits3.chpl](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/pidigits-chapel-3.html) |
  [pidigits4.chpl](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/pidigits-chapel-4.html)
    * Computes digits of pi using GMP, if available.
* [regexredux2.chpl](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/regexredux-chapel-2.html) |
  [regexredux3.chpl](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/regexredux-chapel-3.html)
    * Performs DNA matching
* [revcomp2.chpl](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/revcomp-chapel-2.html) |
  [revcomp3.chpl](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/revcomp-chapel-3.html)
    * Calculate the strand to bind with a given DNA strand.
* [spectralnorm.chpl](https://benchmarksgame-team.pages.debian.net/benchmarksgame/program/spectralnorm-chapel-1.html)
    * Calculates the spectral norm of an infinite matrix.
