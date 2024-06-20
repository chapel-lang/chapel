.. _readme-gpu:

.. default-domain:: chpl

GPU Programming
===============

Chapel can be used to program GPUs. Currently  NVIDIA and AMD GPUs are
supported. Support for Intel GPUs is planned but not implemented, yet.

.. warning::

  This work is under active development. As such, the interface is unstable and
  expected to change.

.. contents::

Overview
--------

The Chapel compiler will generate GPU kernels for certain ``forall`` and
``foreach`` loops and launch these onto a GPU when the current locale (e.g.
``here``) is assigned to a special (sub)locale representing a GPU. To deploy
code to a GPU, put the relevant code in an ``on`` statement targeting a GPU
sublocale (i.e. ``here.gpus[0]``).

Any arrays that are declared by tasks executing on a GPU sublocale will, by
default, be accessible on the GPU (see the `Memory Strategies`_ subsection for
more information about alternate memory strategies).

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
* `SHOC Sort <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/studies/shoc/shoc-sort.chpl>`_ -- SHOC radix sort benchmark
* `asyncTaskComm <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/asynchrony/asyncTaskComm.chpl>`_ -- a synthetic benchmark to test overlap performance using multiple Chapel tasks.

Test examples
~~~~~~~~~~~~~
* `assertOnFailToGpuize <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/assertOnFailToGpuize.chpl>`_ -- various examples of loops that are not eligible for GPU execution
* `mathOps <https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/mathOps.chpl>`_ -- calls to various math functions within kernels that call out to the CUDA Math library
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

First, please make sure you are using Chapel's `preferred configuration
<../usingchapel/QUICKSTART.html#using-chapel-in-its-preferred-configuration>`_
as the starting point. Specifically, the "quickstart" configuration can not be
used with GPU support.

The following are further requirements for GPU support:

* For targeting NVIDIA or AMD GPUs, ``LLVM`` must be used as Chapel's backend
  compiler (i.e.  ``CHPL_LLVM`` must be set to ``system`` or ``bundled``).

  * Note that ``CHPL_TARGET_COMPILER`` must be ``llvm``. This is the default
    when ``CHPL_LLVM`` is set to ``system`` or ``bundled``.

* The environment variable ``CHPL_LOCALE_MODEL`` must be set to ``gpu``.

* Specifically for targeting NVIDIA GPUs:

  * CUDA toolkit version 11.x or 12.x must be installed.

  * We test with system LLVM 18. Older versions may work.

    * Note that LLVM versions older than 16 do not support CUDA 12.

  * If using ``CHPL_LLVM=system``, it must have been built with support for
    NVPTX target. You can check supported targets of your LLVM installation by
    running ``llvm-config --targets-built``.

* Specifically for targeting AMD GPUs:

  * ROCm version 4.x or <5.5 must be installed.

    * You can check the current status of ROCm version support `here
      <https://github.com/chapel-lang/chapel/issues/23480>`_.

  * For ROCm 5.x, ``CHPL_LLVM`` must be set to ``system``. Note that, ROCm
    installations come with LLVM. Setting ``CHPL_LLVM=system`` will allow you to
    use that LLVM.

* Specifically for using the `CPU-as-Device mode`_:

  * ``CHPL_GPU=cpu`` must be explicitly set. In other words, Chapel will not
    automatically fall back to this mode simply because it can't detect GPUs.


GPU-Related Environment Variables
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Several variables affect how Chapel generates code for and interacts with GPUs.
These variables include:

* ``CHPL_LOCALE_MODEL`` --- must be set to ``gpu`` to enable GPU support.
  Chapel will need to be rebuilt if this value is changed.  For more information,
  see :ref:`readme-chplenv.CHPL_LOCALE_MODEL`.

* ``CHPL_GPU`` --- may be set to ``nvidia``, ``amd``, or ``cpu``. If unset, as
  part of its build process, Chapel will attempt to automatically determine what
  type of GPU you're trying to target. Changing this variable requires
  rebuilding the Chapel runtime. For more information, see the `Vendor
  Portability`_ section.

* ``CHPL_GPU_ARCH`` --- specifies GPU architecture to generate kernel code for.
  This must be set while targeting AMD GPUs.  If unset and targeting NVIDIA
  GPUs, will default to ``sm_60``. This may also be set by passing the ``chpl``
  compiler ``--gpu-arch=<architecture>``. For more information, see the `Vendor
  Portability`_ section.

