[![Build Status](https://travis-ci.org/Qthreads/qthreads.svg?branch=master)](https://travis-ci.org/Qthreads/qthreads)

QTHREADS!
=========

The qthreads API is designed to make using large numbers of threads convenient
and easy. The API maps well to both MTA-style threading and PIM-style
threading, and is still quite useful in a standard SMP context. The qthreads
API also provides access to full/empty-bit (FEB) semantics, where every word of
memory can be marked either full or empty, and a thread can wait for any word
to attain either state.

The qthreads library on an SMP is essentially a library for spawning and
controlling coroutines: threads with small (4-8k) stacks. The threads are
entirely in user-space and use their locked/unlocked status as part of their
scheduling.

The library's metaphor is that there are many qthreads and several "shepherds".
Shepherds generally map to specific processors or memory regions, but this is
not an explicit part of the API. Qthreads are assigned to specific shepherds
and do not generally migrate.

The API includes utility functions for making threaded loops, sorting, and
similar operations convenient.

## Performance

On a machine with approximately 2GB of RAM, this library was able to spawn and
handle 350,000 qthreads. With some modifications (mostly in stack-size), it was
able to handle 1,000,000 qthreads. It may be able to do more, but swapping will
become an issue, and you may start to run out of address space.

This library has been tested, and runs well, on a 64-bit machine. It is
occasionally tested on 32-bit machines, and has even been tested under Cygwin.

Currently, the only real limiting factor on the number of threads is the amount
of memory and address space you have available. For more than 2^32 threads, the
thread_id value will need to be made larger (or eliminated, as it is not
*required* for correct operation by the library itself).

For information on how to use qthread or qalloc, there is A LOT of information
in the header files (qthread.h and qalloc.h), but the primary documentation is
man pages.

## FUTURELIB DOCUMENTATION (the 10-minute version)

The most important functions in futurelib that a person is going to use are
mt_loop and mt_loop_returns. The mt_loop function is for parallel iterations
that do not return values, and the mt_loop_returns function is for parallel
iterations that DO return values. The distinction is not always so obvious.

`mt_loop` is used in a format like so:
```
  mt_loop<...argtypelist..., looptype>
         (function, ...arglist..., startval, stopval, stepval);
```
The "stepval" is optional, and defaults to 1.

Essentially what you're doing is in the template setup (in the <>) you're
specifying how to handle the arguments to the parallel functions and what kind
of parallelism you want. Options for 'looptype' (i.e. the kind of parallelism)
are:

  `mt_loop_traits::Par` - fork all iterations, wait for them to finish
  `mt_loop_traits::ParNoJoin` - same as Par, but without the waiting
  `mt_loop_traits::Future` - a resource-constrained version of par, will limit
    the number of threads running at a given time
  `mt_loop_traits::FutureNoJoin` - same as Future, but without waiting for
    threads to finish

The argtypelist is a list of conceptual types defining how the arguments to the
parallel function will be handled. Use one conceptual type per argument, in the
order the arguments will be passed. Valid conceptual types are:

	Iterator - The parallel function will be called with the current loop
		iteration number passed into this argument.
	ArrayPtr - The corresponding argument is a pointer to an array, and each
		iteration will be passed the value of array[iteration]
	Ref - The corresponding argument will be passed as a reference.
	Val - The corresponding argument will be passed as a constant value
		(i.e. the same value will be passed to all iterations)

For example, doing this:
```
  for (int i = 0; i < 10; i++) {
    array[i] = i;
  }
```
Would be achieved like so:
```
  void assign(int &array_value, const int i) {
    array_value = i;
  }

  mt_loop<ArrayPtr, Iterator, mt_loop_traits::Par>
    (assign, array, 0, 0, 10);
```
The `mt_loop_returns` variant adds the specification of what to do with the
return values. The pattern is like this:
```
  mt_loop_returns<returnvaltype, ...argtypelist..., looptype>
    (retval, function, ...args..., start, stop, step);
```
The only difference is in the returnvaltype and the retval. The returnvaltype
can be either an ArrayPtr or a Collect. If it is an ArrayPtr, the loop will
behave similar to the following loop:
```
  for (int i = start; i < stop; i += step) {
    retval[i] = function(args);
  }
```
Each return value will be stored in a separate entry in the retval array. The
Collect type is more interesting, and can be either:

  `Collect<mt_loop_traits::Add>` - this sums all of the return values in
    parallel
  `Collect<mt_loop_traits::Sub>` - this subtracts all of the return values in
    parallel. Note that the answer may be nondeterministic.
  `Collect<mt_loop_traits::Mult>` - this multiplies all of the
    return values in parallel
  `Collect<mt_loop_traits::Div>` - this divides all of the
    return values in parallel. Note that the answer is nondeterministic.

For example, `Collect<mt_loop_traits::Add>` is rougly equivalent to the following loop:
```
  for (int i = start; i < stop; i += step) {
    retval += function(args);
  }
```
******************************************************

##NOTE FOR PGI USERS
pgcc needs the `-c9x` flag in order to correctly process variadic macros (which
are used in qthread.c) and the PRIuMAX format definitions (used in `qalloc.c`).
Use the CFLAGS variable to add this flag. Note that pgcc's support for the full
C90/C99 standards is lousy, so most C90/C99 features that COULD be used are
avoided.

******************************************************

##NOTE FOR IBM XL USERS
make check will probably fail with the error:

`xlc++: 1501-210 command option t contains an incorrect subargument`
`.../.libs/libqthread.so: could not read symbols: Invalid operation`

This does not mean that the library did not compile correctly, but instead
means that your libtool is probably broken (most are). The problem seems to be
that the wrapper script (testloop) is created with incorrect arguments to
xlc++. The other wrapper scripts (e.g. test1/test2/test3/testq) all have the
correct arguments, and if you modify testloop so that $relink_command uses the
`-Wl,--rpath -Wl,directory` syntax rather than the `-rpath,directory` syntax,
it would work just fine.

*******************************************************

## NOTE FOR IBM BLUEGENE/P GCC USERS
Old versions of GCC do not handle builtin atomics correctly on this platform.
The non-existence of `__sync_fetch_and_add()` cannot be reliably detected, so to
use those compilers, you probably need to configure with
`--disable-internal-spinlock`.

*******************************************************

## NOTE FOR TILERA USERS
The Tilera cache coherency protocols, as of the TileGX boards, appear to be
somewhat buggy for large multithreaded programs. And by buggy I mean they cause
kernel panics (at least, I haven't been able to demonstrate data corruption
yet). Thankfully, you can pick from several cache coherency protocols, and one
of them is more stable than the default. What I have found that seems to be
*more* stable, if not perfectly stable, is to force the cache coherency
protocol to hashed. The way you do this is with a boot argument to the Tilera
kernel. The tile-monitor command I use is this:

	`tile-monitor --net <tilera> --hvx ucache_hash=all --`

Good luck!
