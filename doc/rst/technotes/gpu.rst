.. _readme-gpu:

.. default-domain:: chpl

GPU Programming
===============

Chapel includes preliminary work to target NVIDIA and AMD GPUs.  This work is
under active development and has not yet been tested under a wide variety of
environments although we have tested it with NVIDIA Tesla P100, V100, and RTX
A2000 GPUs; for AMD we have tested it with MI60 and MI100 GPUs.

The current implementation will generate GPU kernels for certain ``forall`` and
``foreach`` loops and launch these onto a GPU when the current locale (e.g.
``here``) is assigned to a special (sub)locale representing a GPU.

.. contents::

Overview
--------

To deploy code to a GPU, put the relevant code in an ``on`` statement targeting
a GPU sublocale (i.e. ``here.gpus[0]``).

Any arrays that are declared by tasks executing on a GPU sublocale will, by
default, be allocated into unified memory and be accessible on the GPU (see the
`Memory Strategies`_ subsection for more information about alternate memory
strategies).

Chapel will launch kernels for all eligible loops that are encountered by tasks
executing on a GPU sublocale.  Loops are eligible when:

* They are order-independent. i.e., `forall
  <../users-guide/datapar/forall.html>`_ or `foreach <foreach.html>`_ loops over
  iterators that are also order-independent.
* They only make use of known compiler primitives that are fast and local. Here
  "fast" means "safe to run in a signal handler" and "local" means "doesn't
  cause any network communication".
* They are free of any call to a function that fails to meet the above
  criteria or accesses outer variables.

Any code in an ``on`` statement for a GPU sublocale that is not within an
eligible loop will be executed on the CPU.

Examples
--------

The following example illustrates running a computation on a GPU as well as a
CPU. When ``jacobi`` is called with a GPU locale it will allocate the arrays
``A`` and ``B`` on the device memory of the GPU and we generate three GPU
kernels for the ``forall`` loops in the function.

.. code-block:: chapel

  config const nSteps = 10;
  config const n = 10;

  writeln("on GPU:");
  jacobi(here.gpus[0]);
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

For additional examples we suggest looking at some of our internal tests. Note
that these are not packaged in the Chapel release but are accessible from our
`public Github repository <https://github.com/chapel-lang/chapel>`_.

Tests of particular interest include:

Benchmark examples:
~~~~~~~~~~~~~~~~~~~
* `Jacobi <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/jacobi/jacobi.chpl>`_ -- Jacobi example (shown above)
* `Stream <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/streamPrototype/stream.chpl>`_ -- GPU enabled version of Stream benchmark
* `SHOC Triad (Direct) <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/studies/shoc/triad.chpl>`_ -- a transliterated version of the SHOC Triad benchmark 
* `SHOC Triad (Chapeltastic) <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/studies/shoc/triadchpl.chpl>`_ -- a version of the SHOC benchmark simplified to use Chapel language features (such as promotion)
* `SHOC Sort <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/studies/shoc/sort.chpl>`_ -- SHOC radix sort benchmark

Test examples:
~~~~~~~~~~~~~~~
* `assertOnFailToGpuize <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/assertOnFailToGpuize.chpl>`_ -- various examples of loops that are not eligible for GPU execution
* `math <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/math.chpl>`_ -- calls to various math functions within kernels that call out to the CUDA Math library
* `measureGpuCycles <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/measureGpuCycles.chpl>`_ -- measuring time within a GPU kernel
* `promotion2 <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/promotion2.chpl>`_ -- GPU kernels from promoted expressions

Examples with multiple GPUs:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* `multiGPU <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/multiGPU/multiGPU.chpl>`_ -- simple example using all GPUs within a locale
* `workSharing <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/multiGPU/worksharing.chpl>`_ -- stream-like example showing computation shared between GPUs and CPU
* `onAllGpusOnAllLocales <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/multiLocale/onAllGpusOnAllLocales.chpl>`_ -- simple example using all GPUs and locales
* `copyToLocaleThenToGpu <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/multiLocale/copyToLocaleThenToGpu.chpl>`_ -- stream-like example (with data initialized on Locale 0 then transferred to other locales and GPUs)

Setup
-----

Requirements
~~~~~~~~~~~~

* ``LLVM`` must be used as Chapel's backend compiler (i.e.
  ``CHPL_LLVM`` must be set to ``system`` or ``bundled``). For more information
  about these settings see :ref:`Optional Settings <readme-chplenv>`.

  * If using a ``system`` LLVM it must have been built with support for the
    relevant target of GPU you wish to generate code for (e.g.  NVPTX to target
    NVIDIA GPUs and AMDGPU to target AMD GPUs).

  * If using a system install of ``LLVM`` we expect this to be the same
    version as the bundled version (currently 14). Older versions may
    work; however, we only make efforts to test GPU support with this version.

