.. _readme-gpu:

.. default-domain:: chpl

=============================
Support for GPU targeted code
=============================

Chapel includes preliminary work to target Nvidia GPUs using CUDA. This work is
under active development and has not yet been tested under a wide variety of
environments. We have tested it on systems with Nvidia Tesla P100 GPUs and CUDA
11.0 and a system with Nvidia Tesla K20 GPUs with CUDA 10.2. The current
implementation does not support multiple GPUs on one node and will only apply
to certain `forall` loops.

--------
Overview
--------

To deploy code to a GPU embed the relevant code in an 'on' statement targeting
the GPU locale (i.e. `here.getChild(1)`).

Any arrays that are declared in the body of this `on` statement will be
allocated on the GPU  Chapel will generate CUDA kernels for `forall` statements
in the `on` block that are eligible.  Loops are eligible if they only make use
of known compiler primitives that are fast and local are eligible for GPU code
generation. Here "fast" means "safe to run in a signal handler" and "local"
means "doesn't cause any network communication".

Any non-eligible loop will be executed on the CPU.

---------------------
Setup and compilation
---------------------

To enable GPU support set the environmental variable: `CHPL_LOCALE_MODEL=gpu`
before building Chapel.  Chapel's build system will automatically try and
deduce where your installation of CUDA exists. If the build system fails to do
this, or you would like to use a different CUDA installation set it using the
`CHPL_CUDA_PATH` environmental variable.

We also suggest setting `CHPL_RT_NUM_THREADS_PER_LOCALE=1` (this is necessary
if using CUDA 10).

To compile a program simply execute `chpl` as normal.  By default the generated
code will target compute capability 6.0 (specifically by passing
--cuda-gpu-arch=sm60 when invoking clang).  If you would like to target a
different compute capability (necessary for example, when targeting Tesla K20
GPUs) you can pass `--gpu-arch` to `chpl` and specify a different value there.
This may also be set using the `CHPL_CUDA_ARCH` environmental variable.

If you would like to view debugging information you can pass `--verbose` to
your generated executable. This output will show the number of CUDA kernel
calls that are generated.

-------
Example
-------

The following example illustrates running a computation on a GPU as well as a
CPU.  When `jacobi` is called with the GPU locale it will allocate the arrays
`A` and `B` into the device memory of the GPU and we generate three GPU kernels
for the `forall` loops in the function.

.. code-block:: chapel

  config const nSteps = 10;
  config const n = 10;

  writeln("on GPU:");
  jacobi(here.getChild(1));
  writeln("on CPU:");
  jacobi(here);

  proc jacobi(loc) {
    on loc {
      var A, B: [-1..n+1] real;

      A[-1] = 1; A[n+1] = 1;
      forall i in 0..n { A[i] = i:real; }

      for step in 1..nSteps {
        forall i in 0..n { B[i] = 0.33333 * (A[i-1] + A[i] + A[i+1]); }
        forall i in 0..n { A[i] = 0.33333 * (B[i-1] + B[i] + B[i+1]); }
      }
      writeln(A);
    }
  }