* ``CHPL_CUDA_PATH`` --- specifies path to CUDA toolkit.  If unset, Chapel tries
  to automatically determine this path based on the location of ``nvcc``. This
  variable is unused if not targeting NVIDIA GPUs. For more information, see
  the `Vendor Portability`_ section.

* ``CHPL_ROCM_PATH`` --- specifies the path to the ROCm library. If unset,
  Chapel tries to automatically determine this path based on the location of
  ``hipcc``.  This variable is unused if not targeting AMD GPUs. For more
  information, see the `Vendor Portability`_ section.

* ``CHPL_RT_NUM_GPUS_PER_LOCALE`` --- sets how many GPU sublocales to have per
  locale. If using ``CHPL_GPU=cpu``, may be set to any non negative value,
  otherwise it may be set to any value equal to or lower than the number of GPUs
  available on each node.  If unset, defaults to the number of GPUs available on
  each node, except for when ``CHPL_GPU=cpu``, in which case it defaults to 1.
  For more information, see the `CPU-as-Device mode`_ section.

* ``CHPL_GPU_MEM_STRATEGY`` --- dictates how to allocate data when on a GPU
  locale.  May be set to ``unified_memory`` or ``array_on_device``. If unset,
  defaults to ``array_on_device``. Changing this variable requires rebuilding
  Chapel. For more information, see the `Memory Strategies`_ section.

* ``CHPL_GPU_BLOCK_SIZE`` --- specifies the default block size when launching
  kernels. If unset, defaults to 512. This variable may also be set by passing
  the ``chpl`` compiler ``--gpu-block-size=<block_size>``. It can also be
  overwritten on a per-kernel basis by using the ``@gpu.blockSize(n)`` loop
  attribute (described in more detail in `GPU-Related Attributes`_).

* ``CHPL_GPU_SPECIALIZATION`` --- if set, outlines bodies of 'on' statements
  and clones all functions reachable from that block. The 'on' statement is
  rewritten to call the cloned version of the outlined function when on a  GPU
  locale. This mode increases overall code size but allows the compiler to
  assume that a given function will execute on the GPU locale and optimize
  accordingly. This may also be set by passing the ``chpl`` compiler the
  ``--gpu-specialization`` flag. This is an experimental mode subject to change
  in the future.

* ``CHPL_GPU_NO_CPU_MODE_WARNING`` - this variable is relevant when using the
  `CPU-as-Device mode`_ and if set, uses of
  the ``@assertOnGpu`` attribute do not generate warnings at execution time.
  Alternatively, this behavior can be enabled by passing
  ``--gpuNoCpuModeWarning`` to your application. For more information, see the
  `CPU-as-Device mode`_ section.

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
manually be set to ``cpu`` to use `CPU-as-Device mode`_.

Based on the value of ``CHPL_GPU``, Chapel's build system will also attempt to
automatically detect the path to the relevant runtime. If it is not
automatically detected (or you would like to use a different installation) you
may set ``CHPL_CUDA_PATH`` and/or ``CHPL_ROCM_PATH`` explicitly.

The ``CHPL_GPU_ARCH`` environment variable can be set to control the desired GPU
architecture to compile for. The default value is ``sm_60`` for
``CHPL_GPU=nvidia``. You may also use the ``--gpu-arch`` compiler flag to
set GPU architecture.  If using AMD, this variable must be set. `This table in
the ROCm documentation
<https://rocm.docs.amd.com/en/latest/reference/gpu-arch-specs.html>`_
has possible architecture values (see the "LLVM target name" column). For NVIDIA, see
the `CUDA Compute Capability <https://developer.nvidia.com/cuda-gpus>`_ table.

For NVIDIA, the ``CHPL_GPU_ARCH`` variable can also be set to a comma-separated
list. This causes the Chapel compiler to generate device code for each of the
given compute capabilities, and to bundle the different versions in a single
executable. When the program is executed, the compute capability best suited
for the available GPU will be loaded by the CUDA runtime. Support for this
feature for AMD GPUs is planned, but not currently available.

