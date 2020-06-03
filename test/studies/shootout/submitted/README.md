# Submitted Chapel Computer Language Benchmarks Game Codes

Developer Note:

 *This directory is intended to reflect the code submitted to the
  benchmarks game website. If tests in this directory start failing,
  DO NOT update the tests. Instead, give them a .notest or .future
  file to disable them causing testing noise.*

This directory contains
[Chapel versions](http://benchmarksgame.alioth.debian.org/u64q/chapel.html)
of the
[Computer Language Benchmarks Game](http://benchmarksgame.alioth.debian.org/) programs
that have been officially submitted to the Computer Language
Benchmarks Games website.

* [binarytrees.chpl](http://benchmarksgame.alioth.debian.org/u64q/program.php?test=binarytrees&lang=chapel&id=1)
    * Allocates and deallocates many, many binary trees
* [fannkuchredux.chpl](http://benchmarksgame.alioth.debian.org/u64q/program.php?test=fannkuchredux&lang=chapel&id=1)
    * Performs many operations on small arrays
* [fasta.chpl](http://benchmarksgame.alioth.debian.org/u64q/program.php?test=fasta&lang=chapel&id=1) |
  [fasta.chpl #2](http://benchmarksgame.alioth.debian.org/u64q/program.php?test=fasta&lang=chapel&id=2)
    * Generates and writes random DNA sequences
* [knucleotide.chpl](http://benchmarksgame.alioth.debian.org/u64q/program.php?test=knucleotide&lang=chapel&id=1)
    * Count the frequencies of specific nucleotide sequences
* [mandelbrot.chpl](http://benchmarksgame.alioth.debian.org/u64q/program.php?test=mandelbrot&lang=chapel&id=1)
    * Plots the mandelbrot set [-1.5-i,0.5+i] on a bitmap
* [meteor.chpl](http://benchmarksgame.alioth.debian.org/u64q/program.php?test=meteor&lang=chapel&id=1)
    * Performs a parallel search for all solutions to a puzzle
* [meteor-fast.chpl](http://benchmarksgame.alioth.debian.org/u64q/program.php?test=meteor&lang=chapel&id=2)
    * A less readable, but much faster version of meteor.
* [nbody.chpl](http://benchmarksgame.alioth.debian.org/u64q/program.php?test=nbody&lang=chapel&id=1)
    * Performs an n-body simulation of the Jovian planets
* [pidigits.chpl](http://benchmarksgame.alioth.debian.org/u64q/program.php?test=pidigits&lang=chapel&id=1) |
  [pidigits.chpl #2](http://benchmarksgame.alioth.debian.org/u64q/program.php?test=pidigits&lang=chapel&id=2)
    * Computes digits of pi using GMP, if available
* [regexdna-redux.chpl](http://benchmarksgame.alioth.debian.org/u64q/program.php?test=regexredux&lang=chapel&id=1)
    * Performs DNA matching
* [revcomp.chpl](http://benchmarksgame.alioth.debian.org/u64q/program.php?test=revcomp&lang=chapel&id=1)
    * Calculate the strand to bind with a given DNA strand
* [spectralnorm.chpl](http://benchmarksgame.alioth.debian.org/u64q/program.php?test=spectralnorm&lang=chapel&id=1)
    * Calculates the spectral norm of an infinite matrix
* [threadring.chpl](http://benchmarksgame.alioth.debian.org/u64q/program.php?test=threadring&lang=chapel&id=1)
    * Passes a token between a large number of threads

For the latest versions of these benchmarks
and more information on their implementations, see:
[$CHPL_HOME/test/release/examples/benchmarks/shootout/](https://github.com/chapel-lang/chapel/tree/master/test/release/examples/benchmarks/shootout).
