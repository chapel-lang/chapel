.. _readme-gpu:

.. default-domain:: chpl

GPU Programming
===============

Chapel includes preliminary work to target NVidia GPUs using CUDA. This work is
under active development and has not yet been tested under a wide variety of
environments. We have tested it on systems with NVidia Tesla P100 GPUs and CUDA
11.0 and a system with NVidia Ampere A100 GPUs with CUDA 11.6. The current
implementation will only apply to certain ``forall`` and ``foreach`` loops.

We also require ``LLVM`` to be used as Chapel's backend compiler (i.e.
``CHPL_LLVM`` must be set to ``system`` or ``bundled``). For more information
about these settings see :ref:`Optional Settings <readme-chplenv>`.

GPU support is compatible with various COMM layers to conduct communication
across nodes although we consider this work more experimental (especially for
values other than ``CHPL_COMM=none`` or ``CHPL_COMM=gasnet``).

Overview
--------

To deploy code to a GPU, put the relevant code in an ``on`` statement targeting
the GPU locale (i.e. ``here.getChild(1)``). For a given locale, we represent the
CPU(s) as sublocale 0 and GPU(s) on the system as sublocales with positive IDs.

Any arrays that are declared in the body of this ``on`` statement will be
allocated into unified memory and will be accessible on the GPU. Chapel will
generate CUDA kernels for all eligible loops in the ``on`` block. Loops are
eligible when:

* They are order-independent (e.g., `forall` or `foreach`).
* They only make use of known compiler primitives that are fast and local. Here
  "fast" means "safe to run in a signal handler" and "local" means "doesn't
  cause any network communication". In practice, this means loops not containing
  any non-inlined function calls.
* They are free of any call to a function that fails to meet the above
  criteria, accesses outer variables, or is recursive.

Any non-eligible loop will be executed on the CPU.

Setup and Compilation
---------------------

To enable GPU support set the environment variable: ``CHPL_LOCALE_MODEL=gpu``
before building Chapel. Chapel's build system will automatically try and deduce
where your installation of CUDA exists. If the build system fails to do this,
or you would like to use a different CUDA installation, you can set
``CHPL_CUDA_PATH`` environment variable to the CUDA installation root.

We also suggest setting ``CHPL_RT_NUM_THREADS_PER_LOCALE=1`` (this is necessary
if using CUDA 10).

To compile a program simply execute ``chpl`` as normal. By default the generated
code will target compute capability 6.0 (specifically by passing
``--cuda-gpu-arch=sm_60`` when invoking clang). If you would like to target a
different compute capability (necessary for example, when targeting Tesla K20
GPUs) you can pass ``--gpu-arch`` to ``chpl`` and specify a different value
there.  This may also be set using the ``CHPL_CUDA_ARCH`` environment variable.

If you would like to view debugging information you can pass ``--verbose`` to
your generated executable. This output will show the invocation of CUDA kernel
calls along with various other interactions with the GPU such as memory
operations.

Example
-------

The following example illustrates running a computation on a GPU as well as a
CPU. When ``jacobi`` is called with a GPU locale it will allocate the arrays
``A`` and ``B`` on the device memory of the GPU and we generate three GPU
kernels for the ``forall`` loops in the function.

.. code-block:: chapel

  config const nSteps = 10;
  config const n = 10;

  writeln("on GPU:");
  jacobi(here.getChild(1));
  writeln("on CPU:");
  jacobi(here);

  proc jacobi(loc) {
    on loc {
      var A, B: [0..n+1] real;

      A[0] = 1; A[n+1] = 1;
      forall i in 1..n { A[i] = i:real; }

      for step in 1..nSteps {
        forall i in 1..n { B[i] = 0.33333 * (A[i-1] + A[i] + A[i+1]); }
        forall i in 1..n { A[i] = 0.33333 * (B[i-1] + B[i] + B[i+1]); }
      }
      writeln(A);
    }
  }