* Either the CUDA toolkit (for NVIDIA), or ROCM (for AMD) must be installed.

  * If targeting NVIDIA GPUs, we require CUDA toolkit to be version 10.x or 11.x
    (inclusive). If using version 10.x you must set
    ``CHPL_RT_NUM_THREADS_PER_LOCALE=1``. Versions as early as 7.x may work,
    although we have not tested this.

  * If targeting AMD GPUs, we require ROCM version 4.x; we suspect version 5.x
    will work as well although we have not tested so.


GPU-Related Environment Variables
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To enable GPU support set the environment variable ``CHPL_LOCALE_MODEL=gpu``
before building Chapel.

Chapel's build system will automatically try and deduce what type of GPU you
have and where your installation of relevant runtime (e.g. CUDA or ROCM) are.
If the type of GPU is not detected you may set ``CHPL_GPU_CODEGEN`` manually to
either ``cuda`` (for NVIDIA GPUs) or ``rocm`` (for AMD GPUs). If the relevant
runtime path is not automatically detected (or you would like to use a
different installation) you may set ``CHPL_CUDA_PATH`` and/or
``CHPL_ROCM_PATH``.

``CHPL_GPU_ARCH`` environment variable can be set to control the desired GPU
architecture to compile for.  The default value is ``sm_60`` for
``CHPL_GPU_CODEGEN=cuda`` and ``gfx906`` for ``CHPL_GPU_CODEGEN=rocm``. You may
also use the ``--gpu-arch`` compiler flag to set GPU architecture. For a list
of possible values please refer to `CUDA Programming Guide
<https://docs.nvidia.com/cuda/cuda-c-programming-guide/#features-and-technical-specifications>`_
for NVIDIA or "processor" values in `this table in the LLVM documentation
<https://llvm.org/docs/AMDGPUUsage.html#processors>`_ for AMD.

GPU Support Features
--------------------

In the following subsections we discuss various features or aspects of
GPU supports that are relatively new or otherwise noteworthy.

Diagnostics and Utilities
~~~~~~~~~~~~~~~~~~~~~~~~~

The :mod:`GpuDiagnostics` module contains functions to help users count and
track kernel launches.

To count the number of kernel launches that occur in a section of code,
surround that code with calls to :proc:`~GpuDiagnostics.startGpuDiagnostics`
and :proc:`~GpuDiagnostics.stopGpuDiagnostics` and then call
:proc:`~GpuDiagnostics.getGpuDiagnostics`.  If called in a multi-locale
environment :proc:`~GpuDiagnostics.getGpuDiagnostics` will return an array of
counts of launches on a per-locale basis.

To get verbose output (indicating the location of each kernel launch) surround
the code with calls to :proc:`~GpuDiagnostics.startVerboseGpu` and
:proc:`~GpuDiagnostics.stopVerboseGpu`. This output will directed to
``stdout``.

The :mod:`GPU` module contains additional utility functions. One particularly
useful function is :proc:`~GPU.assertOnGpu()`.  This function will conduct a
runtime assertion that will halt execution when not being performed on a GPU.
If :proc:`~GPU.assertOnGpu()` appears as the first line of ``forall`` or
``foreach`` loop the Chapel compiler will do a compile-time check and produce
an error if one of the aforementioned requirements is not met.  This check
might also occur if :proc:`~GPU.assertOnGpu()` is placed elsewhere in the loop
depending on the presence of control flow.

Utilities in the :mod:`MemDiagnostics` module can be used to
monitor GPU memory allocations and detect memory leaks. For example,
:proc:`startVerboseMem() <MemDiagnostics.startVerboseMem()>` and
:proc:`stopVerboseMem() <MemDiagnostics.stopVerboseMem()>` can be used to enable
and disable output from memory allocations and deallocations. GPU-based
operations will be marked in the generated output.

Multi-Locale Support
~~~~~~~~~~~~~~~~~~~~

As of Chapel 1.27.0 the GPU locale model may be used alongside communication
layers (values of ``CHPL_COMM``) other than ``none``. This enables programs to
use GPUs across nodes.  We have only tested multi-locale support with NVIDIA
GPUs although we intend to support it with AMD GPUs in a future release.

In this mode, normal remote access is supported outside of loops that are
offloaded to the GPU; however, remote access within a kernel is not supported.
An idiomatic way to use all GPUs available across locales is with nested
``coforall`` loops like the following:

.. code-block:: chapel

  coforall loc in Locales do on loc {
    coforall gpu in here.gpus do on gpu {
      foreach {
        // ...
      }
    }
  }


For more examples see the tests under |multi_locale_dir|_ available from our `public Github repository <https://github.com/chapel-lang/chapel>`_.

