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
* They do not call out to ``extern`` functions (aside from those in an exempted
  set of Chapel runtime functions).
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

Benchmark examples
~~~~~~~~~~~~~~~~~~
* `Jacobi <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/jacobi/jacobi.chpl>`_ -- Jacobi example (shown above)
* `Stream <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/streamPrototype/stream.chpl>`_ -- GPU enabled version of Stream benchmark
* `SHOC Triad (Direct) <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/studies/shoc/triad.chpl>`_ -- a transliterated version of the SHOC Triad benchmark 
* `SHOC Triad (Chapeltastic) <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/studies/shoc/triadchpl.chpl>`_ -- a version of the SHOC benchmark simplified to use Chapel language features (such as promotion)
* `SHOC Sort <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/studies/shoc/sort.chpl>`_ -- SHOC radix sort benchmark

Test examples
~~~~~~~~~~~~~
* `assertOnFailToGpuize <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/assertOnFailToGpuize.chpl>`_ -- various examples of loops that are not eligible for GPU execution
* `math <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/math.chpl>`_ -- calls to various math functions within kernels that call out to the CUDA Math library
* `measureGpuCycles <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/measureGpuCycles.chpl>`_ -- measuring time within a GPU kernel
* `promotion2 <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/promotion2.chpl>`_ -- GPU kernels from promoted expressions

Examples with multiple GPUs
~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
    version as the bundled version (currently 15). Older versions may
    work; however, we only make efforts to test GPU support with this version.

* Unless using `CPU as Device mode`_, either the CUDA toolkit (for NVIDIA), or
  ROCm (for AMD) must be installed.

  * If targeting NVIDIA GPUs, we require CUDA toolkit to be version 10.x or 11.x
    (inclusive). If using version 10.x you must set
    ``CHPL_RT_NUM_THREADS_PER_LOCALE=1``. Versions as early as 7.x may work,
    although we have not tested this.

  * If targeting AMD GPUs, we require ROCm version 4.x; we suspect version 5.x
    will work as well although we have not tested so.


GPU-Related Environment Variables
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To enable GPU support set the environment variable ``CHPL_LOCALE_MODEL=gpu``
before building Chapel. Several other variables affect how Chapel generates
code for and interacts with GPUs. These variables include:

* ``CHPL_GPU`` --- may be set to ``nvidia``, ``amd``, or ``cpu``. If unset, as
  part of its build process, Chapel will attempt to automatically determine
  what type of GPU you're trying to target. Changing this variable requires
  rebuilding the Chapel runtime. For more information, see the `Vendor Portability`_
  section.
* ``CHPL_GPU_ARCH`` --- specifies GPU architecture to generate kernel code for.
  If unset and targeting NVIDIA GPUs, will default to ``sm_60``. This may also
  be set by passing the ``chpl`` compiler ``--gpu-arch=<architecture>``. For
  more information, see the `Vendor Portability`_ section.
* ``CHPL_CUDA_PATH`` --- specifies path to CUDA toolkit.  If unset, Chapel tries
  to automatically determine this path based on the location of ``nvcc``. This
  variable is unused if not targeting NVIDIA GPUs. For more information, see
  the `Vendor Portability`_ section.
* ``CHPL_ROCM_PATH`` --- specifies the path to the ROCm library. If unset, Chapel
  tries to automatically determine this path based on the location of ``hipcc``.
  This variable is unused if not targeting AMD GPUs. For more information, see the
  `Vendor Portability`_ section.
* ``CHPL_RT_NUM_GPUS_PER_LOCALE`` --- sets how many GPU sublocales to have per
  locale. If using ``CHPL_GPU=cpu``, may be set to any non negative value,
  otherwise it may be set to any value equal-to or lower than the number of GPUs
  available on each node.  If unset, defaults to the number of GPUs available on
  each node, except for when ``CHPL_GPU=cpu``, in which case it defaults to 1.
  For more information, see the `CPU as Device mode`_ section.
* ``CHPL_GPU_MEM_STRATEGY`` --- dictates how to allocate data when on a GPU
  locale.  May be set to ``unified_memory`` or ``array_on_device``. If unset,
  defaults to ``unified_memory``. Changing this variable requires rebuilding
  Chapel. For more information, see the `Memory Strategies`_ section.
* ``CHPL_GPU_BLOCK_SIZE`` --- specifies default block size when launching
  kernels. If unset, defaults to 512. This variable may also be set by passing
  the ``chpl`` compiler ``--gpu-block-size=<block_size>``. It can also be
  overwritten on a per-kernel basis by using the :proc:`~GPU.setBlockSize`
  function.
* ``CHPL_GPU_SPECIALIZATION`` --- if set, outlines bodies of 'on' statements
  and clones all functions reachable from that block. The 'on' statement is
  rewritten to call the cloned version of the outlined function when on a  GPU
  locale. This mode increases overall code size but allows the compiler to
  assume that a given function will execute on the GPU locale and optimize
  accordingly. This may also be set by passing the ``chpl`` compiler the
  ``--gpu-specialization`` flag. This is an experimental mode subject to change
  in the future.
* ``CHPL_GPU_NO_CPU_MODE_WARNING`` - this variable is relevant when using the
  `CPU as Device mode`_ and if set causes it so that uses of
  :proc:`~GPU.assertOnGpu` to not generate a warning at
  execution time. Alternatively, this behavior can be enabled by passing
  ``--gpuNoCpuModeWarning`` to your application. For more information, see the
  `CPU as Device mode`_ section.

Features
--------------------

In the following subsections we discuss various features of GPU supports.

