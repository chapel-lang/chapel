.. _readme-llvm:

============
LLVM Support
============

The Chapel compiler can be built with LLVM support in able to enable
the following features:

 1) extern block support (see :ref:`readme-extern`). This feature uses the clang
    parser. Note that it is *not* necessary to use the LLVM code generator in
    order to use extern block support.

 2) Experimental LLVM code generator. The ``--llvm`` flag activates the LLVM
    code generator. Note that by default, a Chapel compiler built with LLVM
    support still uses the C backend.

 3) Experimental LLVM communication optimizations. You can activate these
    communication optimizations with ``--llvm --llvm-wide-opt``. Some
    benchmark programs run faster with these LLVM communication optimizations.

-------------------------
Building the LLVM support
-------------------------

To build the compiler with LLVM support for extern blocks, ``--llvm`` code
generation, and support for ``--llvm-wide-opt``:

.. code-block:: sh

  source ./util/setchplenv.bash
  export CHPL_LLVM=llvm
  # or, if you have already installed compatible LLVM libraries
  # export CHPL_LLVM=system

  make # you might want to do e.g. make -j 16 for a parallel build

Note:

* If you have a built llvm in ``third-party/llvm/install``, even if you forget
  to ``export CHPL_LLVM=llvm``, the default will be to use the built llvm.  You
  can override this default by setting ``CHPL_LLVM=none``.

* the Makefile in third-party/llvm will unpack LLVM and Clang source releases
  and build them

* LLVM code generation has not been tested on all supported configurations,
  and some features (such as building a library instead of an executable)
  are not yet supported.

* You can set the environment variable ``CHPL_LLVM_DEVELOPER``
  to request a debug build of LLVM.

* The pre-built LLVM that Apple distributes for Macs does not include
  the header files needed for Chapel to use CHPL_LLVM=system.
  However, you can install a Mac Homebrew version of LLVM with, for
  example, ``brew install llvm@7`` and then set CHPL_LLVM=system.

---------------------------
Activating the LLVM support
---------------------------

To compile a program using the LLVM backend, add ``--llvm`` to the chpl command
line.

If you pass a ``--savec`` directory, the LLVM backend will emit two .bc files
in that directory:

* ``chpl__module.bc`` is the version that will be linked
* ``chpl__module-nopt.bc`` is the generated code without optimizations applied.

Passing ``--fast`` will cause LLVM optimizations to run.

The ``--ccflags`` option can control which LLVM optimizations are run, using the
same syntax as flags to clang.

Additionally, if you compile a program with ``--llvm --llvm-wide-opt
--fast``, you will allow LLVM optimizations to work with global memory.
For example, the Loop Invariant Code Motion (LICM) optimization might be
able to hoist an access of a remote variable - ie, a 'get' - out of a
loop.  This optimization has produced better performance with some
benchmarks.

Caveats:

* ``--llvm-wide-opt`` may add communication to or from a task's stack, so it
  may not function correctly for combinations of tasking and communication
  layers in which some task has a stack outside of an acceptable region for
  communication (e.g. operations on the initial 'main' thread may fail with
  ``CHPL_COMM=gasnet``, ``CHPL_GASNET_SEGMENT=fast``).

-----------------------------
How ``--llvm-wide-opt works``
-----------------------------

Communication optimization within LLVM uses the address space feature of LLVM
in order to create a conceptual global address space. In particular, instead of
generating a call to the runtime functions to 'put' or 'get', when
``--llvm-wide-opt`` is enabled, the Chapel compiler will generate a load,
store, or memcpy using an address space 100 pointer. Address space 100 pointers
represent global memory - and address space 0 pointers continue to represent
local memory. The existing LLVM optimization passes will operate normally on
these address space 100 operations. The LLVM documentation describes these
optimizations and which are normally run.

Because it may be necessary to build a global pointer or to gather information
from it - for example when constructing a global pointer from a node number and
a local address, or extracting the node number or the address - the LLVM code
generated with ``--llvm-wide-opt`` includes calls to nonexistent functions to
mark these operations:

* .gf.addr extracts an address from a global pointer
* .gf.loc extracts a locale from a global pointer
* .gf.node extracts a node number from a global pointer
* .gf.make constructs a global pointer from a locale and an address
* .gf.g2w converts a global pointer to a wide pointer
* .gf.w2g converts a wide pointer to a global pointer

These functions will be replaced with the usual runtime functions once all
global pointers are lowered into wide pointers by the global-to-wide pass.

After the usual LLVM optimization passes run, two Chapel LLVM passes run:

* aggregate-global-ops bundles together sequences of loads or sequences of
  stores on adjacent global memory locations into a single memcpy. That way,
  adjacent loads will generate a single 'get' instead of several 'get' calls.

* global-to-wide converts operations on address space 100 pointers, notably
  including load, store, memcpy, and memset operations, into calls to the
  Chapel runtime. It converts address space 100 pointers into packed pointers
  and any of the special function calls (e.g. .gf.addr to extract the local
  address portion of a global pointer) into the usual operations on a packed
  pointer. In the future, we would like to support converting address space 100
  pointers into the usual Chapel wide pointer format.