.. |multi_locale_dir| replace:: ``test/gpu/native/multiLocale``
.. _multi_locale_dir: https://github.com/chapel-lang/chapel/tree/main/test/gpu/native/multiLocale

Memory Strategies
~~~~~~~~~~~~~~~~~

The ``CHPL_GPU_MEM_STRATEGY`` environment variable can be used to choose between
two different memory strategies.

The current default strategy is ``unified_memory``. The strategy applies to all
data allocated on a GPU sublocale (i.e. ``here.gpus[0]``).  Under unified memory
the underlying GPU implementation implicitly manages the migration of data to
and from the GPU as necessary.

The alternative is to set the environment variable explicitly to
``array_on_device``. This strategy stores array data directly on the device and
store other data on the host in a page-locked manner.  There are multiple
benefits to using this strategy including that it enables users to have more
explicit control over memory management, may be required for Chapel to
interoperate with various third-party communication libraries, and may be
necessary to achieve good performance. As such it may become the default memory
strategy we use in the future. Be aware though that because this strategy is
relatively new addition it hasn't been as thoroughly tested as our unified
memory based approach.

Note that host data can be accessed from within a GPU eligible loop running on
the device via a direct-memory transfer.

Debugger and Profiler Support for NVIDIA
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

As of Chapel 1.30.0 ``cuda-gdb`` and `NVIDIA NSight Compute
<https://developer.nvidia.com/nsight-compute>`_ can be used to debug and profile
GPU kernels. We have limited experience with both of these tools.  However,
compiling with ``-g`` and running the application in ``cuda-gdb`` help uncover
segmentation faults coming from GPU kernels.

Similarly, NSight Compute can be used to collect detailed performance metrics
from GPU kernels generated by the Chapel compiler. By default, using ``-g`` only
enables Chapel line numbers to be associated with performance metrics, however
it thwarts optimizations done by the backend assembler. In our experience, this
can reduce execution performance significantly, making profiling less valuable.
To avoid this, please use ``--gpu-ptxas-enforce-optimization`` while compiling
alongside ``-g``, and of course, ``--fast``.

Known Limitations
-----------------

We are aware of the following limitations and plan to work on them among other
improvements in the future.

* Intel GPUs are not supported, yet.

* For AMD GPUs:

    * Can only be used with local builds (i.e., CHPL_COMM=none)

    * Certain 64-bit math functions are unsupported. To see what does
      and doesn't work see `this test
      <https://github.com/chapel-lang/chapel/blob/release/1.30/test/gpu/native/math.chpl>`_
      and note which operations are executed when ``excludeForRocm == true``.

* Distributed arrays cannot be used within GPU kernels.

* PGAS style communication is not available within GPU kernels; that is:
  reading from or writing to a variable that is stored on a different locale
  from inside a GPU eligible loop (when executing on a GPU) is not supported.

* Runtime checks such as bounds checks and nil-dereference checks are
  automatically disabled for CHPL_LOCALE_MODEL=gpu. i.e., ``--no-checks`` is
  implied when compiling.

* The use of most ``extern`` functions within a GPU eligible loop is not
  supported (a limited set of functions used by Chapel's runtime library are
  supported).

* Associative arrays cannot be used on GPU sublocales with
  ``CHPL_GPU_MEM_STRATEGY=array_on_device``.

* If using CUDA 10, single thread per locale can be used. i.e., you have to set
  ``CHPL_RT_NUM_THREADS_PER_LOCALE=1``.

* ``CHPL_TASKS=fifo`` is not supported. Note that `fifo tasking layer
  <../usingchapel/tasks.html#chpl-tasks-fifo>`_ is the
  default in only Cygwin and NetBSD.

Using C Interoperability
~~~~~~~~~~~~~~~~~~~~~~~~
C interoperability on the host side is supported. However, GPU programming
implies C++ linkage. To handle that, the Chapel compiler compiles the ``.c``
files passed via the command line and/or ``require`` statements with ``clang -x
[cuda|hip]``. This implies that some C features may fail to compile if they are
not supported by the above ``clang`` compilation.

Further Information
-------------------
* Please refer to issues with `GPU Support label
  <https://github.com/chapel-lang/chapel/labels/area%3A%20GPU%20Support>`_ for
  other known limitations and issues.

* Alternatively, you can add the `bug label
  <https://github.com/chapel-lang/chapel/issues?q=is%3Aopen+label%3A%22area%3A+GPU+Support%22+label%3A%22type%3A+Bug%22>`_
  for known bugs only.

* Additional information about GPU Support can be found in the "Ongoing Efforts"
  slide decks of our `release notes
  <https://chapel-lang.org/releaseNotes.html>`_; however, be aware that
  information presented in release notes for prior releases may be out-of-date.

