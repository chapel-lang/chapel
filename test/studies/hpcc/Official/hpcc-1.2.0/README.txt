DARPA/DOE HPC Challenge Benchmark
=================================
Piotr Luszczek <luszczek@cs_utk_edu>
v1.2, June 2006

footnote:[University of Tennessee Knoxville, Innovative Computing Laboratory]

Introduction
------------

This is a suite of benchmarks that measure performance of CPU, memory
subsytem and the interconnect. For details refer to the
http://icl.cs.utk.edu/hpcc/[HPC Challenge web site].

In essence, HPC Challenge consists of a number of subbenchmarks each
of which tests different aspect of the system.

If you familiar with the HPL benchmark code (see the
http://www.netlib.org/benchmark/hpl/[HPL web site]) then you can reuse
the build script file (input for `make(1)`) and the input file that
you already have for HPL. The HPC Challenge benchmark includes HPL and
uses its configuration and input files with only slight modifications.
The most important change must be done to the line that sets the
`TOPdir` variable. For HPC Challenge, the variable's value
should always be `../../..` regardless of what it was in the
HPL build script file.

Source Code Changes
-------------------

Version 1.2 Changes
~~~~~~~~~~~~~~~~~~~

1. Changes in the FFT component:
   - Added flexibility in choosing vector sizes and processor counts:
   now the code can do powers of 2, 3, and 5 both sequentially and in parallel
   tests.
   - FFTW can now run with ESTIMATE (not just MEASURE) flag: it might produce
   worse performance results but often reduces time to run the test and cuases
   less memory fragmentation.
2. Changes in the DGEMM component:
   - Added more comprehensive checking of the numerical properties of the
   test's results.
3. Changes in the RandomAccess component:
   - Removed time-bound functionality: only runs that perform complete
   computation are now possible.
   - Made the timing more accurate: main array initialization is not counted
   towards performance timing.
   - Cleaned up the code: some non-portable C language constructs have been
   removed.
   - Added new algorithms: new algorithms from Sandia based on hypercube
   network topology can now be chosen at compile time which results on much
   better performance results on many types of parallel systems.
   - Fixed potential resource leaks by adding function calls rquired by the MPI
   standard.
4. Changes in the HPL component:
   - Cleaned up reporting of numerics: more accurate printing of scaled
   residual formula.
5. Changes in the PTRANS component:
   - Added randomization of virtual process grids to measure bandwidth of the
   network more accurately.
6. Miscellaneous changes:
   - Added better support for Windows-based clusters by taking advantage of
   Win32 API.
   - Added custom memory allocator to deal with memory fragmentation on some
   systems.
   - Added better reporting of configuration options in the output file.

Compiling
---------

The first step is to create a build script file that reflects
characteristics of your machine. This file is reused by all the
components of the HPC Challenge suite. The build script file should be
created in the `hpl` directory. This directory contains instructions
(the files `README` and `INSTALL`) on how to create the build script
file. The `hpl/setup` directory contains many examples of build script
files. A recommended is to copy one of them to the `hpl` directory and
if it doesn't work then change it.

The build script file has a name that starts with `Make`.
prefix and usally ends with a suffix that identifies the target
system. For example, if the suffix chosen for the system is
`Unix`, the file should be named `Make.Unix`.

To build the benchmark executable (for the system named `Unix`) type:
`make arch=Unix`. This command should be run in the top directory (not
in the `hpl` directory). It will look in the `hpl` directory for the
configuration file and use it to build the benchmark executable.

The runtime behavior of the HPC Challenge source code may be
configured at compiled time by defining a few C preprocessor
symbols. They can be defined by adding appropriate options to
`CCNOOPT` and `CCFLAGS` make variables. The former
controls options for source code files that need to be compiled
without aggressive optimizations to ensure accurate generation of
system-specific parameters. The latter applies to the rest of the
files that need good compiler optimization for best performance. To
define a symbol `S`, the majority of compilers requires option
`-DS` to be used. Currently, the following options are
available in the HPC Challenge source code:

- `HPCC_FFT_235`: if this symbol is defined the FFTE
code (an FFT implementation) will use vector sizes and processor
counts that are not limited to powers of 2. Instead, the vector sizes
and processor counts to be used will be a product of powers of 2, 3,
and 5.
- `HPCC_FFTW_ESTIMATE`: if this symbol is defined it will
affect the way external FFTW library is called (it does not have any
effect if the FFTW library is not used). When defined, this symbol
will call the FFTW planning routine with `FFTW_ESTIMATE`
flag (instead of `FFTW_MEASURE`). This might result with worse
performance results but shorter execution time of the
benchmark. Defining this symbol may also positively affect the memory
fragmentation caused by the FFTW's planning routine.
- `HPCC_MEMALLCTR`: if this symbol is defined a custom
memory allocator will be used to alleviate effects of memory
fragmentation and allow for larger data sets to be used which may
result in obtaining better performance.
- `HPL_USE_GETPROCESSTIMES`: if this symbol is defined
then Windows-specific `GetProcessTimes()` function will be used
to measure the elapsed CPU time.
- `RA_SANDIA_NOPT`: if this symbol is defined the
HPC Challenge standard algorithm for Global RandomAccess will not be
used. Instead, instead an alternative implementation from Sandia
National Laboratory will be used.
- `RA_SANDIA_OPT2`: if this symbol is defined the
HPC Challenge standard algorithm for Global RandomAccess will not be
used. Instead, instead an alternative implementation from Sandia
National Laboratory will be used. This implementation is optimized for
number of processors being powers of two.
- `USING_FFTW`: if this symbol is defined the standard
HPC Challenge FFT implemenation (called FFTE) will not be used.
Instead, FFTW library will be called. Defining the
`USING_FFTW` symbol is not sufficient: appropriate flags have
to be added in the make script so that FFTW headers files can be found
at compile time and the FFTW libraries at link time.

Runtime Configuration
---------------------

The HPC Challenge is driven by a short input file named `hpccinf.txt`
that is almost the same as the input file for HPL (customarily called
`HPL.dat`). Refer to the file `hpl/www/tuning.html` for details about
the input file for HPL. A sample input file is included with the HPC
Challenge distribution.

The differences between HPL input file and HPC Challenge input file
can be summarized as follows:

- Lines 3 and 4 are ignored. The output always goes to the file named
  `hpccoutf.txt`.
- There are additional lines (starting with line 33) that may (but do
not have to) be used to customize the HPC Challenge benchmark.  They
are described below.

The additional lines in the HPC Challenge input file (compared to the
HPL input file) are:

- Lines 33 and 34 describe additional matrix sizes to be used for
running the PTRANS benchmark (one of the components of the HPC
Challenge benchmark).
- Lines 35 and 36 describe additional blocking factors to be used
for running PTRANS benchmark.

Just for completeness, here is the list of lines of the HPC
Challenge's input file with brief descriptions of their meaning:

- Line 1: ignored
- Line 2: ignored
- Line 3: ignored
- Line 4: ignored
- Line 5: number of matrix sizes for HPL (and PTRANS)
- Line 6: matrix sizes for HPL (and PTRANS)
- Line 7: number of blocking factors for HPL (and PTRANS)
- Line 8: blocking factors for HPL (and PTRANS)
- Line 9: type of process ordering for HPL
- Line 10: number of process grids for HPL (and PTRANS)
- Line 11: numbers of process rows of each process grid for HPL (and
PTRANS)
- Line 12: numbers of process columns of each process grid for HPL
(and PTRANS)
- Line 13: threshold value not to be exceeded by scaled residual for
HPL (and PTRANS)
- Line 14: number of panel factorization methods for HPL
- Line 15: panel factorization methods for HPL
- Line 16: number of recursive stopping criteria for HPL
- Line 17: recursive stopping criteria for HPL
- Line 18: number of recursion panel counts for HPL
- Line 19: recursion panel counts for HPL
- Line 20: number of recursive panel factorization methods for HPL
- Line 21: recursive panel factorization methods for HPL
- Line 22: number of broadcast methods for HPL
- Line 23: broadcast methods for HPL
- Line 24: number of look-ahead depths for HPL
- Line 25: look-ahead depths for HPL
- Line 26: swap methods for HPL
- Line 27: swapping threshold for HPL
- Line 28: form of L1 for HPL
- Line 29: form of U for HPL
- Line 30: value that specifies whether equilibration should be used
by HPL
- Line 31: memory alignment for HPL
- Line 32: ignored
- Line 33: number of additional problem sizes for PTRANS
- Line 34: additional problem sizes for PTRANS
- Line 35: number of additional blocking factors for PTRANS
- Line 36: additional blocking factors for PTRANS

Running
-------

The exact way to run the HPC Challenge benchmark depends on the MPI
implementation and system details.  An example command to run the
benchmark could like like this: `mpirun -np 4 hpcc`. The meaning of
the command's components is as follows:

- `mpirun` is the command that starts execution of an MPI code.
Depending on the system, it might also be `aprun`, `mpiexec`, `mprun`,
`poe`, or something appropriate for your computer.
- `-np 4` is the argument that specifies that 4 MPI processes should
be started. The number of MPI processes should be large enough to
accomodate all the process grids specified in the `hpccinf.txt` file.
- `hpcc` is the name of the HPC Challenge executable to run.

After the run, a file called `hpccoutf.txt` is created which contains
results of the benchmark. This file should be uploaded through the web
form at the HPC Challenge website.