Vendor Portability
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Chapel is able to generate code that will execute on either NVIDIA or AMD GPUs.
Chapel’s build system will automatically try and deduce what type of GPU you
have and where your installation of relevant runtime (e.g. CUDA or ROCm) are.
If the type of GPU is not detected you may set the ``CHPL_GPU`` environment
variable manually to either ``nvidia`` or ``amd``.  ``CHPL_GPU`` may also
manually be set to ``cpu`` to use `CPU as Device mode`_.

Based on the value of ``CHPL_GPU``, Chapel's build system will also attempt to
automatically detect the path to the relevant runtime. If it is not
automatically detected (or you would like to use a different installation) you
may set ``CHPL_CUDA_PATH`` and/or ``CHPL_ROCM_PATH`` explicitly.

The ``CHPL_GPU_ARCH`` environment variable can be set to control the desired
GPU architecture to compile for. The default value is ``sm_60`` for
``CHPL_GPU_CODEGEN=cuda``. You may also use the ``--gpu-arch`` compiler flag to
set GPU architecture.  If using AMD, `this table in the LLVM documentation
<https://llvm.org/docs/AMDGPUUsage.html#processors>`_ has possible architecture
values (see the "processor" column). For NVIDIA, see the `CUDA Programming
Guide
<https://docs.nvidia.com/cuda/cuda-c-programming-guide/#features-and-technical-specifications>`_.


CPU as Device Mode
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The ``CHPL_GPU`` environment variable can be set to ``cpu`` to enable many GPU
features to be used without requiring any GPUs and/or vendor SDKs to be
installed. This mode is mainly for initial development steps or quick feature
tests where access to GPUs may be limited. In this mode:

* The compiler will generate GPU kernels from order-independent loops normally.

* It will call the internal runtime API for GPU operations, so that features
  outlined under `Diagnostics and Utilities`_ will work as expected.

  * For example, :proc:`~GPU.assertOnGpu` will fail at compile time normally.
    This can allow testing if a loop is GPU-eligible.

  * It will generate a warning per-iteration at execution time.

  * The ``CHPL_GPU_NO_CPU_MODE_WARNING`` environment can be set to suppress
    these warnings. Alternatively, you can pass ``--gpuNoCpuModeWarning`` to your
    application to the same effect.

* Even though the GPU diagnostics are collected, the loop will be executed for
  correctness testing and there will not be any kernel launch

* Advanced features like ``syncThreads`` and ``createSharedArray`` will compile
  and run, but in all likelihood code that uses those features will not
  generate correct results

* The ``asyncGpuComm`` procedure will do a blocking ``memcpy`` and
  ``gpuCommWait`` will return immediately

* There will be one GPU sublocale per locale by default.
  ``CHPL_RT_NUM_GPUS_PER_LOCALE`` can be set to control how many GPU sublocales
  will be created per locale.

.. warning::

  This mode should not be used for performance studies. Application correctness
  is not guaranteed in complex cases.


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

To get a list of all GPU eligible loops at compile-time (regardless of if they
will actually run on a GPU or not) pass ``chpl`` the ``--report-gpu`` flag.

The :mod:`GPU` module contains additional utility functions. One particularly
useful function is :proc:`~GPU.assertOnGpu()`.  This function will conduct a
runtime assertion that will halt execution when not being performed on a GPU.
If :proc:`~GPU.assertOnGpu()` appears as the first line of ``forall`` or
``foreach`` loop the Chapel compiler will do a compile-time check and produce
an error if one of the aforementioned requirements is not met.  This check
might also occur if :proc:`~GPU.assertOnGpu()` is placed elsewhere in the loop
depending on the presence of control flow.

Utilities in the :mod:`MemDiagnostics` module can be used to monitor GPU memory
allocations and detect memory leaks. For example, :proc:`startVerboseMem()
<MemDiagnostics.startVerboseMem()>` and :proc:`stopVerboseMem()
<MemDiagnostics.stopVerboseMem()>` can be used to enable and disable output
from memory allocations and deallocations. GPU-based operations will be marked
in the generated output.

Multi-Locale Support
~~~~~~~~~~~~~~~~~~~~

As of Chapel 1.27.0 the GPU locale model may be used alongside communication
layers (values of ``CHPL_COMM``) other than ``none``. This enables programs to
use GPUs across nodes. We have tested multi-locale support with both NVIDIA and
AMD GPUs.

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

Device-to-Device Communication Support
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Chapel supports direct communication between interconnected GPUs. The supported
connection types are dictated by the GPU vendor; PCIe and NVLink (on NVIDIA
GPUs) are known to work.

This feature is disabled by default; it can be enabled by
setting the ``enableGpuP2P`` configuration constant using the compiler
flag ``-senableGpuP2P=true``. The following example demonstrates using
Device-to-Device communication to send data between two GPUs:

.. code-block:: chapel

  var dev1 = here.gpus[0],
      dev2 = here.gpus[1];
  on dev1 {
    var dev1Data: [0..#1024] int;
    on dev2 {
      var dev2Data: [0..#1024] int;
      dev2Data = dev1Data;
    }
  }

Notice that in this example, the GPU locales were stored into variables
``dev1`` and ``dev2``. Writing ``on here.gpus[1]`` in the second ``on`` statement
directly would not be correct, since neither GPU locale has GPU sublocales of
its own.

Memory Strategies
~~~~~~~~~~~~~~~~~

The ``CHPL_GPU_MEM_STRATEGY`` environment variable can be used to choose
between two different memory strategies. Memory strategies determine how memory
is allocated when on a GPU locale.

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

* Additional information about GPU Support can be found in the "GPU Support"
  slide decks from our `release notes
  <https://chapel-lang.org/releaseNotes.html>`_; however, be aware that
  information presented in release notes for prior releases may be out-of-date.

