.. _readme-gpu:

.. default-domain:: chpl

GPU Programming
===============

Chapel includes preliminary work to target NVidia GPUs by generating and
packing PTX assembly and linking against and using the CUDA driver API at
runtime. This work is under active development and has not yet been tested
under a wide variety of environments. We have tested it on systems with NVidia
Tesla P100s using CUDA 11.0 and on a system with NVidia Ampere A100 with CUDA
11.6. The current implementation will generate CUDA kernel code (PTX assembly)
for certain ``forall`` and ``foreach`` loops and these kernels will be launched
onto a GPU when the current locale (e.g. ``here``) is assigned to a special
(sub)locale representing the GPU.

For more information about what loops are eligible for GPU execution see the
`Overview`_ section.  For more information about what is supported see the
requirements and `Requirements and Limitations`_ section.  To see an example
program written in Chapel that will execute on a GPU see the code listing in
the `Examples`_ section.  For more information about specific features related
to GPU support see the subsections under `GPU Support Features`_.  Additional
information about GPU Support can be found in the "Ongoing Efforts" slide decks
of our `release notes <https://chapel-lang.org/releaseNotes.html>`_; however,
be aware that information presented in release notes for prior releases may be
out-of-date.

.. contents::

Overview
--------

To deploy code to a GPU, put the relevant code in an ``on`` statement targeting
a GPU sublocale (i.e. ``here.gpus[0]``).

Any arrays that are declared by tasks executing on a GPU sublocale will, by
default, be allocated into unified memory and be accessible on the GPU (see the
`Memory Strategies`_ subsection for more information about alternate memory
strategies).

Chapel will launch CUDA kernels for all eligible loops that are encountered by
tasks executing on a GPU sublocale.  Loops are eligible when:

* They are order-independent (e.g., ``forall`` or ``foreach``).
* They only make use of known compiler primitives that are fast and local. Here
  "fast" means "safe to run in a signal handler" and "local" means "doesn't
  cause any network communication".
* They are free of any call to a function that fails to meet the above
  criteria, accesses outer variables, or are recursive.

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

Setup and Compilation
---------------------

To enable GPU support set the environment variable ``CHPL_LOCALE_MODEL=gpu``
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
there.  This may also be set using the ``CHPL_GPU_ARCH`` environment variable.

If you would like to view debugging information you can pass ``--verbose`` to
your generated executable. This output will show the invocation of CUDA kernel
calls along with various other interactions with the GPU such as memory
operations.  You may also use the :mod:`GPUDiagnostics` module to gather
similar information.

Requirements and Limitations
----------------------------

Because of the early nature of the GPU support project there are a number of
limitations. We provide a (non exhaustive) list of these limitations in this
section; many of them will be addressed in upcoming editions.

* We currently only target NVIDIA GPUs (although we are working on adding
  support for AMD GPUs; see the section under `Prototypical AMD GPU Support`_).

* ``LLVM`` must be used as Chapel's backend compiler (i.e.
  ``CHPL_LLVM`` must be set to ``system`` or ``bundled``). For more information
  about these settings see :ref:`Optional Settings <readme-chplenv>`.

* If using a system install of ``LLVM`` we expect this to be the same
  version as the bundled version (currently 14). Older versions may
  work; however, we only make efforts to test GPU support with this version.

* ``CHPL_TASKS=qthreads`` is required for GPU support.

* PGAS style communication is not available within GPU kernels; that is:
  reading from or writing to a variable that is stored on a different locale
  from inside a GPU eligible loop (when executing on a GPU) is not supported.

* There is no user-level feature to specify GPU block size on a
  per-kernel basis. This can be set on a program wide basis at compile-time by
  passing ``--gpu-block-size=size`` to the compiler or setting it with the
  ``CHPL_GPU_BLOCK_SIZE`` environment variable.

* There is no user-level feature to allocate or access block shared memory.

