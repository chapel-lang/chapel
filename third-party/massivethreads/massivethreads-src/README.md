
MassiveThreads - a Lightweight Thread Library for High Productivity Languages
============================================================================
Verion 0.97
September 20th 2016 

API reference
============================================================================

 * http://www.eidos.ic.i.u-tokyo.ac.jp/massivethreads/massivethreads/docs/reference/

System Requirements
============================================================================

Massivethreads is mainly developed and tested on the following environment:

CPU              : x86_64
Operating system : Linux (>=2.6)
C compiler       : GCC (>=4.4.1)

It can also work on x86, SPARC64IXfx, and XeonPhi, but not tested well.

How to build MassiveThreads library
============================================================================

MassiveThreads can be compiled and installed like many other UNIX programs using the following 3-steps:

$ ./configure --prefix=PREFIX
$ make
$ make install

You will find ${PREFIX}/lib/libmyth.so and ${PREFIX}/include/myth/myth.h, among some other things, where ${PREFIX} needs to be replaced by what you specified to --prefix.

How to use MassiveThreads library from your program
============================================================================

MassiveThreads provides three APIs to use, native intefrace, pthread-compatible interface, and TBB-like interface.

Native interface (libmyth.so)
----------------------------------------------------------------------------

The native interface provides MassiveThreads functionalities.
To use native interface, please include myth/myth.h and specify include path, library path, and shared library like this:

```
$ gcc -o foo foo.c -I${PREFIX}/include -L${PREFIX}/lib -Wl,-R${PREFIX}/lib -lmyth 
```

(-I, -L, -Wl,-R options are not necessary when you install it in the system path, such as /usr)

IMPORTANT CHANGES from 0.97:

 * libmyth.so used to wrap some system functions such as pthread and malloc.  Version 0.97 ceased doing so, as it has often been a source of troubles.  libmyth does not define any function of a name normally found in system libraries.
 * as a result, programs using malloc will experience poor scalability.
 * as of now, we recommend using popular scalable malloc libraries such as tcmalloc.
 * we are considering reviving our own malloc replacement and providing a variety of options in our future release.

Pthread-compatible interface
----------------------------------------------------------------------------

If the configure detects a linker supporting --wrap option, it builds a library libmyth-ld.so.
If the configure detects that dlopen is supported, it builds a library libmyth-dl.so.
Both libraries wrap pthread functions and replace them with similar massivethreads functions.
For example, pthread_create now behaves like myth_create.

### libmyth-ld.so

Recent GCC (GNU linker, more precisely) supports --wrap=FUNC option.  If this option is passed to linker, all references to symbol FUNC now refer to __wrap_FUNC, and all references to __real_FUNC now refer to FUNC in system libraries.  For example, if you pass --wrap=pthread_create, then a reference to pthread_create in your program now refers to __wrap_pthread_create.  libmyth-ld wraps many system functions in this way.

In order to use this version, you should pass --wrap=FUNC options for all functions wrapped by MassiveThreads.  For your convenience, you can give myth-ld.opts file using @ option of GNU linker, found under src/ directory of MassiveThreads.  myth-ld.opts lists all --wrap options wrapped by MassiveThreads.

To summarize, you can uses libmyth-ld.so as follows.

```
$ gcc -o foo @myth-ld.opts foo.c -I${PREFIX}/include -L${PREFIX}/lib -Wl,-R${PREFIX}/lib -lmyth-ld
```

where we assume you copied myth-ld.opts from the src/ directory of your MassiveThreads installation.

### libmyth-dl.so

libmyth-dl.so also wraps pthreads functions, but by using dlopen instead of linker options.  It defines pthread_create, etc. and loads the original pthread_create (in libpthread.so) using dlopen and dlsym.  Unlike libmyth-ld.so, you don't need a special linker flag.

This is more convenient and simpler to use, but it tends to cause problems when used with other libraries that also try to wrap system functions (most often malloc).  If you use such libraries (e.g., jemalloc, tcmalloc, etc.) the best bet is to use the plain libmyth.so or libmyth-ld.so.

Using environmental variables such as LD_PRELOAD, existing binaries using pthreads can be executed on MassiveThreads like this:

```
$ LD_PRELOAD=${PREFIX}/lib/libmyth-dl.so ./a.out
```

## TBB-like interface

 TBB-like interface provides an API compatible with task_group of Intel Threading Building Block.
For more information, please see docs/mtbb_api.txt .

Runtime configuration
============================================================================

Setting the number of worker threads
----------------------------------------------------------------------------

MassiveThreads creates worker thraeds based on the process affinity. Thus you can control the number of worker threads and CPU cores used by 'taskset' command.
Environmental variable MYTH_WORKER_NUM can also be used to change the number of worker threads (= # of CPU cores used).
By default, MassiveThreads uses all the CPU cores in the machine.

Setting the default stack size
----------------------------------------------------------------------------

Setting environmental variable MYTH_DEF_STKSIZE (in bytes), you can change the default stack size. By default, MassiveThreads uses 16KB as a default stack size.

Contact
----------------------------------------------------------------------------

If you have any question, please ask the following address.

   massivethreads-github@eidos.ic.i.u-tokyo.ac.jp

License
----------------------------------------------------------------------------

MassiveThreads is distributed under 2-clause BSD license. See COPYRIGHT for details.

About library names
----------------------------------------------------------------------------

"myth" is the initial codename of MassiveThreads and no more special meaning other than it.