GPU-Related Attributes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Chapel's GPU support makes use of attributes (see `Attributes in Chapel <./attributes.html>`_)
to control various aspects of how code is compiled or executed on the GPU.
Specifically, the two GPU-specific Chapel attributes are ``@assertOnGpu``
(described in `Diagnostics and Utilities`_) and ``@gpu.blockSize``. Because
Chapel's GPU support primarily works by converting eligible loops into GPU
kernels, GPU-specific attributes primarily apply to loops. The following
example demonstrates these attributes:

.. code-block:: chapel

   config const myBlockSize = 128;

   on here.gpus[0] {
     @assertOnGpu
     @gpu.blockSize(myBlockSize)
     foreach i in 1..1024 { /* ... your code here ... */ }
   }

In the above code, ``@assertOnGpu`` ensures that the ``foreach`` loop is
GPU-eligible, and ``@gpu.blockSize`` sets the block size for the kernel to
``myBlockSize``.

In addition to applying GPU attributes to loops, Chapel provides (experimental)
support for applying them to variable declarations. This is intended for use
with variables whose initializers contain GPU-bound code. The following example
demonstrates initializing an array ``A`` from a ``foreach`` expression:

.. code-block:: chapel

   @assertOnGpu
   @gpu.blockSize(128)
   var A = foreach i in 1..1024 do i * i;

CPU-as-Device Mode
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The ``CHPL_GPU`` environment variable can be set to ``cpu`` to enable many GPU
features to be used without requiring any GPUs and/or vendor SDKs to be
installed. This mode is mainly for initial development steps or quick feature
tests where access to GPUs may be limited. In this mode:

* The compiler will generate GPU kernels from eligible loops normally.

* It will call the internal runtime API for GPU operations, so that features
  outlined under `Diagnostics and Utilities`_ will work as expected.

  * For example, the ``@assertOnGpu`` attribute will fail at compile time for
    ineligible loops normally.  This can allow testing if a loop is
    GPU-eligible. It will generate a warning per-iteration at execution time.
    The ``CHPL_GPU_NO_CPU_MODE_WARNING`` environment can be set to suppress
    these warnings. Alternatively, you can pass ``--gpuNoCpuModeWarning`` to
    your application to the same effect.

  * Note that data movements between device and host will not be captured by the
    :mod:`GpuDiagnostics` module in this mode.

* Even though the kernel launches will be registered by GPU diagnostics, the
  loop will be executed for correctness testing and there will not be any actual
  kernel launch even if you have a GPU available.

* Advanced features like ``syncThreads`` and ``createSharedArray`` will compile
  and run, but in all likelihood code that uses those features will not
  generate correct results.

* The ``asyncGpuComm`` procedure will do a blocking ``memcpy`` and
  ``gpuCommWait`` will return immediately.

* There will be one GPU sublocale per locale by default.
  ``CHPL_RT_NUM_GPUS_PER_LOCALE`` can be set to control how many GPU sublocales
  will be created per locale.

* Inner loops in loop nests that consist of GPU-eligible loops will be reported
  as kernel launch whereas in regular GPU modes, such loops will not be launched
  as a kernel as the execution will already be on the GPU. This may cause
  increased kernel launches reported by the :mod:`GpuDiagnostics` utilities with
  loop nests and multidimensional loops.

.. warning::

  This mode should not be used for performance studies. Application correctness
  is not guaranteed in complex cases.


Diagnostics and Utilities
~~~~~~~~~~~~~~~~~~~~~~~~~

The :mod:`GpuDiagnostics` module contains functions to help users count and
track kernel launches and data movement between host and device(s).

To count the number of kernel launches that occur in a section of code,
surround that code with calls to :proc:`~GpuDiagnostics.startGpuDiagnostics`
and :proc:`~GpuDiagnostics.stopGpuDiagnostics` and then call
:proc:`~GpuDiagnostics.getGpuDiagnostics`.  If called in a multi-locale
environment :proc:`~GpuDiagnostics.getGpuDiagnostics` will return an array of
counts of launches on a per-locale basis.

To get verbose output (indicating the location of each kernel launch) surround
the code with calls to :proc:`~GpuDiagnostics.startVerboseGpu` and
:proc:`~GpuDiagnostics.stopVerboseGpu`. This output will be directed to
``stdout``.

To get a list of all GPU eligible loops at compile-time (regardless of if they
will actually run on a GPU or not) pass ``chpl`` the ``--report-gpu`` flag.

