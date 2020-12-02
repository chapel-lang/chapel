.. _readme-allocators:

=================================
Using the Chapel Allocator from C
=================================

The Chapel runtime will select an allocator according to the ``CHPL_MEM``
environment variable. See :ref:`readme-chplenv` for details on how that
works.  The default is usually the system allocator (ie malloc/free as
supported by the C compiler and runtime) but some configurations use a
different allocator in order to get all of memory allocated by Chapel on
to a network registered heap.  Using the registered heap can improve
communication performance, but having two allocators can create
interoperability challenges:

 * memory allocated by one allocator cannot be freed by another
 * the Chapel heap might pre-allocate a significant amount of memory. In this
   situation, programs might run out of memory if there are too many
   allocations in the system heap.
 * memory allocated by the Chapel allocator will generally have better
   performance when communication occurs

This file describes two strategies for overcoming these challenges.

Note that both of these strategies will only allow C code to use the allocator
that the Chapel runtime is providing. In particular, while a Chapel program
might be able to allocate memory on a hardware provided memory region, these
methods will only work with system memory.

--------------------------------------------
Calling Chapel Allocation Functions Directly
--------------------------------------------

You can call Chapel allocation functions directly from an extern block or from
C code that is built with the proper -I paths for the Chapel runtime.

The command:

.. code-block:: sh

  $CHPL_HOME/util/config/compileline --includes-and-defines

should produce the necessary ``-I`` and ``-D`` options so that you can
compile a ``.c`` file to a ``.o`` file for linking with the Chapel
runtime. Note that these include paths are already handled for extern
blocks.

For example, here is a Chapel program that uses the Chapel allocator from C:

.. code-block:: chapel

  extern {
    #include "chpl-mem.h"
    static int64_t* mymalloc(void) {
      int64_t *ret = (int64_t*) chpl_malloc(1024);
      ret[0] = 5;
      return ret;
    }
    static void myfree(int64_t *x) {
      chpl_free(x);
    }
  }


  var x = mymalloc();

  writeln(x.deref());

  myfree(x);

The Chapel runtime includes ``chpl_calloc``, ``chpl_malloc``,
``chpl_memalign``, ``chpl_realloc``, ``chpl_free``,
``chpl_posix_memalign``, ``chpl_valloc``, and ``chpl_pvalloc``.  These
routines take in exactly the same arguments as the C library versions.

------------------------------
Replacing the System Allocator
------------------------------

In some cases, it may not be possible to modify C code that you would like to
allocate. At the same time, some C libraries allow their allocation functions
to be replaced. The manner in which the allocation functions can be replaced is
very platform specific. Because the GNU C library is common in the environments
that Chapel targets, we have provided a library which replaces the C library
functions with the Chapel allocator. It works by defining malloc, free, etc
because these are normally defined as weak symbols in the GNU C library.  This
feature should be considered a demonstration. It is highly platform-specific
and will not work on all platforms! For this reason, we recommend instead
calling the Chapel versions of the allocation routines (such as chpl_malloc) if
allocation interoperability is required.

The following command will compile a Chapel program while replacing the C
allocation functions:

.. code-block:: sh

  chpl myprogram.chpl -lchplmalloc

.. code-block:: chapel

  extern {
    #include <stdlib.h>
    static int64_t* mymalloc(void) {
      // If this pointer is to be freed by the Chapel free function,
      // it's much better to use chpl_malloc instead!
      int64_t *ret = (int64_t*) malloc(1024);
      ret[0] = 5;
      return ret;
    }
  }

  // SysBasic provides c_calloc and c_free to call the Chapel allocator
  // directly for C interoperability purposes
  use SysBasic;

  // Allocate using the system allocator (malloc)
  var x = mymalloc();

  writeln(x.deref());

  // Free using the Chapel allocator - c_free calls the Chapel free function
  // directly. It's named c_free because it's meant to be used for C
  // interoperability purposes.
  // This will generally cause a core dump unless:
  //   * you have configured Chapel to use the system allocator, or
  //   * you link this program with -lchplmalloc
  c_free(x);