* The use of most ``extern`` functions within a GPU eligible loop is not supported
  (a limited set of functions used by Chapel's runtime library are supported). 

   * Various functions within Chapel's standard modules call unsupported
     ``extern`` functions and thus are not supported in GPU eligible loops.

* Runtime checks such as bounds checks and nil-dereference checks are
  automatically disabled for CHPL_LOCALE_MODEL=gpu.

* For loops to be considered eligible for execution on a GPU they
  must fulfill the requirements discussed in the `Overview`_ section.

GPU Support Features
--------------------

In the following subsections we discuss various features or aspects of
GPU supports that are relatively new or otherwise noteworthy.

Diagnostics and Utilities
~~~~~~~~~~~~~~~~~~~~~~~~~

The :mod:`GPUDiagnostics` module contains functions to help users count and
track kernel launches.

To count the number of kernel launches that occur in a section of code,
surround that code with calls to :proc:`~GPUDiagnostics.startGPUDiagnostics`
and :proc:`~GPUDiagnostics.stopGPUDiagnostics` and then call
:proc:`~GPUDiagnostics.getGPUDiagnostics`.  If called in a multi-locale
environment :proc:`~GPUDiagnostics.getGPUDiagnostics` will return an array of
counts of launches on a per-locale basis.

To get verbose output (indicating the location of each kernel launch) surround
the code with calls to :proc:`~GPUDiagnostics.startVerboseGPU` and
:proc:`~GPUDiagnostics.stopVerboseGPU`. This output will directed to
``stdout``.

The :mod:`GPU` module contains additional utility functions. One particularly
useful function is :proc:`~GPU.assertOnGpu()`.  This function will conduct a
runtime assertion that will halt execution when not being performed on a GPU.
If :proc:`~GPU.assertOnGpu()` appears as the first line of ``forall`` or
``foreach`` loop the Chapel compiler will do a compile-time check and produce
an error if one of the aforementioned requirements is not met.  This check
might also occur if :proc:`~GPU.assertOnGpu()` is placed elsewhere in the loop
depending on the presence of control flow.

Multi-Locale Support
~~~~~~~~~~~~~~~~~~~~

As of Chapel 1.27.0 the GPU locale model may be used alongside communication
layers (values of ``CHPL_COMM``) other than ``none``. This enables programs to
use GPUs across nodes.

In this mode, normal remote access is supported outside of loops that are
offloaded to the GPU; however, remote access within a kernel is not supported.
An idiomatic way to use all GPUs available across locales is with nested
``coforall`` loops like the following:

.. code-block:: chapel

  coforall loc in Locales do on loc {
    coforall gpu in here.gpus do on gpu {
      forall {
        // ...
      }
    }
  }


For more examples see the tests under |multi_locale_dir|_ available from our `public Github repository <https://github.com/chapel-lang/chapel>`_.

.. |multi_locale_dir| replace:: ``test/gpu/native/multiLocale``
.. _multi_locale_dir: https://github.com/chapel-lang/chapel/tree/main/test/gpu/native/multiLocale

Memory Strategies
~~~~~~~~~~~~~~~~~

Currently by default Chapel uses NVIDIA's unified memory feature to store data
that is allocated on a GPU sublocale (i.e. ``here.gpus[0]``).  Under unified
memory the CUDA driver implicitly manages the migration of data to and from the
GPU as necessary.

We provide an alternate memory allocation strategy that stores array data
directly on the device and store other data on the host.  There are multiple
benefits to using this strategy including that it enables users to have more
explicit control over memory management, may be required for Chapel to
interoperate with various third-party communication libraries, and may be
necessary to achieve good performance. As such it may become the default memory
strategy we use in the future. Be aware though that because this strategy is
relatively new addition it hasn't been as thoroughly tested as our
unified-memory based approach.

To use this new strategy set the environment variable ``CHPL_GPU_MEM_STRATEGY``
to ``array_on_device``.  For more examples that work with this strategy see
the tests under |page_lock_mem_dir|_  available from our `public Github
repository <https://github.com/chapel-lang/chapel>`_.

.. |page_lock_mem_dir| replace:: ``test/gpu/native/page-locked-mem/``
.. _page_lock_mem_dir: https://github.com/chapel-lang/chapel/tree/main/test/gpu/native/page-locked-mem

Note that host data can be accessed from within a GPU eligible loop running on
the device via a direct-memory transfer.

One limitation with memory access in this mode is that we do not support direct
reads or writes from the host into individual elements of array data allocated
on the GPU (e.g.  ``use(A[i])`` or ``A[i] = ...``). Array data accessed "as a
whole" (e.g. ``writeln(A)``) will work, however.

Prototypical AMD GPU Support
----------------------------

We are working on adding AMD GPU support. A very early stage prototype
is currently available in the compiler. It works in a similar manner to
the NVidia GPU implementation: the Chapel compiler generates AMD HSA binary files and bundles
them into the resulting executable. Currently, there is no runtime implementation
that executes the generated kernels; however, ``extern C``
code can be used to invoke the HIP API and manually launch a kernel. Furthermore,
only procedures marked with ``pragma "codegen for GPU"`` are converted into
kernels. See |extern_kernel_launch|_ for an example this in action.

.. |extern_kernel_launch| replace:: ``test/gpu/native/amd/extern_kernel_launch.chpl``
.. _extern_kernel_launch: https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/amd/extern_kernel_launch.chpl

To try the AMD GPU support prototype, the process is generally the same as that
found in `Setup and Compilation`_. Instead of configuring the path to the CUDA
SDK, you will need to set the ``CHPL_ROCM_PATH`` to the location of the ROCm SDK
on your system. Furthermore, you will need to adjust the ``CHPL_GPU_CODEGEN``
environment variable to ``rocm``. The ``CHPL_GPU_ARCH`` environment variable
(or the ``--gpu-arch`` compiler flag) can be used to select the GPU architecture;
the table in `LLVM's AMD documentation <https://llvm.org/docs/AMDGPUUsage.html#processors>`_
is useful to map GPUs to their architecture.