Since not all Chapel loops are eligible for conversion into GPU kernels, it
is helpful to be able to ensure that a particular loop is being executed
on the GPU. This can be achieved by marking the loop with the ``@assertOnGpu``
attribute. When a ``forall`` or ``foreach`` loop is marked with this attribute,
the compiler will perform a compile-time check and produce an error if one of
the aforementioned requirements is not met. Loops marked with the
``@assertOnGpu`` attribute will also conduct a runtime assertion that will halt
execution when not being performed on a GPU. This can happen when the loop
is eligible for GPU execution, but is being executed outside of a GPU locale.
The :mod:`GPU` module contains additional utility functions.

Utilities in the :mod:`MemDiagnostics` module can be used to monitor GPU memory
allocations and detect memory leaks. For example, :proc:`startVerboseMem()
<MemDiagnostics.startVerboseMem()>` and :proc:`stopVerboseMem()
<MemDiagnostics.stopVerboseMem()>` can be used to enable and disable output
from memory allocations and deallocations. GPU-based operations will be marked
in the generated output.

Multi-Locale Support
~~~~~~~~~~~~~~~~~~~~

The GPU locale model may be used alongside communication layers (values of
``CHPL_COMM`` other than ``none``). This enables programs to use GPUs across
nodes.

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


For more examples see the tests under |multi_locale_dir|_ available from our
`public Github repository <https://github.com/chapel-lang/chapel>`_.

.. |multi_locale_dir| replace:: ``test/gpu/native/multiLocale``
.. _multi_locale_dir: https://github.com/chapel-lang/chapel/tree/main/test/gpu/native/multiLocale

Reductions and Scans
~~~~~~~~~~~~~~~~~~~~
The :mod:`GPU` module has standalone functions for basic reductions (e.g.
:proc:`~GPU.gpuSumReduce`) and scans (e.g.  :proc:`~GPU.gpuScan`). We expect
these functions to be deprecated in favor of ``reduce`` and ``scan`` expressions
in a future release.

As of Chapel 2.1, ``+``, ``min`` and ``max`` reductions are supported via
``reduce`` expressions and intents. We are working towards expanding this to
other kinds of reductions and ``scan`` expressions and deprecating the mentioned
functions in the :mod:`GPU` module.

Device-to-Device Communication Support
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Chapel supports direct communication between interconnected GPUs. The supported
connection types are dictated by the GPU vendor.

For NVIDIA
^^^^^^^^^^
PCIe and NVLink (on NVIDIA GPUs) are known to work.

This feature is disabled by default; it can be enabled by setting the
``enableGpuP2P`` configuration constant using the compiler flag
``-senableGpuP2P=true``. Note that data movement does not require any code
changes. The following example demonstrates using device-to-device communication
to send data between two GPUs:

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

For AMD
^^^^^^^
The ROCm versions we currently support (<=5.4) do not support enabling
peer-to-peer communication in the way above. However, for optimum bandwidth
between two devices ``export HSA_ENABLE_SDMA=0`` can be used. This will enable
using multiple Infinity Fabric links between GPUs/GCDs. However, note that it
will do that by using kernels to move data. These kernel launches will be
internal to ROCm and will not be captured by Chapel's GPU diagnostic utilities.
However, the impacts can be observable when an application needs to overlap
computation and communication, as what the user thinks as "communication" will
also involve kernel execution. More information about this can be found in `in
this article <https://gpuopen.com/learn/amd-lab-notes/amd-lab-notes-gpu-aware-mpi-readme/#gpu-to-gpu-communication-options>`_.

Memory Strategies
~~~~~~~~~~~~~~~~~

The ``CHPL_GPU_MEM_STRATEGY`` environment variable can be used to choose
between two different memory strategies. Memory strategies determine how memory
is allocated when on a GPU locale.

The current default strategy is ``array_on_device``. This strategy stores array
data directly on the device and store other data on the host in a page-locked
manner.  There are multiple benefits to using this strategy including that it
will result in optimal communication performance between the host and the
device and may be required for Chapel to interoperate with various third-party
communication libraries.

The alternative is to set the environment variable explicitly to
``unified_memory``. The strategy applies to all dynamically-allocated data on a
GPU sublocale (i.e. ``here.gpus[0]``).  Under unified memory the underlying GPU
implementation implicitly manages the migration of data to and from the GPU as
necessary. Note that host data can be accessed from within a GPU eligible loop
running on the device via a direct-memory transfer.

Debugger and Profiler Support for NVIDIA
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

``cuda-gdb`` and `NVIDIA NSight Compute
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

Examining Generated Assembly
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

While analyzing performance, users might also wish to look at the assembly
``chpl`` generates for GPU kernels. To do this pass ``chpl`` ``--savec
<dirName>`` (replacing ``<dirname>`` with a directory name to contain the
generate assembly). The Chapel compiler will emit a file ``chpl__gpu.s``, which
contains AMD GCN or NVIDIA PTX instructions as appropriate.

In the generated assembly, kernels are named
``chpl_gpu_kernel_<fileName>_line_<num>_`` (with ``filename`` replaced with the
file containing the outlined loop and ``num`` as the line number of the loop
header. For example, a kernel on line 3 of ``chpl.foo`` will be named
``chpl_gpu_kernel_foo_line_3_``). The kernel name may have a number as a suffix
if the same line of code required multiple kernels to be generated. Typically,
this can happen if the loop in question was in a generic function with multiple
instantiations.

Chapel Tasks and GPU Execution
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Chapel runtime will use a GPU stream per-task, per-device by default. While
individual streams are synchronized with the host after each operation (e.g.,
whole array operations and kernel launches will return only when the operation
is completed), this allows efficiently oversubscribing GPUs by running multiple
tasks on them to gain more performance by allowing the device runtime to overlap
data movement with computation.

* This behavior is disabled for ``CHPL_GPU_MEM_STRATEGY=unified_memory``.

* It can also be disabled for the default
  ``CHPL_GPU_MEM_STRATEGY=array_on_device``, by running the application with
  ``--gpuUseStreamPerTask=false``.

See the `asyncTaskComm
<https://github.com/chapel-lang/chapel/blob/main/test/gpu/native/asynchrony/asyncTaskComm.chpl>`_
benchmark for a full example of a pattern that benefits from oversubscribing
GPUs.

Known Limitations
-----------------

We are aware of the following limitations and plan to work on them among other
improvements in the future.

* Intel GPUs are not supported, yet.

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

* It's not currently possible to compile for multiple AMD GPU architectures
  at the same time.

* Associative arrays cannot be used on GPU sublocales with
  ``CHPL_GPU_MEM_STRATEGY=array_on_device``.

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

Performance Tips
----------------
* If measuring performance, and using an NVIDIA GPU, please be aware that GPU
  initialization may incur a 1-3 second startup cost per GPU due to ECC
  scrubbing.  This initialization occurs when starting a gpu-enabled Chapel
  program when NVIDIA's kernel mode driver is not already loaded and running.
  If you are using Linux and not running an X server on the target GPU, then
  you may wish to install `NVIDIA's `driver persistence daemon
  <https://docs.nvidia.com/deploy/driver-persistence/index.html#persistence-daemon>`_
  to alleviate this issue.

Tested Configurations
---------------------

We have experience with the following hardware and software versions. The ones
marked with * are covered in our nightly testing configuration.

* NVIDIA

  * Hardware: RTX A2000, P100*, V100*, A100* and H100

  * Software: CUDA 11.3*, 11.6, 11.8*, 12.0, 12.2*, 12.4

* AMD

  * Hardware: MI60*, MI100 and MI250X*

  * Software:ROCm 4.2*, 4.4, 5.4*


GPU Support on Windows Subsystem for Linux
------------------------------------------------

NVIDIA GPUs can be used on Windows through through WSL. To enable GPU support on
WSL we require the CUDA Toolkit to be installed in the WSL environment and the
NVIDIA driver to be installed on the Windows host. See the `NVIDIA documentation
<https://docs.nvidia.com/cuda/wsl-user-guide/index.html#getting-started-with-cuda-on-wsl-2>`_
for more information on setting up CUDA on WSL.
See `Using Chapel on WSL <../platforms/windows.html#using-chapel-on-wsl>`_
for more information on using Chapel with WSL.

  .. note::

    This configuration is not currently tested nightly. Please report any issues
    you encounter when using Chapel on WSL by `filing a bug report
    <https://github.com/chapel-lang/chapel/issues/new>`_

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
